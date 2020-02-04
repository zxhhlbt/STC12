#include <STC12C5A60S2.H>

#ifndef IIC_H
#define IIC_H

sbit SDA = P2^1; //模拟IIC数据位 根据实际情况修改IO口
sbit SCL = P2^0; //模拟IIC时钟位 根据实际情况修改IO口

void delay5us(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_ACK(bit ack);
bit  IIC_Recv_ACK();

void IIC_SendByte(unsigned char);
void IIC_Write_One_Byte(unsigned char dev_add, unsigned char reg, unsigned char dat);

unsigned char IIC_ReadByte(void);
unsigned char IIC_Read_One_Byte(unsigned char dev_add, unsigned char reg);

bit  IIC_Write_Str (unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len);
bit  IIC_Read_Str (unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len);

//---------------------------------------------------------------------
//  函数：void delay5us()
//  功能：延时5us
//---------------------------------------------------------------------
void delay5us()  //fosc 11.0592MHZ
{
	unsigned char i = 8;
	while (i--);
}

//---------------------------------------------------------------------
//  函数：void IIC_Start()
//  功能：启动IIC总线（SCL高电平时SDA下降沿）
//---------------------------------------------------------------------

void IIC_Start()
{
	SCL = 1;//SCL高电平
	SDA = 1;   
	delay5us();
	SDA = 0;//SDA下降沿
	delay5us();
	SCL = 0;//最后一定要把这个时钟线拉低，因为只有时钟线拉低的时候才允许数据变化。
	delay5us();
}

//---------------------------------------------------------------------
//  函数：void IIC_Stop()
//  功能：结束IIC总线（SCL高电平时SDA上升沿）
//---------------------------------------------------------------------

void IIC_Stop()
{
	SCL = 1;//SCL高电平
	SDA = 0;	
	delay5us();
	SDA = 1;//SDA上升沿
	delay5us();
}

//---------------------------------------------------------------------
//  函数：void IIC_Send_ACK(bit ack)
//  功能：发送应答位（a=0:应答信号 a=1:非应答信号）
//---------------------------------------------------------------------
void IIC_Send_ACK(bit ack)
{
	if (ack == 0)  SDA = 0;
	else           SDA = 1;
	SCL = 1;
	delay5us();
	SCL = 0;
	delay5us();
}

//---------------------------------------------------------------------
//  函数：void IIC_Send_ACK(bit ack)
//  功能：发送应答位（a=0:应答信号 a=1:非应答信号）
//---------------------------------------------------------------------
bit IIC_Recv_ACK()
{
	SCL = 1;                    //拉高时钟线
	delay5us();                 //延时
	CY = SDA;                   //读应答信号  CY为进位标志位
	SCL = 0;                    //拉低时钟线
	delay5us();                 //延时
	return CY;
}

//---------------------------------------------------------------------
//  函数：void IIC_SendByte(unsigned char dat)
//  功能：将数据dat发送出去，返回back应答位检测
//---------------------------------------------------------------------
void IIC_SendByte(unsigned char dat)
{
	unsigned char i=0;

	SCL = 0;// 拉低时钟线开始发送数据 
	for (i = 0; i < 8; i++) 
	{
		if (dat & 0x80) SDA = 1;
		else          SDA = 0;
		SCL = 1;   //SCL在高电平时发送数据
		delay5us();//SCL高电平发送完数据后 再拉到低电平让SDA移位
		SCL = 0;   //否则就是StartIIC和StopIIC了
		dat = dat << 1;
	}
	IIC_Recv_ACK();
}

//---------------------------------------------------------------------
//  函数：unsigned char IIC_ReadByte()
//  功能：接收数据 不含应答位
//---------------------------------------------------------------------
unsigned char IIC_ReadByte()
{
	unsigned char ReadData = 0;
	unsigned char i = 0;

	SCL = 0;               //"确保"SCL为低，为后续SDA电平变化做准备
	delay5us();
	SDA = 1;              //拉高SDA 准备接收数据  从单片机角度释放SDA总线，为IIC设备操作SDA线做准备
	
	for (i = 0; i < 8; i++)
	{
		ReadData = ReadData << 1;
		SCL = 1;
		delay5us();
		ReadData |= SDA;  //读数据
		SCL = 0;
		delay5us();
	}
	return ReadData;
}

//---------------------------------------------------------------------
//  @brief      模拟IIC写数据到设备寄存器函数
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      dat				写入的数据
//  @return     void
//  Sample usage:  只写一个字节数据
//---------------------------------------------------------------------
void IIC_Write_One_Byte(unsigned char dev_add, unsigned char reg, unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(dev_add);   //发送器件地址加写位
	IIC_SendByte(reg);   	 //发送从机寄存器地址
	IIC_SendByte(dat);   	 //发送需要写入的数据
	IIC_Stop();
}
//---------------------------------------------------------------------
//  @brief      模拟IIC从设备寄存器读取数据
//  @param      dev_add			设备地址(低七位地址)
//  @param      reg				寄存器地址
//  @param      type			选择通信方式是IIC  还是 SCCB
//  @return     uint8			返回寄存器的数据
//---------------------------------------------------------------------
unsigned char IIC_Read_One_Byte(unsigned char dev_add, unsigned char reg)
{
	unsigned char dat;
	
	IIC_Start();
	IIC_SendByte(dev_add);   //发送器件地址加写位
	IIC_SendByte(reg);   	 //发送从机寄存器地址

	IIC_Start();
	IIC_SendByte(dev_add + 0x01);   //发送器件地址加读位
	dat = IIC_ReadByte();   		//读取数据
	IIC_Send_ACK(1);
	IIC_Stop();

	return dat;
}
//---------------------------------------------------------------------
//   函数：bit IIC_Write_Str(dev_add, reg, *s, len)
//   功能：向有子地址的器件发送多字节数据
//   注：  dev_add :  器件地址      reg : 从机寄存器地址 
//   注：  s :    发送内容          len :     发送字节数
//   注：  返回1表示成功
//---------------------------------------------------------------------
bit IIC_Write_Str(unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len)
{
	unsigned char i = 0;
	IIC_Start();//启动总线
	IIC_SendByte(dev_add); //发送器件地址 加“写”位
	IIC_SendByte(reg);//发送从机寄存器地址

	for (i = 0; i < len; i++)
	{
		IIC_SendByte(*s);
		s++;
	}
	IIC_Stop();
	return 1;
}
//---------------------------------------------------------------------
//   函数：bit IIC_Read_Str(add, reg, *s, len)
//   功能：向有子地址的器件读取多字节数据
//   注：  dev_add : 器件地址       reg : 从机寄存器地址
//   注：  s :  接收数据存放地址    len :     接收字节数
//   注：  返回1表示成功
//---------------------------------------------------------------------
bit IIC_Read_Str(unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len)
{
	unsigned char i = 0;

	IIC_Start();          //启动总线
	IIC_SendByte(dev_add);        //发送器件地址加写位
	IIC_SendByte(reg);            //发送从机寄存器地址
	IIC_Start();         //再次启动总线
	IIC_SendByte(dev_add | 0x01); //发送器件地址加读位

	for (i = 0; i < len - 1; i++)//读取len-1个数据
	{
		*s = IIC_ReadByte();  //读取数据
		IIC_Send_ACK(0);           //发送应答位
		s++;
	}
	*s = IIC_ReadByte();//读取最后一个数据
	IIC_Send_ACK(1);//发送非应答 为了结束总线
	IIC_Stop();

	return 1;
}

#endif