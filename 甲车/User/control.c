#include "control.h"
#include "motor.h"
#include "delay.h"
#include "pid.h"
#include "capture.h"
#include "usart.h"
#include "light_senser.h"
#include "micros_time.h"
#include "adc.h"
#include "usart.h"
#include "stm32f4_discovery.h"
#include "remote_control.h"

volatile uint8_t control_flag = 1;
volatile uint8_t done_flag = 1;
uint32_t start_time;

extern uint8_t car_role;

#define TURN_GAIN 4
void car_turn(int8_t dir)
{
	if(dir > 0){
		car_turn_counter(dir*TURN_GAIN, 1, dir*TURN_GAIN, -1);
	}
	else if(dir <0 ){
		car_turn_counter(-dir*TURN_GAIN, -1, -dir*TURN_GAIN, 1);
	}
	else{
		car_set_global_speed();
	}
}

void car_turn_counter(uint32_t counter_l, float dir_l, uint32_t counter_r, float dir_r)
{
	left_dir = dir_l;
	right_dir = dir_r;
	left_pos_target_counter = counter_l;
	right_pos_target_counter = counter_r;
	left_pos_counter = 0;
	right_pos_counter = 0;
	pos_flag = 1;
}

void car_set_speed(uint32_t counter1, float dir1, uint32_t counter2, float dir2)
{
	left_dir = dir1;
	right_dir = dir2;
	left_speed_pid.target = counter1;
	right_speed_pid.target = counter2;
}

#define GLOBAL_SPEED	38
void car_set_global_speed(void)
{
	car_set_speed(GLOBAL_SPEED, 1, GLOBAL_SPEED, 1);
}


//小车盲走一段距离，穿过标志线
void car_pass_line(uint32_t counter)
{
	car_turn_counter(counter, 1, counter, 1);
	while(pos_flag == 1);
}

//左转90度
void car_left_turn_90(void)
{
	car_turn_counter(238, -1, 238, 1);	//左转90度
	while(pos_flag == 1);
}

//右转90度
void car_right_turn_90(void)
{
	car_turn_counter(235, 1, 235, -1);	//左转90度
	while(pos_flag == 1);
}

//小车前进一段距离
void car_go_straight(uint32_t counter)
{
	car_turn_counter(counter, 1, counter, 1);
	while(pos_flag == 1);
}

void control_process(void)
{
	int8_t dir;
	switch(pocess_state){
		case STATE_START:
			pocess_state = STATE_IDEL;
			done_flag = 1;
			break;
		case STATE2:
			pocess_state = STATE_IDEL;
			done_flag = 1;
			break;
		case STATE3:
		case STATE5:
		case STATE11:
		case STATE13:
			car_pass_line(80);
			pocess_state = STATE_IDEL;
			done_flag = 1;
			break;
		case STATE1:
		case STATE4:
		case STATE6:
			car_left_turn_90();
			pocess_state = STATE_IDEL;
			done_flag = 1;
			break;
		case STATE12:
			if(car_role == 1){	//被超车角色
				car_left_turn_90();
				if(car_mode != MODE_SINGLE){
					start_tim//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////e = get_micros_time();
					while(get_micros_time()-start_time < 2400000){
						dir = light_senser_read();
						car_turn(dir);	//循迹
						if(dir != 0){
							//start_time += 300000;
						}
						while(pos_flag == 1);
					}
					control_flag = 0;
					usart_wait_signal();
					delay_ms(2000);
					car_role = 0;
					control_flag = 1;
				}
				pocess_state = STATE_IDEL;
				done_flag = 1;
			}
			else{				//超车角色
				car_left_turn_90();
				adc_threshold = 1500;
				while(adc_flag == 1){
					car_turn(light_senser_read());	//循迹
				}
				adc_valid_flag = 0;    
				adc_flag = 1;
				car_left_turn_90();		//左转90度
				delay_ms(170);
				adc_threshold = 1000;
				adc_valid_flag = 1;
				car_go_straight(750);	//直走一段距离
				car_right_turn_90();	//右转90度
				delay_ms(110);
				car_go_straight(880);	//直走一段距离
				car_right_turn_90();	//右转90度
				car_go_straight(720);	//直走一段距离
				car_left_turn_90();		//左转90度
				usart_send_signal();
				car_role = 1;
				pocess_state = STATE_IDEL;
				done_flag = 1;
			}
			break;
		case STATE7:
		case STATE8:
		case STATE9:
		case STATE10:
			pocess_state = STATE_SPACE;
			done_flag = 1;
			break;
		case STATE_IDEL:
			car_turn(light_senser_read());	//循迹
			break;
		case STATE_SPACE:
			 break;
	}
}


