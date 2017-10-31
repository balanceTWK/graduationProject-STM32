#include "L298P.h"
#include "sys.h"


void L298P_Init() //IO初始化
{ 
	
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
	
 

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA2,3,4,5
	
  Car_Stop();
}

void Car_Stop(void)
{
	LeftWheel_1=1;
	LeftWheel_2=1;
	
	RightWheel_1=1;
	RightWheel_2=1;
}

void GO_Forward(void)
{
	LeftWheel_1=0;
	LeftWheel_2=1;
	
	RightWheel_1=0;
	RightWheel_2=1;
}

void GO_Back(void)
{	
	LeftWheel_1=1;
	LeftWheel_2=0;
	
	RightWheel_1=1;
	RightWheel_2=0;

}

void R_Circle(void)
{
	LeftWheel_1=1;
	LeftWheel_2=0;
	
	RightWheel_1=0;
	RightWheel_2=1;	
}


void L_Circle(void)
{
	LeftWheel_1=0;
	LeftWheel_2=1;
	
	RightWheel_1=1;
	RightWheel_2=0;
}

