#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����2
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)//��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//��ȡ����4


#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) 

 

#define KEY0_PRES 	0	//KEY0����
#define KEY1_PRES	  1	//KEY1����
#define KEY2_PRES 	2	//KEY2����
#define KEY3_PRES	  3	//KEY3����
#define KEY4_PRES 	4	//KEY4����

#define WKUP_PRES   5	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
