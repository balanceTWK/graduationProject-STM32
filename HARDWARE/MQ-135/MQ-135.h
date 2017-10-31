#ifndef __MQ135_H
#define __MQ135_H	 
#include "sys.h"

void MQ135_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 				    
#endif

