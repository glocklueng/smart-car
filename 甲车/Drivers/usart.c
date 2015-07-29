#include "usart.h"
#include "util.h"
#include "gpio.h"

void usart1_init(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_6|GPIO_Pin_7);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	nvic_config(USART1_IRQn, 4);
}

void uart4_init(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_10|GPIO_Pin_11);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	nvic_config(UART4_IRQn, 4);
}

void usart6_init(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_6|GPIO_Pin_7);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure); 
#ifndef DEBUG_HAHA
	USART_ITConfig(USART6, USART_IT_RXNE,ENABLE);
#endif
	USART_Cmd(USART6, ENABLE);
	USART_ClearFlag(USART6, USART_FLAG_TC);
#ifndef DEBUG_HAHA
	nvic_config(USART6_IRQn, 4);
#endif
}

//重定向printf函数
int fputc(int ch, FILE *f)
{	
	USART_SendData(USART6, (uint8_t) ch);
	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	return ch;
}

//重定向scanf函数
int fgetc(FILE *f)
{
	uint8_t val;
	while (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == RESET);
	val = USART_ReceiveData(USART6);
	USART_SendData(USART6, val);
	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	return (int)val;
}

void usart_sendByte(USART_TypeDef *usart, u8 val)
{
	USART_SendData(usart, val);
	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
}

void usart_sendBytes(USART_TypeDef *usart, u8 *buffer, u16 len)
{
	u16 i;
	for(i=0; i<len; i++){
		usart_sendByte(usart, buffer[i]);
	}
}

__IO uint8_t signal = 'p';
void USART6_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART6,USART_FLAG_RXNE)==SET)
	{
		signal = USART_ReceiveData(USART6);
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	}
}

void usart_wait_signal(void)
{
	signal = 'p';
	while(signal != 'a');
	signal = 'p';
}

void usart_send_signal(void)
{
	usart_sendByte(USART6, 'a');
}
