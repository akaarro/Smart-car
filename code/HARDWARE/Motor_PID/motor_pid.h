#ifndef  _MOTOR_PID_
#define  _MOTOR_PID_
#include "sys.h"
void motorPID(int speed_expected);
void motorPID_retreat(int speed_expected);
u32 data_process(u32 *dis_data);
#endif
