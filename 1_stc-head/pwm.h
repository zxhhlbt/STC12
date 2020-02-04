#include <STC12C5A60S2.H>

#ifndef PWM_H
#define PWM_H

void Init_PWM();
void Set_Pwm(int,int);

void Init_PWM()
{
	CCON = 0x00;
	CMOD = 0X02;        //  设置脉冲源 SYSclk/2

	CL = 0;
	CH = 0;
//PWM0输出	
	PCA_PWM0 = 0X00;    //  组成9位比较器，可以设置成1，也可以设置成0

	CCAP0L = 0xff;      //  初始化比较器中的初值
	CCAP0H = 0Xff;      //  初始化比较器初值重装 	
	CCAPM0 = 0X42;      //  开启比较器，允许输出脉宽调制信号
//PWM1输出	
	PCA_PWM1 = 0X00;    //  组成9位比较器，可以设置成1，也可以设置成0	

	CCAP1L = 0xff;      //  初始化比较器中的初值
	CCAP1H = 0Xff;      //  初始化比较器初值重装	
	CCAPM1 = 0X42;      //  开启比较器，允许输出脉宽调制信号

	CR = 1;
}

void Set_Pwm(int pwm0, int pwm1)
{
	CCAP0H = pwm0;
	CCAP1H = pwm1;
}

#endif