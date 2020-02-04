#include <STC12C5A60S2.H>

#ifndef HCSR04_H
#define HCSR04_H

	#ifndef FOSC
	#define FOSC 11059200L
	#endif

sbit Trig = P2 ^ 0;
sbit Echo = P2 ^ 1;

void delay12us(void)  
{
	unsigned char a, b;
	for (b = 1; b > 0; b--)
		for (a = 2; a > 0; a--);
}

void Init_Timer1(void)
{
	AUXR&=0Xbf;//12T MODE
	TH1 = 1;
	TH0 = 0;
	TMOD &= 0x0f;
	TMOD |= 0x10;
	EA = 1;
	TR1 = 0;
	ET1 = 0;//no产生中断
}

//	//P3^3配置为准双向口 弱上拉模式
//	void Init_P1_7(void)
//	{
//		P1M0 &= 0x7f;
//		P1M1 &= 0x7f;
//	}


unsigned int Get_Distance(void)
{
	unsigned int count;
	unsigned int distance;
	
	Trig = 1;
	delay12us();
	Trig = 0;
	
	while (Echo == 0);
	TR1 = 1;
	while (Echo == 1);
	TR1 = 0;

	count = TH1 * 256 + TL1 - 1;

	TL1 = 1;
	TH1 = 0;

	distance = (unsigned int)count * (12 / FOSC) * 344 / 2 * 1000;  //输出mm

	return distance;

}

#endif