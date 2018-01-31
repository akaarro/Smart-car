#include "pwm.h"
#include "led.h"
#include "usart.h"
int speed_count; 

u8  TIM1CH1_CAPTURE_STA=0;
u32 TIM1CH1_CAPTURE_VAL;
u32 s;
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F4工程模板-库函数版本
//淘宝店铺：http://mcudev.taobao.com								  
////////////////////////////////////////////////////////////////////////////////// 	 
void TIM1_Init(u32 arr,u32 psc)
{
	TIM_ICInitTypeDef  TIM1_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
 
		//初始化TIM1输入捕获参数
	TIM1_ICInitStructure.TIM_Channel     = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM1_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
		
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM1,ENABLE ); 	//使能定时器1

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

}
void TIM2_Init(u32 arr,u32 psc)
{		 					 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2时钟使能    

	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//初始化定时器2
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
 
										  
}  
//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //GPIOA6复用为定时器3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOFA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
 
										  
}  
void TIM4_PWM_Init(void)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM4时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_TIM4); //GPIOE0复用为定时器4
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化PE0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=0x00;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=0xffff;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器4
	//TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
	TIM_SetCounter(TIM4,0); 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
 
										  
} 
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0复用为定时器5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA1复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;           //GPIOFA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA0|1
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 Channel 1|2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 ch1
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 ch2
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR2上的预装载寄存器
	
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5								  
}  
void TIM2_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		speed_count=TIM4->CNT;
		TIM4->CNT=0;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) //????
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //???????
		//if(TIM1CH1_CAPTURE_STA<10)
			TIM1CH1_CAPTURE_STA++;
	}
}

void TIM1_CC_IRQHandler(void)
{ 
	static u32 	TIM1CH1_CAPTURE_STA1;		    				
	static u16	TIM1CH1_CAPTURE_VAL1;
	static u32 	TIM1CH1_CAPTURE_STA2;		    				
	static u16	TIM1CH1_CAPTURE_VAL2;
	long temp;
if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
{
		static u8 TimCapFlag1=1;
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		if(TimCapFlag1==1)
		{
			TIM1CH1_CAPTURE_STA1=TIM1CH1_CAPTURE_STA;
			TIM1CH1_CAPTURE_VAL1=TIM_GetCapture1(TIM1);
//			PR("\r\nsta1=%10d;val1=%10d;",TIM1CH1_CAPTURE_STA1,TIM1CH1_CAPTURE_VAL1);
			TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);
			TimCapFlag1=0;
		}
		else if(TimCapFlag1==0)
		{
			TIM1CH1_CAPTURE_STA2=TIM1CH1_CAPTURE_STA;
			TIM1CH1_CAPTURE_VAL2=TIM_GetCapture1(TIM1);
//			PR("\r\nsta2=%10d;val2=%10d;",TIM1CH1_CAPTURE_STA2,TIM1CH1_CAPTURE_VAL2);
//			PR("\r\nval1=%10d;val2=%10d",TIM1CH1_CAPTURE_VAL1,TIM1CH1_CAPTURE_VAL2);
			temp=(TIM1CH1_CAPTURE_STA2-TIM1CH1_CAPTURE_STA1)*20000+TIM1CH1_CAPTURE_VAL2-TIM1CH1_CAPTURE_VAL1;
//			PR("\r\ntimhigh=%10d;",TimCapHigh1);
			TIM1CH1_CAPTURE_STA1=0;
			TIM1CH1_CAPTURE_VAL1=0;
			TIM1CH1_CAPTURE_STA2=0;
			TIM1CH1_CAPTURE_VAL2=0;
			TIM1CH1_CAPTURE_STA=0;
	  	TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);
			TimCapFlag1=1;
		}
}
		s=((temp/2)*344)/10000;//s为距离（mi）
		temp=0;
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1); //清除中断标志位
}
