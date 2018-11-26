#ifdef __LSM9DS1__
	#define	__LSM9DS1__
#else
	#define	__LSM9DS1__	extern
#endif	




//LSM9DS1�Ĵ�������
#define WHO_AM_I 		0x0f		//A/G��id or M��id
/*����M*/
#define CTRL_REG9		0x23		//0x40 page54: �ر�IIC\fifo en\����sleep_G\�¶ȴ�FIFO\DRDY_mask_bit
#define CTRL_REG2_M	          0x21		//0x60 page64���������á�4��8��12��16
#define CTRL_REG3_M	          0x22		//0x84 page64���ر�IIC\����SPI��д\����ת��mode  ��ʱ���ĵ�������ʱ������
#define CTRL_REG4_M	          0x23		//�޸Ĵ�С�˸�ʽ
/*����A or A/G*/
#define CTRL_REG6_XL	0x20		//0xc0 page51:����ODR\����\����
#define CTRL_REG1_G	          0x10		//0xc3 page45:����ODR\����\���� (��ʹ�õ�ʱ��д����)
#define CTRL_REG2_G	          0x11		//�ڲ��������
#define CTRL_REG3_G	          0x12		//��ͨfilter��enable
#define CTRL_REG8		0x12		//�޸Ĵ�С�˸�ʽ��Ĭ��Ϊ0x04����Ϊ���ģʽ0x06
#define FIFO_CTRL		0x2E		//
/*״̬�Ĵ��� & �¶�ֵ*/
#define STATUS_REG	          0x27		//page50: 0.0_A/0.1_G/0.2_T��data�Ƿ�available
#define STATUS_REG_M	0x27		//page66: 0.3 M��data�Ƿ�available
#define OUT_TEMP_L	          0x15		//�¶ȴ�����ֵ
#define OUT_TEMP_H	          0x16		//
/*Gyroscope*/
#define OUT_X_L_G		0x18		//
#define OUT_X_H_G		0x19		//
#define OUT_Y_L_G		0x1A		//
#define OUT_Y_H_G		0x1B		//
#define OUT_Z_L_G		0x1C		//
#define OUT_Z_H_G		0x1D		//
/*Accelerate*/
#define OUT_X_L_XL	          0x28		//
#define OUT_X_H_XL	          0x29		//
#define OUT_Y_L_XL	          0x2A		//
#define OUT_Y_H_XL 	          0x2B		//
#define OUT_Z_L_XL	          0x2C		//
#define OUT_Z_H_XL	          0x2D		//
/*Magnetic*/
#define OUT_X_L_M		0x28		//
#define OUT_X_H_M		0x29		//
#define OUT_Y_L_M		0x2A		//
#define OUT_Y_H_M	 	0x2B		//
#define OUT_Z_L_M		0x2C		//
#define OUT_Z_H_M		0x2D		//

/********************�����̷�Χ����******************/
//���ٶ�
#define SCALE_XL_2G	          0xC0
#define SCALE_XL_16G	0xC8
#define SCALE_XL_4G	          0xD0
#define SCALE_XL_8G	          0xD8
//������
#define SCALE_G_245	          0xC3
#define SCALE_G_500	          0xCB
#define SCALE_G_2000	0xDB
//�ų���
#define SCALE_M_4		0x00
#define SCALE_M_8		0x20
#define SCALE_M_12	          0x40
#define SCALE_M_16	          0x60

#define BIGENDIAN_AG	0x06
#define BIGENDIAN_M	          0x01

#define START_A		0xAA		//ASCII��A  or 0xaa
#define START_M		0x55		//ASCII��M  or 0x55
#define END_A	          0xA5		//


__LSM9DS1__ unsigned char frame_array_9z[22];	//9������֡

//LSM9DS1��������
__LSM9DS1__ void lsm9ds1_module_init(void);
__LSM9DS1__ void lsm9ds1_readID(void);
__LSM9DS1__ void acc_init(void);
__LSM9DS1__ void gyro_init(void);
__LSM9DS1__ void mag_init(void);
__LSM9DS1__ void spi_init(void);
__LSM9DS1__ unsigned char spi_rd_ag(unsigned char address);
__LSM9DS1__ unsigned char spi_rd_m(unsigned char address);
__LSM9DS1__ void spi_wr_ag(unsigned char address, unsigned char dat);
__LSM9DS1__ void spi_wr_m(unsigned char address, unsigned char dat);
__LSM9DS1__ void spi_9z_sample(void);