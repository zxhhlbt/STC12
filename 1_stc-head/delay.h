#ifndef __DELAY_H__
#define __DELAY_H__

void delay_ms(unsigned int ms);
void delay_x10us(unsigned int x);//x��10us
/*--------------------------------------------------------------------------------------
-��������:void Delay_ms(uint ms)
----------------------------------------------------------------------------------------
����11.0592Mhz
-��ڲ���:uint ms ��ʱ����,������Ҫ��ʱ100ms,��ms=100
-���ڲ���:��
---------------------------------------------------------------------------------------*/
void delay_ms(unsigned int ms)
{
	unsigned char t;
	while (ms--)
	{
		t = 580;
		while (t--);
	}
}
void delay_x10us(unsigned int x)   //��� -0.053530092593us
{
	unsigned char a, b;
	for (; x > 0; x--) 
	{
		for (b = 7; b > 0; b--)
			for (a = 2; a > 0; a--);
	}
}
#endif
