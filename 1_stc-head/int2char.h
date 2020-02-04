//----------------------------------------
//ֱ�ӵ�Dis_Int��������
//----------------------------------------

#include "uart1.h"
//#include "uart2.h"

#ifndef DISPLAY_H
#define DISPLAY_H

//------------------------------------------------------------
// ���ܣ�����ת�ַ���
//------------------------------------------------------------
void Int2char(unsigned char * s, int temp_data)
{
	if (temp_data < 0)
	{
		temp_data = -temp_data;
		*s = '-';
	}
	else *s = ' ';

	*++s = temp_data / 10000 + 0x30;

	temp_data = temp_data % 10000;     
	*++s = temp_data / 1000 + 0x30;

	temp_data = temp_data % 1000;     
	*++s = temp_data / 100 + 0x30;

	temp_data = temp_data % 100;    
	*++s = temp_data / 10 + 0x30;

	temp_data = temp_data % 10;
	*++s = temp_data + 0x30;
}
//------------------------------------------------------------
// ���ܣ����ڴ�ӡ�ַ���
//------------------------------------------------------------
void Dis_Int(int value)
{
	char dis[6];

	Int2char(dis, value); //ת��������ʾ
	Uart1_Send_Str(dis);
	//Uart2_Send_Str(dis);
}
#endif