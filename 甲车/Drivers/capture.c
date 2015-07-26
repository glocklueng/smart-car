#include "capture.h"
#include "util.h"
#include "gpio.h"
#include "timer.h"
#include "control.h"
#include "micros_time.h"
#include "usart.h"
#include "pid.h"
#include "motor.h"

uint32_t left_counter  = 0;
uint32_t right_counter = 0;

__IO uint32_t left_pos_counter = 0;
__IO uint32_t right_pos_counter = 0;

__IO uint32_t left_pos_target_counter = 0;
__IO uint32_t right_pos_target_counter = 0;

__IO uint8_t pos_flag = 0;

//输入捕获配置
void capture_config(void)
{
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_8|GPIO_Pin_9);//CH3|CH4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 10;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
	
	nvic_config(TIM3_IRQn, 0);
	TIM7_init(1000, 8400);
}

void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET) 	//接收器A
	{
		TIM_ClearFlag(TIM3, TIM_IT_CC3);
		left_counter++;
	}
	if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET) 	//接收器B
	{
		TIM_ClearFlag(TIM3, TIM_IT_CC4);
		right_counter++;
	}
}

//100ms
void TIM7_IRQHandler(void)
{
	float pwm1, pwm2;
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		if(control_flag == 1){
			if(pos_flag == 1){
				pwm1 = 0;
				pwm2 = 0;
				left_pos_counter += left_counter;
				right_pos_counter += right_counter;
				if(left_pos_counter < left_pos_target_counter){
					pwm1 = 80*left_dir;
				}
				if(right_pos_counter < right_pos_target_counter){
					pwm2 = 80*right_dir;
				}
				//printf("counter:%d,%d;  pos:%d,%d;  target:%d,%d;  dir:%2.1f,%2.1f\r\n", left_counter, right_counter, left_pos_counter, right_pos_counter, left_pos_target_counter, right_pos_target_counter, left_dir, right_dir);
				if(pwm1==0&&pwm2==0){
					left_dir =1;
					right_dir =1;
					pos_flag = 0;
				}
				motor_set_pwm(pwm1, pwm2);
			}
			else{
				left_speed_pid.feedback = left_counter;
				right_speed_pid.feedback = right_counter;
				left_pwm += pid_calc(&left_speed_pid);
				right_pwm += pid_calc(&right_speed_pid);
				left_pwm = range(left_pwm, 0, 100);
				right_pwm = range(right_pwm, 0, 100);
				motor_set_pwm(left_pwm*left_dir, right_pwm*right_dir);
			}
		}
		else{
			motor_set_pwm(0, 0);
		}
		left_counter = 0;
		right_counter = 0;
	}
}


