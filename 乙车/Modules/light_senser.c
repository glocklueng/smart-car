#include "light_senser.h"
#include "gpio.h"
#include "exti.h"
#include "usart.h"
#include "control.h"

uint8_t running_state = STATE_START;
uint8_t pocess_state = STATE_IDEL;

void light_senser_init(void)
{
	gpio_in_up_init(GPIOE, 0XFF81);
	exti_config(GPIOD, 11);
}

int8_t light_senser_read(void)
{
	if(!LIGHT_L5){
		return 5;
	}
	if(!LIGHT_R5){
		return -5;
	}
	
	if(!LIGHT_L4){
		return 4;
	}
	if(!LIGHT_R4){
		return -4;
	}
	
	if(!LIGHT_L3){
		return 3;
	}
	if(!LIGHT_R3){
		return -3;
	}
	
	if(!LIGHT_L2){
		return 2;
	}
	if(!LIGHT_R2){
		return -2;
	}
	
	if(!LIGHT_L1){
		return 1;
	}
	if(!LIGHT_R1){
		return -1;
	}
	return 0;
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line11);
		if(LIGHT_SENTER != 0){
			return;
		}
		if(done_flag == 0){
			return;
		}
		switch(running_state){
			case STATE_START:
			case STATE1:
				running_state = STATE2;
				pocess_state = running_state;
				break;
			case STATE2:
				running_state = STATE3;
				pocess_state = running_state;
				break;
			case STATE3:
				running_state = STATE4;
				pocess_state = running_state;
				break;
			case STATE4:
				running_state = STATE5;
				pocess_state = running_state;
				break;
			case STATE5:
				running_state = STATE6;
				pocess_state = running_state;
				break;
			case STATE6:
				running_state = STATE7;
				pocess_state = running_state;
				break;
			case STATE7:
				running_state = STATE8;
				pocess_state = running_state;
				break;
			case STATE8:
				running_state = STATE9;
				pocess_state = running_state;
				break;
			case STATE9:
				running_state = STATE10;
				pocess_state = running_state;
				break;
			case STATE10:
				running_state = STATE11;
				pocess_state = running_state;
				break;
			case STATE11:
				running_state = STATE12;
				pocess_state = running_state;
				break;
			case STATE12:
				running_state = STATE13;
				pocess_state = running_state;
				break;
			case STATE13:
				running_state = STATE1;
				pocess_state = running_state;
				break;
		}
		done_flag = 0;
	}
}

