/****************************************************
Desp:	entry of project.

Author:	leetoun
Date:	20181019
Modify History:
1.
<reason>:
<based>:
<ver>:
2.
<reason>:
<based>:
<ver>:

****************************************************/
#include "stm8s.h"
//#include <uart2_module.h>
//#include <lsm9ds1_stm8s.h>
#include <delay_time.h>
#include "stdio.h"
#include "stm8s_tim1.h"
#include "stm8s_gpio.h"
#include "stm8s_uart2.h"

#define HC595 GPIOC

#define DS	GPIO_PIN_7
#define OE	GPIO_PIN_6
#define STCLK	GPIO_PIN_5
#define SHCLK	GPIO_PIN_4

#define DS_H	GPIO_WriteHigh(HC595, DS) //DS=1
#define DS_L	GPIO_WriteLow(HC595, DS) //DS=0
#define SHCLK_H GPIO_WriteHigh(HC595, SHCLK) //SHCLK=1
#define SHCLK_L GPIO_WriteLow(HC595, SHCLK) //SHCLK=0
#define STCLK_H GPIO_WriteHigh(HC595, STCLK) //STCLK=1
#define STCLK_L GPIO_WriteLow(HC595, STCLK) //STCLK=0


enum UARTRX {UNGET=0, GET=!UNGET};
enum UARTRX g_flag_rx=UNGET;
#define RXSIZE 
typedef struct
{
	int rx_cnt;
	char rx_buf[RXSIZE];
	
}U2;


/****************************************************
Author:	leetoun
Date:	20181019
Brief:	timer1 interrupt handler
Param1:	void
Return:	void
****************************************************/
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
	if(SET == TIM1_GetITStatus(TIM1_IT_UPDATE))
	{
		TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	}
}

/****************************************************
Author:	leetoun
Date:	20181107
Brief:	uart2 rx interrupt handler
Param1:	void
Return:	void
****************************************************/
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    if(SET == UART2_GetITStatus(UART2_IT_RXNE))
    {
    	UART2_ClearITPendingBit(UART2_IT_RXNE);
	g_flag_rx = GET;
	//uart2_rx_handler_callback();
    }
}

/****************************************************
Author:	leetoun
Date:	20181019
Brief:	init the gpio,for quare signal wave generate.
Param1:	void
Return:	void
****************************************************/
void init_gpio(void)
{
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);//PB0 out
	GPIO_WriteLow(GPIOE, GPIO_PIN_5); //light on
}

/****************************************************
Author:	leetoun
Date:	20181101
Brief:	init the uart2,for communicate with the TDA2x
Param1:	void
Return:	void
****************************************************/
void init_uart2(void)
{
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);//PD5 out PP
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);//PD6 out PP
	UART2_DeInit();
	UART2_Init(115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_ITConfig(UART2_IT_RXNE, ENABLE);
	UART2_Cmd(DISABLE);	
}

/****************************************************
Author:	leetoun
Date:	20181107
Brief:	init the gpio,for 595
Param1:	void
Return:	void
****************************************************/
void init_595(void)
{
	GPIO_Init(HC595, STCLK, GPIO_MODE_OUT_PP_LOW_FAST);//Store-CLK
	GPIO_Init(HC595, SHCLK, GPIO_MODE_OUT_PP_LOW_FAST);//Shift-CLK
	GPIO_Init(HC595, DS, GPIO_MODE_OUT_PP_LOW_FAST);//DataIn
	GPIO_Init(HC595, OE, GPIO_MODE_OUT_PP_LOW_FAST);//OutputEnable
	SHCLK_L;
	STCLK_L;
//	GPIO_WriteLow(HC595, GPIO_PIN_5); 
}

/****************************************************
Author:	leetoun
Date:	20181019
Brief:	init the timer1
Param1:	void
Return:	void
****************************************************/
void init_timer1(void)
{
	TIM1_DeInit();
	TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 2, 0);
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	TIM1_Cmd(DISABLE);
}

/****************************************************
Author:	leetoun
Date:	20181107
Brief:	send n bytes
Param1:	addr of n bytes
Return:	
****************************************************/
void send_bytes(uint8_t* addr, int size)
{
	int num=0;
	for(num=0; num<size; num++)
	{
		UART2_SendData8(*addr);
		while(UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);
		addr++;
	}
}

/****************************************************
Author:	leetoun
Date:	20181107
Brief:	Write 595 one byte.
Param1:	
Return:	
****************************************************/
void write_595_byte(uint8_t data)
{
	uint8_t tmp_cnt=0;
	uint8_t out_bit=0;
	
	for(tmp_cnt=0; tmp_cnt<8; tmp_cnt++)
	{
		out_bit = (data<<tmp_cnt) & 0x80;
		if(out_bit)
		{
			DS_H; //DS=1
		}
		else
		{
			DS_L; //DS=0
		}
//		UART2_SendData8(out_bit);
//		while(UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);
		SHCLK_L;
		delay_us();
		SHCLK_H;
		delay_us();
	}
	//shift reg -> output reg:
	STCLK_L;
	delay_us();
	STCLK_H;
	delay_us();
}

/****************************************************
Author:	leetoun
Date:	20181019
Brief:	entry of project.
Param1:	number of input parameters.
Return:	error/0.

****************************************************/
int main(int argc, char* argv[])
{
	unsigned char rx=0;
	char send[]="hello";
	
	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	init_gpio();//light up led.
	init_uart2();
	init_595();
	init_timer1();
	enableInterrupts();
	
	TIM1_Cmd(ENABLE);
	UART2_Cmd(ENABLE);
	GPIO_WriteHigh(GPIOE, GPIO_PIN_5);//turn on led
	while(1)
	{
//		delay_xms(200);
		if(g_flag_rx)
		{
		  	rx = UART2_ReceiveData8();
		  	UART2_SendData8(rx);
			write_595_byte(rx);
			//send_bytes((uint8_t*)send, sizeof(send));
			g_flag_rx = UNGET;
		}
	}	
}

/* retarget printf func 
int fputc(int ch, FILE *f)
{
	//USART_SendData(USART1, (uint8_t) ch);
	UART2_SendData8((uint8_t) ch);
	while(UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);
	return ch;
}*/