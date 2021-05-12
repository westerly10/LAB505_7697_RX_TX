#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H
#include "sys.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//�q�eĲ�N��-IIC �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/7
//�����GV1.1
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
//********************************************************************************
//�קﻡ��
//V1.1 20140721
//1,�ק�CT_IIC_Read_Byte���,Ū�ƾڧ��.
//2,�ק�CT_IIC_Wait_Ack���,�H���MDK��-O2�u��.
////////////////////////////////////////////////////////////////////////////////// 	

//IO��V�]�m
#define CT_SDA_IN()  {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11��J�Ҧ�
#define CT_SDA_OUT() {GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11��X�Ҧ�
//IO�ާ@���	 
#define CT_IIC_SCL    PBout(0) 	//SCL
#define CT_IIC_SDA    PFout(11) //SDA	 
#define CT_READ_SDA   PFin(11)  //��JSDA 
 

//IIC�Ҧ��ާ@���
void CT_IIC_Init(void);                	//��l��IIC��IO�f				 
void CT_IIC_Start(void);				//�o�eIIC�}�l�H��
void CT_IIC_Stop(void);	  				//�o�eIIC����H��
void CT_IIC_Send_Byte(u8 txd);			//IIC�o�e�@�Ӧr�`
u8 CT_IIC_Read_Byte(unsigned char ack);	//IICŪ���@�Ӧr�`
u8 CT_IIC_Wait_Ack(void); 				//IIC����ACK�H��
void CT_IIC_Ack(void);					//IIC�o�eACK�H��
void CT_IIC_NAck(void);					//IIC���o�eACK�H��

#endif







