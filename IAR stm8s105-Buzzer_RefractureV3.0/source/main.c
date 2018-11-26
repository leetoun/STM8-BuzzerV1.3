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


//for M74HC595
#define HC595 GPIOC
#define DS	GPIO_PIN_7
#define OE	GPIO_PIN_6
#define STCLK	GPIO_PIN_5
#define SHCLK	GPIO_PIN_4
//M74HC595 GPIO 
#define DS_H	GPIO_WriteHigh(HC595, DS) //DS=1
#define DS_L	GPIO_WriteLow(HC595, DS) //DS=0
#define SHCLK_H GPIO_WriteHigh(HC595, SHCLK) //SHCLK=1
#define SHCLK_L GPIO_WriteLow(HC595, SHCLK) //SHCLK=0
#define STCLK_H GPIO_WriteHigh(HC595, STCLK) //STCLK=1
#define STCLK_L GPIO_WriteLow(HC595, STCLK) //STCLK=0

//for protocal decode.
enum FRAME {UNDECODE=0, DECODE=!UNDECODE};

#ifdef PROTO
      #define START		0x11
      #define PATTERN		0x22
      #define TEXT0		0x2a
      #define END		0x27
#else
      #define START0		'a'
      #define START1		'a'
      #define PATTERN0		'5'
      #define PATTERN1		'5'
      #define END	        'f'
#endif

//for uart2
enum UARTRX {UNGET=0, GET=!UNGET};
enum UARTRX g_flag_rx=UNGET;
#define RXBUFSIZE 10
#define FRAMESIZE  4
typedef struct
{
	int rx_cnt;
	char rx_buf[RXBUFSIZE];
	char flag_decode;
}STR_UART;
STR_UART U2;

//for state machine transfering.
char step = 0;

//for buzzer & timer1
#define BUZZTIME 0xff //300
unsigned int g_step_countdown0=BUZZTIME;
unsigned int g_step_countdown1=BUZZTIME;
enum BUZZ{NOACT=0, ACT=1};
enum BUZZ g_flag_buzzer1=NOACT;
enum BUZZ g_flag_buzzer2=NOACT;

//for interrupt of uart2.
char g_int_step=0;

// left channel:
#define BUZZER1_H GPIO_WriteHigh(GPIOC, GPIO_PIN_1)
#define BUZZER1_L GPIO_WriteLow(GPIOC, GPIO_PIN_1)
#define LEFTRED_H GPIO_WriteHigh(GPIOC, GPIO_PIN_2)
#define LEFTRED_L GPIO_WriteLow(GPIOC, GPIO_PIN_2)
#define LEFTGREEN_H GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define LEFTGREEN_L GPIO_WriteLow(GPIOC, GPIO_PIN_3)
//right channel:
#define RIGHTGREEN_H GPIO_WriteHigh(GPIOB, GPIO_PIN_2)
#define RIGHTGREEN_L GPIO_WriteLow(GPIOB, GPIO_PIN_2)
#define RIGHTRED_H GPIO_WriteHigh(GPIOB, GPIO_PIN_3)
#define RIGHTRED_L GPIO_WriteLow(GPIOB, GPIO_PIN_3)
#define BUZZER2_H GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define BUZZER2_L GPIO_WriteLow(GPIOB, GPIO_PIN_4)

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
		if(step%2)
		{
			if(ACT == g_flag_buzzer1)
                        {
                                GPIO_WriteReverse(GPIOC, GPIO_PIN_1); //BUZZER1
                        }
                        if(ACT == g_flag_buzzer2)
                        {
                                GPIO_WriteReverse(GPIOB, GPIO_PIN_4); //BUZZER2
                        }
                        //countdown
			if(g_step_countdown1 > 0)
				g_step_countdown1--;
			else
				step++;
		}
		else //nothing to be done
		{
			if(g_step_countdown0 > 0)
				g_step_countdown0--;
			else
				step++;
		}
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
    	U2.rx_buf[U2.rx_cnt] = UART2_ReceiveData8();
        switch (g_int_step)
        {
            case 0:
                if(START0 == U2.rx_buf[U2.rx_cnt]) //'a'
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                break;
            case 1:
                if(START1 == U2.rx_buf[U2.rx_cnt]) //'a'
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0;
                    U2.rx_cnt = 0;
                }
                break;
            case 2:
                if(PATTERN0 == U2.rx_buf[U2.rx_cnt]) //¡®5¡¯
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff
                }
                break;
            case 3:
                if(PATTERN1 == U2.rx_buf[U2.rx_cnt]) //'5'
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff
                }
                break;	
            case 4:
                if('l'==U2.rx_buf[U2.rx_cnt] || 'r'==U2.rx_buf[U2.rx_cnt]) //is left or right?
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff 
                }

                break;
            case 5:
                if('b'==U2.rx_buf[U2.rx_cnt] || 'g'==U2.rx_buf[U2.rx_cnt] || 'r'==U2.rx_buf[U2.rx_cnt]) //is buzzer or green led or red led?
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff
                }
                break;	
            case 6:
                if(END == U2.rx_buf[U2.rx_cnt])
                {
                    g_int_step++; //next step
                    U2.rx_cnt++; //next buff
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff
                }
                break;	
                
            case 7:
                if(END == U2.rx_buf[U2.rx_cnt])
                {
                    g_int_step=0; //next step,will be clear.
                    U2.flag_decode = (char)DECODE; //global flag frame need to decode.
                }
                else
                {
                    g_int_step = 0; //next step
                    U2.rx_cnt = 0; //next buff
                }
                break;
            default:
                g_int_step = 0;
                U2.rx_cnt = 0;
        }
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
	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);//PB4 out
        GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);//PB3 out
        GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);//PB2 out
	GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST);//PC1 out
        GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);//PC2 out
        GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);//PC3 out
    
	GPIO_WriteLow(GPIOC, GPIO_PIN_1); //BUZZER1
        GPIO_WriteLow(GPIOC, GPIO_PIN_2); //led-red
        GPIO_WriteLow(GPIOC, GPIO_PIN_3); //led-green
        GPIO_WriteLow(GPIOB, GPIO_PIN_4); //BUZZER2
	GPIO_WriteLow(GPIOB, GPIO_PIN_3); //led2-red
        GPIO_WriteLow(GPIOB, GPIO_PIN_2); //led2-green
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
	UART2_Init(115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_RX_ENABLE);
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
	TIM1_TimeBaseInit(1600, TIM1_COUNTERMODE_UP, 4, 0);
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

void clear_U2_buf(void)
{
        U2.rx_buf[0] = 0;
        U2.rx_buf[1] = 0;
        U2.rx_buf[2] = 0;
        U2.rx_buf[3] = 0;
        U2.rx_buf[4] = 0;
        U2.rx_buf[5] = 0;
        U2.rx_buf[6] = 0;
        U2.rx_buf[7] = 0;
}

void default_status(void)
{
        BUZZER1_L;
        BUZZER2_L;
        LEFTGREEN_L;
        LEFTRED_L;
        RIGHTGREEN_L;
        RIGHTRED_L;
        g_flag_buzzer1 = NOACT;
        g_flag_buzzer2 = NOACT;
}


void sys_init(void)
{
        CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	init_gpio();//buzzer&led&595&motor
        init_timer1(); //PWM
	init_uart2();
	init_595();
        TIM1_Cmd(DISABLE);
	UART2_Cmd(ENABLE);
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
	//unsigned char rx=0;
	//char send[]="hello";
	
	sys_init();
	enableInterrupts();
        
	while(1)
	{
		
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