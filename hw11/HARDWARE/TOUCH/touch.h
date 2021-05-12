#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"
#include "ott2001a.h"	    
#include "gt9147.h"	    
#include "ft5206.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//Ĳ�N���X�ʡ]���ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A���^ �N�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/7
//�����GV1.2
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									   
//********************************************************************************
//�קﻡ��
//V1.1 20140721
//�ץ�MDK�b-O2�u�Ʈ�,Ĳ�N�̼ƾڵL�kŪ����bug.�bTP_Write_Byte��ƲK�[�@�ө���,�ѨM���D.
//V1.2 20141130 
//�q�eĲ�N�̼W�[FT5206�����
//////////////////////////////////////////////////////////////////////////////////


#define TP_PRES_DOWN 0x80  //Ĳ�̳Q���U	  
#define TP_CATH_PRES 0x40  //��������U�F 
#define CT_MAX_TOUCH  5    //�q�e�̤�����I��,�T�w��5�I

//Ĳ�N�̱��
typedef struct
{
	u8 (*init)(void);			//��l��Ĳ�N�̱��
	u8 (*scan)(u8);				//����Ĳ�N��.0,�̹�����;1,���z����;	 
	void (*adjust)(void);		//Ĳ�N�̮շ� 
	u16 x[CT_MAX_TOUCH]; 		//��e����
	u16 y[CT_MAX_TOUCH];		//�q�e�̦��̦h5�է���,�q���̫h��x[0],y[0]�N��:�������ˮ�,Ĳ�̪�����,��
								//x[4],y[4]�s�x�Ĥ@�����U�ɪ�����. 
	u8  sta;					//�������A 
								//b7:���U1/�P�}0; 
	                            //b6:0,�S��������U;1,��������U. 
								//b5:�O�d
								//b4~b0:�q�eĲ�N�̫��U���I��(0,��ܥ����U,1��ܫ��U)
/////////////////////Ĳ�N�̮շǰѼ�(�q�e�̤��ݭn�շ�)//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//�s�W���Ѽ�,��Ĳ�N�̪����k�W�U�����A�ˮɻݭn�Ψ�.
//b0:0,�ݫ�(�A�X���k��X����,�W�U��Y���Ъ�TP)
//   1,���(�A�X���k��Y����,�W�U��X���Ъ�TP) 
//b1~6:�O�d.
//b7:0,�q����
//   1,�q�e�� 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//Ĳ�̱���btouch.c�̭��w�q

//�q���̪���s���޸}	   
#define PEN  		PBin(1)  	//T_PEN
#define DOUT 		PBin(2)   	//T_MISO
#define TDIN 		PFout(11)  	//T_MOSI
#define TCLK 		PBout(0)  	//T_SCK
#define TCS  		PCout(13)  	//T_CS  
   
//�q���̨��
void TP_Write_Byte(u8 num);						//�V�������g�J�@�Ӽƾ�
u16 TP_Read_AD(u8 CMD);							//Ū��AD�ഫ��
u16 TP_Read_XOY(u8 xy);							//�a�o�i������Ū��(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//����VŪ��(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//�a�[�j�o�i������V����Ū��
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//�e�@�ӧ��Юշ��I
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//�e�@�Ӥj�I
void TP_Save_Adjdata(void);						//�O�s�շǰѼ�
u8 TP_Get_Adjdata(void);						//Ū���շǰѼ�
void TP_Adjust(void);							//Ĳ�N�̮շ�
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//��ܮշǫH��
//�q����/�q�e�� �@�Ψ��
u8 TP_Scan(u8 tp);								//����
u8 TP_Init(void);								//��l��
 
#endif

















