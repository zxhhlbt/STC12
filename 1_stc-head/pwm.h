#include <STC12C5A60S2.H>

#ifndef PWM_H
#define PWM_H

void Init_PWM();
void Set_Pwm(int,int);

void Init_PWM()
{
	CCON = 0x00;
	CMOD = 0X02;        //  ��������Դ SYSclk/2

	CL = 0;
	CH = 0;
//PWM0���	
	PCA_PWM0 = 0X00;    //  ���9λ�Ƚ������������ó�1��Ҳ�������ó�0

	CCAP0L = 0xff;      //  ��ʼ���Ƚ����еĳ�ֵ
	CCAP0H = 0Xff;      //  ��ʼ���Ƚ�����ֵ��װ 	
	CCAPM0 = 0X42;      //  �����Ƚ��������������������ź�
//PWM1���	
	PCA_PWM1 = 0X00;    //  ���9λ�Ƚ������������ó�1��Ҳ�������ó�0	

	CCAP1L = 0xff;      //  ��ʼ���Ƚ����еĳ�ֵ
	CCAP1H = 0Xff;      //  ��ʼ���Ƚ�����ֵ��װ	
	CCAPM1 = 0X42;      //  �����Ƚ��������������������ź�

	CR = 1;
}

void Set_Pwm(int pwm0, int pwm1)
{
	CCAP0H = pwm0;
	CCAP1H = pwm1;
}

#endif