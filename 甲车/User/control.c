#include "control.h"
#include "motor.h"
#include "delay.h"
#include "pid.h"
#include "capture.h"
#include "usart.h"
#include "light_senser.h"

uint8_t control_flag = 1;
uint8_t pass_flag = 0;

void car_turn(int8_t dir)
{
	if(dir > 0){
		car_turn_counter(dir*10, 1, dir*10, -1);
	}
	else if(dir <0 ){
		car_turn_counter(-dir*10, -1, -dir*10, 1);
	}
	else{
		left_speed_pid.target = 50;
		right_speed_pid.target = 50;
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

void car_pass_line(uint32_t counter)
{
	car_turn_counter(counter, 1, counter, 1);
	while(pos_flag == 1);
}

void control_process(void)
{
	switch(pocess_state){
		case STATE_START:
			pocess_state = STATE_IDEL;
			break;
		case STATE2:
			//printf("into state2\r\n");
			pocess_state = STATE_IDEL;
			break;
		case STATE3:
		case STATE5:
		case STATE9:
			//printf("into state359\r\n");
			car_pass_line(100);
			pocess_state = STATE_IDEL;
			break;
		case STATE1:
		case STATE4:
		case STATE6:
		case STATE8:
			//printf("into state468\r\n");
			car_turn_counter(270, -1, 270, 1);//×ó×ª90¶È
			while(pos_flag == 1);
			pocess_state = STATE_IDEL;
			break;
		case STATE7:
			printf("start pass\r\n");
			car_pass_line(900);
			printf("end pass\r\n");
			pass_flag = 0;
			pocess_state = STATE_IDEL;
			break;
		case STATE_IDEL:
			//printf("into state_idel\r\n");
			car_turn(light_senser_read());	//Ñ­¼£
			break;
		case STATE_STOP:
			
			 break;
	}
}


