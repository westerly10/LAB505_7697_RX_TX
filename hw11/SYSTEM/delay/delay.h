#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//�ϥ�SysTick�����q�p�ƼҦ��冀��i��޲z(���ucosii)
//�]�Adelay_us,delay_ms
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�ק���:2014/5/2
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved
//********************************************************************************
//�קﻡ��
//�L
////////////////////////////////////////////////////////////////////////////////// 	 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























