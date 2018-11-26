/*
	1. ע��SPI��ȡ�������ݵ�ʱ�򣬷����ַ���ݣ���Ҫ����DR��Ȼ���ȡ��ȷ�����ݣ�
	�������ִ���//2017-02-12 lt
	2. SPI��CRC��ʲô��//2017-02-12 lt
      3. �޸ľ���M��AG��ѡͨ�������š�2017-02-21 lt
      4. ��ʼ��������//���м�����������* ���ӿں�����* + ���У�2017-02-21 lt
*/
#define  __LSM9DS1__ 
//#define  SOFT_SPI
#include "stm8s.h"
#include "lsm9ds1_stm8s.h"
#define  CS_M_H  	GPIO_WriteHigh(GPIOE, GPIO_PIN_5)
#define  CS_M_L  	GPIO_WriteLow(GPIOE, GPIO_PIN_5)
#define  CS_AG_H  GPIO_WriteHigh(GPIOC, GPIO_PIN_1)
#define  CS_AG_L  GPIO_WriteLow(GPIOC, GPIO_PIN_1)
//#define  SCLK  


extern unsigned char chip_id;
extern void delay_us(void);
extern void delay_10us(void);
extern void delay_100us(void);
extern void delay_1ms(void);
extern void delay_xms(u16 counter);



// SPI��ʼ�� 
void spi_init(void)
{
      //GPIOģ��SPI���ߣ�GPIO����
      GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//PE5,CS_M
      GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_PP_HIGH_FAST);//PC1,CS_AG
      GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);//PC5,SCLK
      GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_FAST);//PC6,MOSI
      GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);//PC7,MISO
#ifdef SOFT_SPI
#else 
      CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);//enable SPI clock
      SPI_DeInit();//2017-02-12 lt
      SPI_Init(SPI_FIRSTBIT_MSB,SPI_BAUDRATEPRESCALER_2,SPI_MODE_MASTER,SPI_CLOCKPOLARITY_HIGH,
               SPI_CLOCKPHASE_2EDGE,SPI_DATADIRECTION_2LINES_FULLDUPLEX,SPI_NSS_HARD,0X07);//CRCֵ����
      CS_AG_H;
	SPI_Cmd(ENABLE);//2017-02-12 lt
      SPI_CalculateCRCCmd(DISABLE);//2017-02-12 lt
#endif
}

// LSM9DS1ģ���ʼ�� 
void lsm9ds1_module_init(void)
{
      spi_init();
      CS_M_H;
      //acc_init();
      //gyro_init();
      //mag_init();
}





/* LSM9DS1��ȡag���� */
unsigned char spi_rd_ag(unsigned char address)
{
	unsigned char tmp_rx=0;
	
	CS_AG_L;
	while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
	SPI->DR = address|0x80;
	while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == SET);
	tmp_rx = SPI->DR;//�������������
	
	while(SPI_GetFlagStatus(SPI_FLAG_TXE) == RESET);
	SPI->DR = 0x00;
	while(SPI_GetFlagStatus(SPI_FLAG_RXNE) == SET);
	tmp_rx = SPI->DR;
	CS_AG_H;
	SPI_Cmd(DISABLE);
	
	return tmp_rx;
}
/* LSM9DS1д��ag���� */
void spi_wr_ag(unsigned char address, unsigned char dat)
{
	
}
/* 
	LSM9DS1��ȡa/g��id 
*/
void lsm9ds1_readID(void)
{
      unsigned char id=0;
      
      id = spi_rd_ag(0x0f);//��0x0f������
      chip_id = id;
}
