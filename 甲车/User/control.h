#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

extern uint8_t control_flag;

void car_turn(int8_t dir);
void car_turn_counter(uint32_t counter_l, float dir_l, uint32_t counter_r, float dir_r);
void control_process(void);
void car_pass_line(uint32_t counter);

#endif
