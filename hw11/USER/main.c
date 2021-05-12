#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 

//��۽d��28
void led_mov(u8 choice);
void color_set(u8 x);
u8 pointI = 0;
u8 led_run = 0;
//�M�ū̹��æb�k�W�����"RST"
void Load_Drow_Dialog(void)
{
	u8 ci = 0;
	u8 xi = 0;
	LCD_Clear(WHITE);//�M��   
 	POINT_COLOR=BLUE;//�]�m�r�鬰�Ŧ� 
	//LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ܲM�̰ϰ�
	//***********************************************************************************************
	      //�e���C��
	
	for(ci = 0; ci<2; ci++){//verticle
		POINT_COLOR=CYAN; //btn block 
		LCD_DrawLine(0, 50+ci, 240, 50+ci); 
		LCD_DrawLine(0, 100+ci, 240, 100+ci); 
		LCD_DrawLine(0, 150+ci, 240, 150+ci);
		POINT_COLOR=GREEN;
		LCD_DrawLine(0, 150+ci+2, 240, 150+ci+2); //led block
		LCD_DrawLine(0, 180+ci, 240, 180+ci);
	}
	POINT_COLOR=CYAN; 
	for(ci = 0; ci<2; ci++){ //horizontal
		LCD_DrawLine(80-1+ci, 0, 80-1+ci, 150);
		LCD_DrawLine(160-1+ci, 0, 160-1+ci, 150);
	} 
	
	POINT_COLOR=GREEN;//POINT_COLOR=BRRED;      //�e���C��
	for(ci = 10; ci<15; ci++) LCD_Draw_Circle(40, 25 ,ci); //1
	for(ci = 10; ci<15; ci++) LCD_DrawRectangle(120-ci, 25-ci, 120+ci, 25+ci); //2 
	LCD_ShowString(185,20,60,16,16,"CLEAR"); //3
	
	POINT_COLOR=MAGENTA;      //�e���C��
	LCD_ShowString(10,65,60,24,24,"Large");	 //4
	LCD_ShowString(185,70,60,12,12,"Small");  //6
	color_set(0);
	LCD_ShowString(100,70,60,16,16,"Color");  //5

	LCD_ShowString(100,115,60,16,16,"Pause");  //8
	POINT_COLOR=GREEN;
	LCD_ShowString(25,120,60,16,16,"<<<");	 //7
	LCD_ShowString(190,120,60,16,16,">>>");  //9
	
	//POINT_COLOR=CYAN; 
	POINT_COLOR=BRRED;
	for(ci = 0; ci<=9; ci++){ // 10 led
		//for(xi = 0; xi<3; xi++){
				LCD_Draw_Circle(ci*24 +12, 167 , 3);
				LCD_Draw_Circle(ci*24 +12, 167 , 4);
		//}
	}
	POINT_COLOR=RED;
	if(led_run == 1)
		for(xi = 0; xi<3; xi++) 
			LCD_Draw_Circle(pointI*24 +12, 167 , xi);
	
	//LCD_Draw_Circle(120 , 270, 30);
	//************************************************************************************************
}
////////////////////////////////////////////////////////////////////////////////
//�q�eĲ�N�̱M������
//�e�����u
//x0,y0:����
//len:�u����
//color:�C��
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//�e��߶�
//x0,y0:����
//r:�b�|
//color:�C��
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//��ӼƤ��t������� 
//x1,x2�G�ݨ��t�Ȫ���Ӽ�
//��^�ȡG|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//�e�@���ʽu
//(x1,y1),(x2,y2):�u�����_�l����
//size�G�u�����ʲӵ{��
//color�G�u�����C��
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //�p�⧤�мW�q 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //�]�m��B��V 
	else if(delta_x==0)incx=0;//�����u 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//�����u 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //����򥻼W�q���жb 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//�e�u��X 
	{  
		gui_fill_circle(uRow,uCol,size,color);//�e�I 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
 //5��Ĳ���I���C��(�q�eĲ�N�̥�)												 
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//�q��Ĳ�N�̴��ը��
void rtp_test(void)
{	/*
	while(1){
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//Ĳ�N�̳Q���U
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//�M��
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//�e��	  			   
			}
		}else delay_ms(10);	//�S��������U���ɭ� 	    
	}*/
}
//�q�eĲ�N�̴��ը��
void ctp_test(void)
{	
	/*
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//�̫�@�����ƾ� 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<OTT_MAX_TOUCH;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					//lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//�e�u
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//�M��
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0=!LED0;
	}	
	*/
}
	





	u16 circle_r = 30;
	u16 right_path = 0;
	u16 left_path = 0;
	u16 up_path = 0;
	u16 down_path = 0;
	u8 shape_now = 1;
	u8 my_color = 0;

void color_set(u8 x){
	if(x == 1){
		if(my_color < 12){
					my_color ++;
				}
				else my_color = 0;
				Load_Drow_Dialog();
	}
	switch(my_color)
				{
					case 0:POINT_COLOR=BLACK;break;
					case 1:POINT_COLOR=RED;break;
					case 2:POINT_COLOR=BRRED;break;
					case 3:POINT_COLOR=YELLOW;break;
					case 4:POINT_COLOR=GREEN;break;
					case 5:POINT_COLOR=CYAN;break;
					case 6:POINT_COLOR=BLUE;break; 
					case 7:POINT_COLOR=MAGENTA;break;
					case 9:POINT_COLOR=BROWN;break;
					case 10:POINT_COLOR=GRAY;break;
					case 11:POINT_COLOR=LGRAY;break;	
					case 12:POINT_COLOR=WHITE;break;
				}
}
void draw_contorl(u8 choice){
	u8 x = 0;
	x = choice;
	switch(x)
	{
			case 1: //DRAW CIRCLE
				Load_Drow_Dialog();	color_set(0);led_run = 0;
				shape_now = 1;
				circle_r = 30;
 				LCD_Draw_Circle(120 + right_path - left_path, 270+ up_path - down_path, circle_r);
				break;
				
			case 2: //DRAW Rectangle
				Load_Drow_Dialog();	color_set(0);led_run = 0;
				shape_now = 2;
				circle_r = 30;
				LCD_DrawRectangle(120-circle_r  + right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r  + right_path - left_path, 270+circle_r+ up_path - down_path);
				break;
		
			case 8:
				led_mov(x);
				right_path = 0;
				left_path = 0;
				up_path = 0;
				down_path = 0;
				break;
			
			case 4: //LARGE
				if(circle_r < 90) circle_r ++;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);led_run = 0;
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog();	color_set(0);led_run = 0;
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				break;
					
			case 6: //SMALL
				if(circle_r > 5) circle_r --;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);led_run = 0;
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog(); color_set(0);led_run = 0;
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				break;
			case 3: //CLEAR 
				Load_Drow_Dialog();
				break; 
			
			case 7:	//<<
				led_run = 1;
				left_path++;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog();	color_set(0);
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				led_mov(x);
				break;
			case 9: //>>
				led_run = 1;
				right_path ++;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog();	color_set(0);
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				led_mov(x);
				break;
				
			case 5: //COLOR
				color_set(1);led_run = 0;
				if(shape_now == 1){
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				break;
				
			case 10:	//<<
				up_path++; led_run = 0;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);
					LCD_Draw_Circle(120 + right_path - left_path , 270 + up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog();	color_set(0);
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				break;
			
			case 11: //>>
				down_path ++; led_run = 0;
				if(shape_now == 1){
					Load_Drow_Dialog();	color_set(0);
					LCD_Draw_Circle(120 + right_path - left_path , 270+ up_path - down_path, circle_r);
				}
				else if(shape_now == 2){
						Load_Drow_Dialog();	color_set(0);
						LCD_DrawRectangle(120-circle_r+ right_path - left_path, 270-circle_r+ up_path - down_path, 120+circle_r+ right_path - left_path, 270+circle_r+ up_path - down_path);
				}
				break;		
		}
}


void rtp_control(void)
{
	u8 choice;
	u8 choice_from_de0;
	u8 t=0;
	u8 i=0;	  	    
 	//u16 lastpos[5][2];		//�̫�@�����ƾ� 

	while(1)
	{
		tp_dev.scan(0);
		if(tp_dev.sta & TP_PRES_DOWN)
		{
				if(tp_dev.x[t]<lcddev.width && tp_dev.y[t]<lcddev.height)
				{
					if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//�M��
					else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//�e��
					
					if(tp_dev.x[0]>0 && tp_dev.x[0]<80 && tp_dev.y[0] > 0 && tp_dev.y[0]<50)	choice = 1;
					else if(tp_dev.x[0]>80 && tp_dev.x[0]<160 && tp_dev.y[0] > 0 && tp_dev.y[0]<50) choice = 2;
					else if(tp_dev.x[0]>160 && tp_dev.x[0]<240 && tp_dev.y[0] > 0 && tp_dev.y[0]<50) choice = 3;
					
					else if(tp_dev.x[0]>0 && tp_dev.x[0]<80 && tp_dev.y[0] > 50 && tp_dev.y[0]<100) choice = 4;
					else if(tp_dev.x[0]>80 && tp_dev.x[0]<160 && tp_dev.y[0] > 50 && tp_dev.y[0]<100) choice = 5;
					else if(tp_dev.x[0]>160 && tp_dev.x[0]<240 && tp_dev.y[0] > 50 && tp_dev.y[0]<100) choice = 6;
					
					else if(tp_dev.x[0]>0 && tp_dev.x[0]<80 && tp_dev.y[0] > 100 && tp_dev.y[0]<150) choice = 7;
					else if(tp_dev.x[0]>80 && tp_dev.x[0]<160 && tp_dev.y[0] > 100 && tp_dev.y[0]<150) choice = 8;
					else if(tp_dev.x[0]>160 && tp_dev.x[0]<240 && tp_dev.y[0] > 100 && tp_dev.y[0]<150) choice = 9;
					else choice = 0;
					
					draw_contorl(choice);
				}
		}
		// Ū�Jswitch btn��� �ܴ�led �]���O
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) choice_from_de0 = 1;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) choice_from_de0 = 2;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2) == 0) choice_from_de0 = 3;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0) choice_from_de0 = 4;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0) choice_from_de0 = 5;
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0) choice_from_de0 = 6;
		
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0) choice_from_de0 = 7; //left
		else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0) choice_from_de0 = 8;
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0) choice_from_de0 = 9; //E //right
		
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0) choice_from_de0 = 10; //E //up
		else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0) choice_from_de0 = 11; //E //down

		else choice_from_de0 = 0;
		draw_contorl(choice_from_de0);
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
		GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
		
		delay_ms(100);
	}	
}

void led_mov(u8 choice){
		u8 x = 0;
	u8 ci, xi;
		x = choice;
		
		POINT_COLOR=RED;
	switch(x)
	{
			case 7: // left Ū�J10
				GPIO_SetBits(GPIOF,GPIO_Pin_1);
				GPIO_ResetBits(GPIOF,GPIO_Pin_0);
				GPIO_SetBits(GPIOF,GPIO_Pin_0);
				  
				if(pointI == 0) pointI = 9;
				//for(xi = 0; xi<3; xi++) LCD_Draw_Circle(pointI*24 +12, 167 , 3);delay_ms(200);
				pointI--;
			
				break;
			case 8: // all light Ū�J00
				GPIO_ResetBits(GPIOF,GPIO_Pin_0);
				GPIO_ResetBits(GPIOF,GPIO_Pin_1);
				GPIO_SetBits(GPIOF,GPIO_Pin_0);
				GPIO_SetBits(GPIOF,GPIO_Pin_1);

				//for(ci = 0; ci<=9; ci++) for(xi = 0; xi<3; xi++) LCD_Draw_Circle(pointI*24 +12, 167 , 3); delay_ms(200);
				
				break;
			case 9: // right Ū�J01
				GPIO_SetBits(GPIOF,GPIO_Pin_0);
				GPIO_ResetBits(GPIOF,GPIO_Pin_1);
				GPIO_SetBits(GPIOF,GPIO_Pin_1);
			
				if(pointI == 9) pointI = 0;
				//for(xi = 0; xi<3; xi++) LCD_Draw_Circle(pointI*24 +12, 167 , 3);delay_ms(200);
				pointI++;
				
				break;
		}
}

void PBE_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//�ϯ�GPIOE����
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//�ϯ�GPIOB����
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//�ϯ�GPIOF����
	
  //GPIOF9,F10��l�Ƴ]�m
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//DATA����IO�f
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���q��X�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��l��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;//Addr1,Addr0����IO�f
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//���q��X�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��l��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//Addr1,Addr0����IO�f
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���q��X�Ҧ�
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������X
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//�W��
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��l��GPIO
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
	GPIO_SetBits(GPIOF,GPIO_Pin_0 | GPIO_Pin_1);
	//GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1);//GPIOE, set
	}

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�]�m�t�Τ��_�u���Ť���2
	delay_init(168);  //��l�Ʃ��ɨ��
	uart_init(115200);		//��l�Ʀ�f�i�S�v��115200
	
	LED_Init();					//��l��LED 
 	LCD_Init();					//LCD��l�� 
	KEY_Init(); 				//�����l��  
	tp_dev.init();				//Ĳ�N�̪�l��
	
	
	
 	POINT_COLOR=BLUE;
	LCD_ShowString(30,50,200,16,16,"0616");	
	LCD_ShowString(30,70,200,16,16,"Final Project!!!");	
	LCD_ShowString(30,90,200,16,16,"00552145");
	LCD_ShowString(30,110,200,16,16,"");
   	if(tp_dev.touchtype!=0XFF)LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//�q���̤~���
	delay_ms(1500);
 	
	
	Load_Drow_Dialog();	 	
	if(tp_dev.touchtype&0X80){
		ctp_test();//�q�e�̴�//Ĳ���^�X
	}
	else {
		//rtp_test(); 					//�q���̴���
		rtp_control(); // my code
	}
}
