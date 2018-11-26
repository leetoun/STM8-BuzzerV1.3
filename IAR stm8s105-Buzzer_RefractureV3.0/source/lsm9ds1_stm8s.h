#ifdef __LSM9DS1__
	#define	__LSM9DS1__
#else
	#define	__LSM9DS1__	extern
#endif	




//LSM9DS1寄存器定义
#define WHO_AM_I 		0x0f		//A/G的id or M的id
/*配置M*/
#define CTRL_REG9		0x23		//0x40 page54: 关闭IIC\fifo en\设置sleep_G\温度存FIFO\DRDY_mask_bit
#define CTRL_REG2_M	          0x21		//0x60 page64：量程设置。4、8、12、16
#define CTRL_REG3_M	          0x22		//0x84 page64：关闭IIC\允许SPI读写\连续转换mode  有时读的到数，有时读不到
#define CTRL_REG4_M	          0x23		//修改大小端格式
/*配置A or A/G*/
#define CTRL_REG6_XL	0x20		//0xc0 page51:设置ODR\量程\带宽
#define CTRL_REG1_G	          0x10		//0xc3 page45:设置ODR\量程\带宽 (不使用的时候不写数据)
#define CTRL_REG2_G	          0x11		//内部输出配置
#define CTRL_REG3_G	          0x12		//高通filter的enable
#define CTRL_REG8		0x12		//修改大小端格式，默认为0x04；改为大端模式0x06
#define FIFO_CTRL		0x2E		//
/*状态寄存器 & 温度值*/
#define STATUS_REG	          0x27		//page50: 0.0_A/0.1_G/0.2_T的data是否available
#define STATUS_REG_M	0x27		//page66: 0.3 M的data是否available
#define OUT_TEMP_L	          0x15		//温度传感器值
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

/********************改量程范围定义******************/
//加速度
#define SCALE_XL_2G	          0xC0
#define SCALE_XL_16G	0xC8
#define SCALE_XL_4G	          0xD0
#define SCALE_XL_8G	          0xD8
//陀螺仪
#define SCALE_G_245	          0xC3
#define SCALE_G_500	          0xCB
#define SCALE_G_2000	0xDB
//磁场计
#define SCALE_M_4		0x00
#define SCALE_M_8		0x20
#define SCALE_M_12	          0x40
#define SCALE_M_16	          0x60

#define BIGENDIAN_AG	0x06
#define BIGENDIAN_M	          0x01

#define START_A		0xAA		//ASCII的A  or 0xaa
#define START_M		0x55		//ASCII的M  or 0x55
#define END_A	          0xA5		//


__LSM9DS1__ unsigned char frame_array_9z[22];	//9轴数据帧

//LSM9DS1函数声明
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