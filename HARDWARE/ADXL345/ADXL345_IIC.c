#include "ADXL345_IIC.h"
#include "delay.h"
#include "message.h"

extern CarMessage_typedef CarMessage_My;

void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
	delay_ms(5);
}

void ADXL345_IICAck() //ADXL345 IICӦ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
	
}
/*��������������ʱ�����ܵ����һ�������ֽں󣬱�����ӻ�����һ���������͵��źš�
  ����ź��ǶԴӻ��ġ���Ӧ����ʵ�ֵġ�Ȼ�󣬴ӻ��Ż��ͷ�SDA�ߣ�������������STOP�źš�*/
void ADXL345_IICNAck()//ADXL345 IIC��Ӧ�� 
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(5);
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;	
}

u8 ADXL345_IICRevAck()//ADXL345 IIC����Ӧ��
{
	u8 x;
	IIC_SCL=1;
	delay_us(5);
	SDA_IN();
	x=IIC_SDA;
	IIC_SCL=0;
	delay_us(5);
	return  x;
}

void ADXL345_IICStart()//ADXL345 IIC��ʼ�ź�
{
	SDA_OUT();
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=0;
}

void ADXL345_IICStop() //ADXL345 IICֹͣ�ź�
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=1;
	IIC_SDA=1;
	delay_us(5);
}
void ADXL345_IIC_SendByte(u8 txd) //ADXL345 IIC ����һ���ֽ�
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;
	for(t=0;t<8;t++)
	{
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(5);  
		IIC_SCL=1;
		delay_us(5); 
		IIC_SCL=0;	
		delay_us(5);		
	}
	ADXL345_IICRevAck();
}

u8 ADXL345_IIC_ReadByte()//ADXL345 IIC ��ȡһ���ֽ�
{
	u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(5);
		    IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive|=0x01;   
		delay_us(5); 
   }					 
    return receive;	
}

//���ֽ�д��
void Single_write_ADXL345(u8 reg_address,u8 data)
{
	ADXL345_IICStart();                   //������ʼ�ź�
	ADXL345_IIC_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
	ADXL345_IIC_SendByte(reg_address);        //�ڲ��Ĵ�����ַ
	ADXL345_IIC_SendByte(data);           //�ڲ��Ĵ���д������
	ADXL345_IICStop();                    //����ֹͣ�ź�
}

//���ֽڶ�ȡ
u8 Single_Read_ADXL345(u8 reg_address)
{
	u8 data;
	ADXL345_IICStart();//��ʼ�ź�
	ADXL345_IIC_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
	ADXL345_IIC_SendByte(reg_address);        //�ڲ��Ĵ�����ַ	
	ADXL345_IICStart();
	ADXL345_IIC_SendByte(SlaveAddress+1); //�����豸��ַ+���ź�
	data=ADXL345_IIC_ReadByte();//��ȡ����
	ADXL345_IICNAck();//��Ӧ��
	ADXL345_IICStop();//ֹͣ�ź�
	return data;
}

//��ȡADXL345�ڲ����ٶ�����,��ַ��Χ0x32~0x37
/*ÿ���ֽڱ�����8bit���ȡ����ݴ���ʱ���ȴ����λMSB,
  ÿһ���������ֽں�������һ��Ӧ��λ����һ֡��9��bit����*/
void Multiple_read_ADXL345()
{
	u8 i;
	u8 BUF[8];
	ADXL345_IICStart();//��ʼ�ź�
	ADXL345_IIC_SendByte(SlaveAddress);//�����豸��ַ+д�ź�
	ADXL345_IIC_SendByte(0x32);//���ʹ洢��Ԫ��ַ ��0x32
	ADXL345_IICStart();//��ʼ�ź�
	ADXL345_IIC_SendByte(SlaveAddress+1);//���ź�
	for(i=0;i<6;i++)//������ȡ6����ַ�����ݴ���BUF[]
	{
		BUF[i]=ADXL345_IIC_ReadByte();
		if(i==5) ADXL345_IICNAck();	//��ȡ���һ�����ݺ� ��Ҫ��NACK
		else ADXL345_IICAck(); //ÿ��ȡһ�� ����һ����ӦACK
	}
	ADXL345_IICStop();//ֹͣ�ź�
	CarMessage_My.acc_x=(BUF[1]<<8)+BUF[0]; 
	CarMessage_My.acc_y=(BUF[3]<<8)+BUF[2]; 
	CarMessage_My.acc_z=(BUF[5]<<8)+BUF[4]; 
	delay_ms(5);
}

//��ʼ��ADXL345
void Init_ADXL345()
{
	IIC_Init();
	Single_write_ADXL345(0x31,0x0B);//������Χ����16g,13λģʽ
	Single_write_ADXL345(0x2C,0x08);//�����趨Ϊ12.5 �ο�pdf13ҳ
	Single_write_ADXL345(0x2D,0x08);//ѡ���Դģʽ �ο�pdf24ҳ
	Single_write_ADXL345(0x2E,0x80);//ʹ�� data ready �ж�
	Single_write_ADXL345(0x1E,0x00);//xƫ���� ���ݲ��Դ�����д��  pdf29
	Single_write_ADXL345(0x1F,0x00);//yƫ���� ���ݲ��Դ�����д��  pdf29
	Single_write_ADXL345(0x20,0x05);//zƫ���� ���ݲ��Դ�����д��  pdf29
	
}






























