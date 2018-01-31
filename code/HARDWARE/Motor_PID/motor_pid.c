#include "motor_pid.h"
#include "pwm.h"
#include "usart.h"
extern int  speed_count;
void motorPID(int speed_expected)
{
  static float mLastErr,mLastlastErr;
  float  mKp,mKi,mkd;
  float  mSpeedOut,mErr;
  static float OUT_speed;
  static float mLastSpeedOut;
  mKp = 0.2;
  mKi = 0.0;
  mkd = 0.03;
  mErr =(float)(speed_expected - speed_count);
	//printf("mErr1:%f\n",mErr);
	//printf("mLastSpeedOut:%f\n",mSpeedOut);
  mSpeedOut = mLastSpeedOut 
              + mKp * (mErr-mLastErr)                       // ???
              + mKi * mErr                                  
              + mkd * (mErr - 2*mLastErr + mLastlastErr);   // ???
  
  //printf("mSpeedOut1:%f\n",mSpeedOut);
  if(mSpeedOut>=0.3*speed_expected)     mSpeedOut = 0.3*speed_expected;
  if(mSpeedOut<=-0.3*speed_expected)    mSpeedOut = -0.3*speed_expected;
    
  OUT_speed=(unsigned int)( mSpeedOut);
  if(OUT_speed>=15)        OUT_speed = 15;
  if(OUT_speed<=0)        OUT_speed = 0;

  mLastlastErr = mLastErr;
  mLastErr = mErr;

  mLastSpeedOut=mSpeedOut;
	//printf("SPEED COUNT:%d\n",speed_count);
  //printf("outspeed1:%f\n",OUT_speed);
   
  TIM_SetCompare1(TIM5,(unsigned int)(50.0-1.5*OUT_speed));
}
void motorPID_retreat(int speed_expected)
{
  static float mLastErr,mLastlastErr;
  float  mKp,mKi,mkd;
  float  mSpeedOut,mErr;
  static float OUT_speed;
  static float mLastSpeedOut;
  mKp = 0.2;
  mKi = 0.0;
  mkd = 0.03;
  mErr =(float)(speed_expected - speed_count);
	//printf("mErr:%f\n",mErr);
  mSpeedOut = mLastSpeedOut 
              + mKp * (mErr-mLastErr)                       // ???
              + mKi * mErr                                  
              + mkd * (mErr - 2*mLastErr + mLastlastErr);   // ???
  
  //printf("mSpeedOut:%f\n",mSpeedOut);
  if(mSpeedOut>=0.3*speed_expected)     mSpeedOut = 0.3*speed_expected;
  if(mSpeedOut<=-0.3*speed_expected)    mSpeedOut = -0.3*speed_expected;
    
  OUT_speed=(unsigned int)( mSpeedOut);
  if(OUT_speed>=15)        OUT_speed = 15;
  if(OUT_speed<=0)        OUT_speed = 0;

  mLastlastErr = mLastErr;
  mLastErr = mErr;
    
  mLastSpeedOut=mSpeedOut;
	//printf("SPEED COUNT:%d\n",speed_count);
 // printf("outspeed:%f\n",OUT_speed);
   
  TIM_SetCompare2(TIM5,(unsigned int)(50.0-1.5*OUT_speed));
}
u32 data_process(u32 *dis_data)
{
	u8 i,j;
	u32 distance_max,distance_min;
	u32 dis_count=0;
	//for(i=0;i<5;i++)  dis_data--;
	distance_min=*dis_data;
	distance_max=*dis_data;
//	printf("distance_min%d\r\n",distance_min);
//	printf("distance_max%d\r\n",distance_max);
	for(i=0;i<5;i++)
	 { 
		 if(*dis_data<distance_min)
			distance_min=*dis_data;
		 if(*dis_data>distance_max)
			distance_max=*dis_data;
	  dis_data++;
	}
//	printf("distance_min%d\r\n",distance_min);
	//printf("distance_max%d\r\n",distance_max);
	for(i=0;i<5;i++)  dis_data--;
	for(i=0;i<5;i++)
	{
//		printf("dis_data%d\r\n",*dis_data);
		if((*dis_data!=distance_max)&&(*dis_data!=distance_min))
		{
			dis_count+=*dis_data;
		//	printf("dis_count%d\r\n",dis_count);
			j++;
		}		
		dis_data++;
	}
	if(j==0)
		return distance_min;
		//printf("dis_count%d\r\n",dis_count);
	else
		return (dis_count/j);
	
}
