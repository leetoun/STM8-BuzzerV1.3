#define  _UART2_MODULE_
#include "stm8s.h"
#include "uart2_module.h"

extern u16 G_LED_Flash_Times;

void uart2_module_init(void)
{  
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);//enable uart2 clock
      GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//PD5/uart_tx
      GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);//PD6/uart_rx
	UART2_Init(115200, UART2_WORDLENGTH_9D, UART2_STOPBITS_1, \
              UART2_PARITY_EVEN, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
      //UART2_ClearITPendingBit(UART2_IT_RXNE);
	UART2->CR2 |= 0x20;//开启接收中断RXNE
	
}
/* 发送多字节数据 */
void uart2_sendbytes(u8* startaddr, u16 numbertosend)
{
      while(numbertosend--)
      {
            UART2_SendData8(*startaddr);
      }
}
/* 中断接收ISR */
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
      if(SET == UART2_GetITStatus(UART2_IT_RXNE))//assert RXNE interrupt
      {
            UART2_ClearITPendingBit(UART2_IT_RXNE);//clear pending interrupt
            GPIO_WriteReverse(GPIOE, GPIO_PIN_5);//reverse state of LED
            G_LED_Flash_Times++;
      }
}
