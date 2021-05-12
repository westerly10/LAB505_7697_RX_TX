#include "ft5206.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "delay.h" 
#include "string.h" 
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
 
 
//�VFT5206�g�J�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:�g�ƾڪ���
//��^��:0,���\;1,����.
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//�o�e�g�R�O 	 
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
//�qFT5206Ū�X�@���ƾ�
//reg:�_�l�H�s���a�}
//buf:�ƾڽw�w�s��
//len:Ū�ƾڪ���			  
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//�o�e�g�R�O 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//�o�e�C8��a�}
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//�o�eŪ�R�O		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //�o�ƾ�	  
	} 
    CT_IIC_Stop();//���ͤ@�Ӱ������     
} 
//��l��FT5206Ĳ�N��
//��^��:0,��l�Ʀ��\;1,��l�ƥ��� 
u8 FT5206_Init(void)
{
	u8 temp[2]; 
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
	FT_RST=0;				//�_��
	delay_ms(20);
 	FT_RST=1;				//����_��		    
	delay_ms(50);  	
	temp[0]=0;
	FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	//�i�J���`�ާ@�Ҧ� 
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		//�d�߼Ҧ� 
	temp[0]=22;								//Ĳ�N���ĭȡA22�A�V�p�V�F��	
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);	//�]�mĲ�N���ĭ�
	temp[0]=12;								//�E���g���A����p��12�A�̤j14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	//Ū���������A�ѦҭȡG0x3003
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if(temp[0]==0X30&&temp[1]==0X03)//����:0X3003
	{
		printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
		return 0;
	} 
	return 1;
}
const u16 FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
//����Ĳ�N��(�ĥάd�ߤ覡)
//mode:0,���`����.
//��^��:��eĲ�̪��A.
//0,Ĳ�̵LĲ�N;1,Ĳ�̦�Ĳ�N
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	static u8 t=0;//����d�߶��j,�q�ӭ��CCPU���βv   
	t++;
	if((t%10)==0||t<10)//�Ŷ���,�C�i�J10��CTP_Scan��Ƥ~�˴�1��,�q�Ӹ`��CPU�ϥβv
	{
		FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//Ū��Ĳ�N�I�����A  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//�N�I���Ӽ��ഫ��1�����,�ǰttp_dev.sta�w�q 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//Ĳ�N����?
				{
					FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//Ū��XY���Э� 
					if(tp_dev.touchtype&0X01)//���
					{
						tp_dev.y[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
						tp_dev.x[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
					}else
					{
						tp_dev.x[i]=480-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
						tp_dev.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
					}  
					if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//�����Ocontact�ƥ�A�~�{������
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
 



























