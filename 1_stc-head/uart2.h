#include <STC12C5A60S2.H>

#ifndef   UART2_H
#define  UART2_H

#define FOSC 11059200L

//AUXR    ����λѰַ
#define BRTR 0x10;
#define S2SMOD 0x08;
#define BRTx12 0x04;
//AUXR1   ����λѰַ
#define S2_P4 0x10;
//S2SCON  ����λѰַ
#define S2RI 0x01
#define S2TI 0x02
#define S2RB8 0x04
#define S2TB8 0x08
//IE2  ����λѰַ
#define ES2 0x01

unsigned char Rec;
bit busy;

void Init_Uart2(int Baud);
void Uart2_Send_Data(unsigned char dat);
void Uart2_Send_Str(unsigned char* s);

void Init_Uart2(int Baud)
{
	AUXR |= BRTR;
	AUXR  &= ~BRTx12;//12T
	AUXR  &= ~S2SMOD;//���ӱ�
	AUXR1 &= ~S2_P4; //UART2������P1��

	BRT = -(FOSC / 32 / 12 / Baud);
	
	S2CON = 0x50;//��У��λ
	S2CON |= S2TI;

	IE2 |= ES2;
	EA = 1;
}
void Uart2_Send_Data(unsigned char dat)
{
	while (busy);//�ȴ�ǰ������ݷ��ͽ���
	busy = 1;
	S2BUF = dat;
}
void Uart2_Send_Str(unsigned char* s)
{
	while (*s)
	{
		Uart2_Send_Data(*s++);
	}
}

void Uart2() interrupt 8
{
	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;
		Rec = S2BUF;
	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;
		busy = 0;
	}
}

#endif // UART2_H


