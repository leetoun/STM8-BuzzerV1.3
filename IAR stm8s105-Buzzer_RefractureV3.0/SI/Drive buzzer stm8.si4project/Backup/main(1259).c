#include <stm8s.h>
#include <uart2_module.h>
#include <lsm9ds1_stm8s.h>
#include <delay_time.h>
#include "stdio.h"

u16 G_LED_Flash_Times=0;
unsigned char chip_id=0;

/* ���*/
void main(void)
{
	//uint8_t count=07;
	CLK_DeInit();
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	uart2_module_init();
	lsm9ds1_module_init();
	enableInterrupts();
	//delay_xms(500);
	lsm9ds1_readID();
	UART2_SendData8(chip_id);//��ӡ��ȡ�����id   
	while(1)
	{
		delay_xms(1000);
		UART2_SendData8(0x7f);//��ӡ��ȡ�����id   
		//lsm9ds1_readID();
		//UART2_SendData8(chip_id);//��ӡ��ȡ�����id   
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