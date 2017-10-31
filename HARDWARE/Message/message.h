#ifndef __MESSAGE_H
#define __MESSAGE_H	 
#include "sys.h"
#define Myaddress 0x01
typedef struct
{
	u8 address;								//��ַ
	
	short int acc_x;					//x����ٶ�
	short int acc_y;					//y����ٶ�
	short int acc_z;					//z����ٶ�
	
	short int airquality;		  //��������
	
	short int temperature;		//�¶�
	
	short int humidity;				//ʪ��
	
	u8 ctrl;									//�������
	
	short int crc;						//У��
	
}CarMessage_typedef;


#endif

