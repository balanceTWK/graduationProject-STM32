#include "task.h"
#include "key.h"
#include "message.h"
#include "24l01.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "ADXL345_IIC.h"
#include "MQ-135.h"
#include "L298P.h"
#include "OLED_I2C.h"
#include "BEEP.h"
#include "time.h"

extern CarMessage_typedef CarMessage_My;
extern CarMessage_typedef CarMessage_1;
extern volatile u16 Task_Delay[NumOfTask];
extern u8 usart_ctrl;
extern volatile u8 nrf_buf[16];
extern u8 key,mode;
extern volatile u8 usart_key;

volatile u8 usart_buf[18]={0xFE,0x01,0xFE,0x01};

void get_message()
{
	CarMessage_My.address=Myaddress;
	Multiple_read_ADXL345();
													
	CarMessage_My.airquality=Get_Adc_Average(ADC_Channel_1,10);
	
	CarMessage_My.ctrl=KEY_Scan(1);
}

void get_usartctrl()
{
	if(usart_key==0xFF)
	{
		if(usart_ctrl==0x01)L_Circle();
		else if(usart_ctrl==0x02)GO_Back();
		else if(usart_ctrl==0x03)R_Circle();
		else if(usart_ctrl==0x04)GO_Forward();
		else if(usart_ctrl==0x05)Car_Stop();
	}
}


void nrf_task()
{
	u8 t;
	while(NRF24L01_Check())
	{
		printf("\r\nNRF24L01_Checking!!!!!");
		delay_ms(200);
	}
	
		printf("\r\nNRF24L01 is ok !!!!!");

	  printf("\r\nSelect send or receive.");
	
		OLED_Fill(0x00);//全屏灭	
		OLED_ShowStr(18,0,(unsigned char*)"Select NRF mode!",1);				//测试8*16字符	
	  OLED_ShowStr(0,2,(unsigned char*)"Select key0:send             key1:receive.",1);				//测试8*16字符
		while(1)//等待选择发送接收模式
		{
			key=KEY_Scan(0);
			if(key==KEY0_PRES){mode=0;break;}
			else if(key==KEY1_PRES){mode=1;break;}
			if(usart_key==0xFF)
			{
				if(usart_ctrl==0x01)L_Circle();
				else if(usart_ctrl==0x02)GO_Back();
				else if(usart_ctrl==0x03)R_Circle();
				else if(usart_ctrl==0x04)GO_Forward();
				else if(usart_ctrl==0x05)Car_Stop();
				else if(usart_ctrl==0x06){mode=0;break;}
				else if(usart_ctrl==0x07){mode=1;break;}
			}
		}

			if(mode==0)//发送模式
			{
			OLED_Fill(0x00);//全屏灭	
			OLED_ShowStr(2,0,(unsigned char*)"NRF24L01_TX_Mode",1);				//测试8*16字符			
						printf("\r\nSelect NRF24L01_TX_Mode .");
						NRF24L01_TX_Mode(); 
						while(1)
						{	  

							get_message();
							nrf_buf[0]=CarMessage_My.address;
							
							nrf_buf[1]=CarMessage_My.acc_x>>8;//加速度
							nrf_buf[2]=CarMessage_My.acc_x;
							
							nrf_buf[3]=CarMessage_My.acc_y>>8;
							nrf_buf[4]=CarMessage_My.acc_y;
							
							nrf_buf[5]=CarMessage_My.acc_z>>8;
							nrf_buf[6]=CarMessage_My.acc_z;
							
							nrf_buf[7]=CarMessage_My.airquality>>8;
							nrf_buf[8]=CarMessage_My.airquality;
							
							nrf_buf[9]=CarMessage_My.temperature>>8;
							nrf_buf[10]=CarMessage_My.temperature;
							
							nrf_buf[11]=CarMessage_My.humidity>>8;
							nrf_buf[12]=CarMessage_My.humidity;
							
							nrf_buf[13]=CarMessage_My.ctrl;
							
//							nrf_buf[14]=CarMessage_My.crc>>8;
//							nrf_buf[15]=CarMessage_My.crc;
							
							if(usart_key==0xFF)
							{
								usart_key==0x00;
								if(usart_ctrl==0x01)L_Circle();
								else if(usart_ctrl==0x02)GO_Back();
								else if(usart_ctrl==0x03)R_Circle();
								else if(usart_ctrl==0x04)GO_Forward();
								else if(usart_ctrl==0x05)Car_Stop();
								else if(usart_ctrl==0x08){Car_Stop();break;}
								
								else if(usart_ctrl==0x09){nrf_buf[13]=KEY0_PRES;}
								else if(usart_ctrl==0x0A){nrf_buf[13]=KEY1_PRES;}
								else if(usart_ctrl==0x0B){nrf_buf[13]=KEY2_PRES;}
								else if(usart_ctrl==0x0C){nrf_buf[13]=KEY3_PRES;}
								else if(usart_ctrl==0x0D){nrf_buf[13]=KEY4_PRES;}
								
								else if(usart_ctrl==0x0E){nrf_buf[13]=KEY0_PRES;L_Circle();}
								else if(usart_ctrl==0x0F){nrf_buf[13]=KEY1_PRES;GO_Back();}
								else if(usart_ctrl==0x10){nrf_buf[13]=KEY2_PRES;R_Circle();}
								else if(usart_ctrl==0x11){nrf_buf[13]=KEY3_PRES;GO_Forward();}
								else if(usart_ctrl==0x12){nrf_buf[13]=KEY4_PRES;Car_Stop();}
							}							
							
							if(NRF24L01_TxPacket(nrf_buf)==TX_OK)
							{
																								usart_buf[0]=0xFE;
																								usart_buf[1]=0x01;
																								usart_buf[2]=0xFE;
																								usart_buf[3]=0x01;
																								////usart_buf=nrf_buf>>32;
																								for(t=0;t<14;t++)
																								{
																									usart_buf[t+4]=nrf_buf[t];
																									//usart_buf[t+4]=0x64;
																								}
																								for(t=0;t<18;t++)
																								{
																									USART1->DR=usart_buf[t];
																									while((USART1->SR&0X40)==0);//等待发送结束
//																									{
//																										NRF24L01_TxPacket(nrf_buf);
//																									}
																								}
								//printf("\r\nSend OK !!!!!");
								//printf("\r\n%d %d %d %d %d %d %d %d %d ",CarMessage_My.address,CarMessage_My.acc_x,CarMessage_My.acc_y,CarMessage_My.acc_z,CarMessage_My.airquality,CarMessage_My.temperature,CarMessage_My.humidity,CarMessage_My.ctrl,CarMessage_My.crc);							
							}else
							{										   	
								 printf("\r\nSend Fail !!!!!"); 
							}
	//						LED0=!LED0;
	//						LED1=!LED1;
	//						delay_us(50);
							if(CarMessage_My.ctrl==KEY4_PRES)
							{
								break;
							}	

																							if(Task_Delay[0]==0)//并行小任务
																							{
																								Task_Delay[0]=1;

																								
																								usart_buf[0]=0xFE;
																								usart_buf[1]=0x01;
																								usart_buf[2]=0xFE;
																								usart_buf[3]=0x01;
																								////usart_buf=nrf_buf>>32;
																								for(t=0;t<14;t++)
																								{
																									usart_buf[t+4]=nrf_buf[t];
																									//usart_buf[t+4]=0x64;
																								}
																								for(t=0;t<18;t++)
																								{
																									USART1->DR=usart_buf[t];
																									while((USART1->SR&0X40)==0);//等待发送结束
//																									{
//																										NRF24L01_TxPacket(nrf_buf);
//																									}
																								}
																							}
							
//																							if(Task_Delay[0]==0)//并行小任务
//																							{
//																								Task_Delay[0]=1;

//																								get_message();
//																								nrf_buf[0]=CarMessage_My.address;
//																								
//																								nrf_buf[1]=CarMessage_My.acc_x>>8;//加速度
//																								nrf_buf[2]=CarMessage_My.acc_x;
//																								
//																								nrf_buf[3]=CarMessage_My.acc_y>>8;
//																								nrf_buf[4]=CarMessage_My.acc_y;
//																								
//																								nrf_buf[5]=CarMessage_My.acc_z>>8;
//																								nrf_buf[6]=CarMessage_My.acc_z;
//																								
//																								nrf_buf[7]=CarMessage_My.airquality>>8;
//																								nrf_buf[8]=CarMessage_My.airquality;
//																								
//																								nrf_buf[9]=CarMessage_My.temperature>>8;
//																								nrf_buf[10]=CarMessage_My.temperature;
//																								
//																								nrf_buf[11]=CarMessage_My.humidity>>8;
//																								nrf_buf[12]=CarMessage_My.humidity;
//																								
//																								nrf_buf[13]=CarMessage_My.ctrl;
//																								
//																								usart_buf[0]=0xFE;
//																								usart_buf[1]=0x01;
//																								usart_buf[2]=0xFE;
//																								usart_buf[3]=0x01;
//																								////usart_buf=nrf_buf>>32;
//																								for(t=0;t<14;t++)
//																								{
//																									usart_buf[t+4]=nrf_buf[t];
//																									//usart_buf[t+4]=0x64;
//																								}
//																								for(t=0;t<18;t++)
//																								{
//																									USART1->DR=usart_buf[t];
//																									while((USART1->SR&0X40)==0)//等待发送结束
//																									{
//																										NRF24L01_TxPacket(nrf_buf);
//																									}
//																								}
//																							}
																							

						}	
				}else 
				if(mode==1)//接收模式
				{
			OLED_Fill(0x00);//全屏灭	
			OLED_ShowStr(2,0,(unsigned char*)"NRF24L01_RX_Mode",1);				//测试8*16字符		
						printf("\r\nSelect NRF24L01_RX_Mode .");
						NRF24L01_RX_Mode();		  
						while(1)
						{	  		    		    				 
							if(NRF24L01_RxPacket(nrf_buf)==0)//一旦接收到信息,则显示出来.
							{
								CarMessage_1.address=nrf_buf[0];
								CarMessage_1.acc_x=((nrf_buf[1]<<8)+nrf_buf[2]);
								CarMessage_1.acc_y=((nrf_buf[3]<<8)+nrf_buf[4]);
								CarMessage_1.acc_z=((nrf_buf[5]<<8)+nrf_buf[6]);
								CarMessage_1.airquality=((nrf_buf[7]<<8)+nrf_buf[8]);
								CarMessage_1.temperature=((nrf_buf[9]<<8)+nrf_buf[10]);
								CarMessage_1.humidity=((nrf_buf[11]<<8)+nrf_buf[12]);
								CarMessage_1.ctrl=nrf_buf[13];
								CarMessage_1.crc=((nrf_buf[14]<<8)+nrf_buf[15]);
								printf("\r\nReceive OK !!!!!");
								printf("\r\n%d %d %d %d %d %d %d %d %d ",CarMessage_1.address,CarMessage_1.acc_x,CarMessage_1.acc_y,CarMessage_1.acc_z,CarMessage_1.airquality,CarMessage_1.temperature,CarMessage_1.humidity,CarMessage_1.ctrl,CarMessage_1.crc);
								if(CarMessage_1.ctrl==KEY0_PRES)L_Circle();
								else if(CarMessage_1.ctrl==KEY1_PRES)GO_Back();
								else if(CarMessage_1.ctrl==KEY2_PRES)R_Circle();
								else if(CarMessage_1.ctrl==KEY3_PRES)GO_Forward();
								else if(CarMessage_1.ctrl==KEY4_PRES)Car_Stop();
								else if(CarMessage_1.acc_x<=-100)L_Circle();
								else if(CarMessage_1.acc_x>=100)R_Circle();
								else if(CarMessage_1.acc_y<=-100)GO_Back();
								else if(CarMessage_1.acc_y>=100)GO_Forward();
								//else {Car_Stop();}
							}else 
							{
								delay_us(100);						
							}
							if(KEY_Scan(0)==KEY4_PRES)
							{
								Car_Stop();
								break;
							}
							if(usart_key==0xFF)
							{
								if(usart_ctrl==0x01)L_Circle();
								else if(usart_ctrl==0x02)GO_Back();
								else if(usart_ctrl==0x03)R_Circle();
								else if(usart_ctrl==0x04)GO_Forward();
								else if(usart_ctrl==0x05)Car_Stop();
								else if(usart_ctrl==0x08){Car_Stop();break;}
							}
							

		
						}
					}



Car_Stop();
printf("\r\nnrf_task exit.");
mode=3;				
}


void usartCtrl_task()
{
		u32 i=0;
		printf("\r\nusartCtrl_task .");
	
		OLED_Fill(0x00);//全屏灭	
		OLED_ShowStr(18,0,(unsigned char*)"UsartCtrl_task",1);				//测试8*16字符	
		
	while(1)
	{
		if(usart_key==0xFF)
		{
			usart_key=0;
      /*处理串口接收到的指令              */
			
		}
		if(KEY_Scan(0)==KEY4_PRES)
		{
			Car_Stop();
			printf("\r\nusartCtrl_task exit.");			
			break;
		}
		i++;
	}
}

















