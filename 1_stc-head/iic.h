#include <STC12C5A60S2.H>

#ifndef IIC_H
#define IIC_H

sbit SDA = P2^1; //ģ��IIC����λ ����ʵ������޸�IO��
sbit SCL = P2^0; //ģ��IICʱ��λ ����ʵ������޸�IO��

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
//  ������void delay5us()
//  ���ܣ���ʱ5us
//---------------------------------------------------------------------
void delay5us()  //fosc 11.0592MHZ
{
	unsigned char i = 8;
	while (i--);
}

//---------------------------------------------------------------------
//  ������void IIC_Start()
//  ���ܣ�����IIC���ߣ�SCL�ߵ�ƽʱSDA�½��أ�
//---------------------------------------------------------------------

void IIC_Start()
{
	SCL = 1;//SCL�ߵ�ƽ
	SDA = 1;   
	delay5us();
	SDA = 0;//SDA�½���
	delay5us();
	SCL = 0;//���һ��Ҫ�����ʱ�������ͣ���Ϊֻ��ʱ�������͵�ʱ����������ݱ仯��
	delay5us();
}

//---------------------------------------------------------------------
//  ������void IIC_Stop()
//  ���ܣ�����IIC���ߣ�SCL�ߵ�ƽʱSDA�����أ�
//---------------------------------------------------------------------

void IIC_Stop()
{
	SCL = 1;//SCL�ߵ�ƽ
	SDA = 0;	
	delay5us();
	SDA = 1;//SDA������
	delay5us();
}

//---------------------------------------------------------------------
//  ������void IIC_Send_ACK(bit ack)
//  ���ܣ�����Ӧ��λ��a=0:Ӧ���ź� a=1:��Ӧ���źţ�
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
//  ������void IIC_Send_ACK(bit ack)
//  ���ܣ�����Ӧ��λ��a=0:Ӧ���ź� a=1:��Ӧ���źţ�
//---------------------------------------------------------------------
bit IIC_Recv_ACK()
{
	SCL = 1;                    //����ʱ����
	delay5us();                 //��ʱ
	CY = SDA;                   //��Ӧ���ź�  CYΪ��λ��־λ
	SCL = 0;                    //����ʱ����
	delay5us();                 //��ʱ
	return CY;
}

//---------------------------------------------------------------------
//  ������void IIC_SendByte(unsigned char dat)
//  ���ܣ�������dat���ͳ�ȥ������backӦ��λ���
//---------------------------------------------------------------------
void IIC_SendByte(unsigned char dat)
{
	unsigned char i=0;

	SCL = 0;// ����ʱ���߿�ʼ�������� 
	for (i = 0; i < 8; i++) 
	{
		if (dat & 0x80) SDA = 1;
		else          SDA = 0;
		SCL = 1;   //SCL�ڸߵ�ƽʱ��������
		delay5us();//SCL�ߵ�ƽ���������ݺ� �������͵�ƽ��SDA��λ
		SCL = 0;   //�������StartIIC��StopIIC��
		dat = dat << 1;
	}
	IIC_Recv_ACK();
}

//---------------------------------------------------------------------
//  ������unsigned char IIC_ReadByte()
//  ���ܣ��������� ����Ӧ��λ
//---------------------------------------------------------------------
unsigned char IIC_ReadByte()
{
	unsigned char ReadData = 0;
	unsigned char i = 0;

	SCL = 0;               //"ȷ��"SCLΪ�ͣ�Ϊ����SDA��ƽ�仯��׼��
	delay5us();
	SDA = 1;              //����SDA ׼����������  �ӵ�Ƭ���Ƕ��ͷ�SDA���ߣ�ΪIIC�豸����SDA����׼��
	
	for (i = 0; i < 8; i++)
	{
		ReadData = ReadData << 1;
		SCL = 1;
		delay5us();
		ReadData |= SDA;  //������
		SCL = 0;
		delay5us();
	}
	return ReadData;
}

//---------------------------------------------------------------------
//  @brief      ģ��IICд���ݵ��豸�Ĵ�������
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      dat				д�������
//  @return     void
//  Sample usage:  ֻдһ���ֽ�����
//---------------------------------------------------------------------
void IIC_Write_One_Byte(unsigned char dev_add, unsigned char reg, unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(dev_add);   //����������ַ��дλ
	IIC_SendByte(reg);   	 //���ʹӻ��Ĵ�����ַ
	IIC_SendByte(dat);   	 //������Ҫд�������
	IIC_Stop();
}
//---------------------------------------------------------------------
//  @brief      ģ��IIC���豸�Ĵ�����ȡ����
//  @param      dev_add			�豸��ַ(����λ��ַ)
//  @param      reg				�Ĵ�����ַ
//  @param      type			ѡ��ͨ�ŷ�ʽ��IIC  ���� SCCB
//  @return     uint8			���ؼĴ���������
//---------------------------------------------------------------------
unsigned char IIC_Read_One_Byte(unsigned char dev_add, unsigned char reg)
{
	unsigned char dat;
	
	IIC_Start();
	IIC_SendByte(dev_add);   //����������ַ��дλ
	IIC_SendByte(reg);   	 //���ʹӻ��Ĵ�����ַ

	IIC_Start();
	IIC_SendByte(dev_add + 0x01);   //����������ַ�Ӷ�λ
	dat = IIC_ReadByte();   		//��ȡ����
	IIC_Send_ACK(1);
	IIC_Stop();

	return dat;
}
//---------------------------------------------------------------------
//   ������bit IIC_Write_Str(dev_add, reg, *s, len)
//   ���ܣ������ӵ�ַ���������Ͷ��ֽ�����
//   ע��  dev_add :  ������ַ      reg : �ӻ��Ĵ�����ַ 
//   ע��  s :    ��������          len :     �����ֽ���
//   ע��  ����1��ʾ�ɹ�
//---------------------------------------------------------------------
bit IIC_Write_Str(unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len)
{
	unsigned char i = 0;
	IIC_Start();//��������
	IIC_SendByte(dev_add); //����������ַ �ӡ�д��λ
	IIC_SendByte(reg);//���ʹӻ��Ĵ�����ַ

	for (i = 0; i < len; i++)
	{
		IIC_SendByte(*s);
		s++;
	}
	IIC_Stop();
	return 1;
}
//---------------------------------------------------------------------
//   ������bit IIC_Read_Str(add, reg, *s, len)
//   ���ܣ������ӵ�ַ��������ȡ���ֽ�����
//   ע��  dev_add : ������ַ       reg : �ӻ��Ĵ�����ַ
//   ע��  s :  �������ݴ�ŵ�ַ    len :     �����ֽ���
//   ע��  ����1��ʾ�ɹ�
//---------------------------------------------------------------------
bit IIC_Read_Str(unsigned char dev_add, unsigned char reg, unsigned char* s, unsigned char len)
{
	unsigned char i = 0;

	IIC_Start();          //��������
	IIC_SendByte(dev_add);        //����������ַ��дλ
	IIC_SendByte(reg);            //���ʹӻ��Ĵ�����ַ
	IIC_Start();         //�ٴ���������
	IIC_SendByte(dev_add | 0x01); //����������ַ�Ӷ�λ

	for (i = 0; i < len - 1; i++)//��ȡlen-1������
	{
		*s = IIC_ReadByte();  //��ȡ����
		IIC_Send_ACK(0);           //����Ӧ��λ
		s++;
	}
	*s = IIC_ReadByte();//��ȡ���һ������
	IIC_Send_ACK(1);//���ͷ�Ӧ�� Ϊ�˽�������
	IIC_Stop();

	return 1;
}

#endif