/*******************************************
ע��ʹ��ʱҪ����������Init
*******************************************/

#include <STC12C5A60S2.h>
#include "iic.h"
#include "delay.h"
#include "codetab.h"

#define	Brightness	0xCF   //������ʾ����
#define X_WIDTH 	128    //OLED���������ص���Ŀ
#define Y_WIDTH 	64	   //OLED���������ص���Ŀ

void Oled_Display(void);
void OLED_WrDat(unsigned char IIC_Data);
void OLED_WrCmd(unsigned char IIC_Command);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char bmp_dat);
void OLED_CLS(void);
void OLED_Init(void);
void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]);
void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]);
void OLED_P16x16Ch(unsigned char x, y, N);
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]);

void Oled_Display()
{
	unsigned char i;
		
//		OLED_Fill(0xff); //��ȫ��
//		Delay_ms(2000);
//		OLED_Fill(0x00); //��ȫ��
//		Delay_ms(200);
	
//-------------------------------------------------
//��ʾ����  ����һ�� ��������һ�棩
//ǰ���⼸���ڵ�һ�У�ÿ�����ֵļ�಻̫һ��
//���û����forѭ������
//����2��4��6����ÿ�����ӽ����ŵģ�����ѭ�� 
//-------------------------------------------------	      
		OLED_P16x16Ch(24, 0, 1);  //��1�� ��24���㿪ʼ ��ʾFF16X16�����2������
		OLED_P16x16Ch(40, 0, 2);  //��1�� ��40���㿪ʼ ��ʾFF16X16�����3������
		OLED_P16x16Ch(57, 0, 3);
		OLED_P16x16Ch(74, 0, 4);
		OLED_P16x16Ch(91, 0, 5);
		for (i = 0; i < 8; i++)//ͨ��������ʾ���� -- i��ʾ�ֱ������λ��
		{
			//			OLED_P16x16Ch(i*16,0,i);
			OLED_P16x16Ch(i * 16, 2, i + 8);   //16X16���֣���16��ÿ����������
			OLED_P16x16Ch(i * 16, 4, i + 16);
			OLED_P16x16Ch(i * 16, 6, i + 24);
		}
		Delay_ms(4000);
		OLED_CLS();//����  ����һҳ

//-------------------------------------------------
//��ʾ8x16�ַ�   ���ڶ��棩
//8x16ÿ���ַ���16��������ռ���� ����ÿ������2
//6x8ÿ���ַ���8��������ռһ�У�����ÿ������1
//-------------------------------------------------
		OLED_P8x16Str(1, 0, "HelTec");//��һ�� -- 8x16����ʾ��Ԫ��ʾASCII��
		OLED_P8x16Str(1, 2, "OLED Display");
		OLED_P8x16Str(1, 4, "www.heltec.cn");
//-------------------------------------------------
//��ʾ6x8�ַ�   �������棩
//-------------------------------------------------
		OLED_P6x8Str(1, 6, "cn.heltec@gmail.com");//������ �ӵ�һ���㿪ʼ
		OLED_P6x8Str(1, 7, "heltec.taobao.com");
		Delay_ms(4000);
		OLED_CLS();
//-------------------------------------------------
//��ʾBMPͼ�� 
//-------------------------------------------------
		Draw_BMP(0, 0, 128, 8, BMP2);
		Delay_ms(8000);
		OLED_CLS();
}
/*********************OLEDд����************************************/
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_SendByte(0x78);
	IIC_SendByte(0x40);			//write data
	IIC_SendByte(IIC_Data);
	IIC_Stop();
}
/*********************OLEDд����************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	IIC_SendByte(0x78);         //Slave address,SA0=0
	IIC_SendByte(0x00);			//write command
	IIC_SendByte(IIC_Command);
	IIC_Stop();
}

/*********************OLED ��������************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDȫ��************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y, x;
	for (y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for (x = 0; x < X_WIDTH; x++)
			OLED_WrDat(bmp_dat);
	}
}
/*********************OLED��λ************************************/
void OLED_CLS(void)
{
	unsigned char y, x;
	for (y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for (x = 0; x < X_WIDTH; x++)
			OLED_WrDat(0);
	}
}

/*********************OLED��ʼ��************************************/
void OLED_Init(void)
{
	Delay_ms(500);//��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_Pos(0, 0);
}
/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P6x8Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32; //��ʼ��ַ
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		
		for(i=0;i<6;i++)
			OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P8x16Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32; //��ʼ��ַ
		if(x>120){x=0;y++;}
		
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i]);
		
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_P16x16Ch(unsigned char x, y, N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;   //16x16�ĺ��� ÿ��������32��BYTE��ʾ
	
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}

