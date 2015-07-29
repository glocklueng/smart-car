#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__

#include "stm32f4xx.h"

#define MODE_SINGLE 	1
#define MODE_CONNECT	0

extern uint8_t car_mode;
extern __IO uint8_t remote_flag;

void remote_control_init(void);

#endif
