#ifndef __GT9147_H
#define __GT9147_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//4.3�o�q�eĲ�N��-GT9147 �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/7
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//IO�ާ@���	 
#define GT_RST    		PCout(13)	//GT9147�_��޸}
#define GT_INT    		PBin(1)		//GT9147���_�޸}	
   	
 
//I2CŪ�g�R�O	
#define GT_CMD_WR 		0X28     	//�g�R�O
#define GT_CMD_RD 		0X29		//Ū�R�O
  
//GT9147 �����H�s���w�q 
#define GT_CTRL_REG 	0X8040   	//GT9147����H�s��
#define GT_CFGS_REG 	0X8047   	//GT9147�t�m�_�l�a�}�H�s��
#define GT_CHECK_REG 	0X80FF   	//GT9147����M�H�s��
#define GT_PID_REG 		0X8140   	//GT9147���~ID�H�s��

#define GT_GSTID_REG 	0X814E   	//GT9147��e�˴��쪺Ĳ�N���p
#define GT_TP1_REG 		0X8150  	//�Ĥ@��Ĳ�N�I�ƾڦa�}
#define GT_TP2_REG 		0X8158		//�ĤG��Ĳ�N�I�ƾڦa�}
#define GT_TP3_REG 		0X8160		//�ĤT��Ĳ�N�I�ƾڦa�}
#define GT_TP4_REG 		0X8168		//�ĥ|��Ĳ�N�I�ƾڦa�}
#define GT_TP5_REG 		0X8170		//�Ĥ���Ĳ�N�I�ƾڦa�}  
 
 
u8 GT9147_Send_Cfg(u8 mode);
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len); 
u8 GT9147_Init(void);
u8 GT9147_Scan(u8 mode); 
#endif













