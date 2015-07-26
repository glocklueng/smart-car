#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"

typedef struct{
	float limit;	//输出限幅
	float deadband;	//死区
	float target;	//目标输出量
	float feedback;	//实际输出量	
	float A;	//e(k)的系数
	float B;	//e(k-1)的系数
	float C;	//e(k-2)的系数
	float e_0;	//当前误差
	float e_1;	//上一次误差
	float e_2;	//上上次误差
}PID;

extern PID left_speed_pid;
extern PID right_speed_pid;

float pid_calc(PID *pid);

#endif
