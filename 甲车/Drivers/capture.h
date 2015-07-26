#ifndef __CAPTURE_H__
#define __CAPTURE_H__

#include "stm32f4xx.h"

extern uint32_t left_counter;
extern uint32_t right_counter;

extern __IO uint32_t left_pos_counter;
extern __IO uint32_t right_pos_counter;

extern __IO uint32_t left_pos_target_counter;
extern __IO uint32_t right_pos_target_counter;

extern __IO uint8_t pos_flag;

void capture_config(void);

#endif
