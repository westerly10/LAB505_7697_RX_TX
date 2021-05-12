#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//Mini STM32�}�o�O
//��f1��l��		   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.csom
//�ק���:2011/6/14
//�����GV1.4
//���v�Ҧ��A�s�����s�C
//Copyright(C) ���I��l 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�קﻡ�� 
//����A�����P�W�v�U����f�i�S�v�]�m.
//�[�J�F��printf�����
//�W�[�F��f�����R�O�\��.
//�ץ��Fprintf�Ĥ@�Ӧr�ťᥢ��bug
//V1.4�קﻡ��
//1,�ק��f��l��IO��bug
//2,�ק�FUSART_RX_STA,�ϱo��f�̤j�����r�`�Ƭ�2��14����
//3,�W�[�FUSART_REC_LEN,�Ω�w�q��f�̤j���\�������r�`��(���j��2��14����)
//4,�ק�FEN_USART1_RX���ϯ�覡
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�w�q�̤j�����r�`�� 200
#define EN_USART1_RX 			1		//�ϯ�]1�^/�T��]0�^��f1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //�����w��,�̤jUSART_REC_LEN�Ӧr�`.���r�`������� 
extern u16 USART_RX_STA;         		//�������A�аO	
//�p�G�Q��f���_�����A�Ф��n�����H�U���w�q
void uart_init(u32 bound);
#endif


