#ifndef __ADXL345_IIC_H
#define __ADXL345_IIC_H
#include "sys.h"

#define	SlaveAddress   0xA6	

//IO方向设置
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=7<<28;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

void IIC_Init(void);//IIC初始化
void ADXL345_IICAck(void);//IIC应答
void ADXL345_IICNAck(void);//IIC不应答
u8 ADXL345_IICRevAck(void);//IIC接收应答
void ADXL345_IICStart(void);//IIC开始信号
void ADXL345_IICStop(void);//IIC停止信号
void ADXL345_IIC_SendByte(u8 txd);//发送一个字节
u8 ADXL345_IIC_ReadByte(void);//读取一个字节
void Single_write_ADXL345(u8 reg_address,u8 data);//单字节写入
u8 Single_Read_ADXL345(u8 reg_address);//单字节读取
void Multiple_read_ADXL345(void);//ADXL345数据读取
void Init_ADXL345(void);//初始化ADXL345



#endif




