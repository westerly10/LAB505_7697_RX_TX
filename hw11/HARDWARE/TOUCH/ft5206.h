#ifndef __FT5206_H
#define __FT5206_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32�}�o�O
//7�o�q�eĲ�N��-FT5206 �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/11/30
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 


//�P�q�eĲ�N�̳s��������޸}(���]�tIIC�޸}) 
//IO�ާ@���	 
#define FT_RST    				PCout(13)	//FT5206�_��޸}
#define FT_INT    				PBin(1)		//FT5206���_�޸}	

//I2CŪ�g�R�O	
#define FT_CMD_WR 				0X70    	//�g�R�O
#define FT_CMD_RD 				0X71		//Ū�R�O
  
//FT5206 �����H�s���w�q 
#define FT_DEVIDE_MODE 			0x00   		//FT5206�Ҧ�����H�s��
#define FT_REG_NUM_FINGER       0x02		//Ĳ�N���A�H�s��

#define FT_TP1_REG 				0X03	  	//�Ĥ@��Ĳ�N�I�ƾڦa�}
#define FT_TP2_REG 				0X09		//�ĤG��Ĳ�N�I�ƾڦa�}
#define FT_TP3_REG 				0X0F		//�ĤT��Ĳ�N�I�ƾڦa�}
#define FT_TP4_REG 				0X15		//�ĥ|��Ĳ�N�I�ƾڦa�}
#define FT_TP5_REG 				0X1B		//�Ĥ���Ĳ�N�I�ƾڦa�}  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//����		
#define FT_ID_G_MODE 			0xA4   		//FT5206���_�Ҧ�����H�s��
#define FT_ID_G_THGROUP			0x80   		//Ĳ�N���ĭȳ]�m�H�s��
#define FT_ID_G_PERIODACTIVE	0x88   		//�E�����A�g���]�m�H�s��


u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT5206_Init(void);
u8 FT5206_Scan(u8 mode);

#endif

















