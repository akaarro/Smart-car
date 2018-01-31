#include "sys.h"
#include "hcsr04.h"
#include "delay.h"
void hcsr04_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//trig  A2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//echo  A8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //PA8复用位定时器1
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//trig设置低
}
	
void hcsr04_start()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);	// 启动超声波模块 trig  超声波模块5v供电
	delay_us(20);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
}
