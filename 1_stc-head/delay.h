#ifndef __DELAY_H__
#define __DELAY_H__

void delay_ms(unsigned int ms);
void delay_x10us(unsigned int x);//x个10us
/*--------------------------------------------------------------------------------------
-函数名称:void Delay_ms(uint ms)
----------------------------------------------------------------------------------------
晶振11.0592Mhz
-入口参数:uint ms 延时长度,比如需要延时100ms,则ms=100
-出口参数:无
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
void delay_x10us(unsigned int x)   //误差 -0.053530092593us
{
	unsigned char a, b;
	for (; x > 0; x--) 
	{
		for (b = 7; b > 0; b--)
			for (a = 2; a > 0; a--);
	}
}
#endif
