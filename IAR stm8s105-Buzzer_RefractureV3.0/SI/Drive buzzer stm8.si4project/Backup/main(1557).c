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
#include <stm8s.h>
#include <uart2_module.h>
#include <lsm9ds1_stm8s.h>
#include <delay_time.h>
#include "stdio.h"
#include "stm8s_tim1.h"
#include "stm8s_gpio.h"




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
	
        	GPIO_WriteReverse(GPIOB, GPIO_PIN_0);  
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
	GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);//PB0 out
	GPIO_WriteLow(GPIOB, GPIO_PIN_0); //light on
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
	TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 10, 0);
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	TIM1_Cmd(DISABLE);
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
	//uint8_t count=07;
	
	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

	init_gpio();//light up led.

	init_timer1();
	enableInterrupts();
	
	delay_xms(2000);
	TIM1_Cmd(ENABLE);
	GPIO_WriteHight(GPIOB, GPIO_PIN_0);
	while(1)
	{
              delay_xms(1000);
 
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