#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "stm32f4_discovery.h"
#include "gpio.h"
#include "remote_control.h"
#include "motor.h"
#include "capture.h"
#include "pid.h"
#include "light_senser.h"
#include "control.h"
#include "adc.h"

int main()
{
	int16_t i;
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	//print_clock_freq();
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	
	//adc_tim_trig_config(1000, 8400);
	capture_config();
	remote_control_init();
	motor_init();
	light_senser_init();
	left_speed_pid.target = 50;
	right_speed_pid.target = 50;
	while(1){
		control_process();
	}
}

