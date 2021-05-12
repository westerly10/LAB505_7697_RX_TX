#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h" 
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

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//�q�{��touchtype=0���ƾ�.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
 	 			    					   
//SPI�g�ƾ�
//�VĲ�N��IC�g�J1byte�ƾ�    
//num:�n�g�J���ƾ�
void TP_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 
		delay_us(1);
		TCLK=1;		//�W�ɪu����	        
	}		 			    
} 		 
//SPIŪ�ƾ� 
//�qĲ�N��ICŪ��adc��
//CMD:���O
//��^��:Ū�쪺�ƾ�	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//���ԧC���� 	 
	TDIN=0; 	//�ԧC�ƾڽu
	TCS=0; 		//�襤Ĳ�N��IC
	TP_Write_Byte(CMD);//�o�e�R�O�r
	delay_us(6);//ADS7846���ഫ�ɶ��̪���6us
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		//��1�Ӯ����A�M��BUSY
	delay_us(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//Ū�X16��ƾ�,�u����12�즳�� 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//�U���u����  	    	   
		delay_us(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//�u����12�즳��.
	TCS=1;		//�������	 
	return(Num);   
}
//Ū���@�ӧ��Э�(x�Ϊ�y)
//�s��Ū��READ_TIMES���ƾ�,��o�ǼƾڤɧǱƦC,
//�M��h���̧C�M�̰�LOST_VAL�Ӽ�,�������� 
//xy:���O�]CMD_RDX/CMD_RDY�^
//��^��:Ū�쪺�ƾ�
#define READ_TIMES 5 	//Ū������
#define LOST_VAL 1	  	//����
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//�Ƨ�
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//�ɧǱƦC
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//Ū��x,y����
//�̤p�Ȥ���֩�100.
//x,y:Ū���쪺���Э�
//��^��:0,����;1,���\�C
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//Ū�ƥ���
	*x=xtemp;
	*y=ytemp;
	return 1;//Ū�Ʀ��\
}
//�s��2��Ū��Ĳ�N��IC,�B�o�⦸�����t����W�L
//ERR_RANGE,��������,�h�{��Ū�ƥ��T,�_�hŪ�ƿ��~.	   
//�Ө�Ư�j�j�����ǽT��
//x,y:Ū���쪺���Э�
//��^��:0,����;1,���\�C
#define ERR_RANGE 50 //�~�t�d�� 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//�e��⦸�ļ˦b+-50��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//�PLCD�������������  
//�e�@��Ĳ�N�I
//�ΨӮշǥΪ�
//x,y:����
//color:�C��
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//��u
	LCD_DrawLine(x,y-12,x,y+13);//�ݽu
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	LCD_Draw_Circle(x,y,6);//�e���߰�
}	  
//�e�@�Ӥj�I(2*2���I)		   
//x,y:����
//color:�C��
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//�����I 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////		  
//Ĳ�N���䱽��
//tp:0,�̹�����;1,���z����(�շǵ��S����X��)
//��^��:��eĲ�̪��A.
//0,Ĳ�̵LĲ�N;1,Ĳ�̦�Ĳ�N
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//��������U
	{
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//Ū�����z����
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//Ū���̹�����
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�N���G�ഫ���̹�����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//���e�S���Q���U
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//������U  
			tp_dev.x[4]=tp_dev.x[0];//�O���Ĥ@�����U�ɪ�����
			tp_dev.y[4]=tp_dev.y[0];  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//���e�O�Q���U��
		{
			tp_dev.sta&=~(1<<7);//�аO�����P�}	
		}else//���e�N�S���Q���U
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//��^��e��Ĳ�̪��A
}	  
//////////////////////////////////////////////////////////////////////////	 
//�O�s�bEEPROM�̭����a�}�϶���},����13�Ӧr�`(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//�O�s�շǰѼ�										    
void TP_Save_Adjdata(void)
{
	s32 temp;			 
	//�O�s�ե����G!		   							  
	temp=tp_dev.xfac*100000000;//�O�sx�ե��]��      
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
	temp=tp_dev.yfac*100000000;//�O�sy�ե��]��    
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
	//�O�sx�����q
    AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);		    
	//�O�sy�����q
	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
	//�O�sĲ������
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);	
	temp=0X0A;//�аO�շǹL�F
	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
}
//�o��O�s�bEEPROM�̭����շǭ�
//��^�ȡG1�A���\����ƾ�
//        0�A������ѡA�n���s�շ�
u8 TP_Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);//Ū���аO�r,�ݬO�_�շǹL�I 		 
	if(tempfac==0X0A)//Ĳ�N�̤w�g�շǹL�F			   
	{    												 
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
		tp_dev.xfac=(float)tempfac/100000000;//�o��x�շǰѼ�
		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
		tp_dev.yfac=(float)tempfac/100000000;//�o��y�շǰѼ�
	    //�o��x�����q
		tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
 	    //�o��y�����q
		tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
 		tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//Ū��Ĳ�������аO
		if(tp_dev.touchtype)//X,Y��V�P�̹��ۤ�
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y��V�P�̹��ۦP
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
}	 
//���ܦr�Ŧ�
u8* const TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  
//���ܮշǵ��G(�U�ӰѼ�)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
	POINT_COLOR=RED;
	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
 	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
 	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
 	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
 	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
 	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");  
 	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");     
	LCD_ShowNum(40+24,160,x0,4,16);		//��ܼƭ�
	LCD_ShowNum(40+24+80,160,y0,4,16);	//��ܼƭ�
	LCD_ShowNum(40+24,180,x1,4,16);		//��ܼƭ�
	LCD_ShowNum(40+24+80,180,y1,4,16);	//��ܼƭ�
	LCD_ShowNum(40+24,200,x2,4,16);		//��ܼƭ�
	LCD_ShowNum(40+24+80,200,y2,4,16);	//��ܼƭ�
	LCD_ShowNum(40+24,220,x3,4,16);		//��ܼƭ�
	LCD_ShowNum(40+24+80,220,y3,4,16);	//��ܼƭ�
 	LCD_ShowNum(40+56,240,fac,3,16); 	//��ܼƭ�,�Ӽƭȥ����b95~105�d�򤧤�.

}
		 
//Ĳ�N�̮շǥN�X
//�o��|�ӮշǰѼ�
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//���нw�s��
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
	u16 outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	LCD_Clear(WHITE);//�M��   
	POINT_COLOR=RED;//���� 
	LCD_Clear(WHITE);//�M�� 	   
	POINT_COLOR=BLACK;
	LCD_ShowString(40,40,160,100,16,(u8*)TP_REMIND_MSG_TBL);//��ܴ��ܫH��
	TP_Drow_Touch_Point(20,20,RED);//�e�I1 
	tp_dev.sta=0;//����Ĳ�o�H�� 
	tp_dev.xfac=0;//xfac�ΨӼаO�O�_�շǹL,�ҥH�շǤ��e�����M��!�H�K���~	 
	while(1)//�p�G�s��10�����S�����U,�h�۰ʰh�X
	{
		tp_dev.scan(1);//���˪��z����
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//������U�F�@��(���ɫ����P�}�F.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);//�аO����w�g�Q�B�z�L�F.
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//�M���I1 
					TP_Drow_Touch_Point(lcddev.width-20,20,RED);	//�e�I2
					break;
				case 2:
 					TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);	//�M���I2
					TP_Drow_Touch_Point(20,lcddev.height-20,RED);	//�e�I3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);			//�M���I3
 					TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);	//�e�I4
					break;
				case 4:	 //�����|���I�w�g�o��
	    		    //����۵�
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�o��1,2���Z��
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�o��3,4���Z��
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//���X��
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�M���I4
   	 					TP_Drow_Touch_Point(20,20,RED);								//�e�I1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ܼƾ�   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�o��1,3���Z��
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�o��2,4���Z��
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���X��
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�M���I4
   	 					TP_Drow_Touch_Point(20,20,RED);								//�e�I1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ܼƾ�   
						continue;
					}//���T�F
								   
					//�﨤�u�۵�
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�o��1,4���Z��
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�o��2,3���Z��
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���X��
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�M���I4
   	 					TP_Drow_Touch_Point(20,20,RED);								//�e�I1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ܼƾ�   
						continue;
					}//���T�F
					//�p�⵲�G
					tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);//�o��xfac		 
					tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�o��xoff
						  
					tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);//�o��yfac
					tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�o��yoff  
					if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2)//Ĳ�̩M�w�]���ۤϤF.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	//�M���I4
   	 					TP_Drow_Touch_Point(20,20,RED);								//�e�I1
						LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
						tp_dev.touchtype=!tp_dev.touchtype;//�ק�Ĳ������.
						if(tp_dev.touchtype)//X,Y��V�P�̹��ۤ�
						{
							CMD_RDX=0X90;
							CMD_RDY=0XD0;	 
						}else				   //X,Y��V�P�̹��ۦP
						{
							CMD_RDX=0XD0;
							CMD_RDY=0X90;	 
						}			    
						continue;
					}		
					POINT_COLOR=BLUE;
					LCD_Clear(WHITE);//�M��
					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");//�ե�����
					delay_ms(1000);
					TP_Save_Adjdata();  
 					LCD_Clear(WHITE);//�M��   
					return;//�ե�����				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}	
//Ĳ�N�̪�l��  		    
//��^��:0,�S���i��շ�
//       1,�i��L�շ�
u8 TP_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;	
	
	if(lcddev.id==0X5510)		//�q�eĲ�N��
	{
		if(GT9147_Init()==0)	//�OGT9147
		{ 
			tp_dev.scan=GT9147_Scan;	//���˨�ƫ��VGT9147Ĳ�N�̱���
		}else
		{
			OTT2001A_Init();
			tp_dev.scan=OTT2001A_Scan;	//���˨�ƫ��VOTT2001AĲ�N�̱���
		}
		tp_dev.touchtype|=0X80;	//�q�e�� 
		tp_dev.touchtype|=lcddev.dir&0X01;//����٬O�ݫ� 
		return 0;
	}else if(lcddev.id==0X1963)
	{
		FT5206_Init();
		tp_dev.scan=FT5206_Scan;		//���˨�ƫ��VGT9147Ĳ�N�̱���		
		tp_dev.touchtype|=0X80;			//�q�e�� 
		tp_dev.touchtype|=lcddev.dir&0X01;//����٬O�ݫ� 
		return 0;
	}else
	{
		
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//�ϯ�GPIOB,C,F����

    //GPIOB1,2��l�Ƴ]�m
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//PB1/PB2 �]�m���W�Կ�J
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��J�Ҧ�
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0�]�m��������X
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	  GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13�]�m��������X
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	  GPIO_Init(GPIOC, &GPIO_InitStructure);//��l��	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PF11�]�m������X
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��X�Ҧ�
	  GPIO_Init(GPIOF, &GPIO_InitStructure);//��l��			
		
   
		TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//�Ĥ@��Ū����l��	 
		AT24CXX_Init();		//��l��24CXX
		if(TP_Get_Adjdata())return 0;//�w�g�շ�
		else			   //���շ�?
		{ 										    
			LCD_Clear(WHITE);//�M��
			TP_Adjust();  	//�̹��շ� 
			TP_Save_Adjdata();	 
		}			
		TP_Get_Adjdata();	
	}
	return 1; 									 
}
