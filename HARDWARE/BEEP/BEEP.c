#include "BEEP.h"

void BEEP_Init() //IO��ʼ��
{ 
	
 	GPIO_InitTypeDef GPIO_InitStructure; 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE0
	
	BEEP=0;

}