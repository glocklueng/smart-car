#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"
#include <stdio.h>

//#define DEBUG_HAHA

void usart1_init(uint32_t baudRate);
void uart4_init(uint32_t baudRate);
void usart6_init(uint32_t baudRate);

void usart_sendByte(USART_TypeDef *usart, u8 val);
void usart_sendBytes(USART_TypeDef *usart, u8 *buffer, u16 len);

void usart_wait_signal(void);
void usart_send_signal(void);

#endif
