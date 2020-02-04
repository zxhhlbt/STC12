#include "iic.h"
#include <math.h> 


#ifndef MPU6050_H
#define MPU6050_H

//------------------------------------------------
// ����MPU6050�ڲ���ַ
//------------------------------------------------
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG		    0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)

#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define	ACCEL_XOUT_H	0x3B  //���ٶȴ�������������Ĵ���
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	GYRO_XOUT_H		0x43  //��������������Ĵ���
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define TEMP_OUT_H      0x41        //�¶ȴ�������������Ĵ���
#define TEMP_OUT_L      0x42

#define	PWR_MGMT_1		0x6B	    //��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	    0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
//------------------------Other-------------------------
#define	MPU6050_DEV_ADDR	    0xD0>>1	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

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

// ��������������ֵ������оƬ�̶�������Ӳ�����е����� 
// ���ٶ���������4gʱ��ƫ��  
#define X_ACCEL_OFFSET   274 
#define Y_ACCEL_OFFSET   256 //���� Ӧ���������
#define Z_ACCEL_OFFSET   175 

#define X_GYRO_OFFSET    183 //���� 
#define Y_GYRO_OFFSET    19 
#define Z_GYRO_OFFSET    166 //���� 


//���ٶ�-������ ԭʼ����
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
//��ʼ��MPU6050
//----------------------------------------------------------------------
void InitMPU6050(void)
{
	IIC_Write_One_Byte(SlaveAddress, PWR_MGMT_1, 0x00);	  //�������
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
	return ((H << 8) + L);   //�ϳ�����
}
//---------------------------------------------------------------------
//  @brief      ��ȡMPU6050���ٶȼ�����
//  @param      NULL
//  @return     void
//  Sample usage:		ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//---------------------------------------------------------------------
void Get_AccData(void)
{
	mpu_acc_x = GetData(ACCEL_XOUT_H)  -X_ACCEL_OFFSET;
	mpu_acc_y = GetData(ACCEL_YOUT_H)  +Y_ACCEL_OFFSET;
	mpu_acc_z = GetData(ACCEL_ZOUT_H)  -Z_ACCEL_OFFSET;

}
//----------------------------------------------------------------------
//  @brief      ��ȡMPU6050����������
//  @param      NULL
//  @return     void
//  Sample usage:		ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//----------------------------------------------------------------------
void Get_Gyro(void)
{
	mpu_gyro_x = GetData(GYRO_XOUT_H)  +X_GYRO_OFFSET;
	mpu_gyro_y = GetData(GYRO_YOUT_H)  -Y_GYRO_OFFSET;
	mpu_gyro_z = GetData(GYRO_ZOUT_H)  +Z_GYRO_OFFSET;
}

#endif