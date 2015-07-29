#include "remote_control.h"
#include "usart.h"
#include "motor.h"
#include "stm32f4_discovery.h"

void remote_control_init(void)
{
	uart4_init(115200);
}

#define MSG_STOP	('s')
#define MSG_FRONT	('a')
#define MSG_BACK	('b')
#define MSG_LEFT	('c')
#define MSG_RIGHT	('d')
void UART4_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)==SET)
	{
		ch = USART_ReceiveData(UART4);
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		switch(ch){
			
		}
	}
}

