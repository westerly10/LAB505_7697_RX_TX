#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//�t�ή�����l��	
//�]�A�����]�m/���_�޲z/GPIO�]�m��
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/2
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved
//********************************************************************************
//�קﻡ��
//�L
//////////////////////////////////////////////////////////////////////////////////  


//THUMB���O������J�s���p
//�ĥΦp�U��k��{����J�s���OWFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�����Ҧ����_(���O���]�Afault�MNMI���_)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//�}�ҩҦ����_
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//�]�m�̳��a�}
//addr:�̳��a�}
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
















