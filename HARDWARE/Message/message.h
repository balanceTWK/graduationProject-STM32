#ifndef __MESSAGE_H
#define __MESSAGE_H	 
#include "sys.h"
#define Myaddress 0x01
typedef struct
{
	u8 address;								//地址
	
	short int acc_x;					//x轴加速度
	short int acc_y;					//y轴加速度
	short int acc_z;					//z轴加速度
	
	short int airquality;		  //空气质量
	
	short int temperature;		//温度
	
	short int humidity;				//湿度
	
	u8 ctrl;									//电机控制
	
	short int crc;						//校验
	
}CarMessage_typedef;


#endif

