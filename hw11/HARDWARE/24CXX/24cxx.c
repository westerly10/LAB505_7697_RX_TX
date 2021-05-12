#include "24cxx.h" 
#include "delay.h" 				 
//////////////////////////////////////////////////////////////////////////////////	 
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//24CXX �X�ʥN�X	   
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/6
//�����GV1.0
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��l��IIC���f
void AT24CXX_Init(void)
{
	IIC_Init();//IIC��l��
}
//�bAT24CXX���w�a�}Ū�X�@�Ӽƾ�
//ReadAddr:�}�lŪ�ƪ��a�}  
//��^��  :Ū�쪺�ƾ�
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //�o�e�g�R�O
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//�o�e���a�}	    
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //�o�e����a�}0XA0,�g�ƾ� 	   
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //�o�e�C�a�}
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�i�J�����Ҧ�			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//���ͤ@�Ӱ������	    
	return temp;
}
//�bAT24CXX���w�a�}�g�J�@�Ӽƾ�
//WriteAddr  :�g�J�ƾڪ��ت��a�}    
//DataToWrite:�n�g�J���ƾ�
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //�o�e�g�R�O
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//�o�e���a�}	  
	}else IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //�o�e����a�}0XA0,�g�ƾ� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //�o�e�C�a�}
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�o�e�r�`							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//���ͤ@�Ӱ������ 
	delay_ms(10);	 
}
//�bAT24CXX�̭������w�a�}�}�l�g�J���׬�Len���ƾ�
//�Ө�ƥΩ�g�J16bit�Ϊ�32bit���ƾ�.
//WriteAddr  :�}�l�g�J���a�}  
//DataToWrite:�ƾڼƲխ��a�}
//Len        :�n�g�J�ƾڪ�����2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//�bAT24CXX�̭������w�a�}�}�lŪ�X���׬�Len���ƾ�
//�Ө�ƥΩ�Ū�X16bit�Ϊ�32bit���ƾ�.
//ReadAddr   :�}�lŪ�X���a�} 
//��^��     :�ƾ�
//Len        :�nŪ�X�ƾڪ�����2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//�ˬdAT24CXX�O�_���`
//�o�̥ΤF24XX���̫�@�Ӧa�}(255)�Ӧs�x�лx�r.
//�p�G�Ψ�L24C�t�C,�o�Ӧa�}�n�ק�
//��^1:�˴�����
//��^0:�˴����\
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//�קK�C���}�����gAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ư��Ĥ@����l�ƪ����p
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//�bAT24CXX�̭������w�a�}�}�lŪ�X���w�Ӽƪ��ƾ�
//ReadAddr :�}�lŪ�X���a�} ��24c02��0~255
//pBuffer  :�ƾڼƲխ��a�}
//NumToRead:�nŪ�X�ƾڪ��Ӽ�
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//�bAT24CXX�̭������w�a�}�}�l�g�J���w�Ӽƪ��ƾ�
//WriteAddr :�}�l�g�J���a�} ��24c02��0~255
//pBuffer   :�ƾڼƲխ��a�}
//NumToWrite:�n�g�J�ƾڪ��Ӽ�
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








