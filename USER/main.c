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
 STM32F103ZET6工程
 作者：唐伟康
************************************************/
volatile u8 usart_key=0;
extern u8 usart_ctrl;
CarMessage_typedef CarMessage_My;
CarMessage_typedef CarMessage_1;
volatile u8 nrf_buf[14];
u8 key,mode;
void all_init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//Input_GPIO_Config();
  uart_init(9600);	 //串口初始化为9600
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
	
	KEY_Init();//按键初始化
	printf("\r\nKEY_Init is ok .");
	BEEP_Init();
	printf("\r\nBEEP_Init is ok .");
	L298P_Init();//电机控制初始化
	printf("\r\nL298P_Init is ok .");
	
	MQ135_Init();//空气质量检测模块初始化
	printf("\r\nMQ135_Init is ok .");
	
	NRF24L01_Init();//无线模块初始化
	printf("\r\nNRF24L01_Init is ok .");
		
	OLED_Fill(0xFF);//全屏点亮
	BEEP=1;
	delay_ms(25);
	BEEP=0;
	OLED_Fill(0x00);//全屏灭	
	delay_ms(10);
	BEEP=1;
	delay_ms(25);
	BEEP=0;
	
	
	while(NRF24L01_Check())
	{
		printf("\r\nNRF24L01_Checking!!!!!");
		delay_ms(200);
	}
	
	TIM2_Int_Init(49,7199);//=====10MS进一次中断服务函数，中断服务函数在TIMER.c 溢出时间（单位为 us）= ((arr+1)*(psc+1))/72；
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
	

 
 
 
