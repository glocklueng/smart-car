#include "pid.h"
#include "util.h"
#include <math.h>

PID left_speed_pid = {
	100,		//����޷�
	0,	//����
	0,	//Ŀ�������
	0,	//����������

	1,		//A
	0.5f,		//B
	0.1f,		//C
	
	0, 0, 0	//e
};

PID right_speed_pid = {
	100,		//����޷�
	0,	//����
	0,	//Ŀ�������
	0,	//����������

	1,		//A
	0.5f,		//B
	0.1f,		//C
	
	0, 0, 0	//e
};

//����ʽPID�㷨
float pid_calc(PID *pid)
{
	float out;
	
	pid->e_0 = pid->target - pid->feedback;
	
	if(fabs(pid->e_0) < pid->deadband){
		out = 0;
	}
	else{
		out = (float)(pid->A*pid->e_0 - pid->B*pid->e_1 + pid->C*pid->e_2);
	}
	
	out = range(out, -pid->limit, pid->limit);
	
	pid->e_2 = pid->e_1;
	pid->e_1 = pid->e_0;
	
	return out;
}

