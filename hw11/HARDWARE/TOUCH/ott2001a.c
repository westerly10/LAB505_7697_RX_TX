#include "ott2001a.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
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


//�VOTT2001A�g�J�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:�g�ƾڪ���
//��^��:0,���\;1,����.
u8 OTT2001A_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //�o�e�g�R�O 	 
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
//�qOTT2001AŪ�X�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:Ū�ƾڪ���			  
void OTT2001A_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(OTT_CMD_WR);   //�o�e�g�R�O 	 
	CT_IIC_Wait_Ack();
 	CT_IIC_Send_Byte(reg>>8);   	//�o�e��8��a�}
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//�o�e�C8��a�}
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(OTT_CMD_RD);   //�o�eŪ�R�O		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //�o�ƾ�	  
	} 
    CT_IIC_Stop();//���ͤ@�Ӱ������    
}
//�ǷP�����}/�����ާ@
//cmd:1,���}�ǷP��;0,�����ǷP��
void OTT2001A_SensorControl(u8 cmd)
{
	u8 regval=0X00;
	if(cmd)regval=0X80;
	OTT2001A_WR_Reg(OTT_CTRL_REG,&regval,1); 
} 
//��l��Ĳ�N��
//��^��:0,��l�Ʀ��\;1,��l�ƥ��� 
u8 OTT2001A_Init(void)
{
 	u8 regval=0; 
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//�ϯ�GPIOB,C����
	
    //GPIOB1��l�Ƴ]�m
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PB1�]�m���W�Կ�J
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��J�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13�]�m��������X
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��l��	
 
	CT_IIC_Init();      	//��l�ƹq�e�̪�I2C�`�u  
	OTT_RST=0;				//�_��
	delay_ms(100);
 	OTT_RST=1;				//����_��		    
	delay_ms(100); 
	OTT2001A_SensorControl(1);	//���}�ǷP�� 
	OTT2001A_RD_Reg(OTT_CTRL_REG,&regval,1);//Ū���ǷP���B��H�s�����ȨӧP�_I2C�q�H�O�_���`
	printf("CTP ID:%x\r\n",regval);
    if(regval==0x80)return 0;
	return 1;
}

const u16 OTT_TPX_TBL[5]={OTT_TP1_REG,OTT_TP2_REG,OTT_TP3_REG,OTT_TP4_REG,OTT_TP5_REG};
//����Ĳ�N��(�ĥάd�ߤ覡)
//mode:0,���`����.
//��^��:��eĲ�̪��A.
//0,Ĳ�̵LĲ�N;1,Ĳ�̦�Ĳ�N
u8 OTT2001A_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	static u8 t=0;//����d�߶��j,�q�ӭ��CCPU���βv   
	t++;
	if((t%10)==0||t<10)//�Ŷ���,�C�i�J10��CTP_Scan��Ƥ~�˴�1��,�q�Ӹ`��CPU�ϥβv
	{
		OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1);//Ū��Ĳ�N�I�����A  
		if(mode&0X1F)
		{
			tp_dev.sta=(mode&0X1F)|TP_PRES_DOWN|TP_CATH_PRES;
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//Ĳ�N����?
				{
					OTT2001A_RD_Reg(OTT_TPX_TBL[i],buf,4);	//Ū��XY���Э�
					if(tp_dev.touchtype&0X01)//���
					{
						tp_dev.y[i]=(((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
						tp_dev.x[i]=800-((((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X);
					}else
					{
						tp_dev.x[i]=(((u16)buf[2]<<8)+buf[3])*OTT_SCAL_Y;
						tp_dev.y[i]=(((u16)buf[0]<<8)+buf[1])*OTT_SCAL_X;
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//Ū�쪺�ƾڳ��O0,�h���������ƾ�
			t=0;		//Ĳ�o�@��,�h�|�ֳ̤s��ʴ�10��,�q�Ӵ����R���v
		}
	}
	if((mode&0X1F)==0)//�LĲ�N�I���U
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
 































 





























