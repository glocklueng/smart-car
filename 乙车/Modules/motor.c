#include "motor.h"
#include "pwm.h"
#include "gpio.h"

float left_pwm = 0;
float right_pwm = 0;
__IO float left_dir = 1;
__IO float right_dir = 1;

void motor_init(void)
{
	gpio_out_pp_up_init(GPIOE, GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	MOTOR_IN1 = 0;
	MOTOR_IN2 = 0;
	MOTOR_IN3 = 0;
	MOTOR_IN4 = 0;
	pwm_config(1000);
}

void motor_set_pwm(float pwm1, float pwm2)
{
	if(pwm1>0){
		MOTOR_IN1 = 0;
		MOTOR_IN2 = 1;
		set_pwm1(pwm1);
	}
	else if(pwm1<0){
		MOTOR_IN1 = 1;
		MOTOR_IN2 = 0;
		set_pwm1(-pwm1);
	}
	else{
		MOTOR_IN1 = 0;
		MOTOR_IN2 = 0;
		set_pwm1(0);
	}
	
	if(pwm2>0){
		MOTOR_IN3 = 1;
		MOTOR_IN4 = 0;
		set_pwm2(pwm2);
	}
	else if(pwm2<0){
		MOTOR_IN3 = 0;
		MOTOR_IN4 = 1;
		set_pwm2(-pwm2);
	}
	else{
		MOTOR_IN3 = 0;
		MOTOR_IN4 = 0;
		set_pwm2(0);
	}
}

