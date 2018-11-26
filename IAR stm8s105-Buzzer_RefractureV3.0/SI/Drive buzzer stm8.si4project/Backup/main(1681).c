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
	TIM1_DeInit();

	enableInterrupts();
	//delay_xms(500);

  
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