#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "bitband.h"

#define MOTOR_IN1 PeOutBit(1)
#define MOTOR_IN2 PeOutBit(3)
#define MOTOR_IN3 PeOutBit(2)
#define MOTOR_IN4 PeOutBit(4)

extern float left_pwm;
extern float right_pwm;
extern __IO  float left_dir;
extern __IO  float right_dir;

void motor_init(void);
void motor_set_pwm(float pwm1, float pwm2);

#endif

