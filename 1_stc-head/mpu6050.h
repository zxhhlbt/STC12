#include "iic.h"
#include <math.h> 


#ifndef MPU6050_H
#define MPU6050_H

//------------------------------------------------
// 定义MPU6050内部地址
//------------------------------------------------
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG		    0x1A	//低通滤波频率，典型值：0x06(5Hz)

#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	ACCEL_XOUT_H	0x3B  //加速度传感器数据输出寄存器
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	GYRO_XOUT_H		0x43  //陀螺仪数据输出寄存器
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define TEMP_OUT_H      0x41        //温度传感器数据输出寄存器
#define TEMP_OUT_L      0x42

#define	PWR_MGMT_1		0x6B	    //电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	    0xD0	//IIC写入时的地址字节数据，+1为读取
//------------------------Other-------------------------
#define	MPU6050_DEV_ADDR	    0xD0>>1	//IIC写入时的地址字节数据，+1为读取

//The measurement range of Gyroscope
#define P_M_250dps   0x00  //plus or minus 250 degree per second
#define P_M_500dps   0x08
#define P_M_1000dps  0x10
#define P_M_2000dps  0x18

//The measurement range of Accelerometer
#define P_M_2g   0x00      //plus or minus 2g
#define P_M_4g   0x08
#define P_M_8g   0x10
#define P_M_16g  0x18

// 传感器数据修正值（消除芯片固定误差，根据硬件进行调整） 
// 加速度量程正负4g时的偏差  
#define X_ACCEL_OFFSET   274 
#define Y_ACCEL_OFFSET   256 //负数 应加上这个数
#define Z_ACCEL_OFFSET   175 

#define X_GYRO_OFFSET    183 //负数 
#define Y_GYRO_OFFSET    19 
#define Z_GYRO_OFFSET    166 //负数 


//加速度-陀螺仪 原始数据
int mpu_gyro_x;
int mpu_gyro_y;
int mpu_gyro_z;
int mpu_acc_x;
int mpu_acc_y;
int mpu_acc_z;


void InitMPU6050(void);
void Get_AccData(void);
void Get_Gyro(void);
int GetData(unsigned char REG_Address);
//----------------------------------------------------------------------
//初始化MPU6050
//----------------------------------------------------------------------
void InitMPU6050(void)
{
	IIC_Write_One_Byte(SlaveAddress, PWR_MGMT_1, 0x00);	  //解除休眠
	IIC_Write_One_Byte(SlaveAddress, SMPLRT_DIV, 0x07);
	IIC_Write_One_Byte(SlaveAddress, CONFIG, 0x06);
	IIC_Write_One_Byte(SlaveAddress, ACCEL_CONFIG, P_M_4g);
	IIC_Write_One_Byte(SlaveAddress, GYRO_CONFIG, P_M_1000dps);

}

int GetData(unsigned char REG_Address)
{
	unsigned char H, L;
	H = IIC_Read_One_Byte(SlaveAddress, REG_Address);
	L = IIC_Read_One_Byte(SlaveAddress, REG_Address + 1);
	return ((H << 8) + L);   //合成数据
}
//---------------------------------------------------------------------
//  @brief      获取MPU6050加速度计数据
//  @param      NULL
//  @return     void
//  Sample usage:		执行该函数后，直接查看对应的变量即可
//---------------------------------------------------------------------
void Get_AccData(void)
{
	mpu_acc_x = GetData(ACCEL_XOUT_H)  -X_ACCEL_OFFSET;
	mpu_acc_y = GetData(ACCEL_YOUT_H)  +Y_ACCEL_OFFSET;
	mpu_acc_z = GetData(ACCEL_ZOUT_H)  -Z_ACCEL_OFFSET;

}
//----------------------------------------------------------------------
//  @brief      获取MPU6050陀螺仪数据
//  @param      NULL
//  @return     void
//  Sample usage:		执行该函数后，直接查看对应的变量即可
//----------------------------------------------------------------------
void Get_Gyro(void)
{
	mpu_gyro_x = GetData(GYRO_XOUT_H)  +X_GYRO_OFFSET;
	mpu_gyro_y = GetData(GYRO_YOUT_H)  -Y_GYRO_OFFSET;
	mpu_gyro_z = GetData(GYRO_ZOUT_H)  +Z_GYRO_OFFSET;
}

#endif