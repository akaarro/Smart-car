#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4����ģ��-�⺯���汾
//�Ա����̣�http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	
void TIM1_Init(u32 arr,u32 psc);
void TIM2_Init(u32 arr,u32 psc);
void TIM5_PWM_Init(u32 arr,u32 psc);
void TIM3_PWM_Init(u32 arr,u32 psc);
void TIM4_PWM_Init(void);
void TIM2_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
#endif
