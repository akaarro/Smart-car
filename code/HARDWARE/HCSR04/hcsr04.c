#include "sys.h"
#include "hcsr04.h"
#include "delay.h"
void hcsr04_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//trig  A2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//echo  A8
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //PA8����λ��ʱ��1
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);//trig���õ�
}
	
void hcsr04_start()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_2);	// ����������ģ�� trig  ������ģ��5v����
	delay_us(20);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
}