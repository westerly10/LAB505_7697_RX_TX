#include "ctiic.h"
#include "delay.h"	 
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

//����I2C�t�ת�����
void CT_Delay(void)
{
	delay_us(2);
} 
//�q�eĲ�N���IIC���f��l��
void CT_IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOF, ENABLE);//�ϯ�GPIOB,F����
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0�]�m��������X
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PF11�]�m������X
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��l��	

}
//����IIC�_�l�H��
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda�u��X
	CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
	delay_us(30);
 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL=0;//�X��I2C�`�u�A�ǳƵo�e�α����ƾ� 
}	  
//����IIC����H��
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();//sda�u��X
	CT_IIC_SCL=1;
	delay_us(30);
	CT_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	CT_Delay();
	CT_IIC_SDA=1;//�o�eI2C�`�u�����H��  
}
//���������H�����
//��^�ȡG1�A������������
//        0�A�����������\
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA�]�m����J  
	CT_IIC_SDA=1;	   
	CT_IIC_SCL=1;
	CT_Delay();
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		CT_Delay();
	}
	CT_IIC_SCL=0;//������X0 	   
	return 0;  
} 
//����ACK����
void CT_IIC_Ack(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA=0;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}
//������ACK����		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_Delay();
	CT_IIC_SDA=1;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}					 				     
//IIC�o�e�@�Ӧr�`
//��^�q�����L����
//1�A������
//0�A�L����			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL=0;//�ԧC�����}�l�ƾڶǿ�
	CT_Delay();
	for(t=0;t<8;t++)
    {              
        CT_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	      
		CT_IIC_SCL=1; 
		CT_Delay();
		CT_IIC_SCL=0;	
		CT_Delay();
    }	 
} 	    
//Ū1�Ӧr�`�Aack=1�ɡA�o�eACK�Aack=0�A�o�enACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA�]�m����J
	delay_us(30);
	for(i=0;i<8;i++ )
	{ 
		CT_IIC_SCL=0; 	    	   
		CT_Delay();
		CT_IIC_SCL=1;	 
		receive<<=1;
		if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//�o�enACK
	else CT_IIC_Ack(); //�o�eACK   
 	return receive;
}





























