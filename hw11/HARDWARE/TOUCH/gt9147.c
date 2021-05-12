#include "gt9147.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
#include "lcd.h" 
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

//GT9147�t�m�Ѽƪ�
//�Ĥ@�Ӧr�`��������(0X60),�����O�ҷs���������j�󵥩�GT9147����
//flash�즳������,�~�|��s�t�m.
const u8 GT9147_CFG_TBL[]=
{ 
	0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF,
};  
//�o�eGT9147�t�m�Ѽ�
//mode:0,�ѼƤ��O�s��flash
//     1,�ѼƫO�s��flash
u8 GT9147_Send_Cfg(u8 mode)
{
	u8 buf[2];
	u8 i=0;
	buf[0]=0;
	buf[1]=mode;	//�O�_�g�J��GT9147 FLASH?  �Y�O�_���q�O�s
	for(i=0;i<sizeof(GT9147_CFG_TBL);i++)buf[0]+=GT9147_CFG_TBL[i];//�p�����M
    buf[0]=(~buf[0])+1;
	GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//�o�e�H�s���t�m
	GT9147_WR_Reg(GT_CHECK_REG,buf,2);//�g�J����M,�M�t�m��s�аO
	return 0;
} 
//�VGT9147�g�J�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:�g�ƾڪ���
//��^��:0,���\;1,����.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   	//�o�e�g�R�O 	 
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte(reg>>8);   	//�o�e��8��a�}
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//�o�e�C8��a�}
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//�o�ƾ�
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//���ͤ@�Ӱ������	    
	return ret; 
}
//�qGT9147Ū�X�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:Ū�ƾڪ���			  
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(GT_CMD_WR);   //�o�e�g�R�O 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//�o�e��8��a�}
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//�o�e�C8��a�}
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(GT_CMD_RD);   //�o�eŪ�R�O		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //�o�ƾ�	  
	} 
    CT_IIC_Stop();//���ͤ@�Ӱ������    
} 
//��l��GT9147Ĳ�N��
//��^��:0,��l�Ʀ��\;1,��l�ƥ��� 
u8 GT9147_Init(void)
{
	u8 temp[5]; 
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//�ϯ�GPIOB,C����

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;//PB1�]�m���W�Կ�J
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��J�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13�]�m��������X
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��l��	
	
	CT_IIC_Init();      	//��l�ƹq�e�̪�I2C�`�u  
	GT_RST=0;				//�_��
	delay_ms(10);
 	GT_RST=1;				//����_��		    
	delay_ms(10); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1�]�m���B�ſ�J
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��	
	
	delay_ms(100);  
	GT9147_RD_Reg(GT_PID_REG,temp,4);//Ū�����~ID
	temp[4]=0;
	printf("CTP ID:%s\r\n",temp);	//���LID
	if(strcmp((char*)temp,"9147")==0)//ID==9147
	{
		temp[0]=0X02;			
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//�n�_��GT9147
		GT9147_RD_Reg(GT_CFGS_REG,temp,1);//Ū��GT_CFGS_REG�H�s��
		if(temp[0]<0X60)//�q�{��������C,�ݭn��sflash�t�m
		{
			printf("Default Ver:%d\r\n",temp[0]);
			GT9147_Send_Cfg(1);//��s�ëO�s�t�m
		}
		delay_ms(10);
		temp[0]=0X00;	 
		GT9147_WR_Reg(GT_CTRL_REG,temp,1);//�����_��   
		return 0;
	} 
	return 1;
}
const u16 GT9147_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
//����Ĳ�N��(�ĥάd�ߤ覡)
//mode:0,���`����.
//��^��:��eĲ�̪��A.
//0,Ĳ�̵LĲ�N;1,Ĳ�̦�Ĳ�N
u8 GT9147_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
 	static u8 t=0;//����d�߶��j,�q�ӭ��CCPU���βv   
	t++;
	if((t%10)==0||t<10)//�Ŷ���,�C�i�J10��CTP_Scan��Ƥ~�˴�1��,�q�Ӹ`��CPU�ϥβv
	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//Ū��Ĳ�N�I�����A  
 		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;
			GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//�M�лx 		
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//�N�I���Ӽ��ഫ��1�����,�ǰttp_dev.sta�w�q 
			tempsta=tp_dev.sta;			//�O�s��e��tp_dev.sta��
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//�O�sĲ�I0���ƾ�
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//Ĳ�N����?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//Ū��XY���Э�
					if(tp_dev.touchtype&0X01)//���
					{
						tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.x[i]=800-(((u16)buf[3]<<8)+buf[2]);
					}else
					{
						tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//�D�k�ƾ�(���жW�X�F)
			{ 
				if((mode&0XF)>1)		//����L�I���ƾ�,�h�_�ĤG��Ĳ�I���ƾڨ�Ĥ@��Ĳ�I.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//Ĳ�o�@��,�h�|�ֳ̤s��ʴ�10��,�q�Ӵ����R���v
				}else					//�D�k�ƾ�,�h���������ƾ�(�٭��Ӫ�)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//��_tp_dev.sta
				}
			}else t=0;					//Ĳ�o�@��,�h�|�ֳ̤s��ʴ�10��,�q�Ӵ����R���v
		}
	}
	if((mode&0X8F)==0X80)//�LĲ�N�I���U
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//���e�O�Q���U��
		{
			tp_dev.sta&=~(1<<7);	//�аO�����P�}
		}else						//���e�N�S���Q���U
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�M���I���ļаO	
		}	 
	} 	
	if(t>240)t=10;//���s�q10�}�l�p��
	return res;
}
 



























