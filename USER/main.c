#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "ADXL345_IIC.h"
#include "led.h"
#include "OLED_I2C.h"
#include "key.h"
#include "L298P.h"
#include "BEEP.h"
#include "message.h"
#include "MQ-135.h"
#include "24l01.h"
#include "dht11.h"
#include "task.h"
#include "time.h"
//#include "DHT11.h"
/************************************************
 STM32F103ZET6����
 ���ߣ���ΰ��
************************************************/
volatile u8 usart_key=0;
extern u8 usart_ctrl;
CarMessage_typedef CarMessage_My;
CarMessage_typedef CarMessage_1;
volatile u8 nrf_buf[14];
u8 key,mode;
void all_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//Input_GPIO_Config();
  uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	printf("\r\nLoading...... ");	
	
  delay_init();
	I2C_Configuration();
	printf("\r\nI2C_Configuration is ok .");
	OLED_Init();	
	printf("\r\nOLED_Init is ok .");
	printf("\r\nDelay_init is ok .");	
	LED_Init();
	printf("\r\nLED_Init is ok .");
	Init_ADXL345();
	printf("\r\nADXL345_Init is ok .");
	
	KEY_Init();//������ʼ��
	printf("\r\nKEY_Init is ok .");
	BEEP_Init();
	printf("\r\nBEEP_Init is ok .");
	L298P_Init();//������Ƴ�ʼ��
	printf("\r\nL298P_Init is ok .");
	
	MQ135_Init();//�����������ģ���ʼ��
	printf("\r\nMQ135_Init is ok .");
	
	NRF24L01_Init();//����ģ���ʼ��
	printf("\r\nNRF24L01_Init is ok .");
		
	OLED_Fill(0xFF);//ȫ������
	BEEP=1;
	delay_ms(25);
	BEEP=0;
	OLED_Fill(0x00);//ȫ����	
	delay_ms(10);
	BEEP=1;
	delay_ms(25);
	BEEP=0;
	
	
	while(NRF24L01_Check())
	{
		printf("\r\nNRF24L01_Checking!!!!!");
		delay_ms(200);
	}
	
	TIM2_Int_Init(49,7199);//=====10MS��һ���жϷ��������жϷ�������TIMER.c ���ʱ�䣨��λΪ us��= ((arr+1)*(psc+1))/72��
}

//void task_select()
//{
//	while(1)
//	{
//		if(usart_key==0xFF)
//		{
//			switch(usart_ctrl)
//			{
//				case 0:
//					L_Circle();
//					break;
//				
//				case 1:
//					GO_Back();
//					break;
//				
//				case 2:
//					R_Circle();
//					break;
//				
//				case 3:
//					GO_Forward();
//					break;
//				
//				case 4:
//					Car_Stop();
//					break;
//				
//				default:
//					break;
//			}
//			if(usart_ctrl==5)
//			{
//				break;
//			}
//			
//		}
//	}

//}

int main(void)
{			
  all_init();
	while(1)
	{
		printf("\r\nRuning...... ");			
		nrf_task();		
	}
}
	

 
 
 
