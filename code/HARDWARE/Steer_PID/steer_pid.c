#include "steer_pid.h"
#include "pwm.h"
#include "usart.h"
void STEER_PID_init(struct STEER_PID *SD_PID,float Kp,float Ki,float Kd)
{
  SD_PID->fKp = Kp;
  SD_PID->fKi = Ki;
  SD_PID->fKd = Kd;
}
void steerPIDA(struct STEER_PID *SD_PID, float angle_expected)
{
  static float mLastErr,mLastlastErr;
  float  mAngleOut,mErr;
  int OUT_angle;
  static float mLastAngleOut;  
  mErr =(angle_expected - angle);        
  if(mErr>180)
    mErr=mErr-360;  
  else if(mErr<-180)
    mErr=mErr+360;
	//if(mErr>40||mErr<-40)
		//flag_turn_big=1;
	//printf("mErr  %f\r\n",mErr);
	//printf("mLastErr  %f\r\n",mLastErr);
	//printf("mLastlastErr  %f\r\n",mLastlastErr);
   mAngleOut = mLastAngleOut 
                   + SD_PID->fKp * (mErr-mLastErr)                       
                   + SD_PID->fKi * mErr                                  
                   + SD_PID->fKd * (mErr - 2*mLastErr + mLastlastErr);   
	//printf("mLastAngleOut  %f\r\n",mLastAngleOut);
	//printf("mAngleOut  %f\r\n",mAngleOut);
       OUT_angle=(int)mAngleOut;//70~90
		//printf("OUT_angle  %d\r\n",OUT_angle);
       if(mErr<30&&mErr>-30)
			 {
         OUT_angle=0;
				 //mErr=0.0;
				 //mAngleOut=0.0;
			 }
       if(OUT_angle>=10)      OUT_angle = 10;
       if(OUT_angle<=(-10))   OUT_angle =-10;
			 if(OUT_angle==10)
				 flag_turn_right=1;
			 else if(OUT_angle==-10)
				 flag_turn_left=1;
       mLastlastErr = mLastErr;
       mLastErr = mErr;
       mLastAngleOut=mAngleOut;
			 if(mLastAngleOut>10||mLastAngleOut<-10)
				 mLastAngleOut=mLastAngleOut*0.6;
      // printf("OUT_angle  %d\r\n",OUT_angle);
			 TIM_SetCompare1(TIM3,(18400-20*OUT_angle));
			 if(OUT_angle==10||OUT_angle==-10)
				 flag_turn=1;
			 else
				 flag_turn=0;
			 //printf("flag_turn  %d\r\n",flag_turn);		 
}
