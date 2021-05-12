#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//24CXX �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/6
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//Mini STM32�}�o�O�ϥΪ��O24c02�A�ҥH�w�qEE_TYPE��AT24C02
#define EE_TYPE AT24C02
					  
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//���w�a�}Ū���@�Ӧr�`
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//���w�a�}�g�J�@�Ӧr�`
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//���w�a�}�}�l�g�J���w���ת��ƾ�
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//���w�a�}�}�lŪ�����w���׼ƾ�
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//�q���w�a�}�}�l�g�J���w���ת��ƾ�
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//�q���w�a�}�}�lŪ�X���w���ת��ƾ�

u8 AT24CXX_Check(void);  //�ˬd����
void AT24CXX_Init(void); //��l��IIC
#endif
















