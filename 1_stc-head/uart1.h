#include <STC12C5A60S2.H>

#ifndef   UART1_H
#define  UART1_H

#define FOSC 11059200L

//AUXR    ����λѰַ
#define BRTR 0x10;
#define BRTx12 0x04;
#define S1BRS 0x01;


unsigned char Rec;
bit busy;

void Init_Uart1(int Baud);
void Uart1_Send_Data(unsigned char dat);
void Uart1_Send_Str(unsigned char* s);

void Init_Uart1(int Baud)
{
	AUXR |= BRTR;
	AUXR  &= ~BRTx12;//12T
	AUXR |= S1BRS;//����1ѡ����������ʷ�����

	SCON = 0x50;//��У��λ
	PCON &= ~0x80;//���ӱ�
	BRT = -(FOSC / 32 / 12 / Baud);
	REN = 1;
	ES = 1;
	EA = 1;
}
void Uart1_Send_Data(unsigned char dat)
{
	while (busy);//�ȴ�ǰ������ݷ��ͽ���
	busy = 1;
	SBUF = dat;
}
void Uart1_Send_Str(unsigned char* s)
{
	while (*s)
	{
		Uart1_Send_Data(*s++);
	}
}

void Uart1() interrupt 4
{
	if (RI)
	{
		RI = 0;
		Rec = SBUF;
	}
	if (TI)
	{
		TI = 0;
		busy = 0;
	}
}

#endif // UART1_H


