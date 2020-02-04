//----------------------------------------
//直接调Dis_Int函数即可
//----------------------------------------

#include "uart1.h"
//#include "uart2.h"

#ifndef DISPLAY_H
#define DISPLAY_H

//------------------------------------------------------------
// 功能：整数转字符串
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
// 功能：串口打印字符串
//------------------------------------------------------------
void Dis_Int(int value)
{
	char dis[6];

	Int2char(dis, value); //转换数据显示
	Uart1_Send_Str(dis);
	//Uart2_Send_Str(dis);
}
#endif