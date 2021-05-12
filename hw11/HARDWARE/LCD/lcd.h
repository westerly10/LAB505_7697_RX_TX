#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//2.4�o/2.8�o/3.5�o/4.3�o/7�o TFT�G���X��	  
//����X��IC�����]�A:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963��		    
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2010/7/4
//�����GV3.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2�קﻡ��
//����FSPFD5408���X��,�t�~��G��ID�������L��HEX�榡.��K�d��LCD�X��IC.
//V1.3
//�[�J�F�ֳtIO�����
//�ק�F�I��������ʡ]�A�Ω�V1.8�ΥH�᪺�}�o�O�����^
//���1.8�������e(���]�A1.8)���G���Ҷ�,�Эק�LCD_Init��ƪ�LCD_LED=1;��LCD_LED=1;
//V1.4
//�ק�FLCD_ShowChar��ơA�ϥεe�I�\��e�r�šC
//�[�J�F��ݫ���ܪ����
//V1.5 20110730
//1,�ק�FB505�G��Ū�C�⦳�~��bug.
//2,�ק�F�ֳtIO�ξ�ݫ̪��]�m�覡.
//V1.6 20111116
//1,�[�J��LGDP4535�G�����X�ʤ��
//V1.7 20120713
//1,�W�[LCD_RD_DATA���
//2,�W�[��ILI9341�����
//3,�W�[ILI9325���W���X�ʥN�X
//4,�W�[LCD_Scan_Dir���(�V���ϥ�)	  
//6,�t�~�ק�F������Ӫ����,�H�A��9341���ާ@
//V1.8 20120905
//1,�[�JLCD���n�ѼƳ]�m���c��lcddev
//2,�[�JLCD_Display_Dir���,����b�u��ݫ̤���
//V1.9 20120911
//1,�s�WRM68042�X�ʡ]ID:6804�^�A���O6804����������ܡI�I��]�G���ܱ��ˤ覡�A
//�ɭP6804���г]�m���ġA�չL�ܦh��k������A�ȮɵL�ѡC
//V2.0 20120924
//�b���w��_�쪺���p�U,ILI9341��IDŪ���|�Q�~Ū��9300,�ק�LCD_Init,�N�L�k�ѧO
//�����p�]Ū��ID��9300/�D�kID�^,�j����w�X��IC��ILI9341�A����9341����l�ơC
//V2.1 20120930
//�ץ�ILI9325Ū�C�⪺bug�C
//V2.2 20121007
//�ץ�LCD_Scan_Dir��bug�C
//V2.3 20130120
//�s�W6804���������
//V2.4 20131120
//1,�s�WNT35310�]ID:5310�^�X�ʾ������
//2,�s�WLCD_Set_Window���,�Ω�]�m���f,��ֳt��R,�������,���O�Ө�Ʀb��̮�,�����6804.
//V2.5 20140211
//1,�s�WNT35510�]ID:5510�^�X�ʾ������
//V2.6 20140504
//1,�s�WASCII 24*24�r�骺���(��h�r��Τ�i�H�ۦ�K�[)  
//2,�קﳡ����ưѼ�,�H���MDK -O2�u��
//3,�w��9341/35310/35510,�g�ɶ��]�m���̧�,�ɥi�઺�����t��
//4,�h���FSSD1289�����,�]��1289��b�O�ӺC�F,Ū�g���n1us...²���_��.���A�XF4�ϥ�
//5,�ץ�68042��C505��IC��Ū�C���ƪ�bug.
//V2.7 20140710
//1,�ץ�LCD_Color_Fill��ƪ��@��bug. 
//2,�ץ�LCD_Scan_Dir��ƪ��@��bug.
//V2.8 20140721
//1,�ѨMMDK�ϥ�-O2�u�Ʈ�LCD_ReadPoint���Ū�I���Ī����D.
//2,�ץ�LCD_Scan_Dir��̮ɳ]�m�����ˤ覡��ܤ�����bug.
//V2.9 20141130
//1,�s�W��SSD1963 LCD�����.
//2,�s�WLCD_SSD_BackLightSet���
//3,����ILI93XX��Rxx�H�s���w�q
//V3.0 20150423
//�ק�SSD1963 LCD�̪��X�ʰѼ�.
//////////////////////////////////////////////////////////////////////////////////	 

//LCD���n�Ѽƶ�
typedef struct  
{										    
	u16 width;			//LCD �e��
	u16 height;			//LCD ����
	u16 id;				//LCD ID
	u8  dir;			//����٬O�ݫ̱���G0�A�ݫ̡F1�A��̡C	
	u16	wramcmd;		//�}�l�ggram���O
	u16  setxcmd;		//�]�mx���Ы��O
	u16  setycmd;		//�]�my���Ы��O 
}_lcd_dev; 	  

//LCD�Ѽ�
extern _lcd_dev lcddev;	//�޲zLCD���n�Ѽ�
//LCD���e���C��M�I����	   
extern u16  POINT_COLOR;//�q�{����    
extern u16  BACK_COLOR; //�I���C��.�q�{���զ�


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�ݤf�w�q---------------- 
#define	LCD_LED PBout(15)  		//LCD�I��    		 PB15 	    
//LCD�a�}���c��
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//�ϥ�NOR/SRAM�� Bank1.sector4,�a�}��HADDR[27,26]=11 A6�@���ƾکR�O�Ϥ��u 
//�`�N�]�m��STM32�����|�k���@����! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//���ˤ�V�w�q
#define L2R_U2D  0 //�q����k,�q�W��U
#define L2R_D2U  1 //�q����k,�q�U��W
#define R2L_U2D  2 //�q�k�쥪,�q�W��U
#define R2L_D2U  3 //�q�k�쥪,�q�U��W

#define U2D_L2R  4 //�q�W��U,�q����k
#define U2D_R2L  5 //�q�W��U,�q�k�쥪
#define D2U_L2R  6 //�q�U��W,�q����k
#define D2U_R2L  7 //�q�U��W,�q�k�쥪	 

#define DFT_SCAN_DIR  L2R_U2D  //�q�{�����ˤ�V

//�e���C��
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //�Ħ�
#define BRRED 			 0XFC07 //�Ĭ���
#define GRAY  			 0X8430 //�Ǧ�
//GUI�C��

#define DARKBLUE      	 0X01CF	//�`�Ŧ�
#define LIGHTBLUE      	 0X7D7C	//�L�Ŧ�  
#define GRAYBLUE       	 0X5458 //���Ŧ�
//�H�W�T�⬰PANEL���C�� 
 
#define LIGHTGREEN     	 0X841F //�L���
//#define LIGHTGRAY        0XEF5B //�L�Ǧ�(PANNEL)
#define LGRAY 			 0XC618 //�L�Ǧ�(PANNEL),����I����

#define LGRAYBLUE        0XA651 //�L���Ŧ�(�����h�C��)
#define LBBLUE           0X2B12 //�L���Ŧ�(��ܱ��ت��Ϧ�)
	    															  
void LCD_Init(void);													   	//��l��
void LCD_DisplayOn(void);													//�}���
void LCD_DisplayOff(void);													//�����
void LCD_Clear(u16 Color);	 												//�M��
void LCD_SetCursor(u16 Xpos, u16 Ypos);										//�]�m����
void LCD_DrawPoint(u16 x,u16 y);											//�e�I
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//�ֳt�e�I
u16  LCD_ReadPoint(u16 x,u16 y); 											//Ū�I 
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 			//�e��
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);							//�e�u
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//�e�x��
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//��R���
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//��R���w�C��
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ܤ@�Ӧr��
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ܤ@�ӼƦr
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��� �Ʀr
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ܤ@�Ӧr�Ŧ�,12/16�r��

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);							//SSD1963 �I������
void LCD_Scan_Dir(u8 dir);									//�]�m�̱��ˤ�V
void LCD_Display_Dir(u8 dir);								//�]�m�̹���ܤ�V
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);	//�]�m���f					   						   																			 
//LCD����v�]�m
#define SSD_HOR_RESOLUTION		800		//LCD��������v
#define SSD_VER_RESOLUTION		480		//LCD��������v
//LCD�X�ʰѼƳ]�m
#define SSD_HOR_PULSE_WIDTH		1		//�����߼e
#define SSD_HOR_BACK_PORCH		46		//�����e�Y
#define SSD_HOR_FRONT_PORCH		210		//������Y

#define SSD_VER_PULSE_WIDTH		1		//�����߼e
#define SSD_VER_BACK_PORCH		23		//�����e�Y
#define SSD_VER_FRONT_PORCH		22		//�����e�Y
//�p�U�X�ӰѼơA�۰ʭp��
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



