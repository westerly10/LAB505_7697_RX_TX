#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//IIC �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/6
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��V�]�m
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9��J�Ҧ�
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9��X�Ҧ�
//IO�ާ@���	 
#define IIC_SCL    PBout(8) //SCL
#define IIC_SDA    PBout(9) //SDA	 
#define READ_SDA   PBin(9)  //��JSDA 

//IIC�Ҧ��ާ@���
void IIC_Init(void);                //��l��IIC��IO�f				 
void IIC_Start(void);				//�o�eIIC�}�l�H��
void IIC_Stop(void);	  			//�o�eIIC����H��
void IIC_Send_Byte(u8 txd);			//IIC�o�e�@�Ӧr�`
u8 IIC_Read_Byte(unsigned char ack);//IICŪ���@�Ӧr�`
u8 IIC_Wait_Ack(void); 				//IIC����ACK�H��
void IIC_Ack(void);					//IIC�o�eACK�H��
void IIC_NAck(void);				//IIC���o�eACK�H��

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















