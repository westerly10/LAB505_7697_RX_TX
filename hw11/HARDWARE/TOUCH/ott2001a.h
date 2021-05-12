#ifndef __OTT2001A_H
#define __OTT2001A_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//4.3�o�q�eĲ�N��-OTT2001A �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/7
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//IO�ާ@���	 
#define OTT_RST    		PCout(13)	//OTT2001A�_��޸}
#define OTT_INT    		PBin(1)		//OTT2001A���_�޸}	
  
//�q�LOTT_SET_REG���O,�i�H�d�ߨ�o�ӫH�� 
//�`�N,�o�̪�X,Y�M�̹������Шt��n�O�Ϫ�.
#define OTT_MAX_X 		2700	 	//TP X��V���̤j��(�ݤ�V)
#define OTT_MAX_Y 		1500    	//TP Y��V���̤j��(���V)

//�Y��]�l
#define OTT_SCAL_X		0.2963		//�̹��� �a����/OTT_MAX_X		
#define OTT_SCAL_Y		0.32		//�̹��� ���/OTT_MAX_Y		
 
//I2CŪ�g�R�O	
#define OTT_CMD_WR 		0XB2     	//�g�R�O
#define OTT_CMD_RD 		0XB3		//Ū�R�O
 
//�H�s���a�}
#define OTT_GSTID_REG 	0X0000   	//OTT2001A��e�˴��쪺Ĳ�N���p
#define OTT_TP1_REG 	0X0100  	//�Ĥ@��Ĳ�N�I�ƾڦa�}
#define OTT_TP2_REG 	0X0500		//�ĤG��Ĳ�N�I�ƾڦa�}
#define OTT_TP3_REG 	0X1000		//�ĤT��Ĳ�N�I�ƾڦa�}
#define OTT_TP4_REG 	0X1400		//�ĥ|��Ĳ�N�I�ƾڦa�}
#define OTT_TP5_REG 	0X1800		//�Ĥ���Ĳ�N�I�ƾڦa�}  
#define OTT_SET_REG   	0X0900   	//����v�]�m�H�s���a�}
#define OTT_CTRL_REG  	0X0D00   	//�ǷP������(�}/��)  

#define OTT_MAX_TOUCH  5    		//�q�e�̤�����I��,�T�w��5�I
 	

u8 OTT2001A_WR_Reg(u16 reg,u8 *buf,u8 len);		//�g�H�s��(��ڵL��)
void OTT2001A_RD_Reg(u16 reg,u8 *buf,u8 len);	//Ū�H�s��
void OTT2001A_SensorControl(u8 cmd);//�ǷP�����}/�����ާ@
u8 OTT2001A_Init(void); 			//4.3�q�eĲ�N�̩l�ƨ��
u8 OTT2001A_Scan(u8 mode);			//�q�eĲ�N�̱��˨��

#endif













