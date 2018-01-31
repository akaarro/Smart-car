#ifndef _PID_H_
#define _PID_H_
#include "sys.h"
struct STEER_PID
{
    float fKp; 
    float fKi;
    float fKd;
};
extern struct STEER_PID  SD5_PID;
extern float err;
extern float angle;
extern u8 flag_turn;
extern u8 flag_turn_left;
extern u8 flag_turn_right;
extern u8 flag_turn_big;
extern void STEER_PID_init(struct STEER_PID *SD_PID,float Kp,float Ki,float Kd);
extern void steerPIDA(struct STEER_PID *SD_PID,float angle_expected); //??PID
#endif
