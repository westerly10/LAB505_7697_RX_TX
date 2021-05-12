#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//�p�G�ϥ�OS,�h�]�A�U�����Y���]�Hucos���ҡ^�Y�i.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//���OS�ɡA�ϥ�	  
#endif
//////////////////////////////////////////////////////////////////////////////////  
//���{�ǥu�ѾǲߨϥΡA���g�@�̳\�i�A���o�Ω�䥦����γ~
//ALIENTEK STM32F407�}�o�O
//�ϥ�SysTick�����q�p�ƼҦ��冀��i��޲z(���OS)
//�]�Adelay_us,delay_ms
//���I��l@ALIENTEK
//�޳N�׾�:www.openedv.com
//�Ыؤ��:2014/5/2
//�����GV1.3
//���v�Ҧ��A�s�����s�C
//Copyright(C) �s�{���P�l�q�l��ަ������q 2014-2024
//All rights reserved
//********************************************************************************
//�קﻡ��
//V1.1 20140803 
//1,delay_us,�K�[�ѼƵ���0�P�_,�p�G�ѼƵ���0,�h�����h�X. 
//2,�ק�ucosii�U,delay_ms���,�[�JOSLockNesting���P�_,�b�i�J���_��,�]�i�H�ǽT����.
//V1.2 20150411  
//�ק�OS����覡,�H������NOS(������UCOSII�MUCOSIII,�z�פW���NOS���i�H���)
//�K�[:delay_osrunning/delay_ostickspersec/delay_osintnesting�T�ӧ��w�q
//�K�[:delay_osschedlock/delay_osschedunlock/delay_ostimedly�T�Ө��
//V1.3 20150521
//�ץ�UCOSIII����ɪ�2��bug�G
//delay_tickspersec�אּ�Gdelay_ostickspersec
//delay_intnesting�אּ�Gdelay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 

static u8  fac_us=0;							//us���ɭ�����			   
static u16 fac_ms=0;							//ms���ɭ�����,�bos�U,�N��C�Ӹ`�窺ms��
	
#if SYSTEM_SUPPORT_OS							//�p�GSYSTEM_SUPPORT_OS�w�q�F,�����n���OS�F(������UCOS).
//��delay_us/delay_ms�ݭn���OS���ɭԻݭn�T�ӻPOS���������w�q�M��ƨӤ��
//�����O3�ӧ��w�q:
//    delay_osrunning:�Ω���OS��e�O�_���b�B��,�H�M�w�O�_�i�H�ϥά������
//delay_ostickspersec:�Ω���OS�]�w�������`��,delay_init�N�ھڳo�ӰѼƨӪ�l��systick
// delay_osintnesting:�Ω���OS���_�O�M�ŧO,�]�����_�̭����i�H�ի�,delay_ms�ϥθӰѼƨӨM�w�p��B��
//�M��O3�Ө��:
//  delay_osschedlock:�Ω���wOS���Ƚի�,�T��ի�
//delay_osschedunlock:�Ω����OS���Ƚի�,���s�}�ҽի�
//    delay_ostimedly:�Ω�OS����,�i�H�ް_���Ƚի�.

//���ҵ{�ȧ@UCOSII�MUCOSIII�����,��LOS,�Цۦ�Ѧҵ۲���
//���UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD�w�q�F,�����n���UCOSII				
#define delay_osrunning		OSRunning			//OS�O�_�B��аO,0,���B��;1,�b�B��
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS�����`��,�Y�C��իצ���
#define delay_osintnesting 	OSIntNesting		//���_�O�M�ŧO,�Y���_�O�M����
#endif

//���UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD�w�q�F,�����n���UCOSIII	
#define delay_osrunning		OSRunning			//OS�O�_�B��аO,0,���B��;1,�b�B��
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS�����`��,�Y�C��իצ���
#define delay_osintnesting 	OSIntNestingCtr		//���_�O�M�ŧO,�Y���_�O�M����
#endif


//us�ũ��ɮ�,�������Ƚի�(����_us�ũ���)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//�ϥ�UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII���覡,�T��իסA����_us����
#else										//�_�hUCOSII
	OSSchedLock();							//UCOSII���覡,�T��իסA����_us����
#endif
}

//us�ũ��ɮ�,��_���Ƚի�
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//�ϥ�UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII���覡,��_�ի�
#else										//�_�hUCOSII
	OSSchedUnlock();						//UCOSII���覡,��_�ի�
#endif
}

//�ե�OS�۱a�����ɨ�Ʃ���
//ticks:���ɪ��`���
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);//UCOSIII���ɱĥζg���Ҧ�
#else
	OSTimeDly(ticks);						//UCOSII����
#endif 
}
 
//systick���_�A�Ȩ��,�ϥ�OS�ɥΨ�
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)					//OS�}�l�]�F,�~���楿�`���ի׳B�z
	{
		OSIntEnter();						//�i�J���_
		OSTimeTick();       				//�ե�ucos�������A�ȵ{��               
		OSIntExit();       	 				//Ĳ�o���Ȥ����n���_
	}
}
#endif
			   
//��l�Ʃ�����
//��ϥ�OS���ɭ�,����Ʒ|��l��OS�������`��
//SYSTICK�������T�w��AHB������1/8
//SYSCLK:�t�ή����W�v
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//�p�G�ݭn���OS.
	u32 reload;
#endif
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//���׬O�_�ϥ�OS,fac_us���ݭn�ϥ�
#if SYSTEM_SUPPORT_OS 						//�p�G�ݭn���OS.
	reload=SYSCLK/8;						//�C�������p�Ʀ��� ��쬰M	   
	reload*=1000000/delay_ostickspersec;	//�ھ�delay_ostickspersec�]�w���X�ɶ�
											//reload��24��H�s��,�̤j��:16777216,�b168M�U,���X0.7989s���k	
	fac_ms=1000/delay_ostickspersec;		//�N��OS�i�H���ɪ��ֳ̤��	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//�}��SYSTICK���_
	SysTick->LOAD=reload; 					//�C1/delay_ostickspersec���_�@��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//�}��SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;				//�DOS�U,�N��C��ms�ݭn��systick������   
#endif
}								    

#if SYSTEM_SUPPORT_OS 						//�p�G�ݭn���OS.
//����nus
//nus:�n���ɪ�us��.	
//nus:0~204522252(�̤j�ȧY2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD����	    	 
	ticks=nus*fac_us; 						//�ݭn���`��� 
	delay_osschedlock();					//����OS�իסA����_us����
	told=SysTick->VAL;        				//��i�J�ɪ��p�ƾ���
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//�o�̪`�N�@�USYSTICK�O�@�ӻ���p�ƾ��N�i�H�F.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//�ɶ��W�L/����n���𪺮ɶ�,�h�h�X.
		}  
	};
	delay_osschedunlock();					//��_OS�ի�											    
}  
//����nms
//nms:�n���ɪ�ms��
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//�p�GOS�w�g�b�]�F,�åB���O�b���_�̭�(���_�̭�������Ƚի�)	    
	{		 
		if(nms>=fac_ms)						//���ɪ��ɶ��j��OS���̤֮ɶ��g�� 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS����
		}
		nms%=fac_ms;						//OS�w�g�L�k���ѳo��p�����ɤF,�ĥδ��q�覡����    
	}
	delay_us((u32)(nms*1000));				//���q�覡����
}
#else  //����ucos��
//����nus
//nus���n���ɪ�us��.	
//�`�N:nus����,���n�j��798915us(�̤j�ȧY2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//�ɶ��[��	  		 
	SysTick->VAL=0x00;        				//�M�ŭp�ƾ�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //�}�l�˼� 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//���ݮɶ���F   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�����p�ƾ�
	SysTick->VAL =0X00;       				//�M�ŭp�ƾ� 
}
//����nms
//�`�Nnms���d��
//SysTick->LOAD��24��H�s��,�ҥH,�̤j���ɬ�:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��쬰Hz,nms��쬰ms
//��168M����U,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//�ɶ��[��(SysTick->LOAD��24bit)
	SysTick->VAL =0x00;           			//�M�ŭp�ƾ�
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //�}�l�˼� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//���ݮɶ���F   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�����p�ƾ�
	SysTick->VAL =0X00;     		  		//�M�ŭp�ƾ�	  	    
} 
//����nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//�o�̥�540,�O�Ҽ{��Y�ǫȤ�i��W�W�ϥ�,
											//��p�W�W��248M���ɭ�,delay_xms�̤j�u�ੵ��541ms���k�F
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif
			 



































