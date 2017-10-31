#include "BEEP.h"

void BEEP_Init() //IO初始化
{ 
	
 	GPIO_InitTypeDef GPIO_InitStructure; 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0
	
	BEEP=0;

}