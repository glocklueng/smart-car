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
#include "micros_time.h"

//小车角色
uint8_t car_role = 1;

#ifdef DEBUG_HAHA
void my_debug(void)
{
	int in = 0;
	uint32_t counter, counter1, counter2;
	printf("1->左转90度\r\n");
	printf("2->右转90度\r\n");
	printf("3->直线行走一段距离\r\n");
	printf("intput option: ");
	scanf("%d", &in);
	printf("\r\n");
	switch(in){
		case 1:
			printf("input counter1 counter2: ");
			scanf("%d", &counter1);
			printf("\r\n");
			scanf("%d", &counter2);
			printf("\r\ncounter: %d %d\r\n",counter1, counter2);
			car_turn_counter(counter1, -1, counter2, 1);
			while(pos_flag == 1);
			break;
		case 2:
			printf("input counter1 counter2: ");
			scanf("%d", &counter1);
			printf("\r\n");
			scanf("%d", &counter2);
			printf("\r\ncounter: %d %d\r\n",counter1, counter2);
			car_turn_counter(counter1, 1, counter2, -1);
			while(pos_flag == 1);
			break;
		case 3:
			printf("input counter: ");
			scanf("%d", &counter);
			printf("\r\ncounter: %d\r\n",counter);
			car_turn_counter(counter, 1, counter, 1);
			while(pos_flag == 1);
			break;
		default:
			return;
	}
}
#endif

int main()
{
	rcc_clock_enable();
	delay_config();
	usart6_init(115200);
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	
	//android远程控制初始化
	remote_control_init();
	//红外测距模块ADC初始化
	adc_tim_trig_config(500, 8400);
	//电机初始化
	motor_init();
	//光电开关初始化
	light_senser_init();
	//编码器初始化
	capture_config();
	//系统运行时间初始化
	micros_time_init();
#ifndef DEBUG_HAHA
//	//等待开始信号
//	usart_wait_signal();
//	//发送应答信号
//	usart_send_signal();
	while(remote_flag == 0);
	if(car_mode == MODE_SINGLE){
		usart_sendByte(USART6, '1');
	}
	else{
		usart_sendByte(USART6, '0');
	}
	//设置小车前进速度
	car_set_global_speed();
#endif
	STM_EVAL_LEDOn(LED3);
	while(1){
#ifdef DEBUG_HAHA
		my_debug();
#else
		control_process();
#endif
	}
}

