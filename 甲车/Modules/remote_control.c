#include "remote_control.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4_discovery.h"
#include "light_senser.h"

uint8_t car_mode = MODE_CONNECT;
__IO uint8_t remote_flag = 0;

void remote_control_init(void)
{
	uart4_init(115200);
}

void UART4_IRQHandler(void)
{
	static uint8_t buffer[3] = {0};
	static uint8_t counter = 0;
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)==SET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		buffer[counter] = USART_ReceiveData(UART4);
		if(counter == 0 && buffer[0] != 'x'){
			return;
		}
		counter++;
		if(counter == 3){
			counter = 0;
			if(buffer[1] == '1'){
				STM_EVAL_LEDOn(LED4);
				car_mode = MODE_SINGLE;
			}
			else{
				STM_EVAL_LEDOff(LED4);
				car_mode = MODE_CONNECT;
			}
			if(buffer[2] == '1'){
				STM_EVAL_LEDOn(LED5);
				running_state = STATE_START;
			}
			else{
				STM_EVAL_LEDOff(LED5);
				running_state = STATE2;
			}
			remote_flag = 1;
		}
	}
}
