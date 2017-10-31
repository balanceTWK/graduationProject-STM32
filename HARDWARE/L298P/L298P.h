#ifndef __L298P_H
#define __L298P_H		
#include "sys.h"

#define LeftWheel_1    PAout(2)// PA2
#define LeftWheel_2    PAout(3)// PA3	

#define RightWheel_2   PAout(4)// PA4
#define RightWheel_1   PAout(5)// PA5

void L298P_Init(void);
void GO_Forward(void);
void GO_Back(void);
void R_Circle(void);
void L_Circle(void);
void Car_Stop(void);


#endif


