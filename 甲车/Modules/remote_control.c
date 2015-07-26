#include "remote_control.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4_discovery.h"

void remote_control_init(void)
{
	usart6_init(115200);
}

#define MSG_STOP	('s')
#define MSG_FRONT	('a')
#define MSG_BACK	('b')
#define MSG_LEFT	('c')
#define MSG_RIGHT	('d')
void USART6_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetFlagStatus(USART6,USART_FLAG_RXNE)==SET)
	{
		ch = USART_ReceiveData(USART6);
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
		switch(ch){
			case MSG_STOP:
				motor_set_pwm(0, 0);
				STM_EVAL_LEDOff(LED3);
				STM_EVAL_LEDOff(LED4);
				STM_EVAL_LEDOff(LED5);
				STM_EVAL_LEDOff(LED6);
				break;
			case MSG_FRONT:
				motor_set_pwm(100, 100);
				STM_EVAL_LEDOn(LED3);
				break;
			case MSG_BACK:
				motor_set_pwm(-100, -100);
				STM_EVAL_LEDOn(LED4);
				break;
			case MSG_LEFT:
				motor_set_pwm(-90, 90);
				STM_EVAL_LEDOn(LED5);
				break;
			case MSG_RIGHT:
				motor_set_pwm(90, -90);
				STM_EVAL_LEDOn(LED6);
				break;
		}
	}
}
