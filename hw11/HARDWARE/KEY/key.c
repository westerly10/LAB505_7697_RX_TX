#include "key.h"
#include "delay.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//�����J�X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/3
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

//�����l�ƨ��
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//�ϯ�GPIOA,GPIOE����
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2�����޸}
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���q��J�Ҧ�
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��l��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP�����޸}PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//�U��
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��l��GPIOA0
 
} 
//����B�z���
//��^�����
//mode:0,������s���;1,����s���;
//0�A�S�����������U
//1�AKEY0���U
//2�AKEY1���U
//3�AKEY2���U 
//4�AWKUP���U WK_UP
//�`�N����Ʀ��T���u����,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//������P�}�лx
	if(mode)key_up=1;  //����s��		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//�h�ݰ� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �L������U
}




















