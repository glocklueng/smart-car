#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

extern volatile uint8_t control_flag;
extern volatile uint8_t done_flag;

void car_turn(int8_t dir);
void car_turn_counter(uint32_t counter_l, float dir_l, uint32_t counter_r, float dir_r);
void car_set_speed(uint32_t counter1, float dir1, uint32_t counter2, float dir2);
void control_process(void);
void car_pass_line(uint32_t counter);

void car_left_turn_90(void);
void car_right_turn_90(void);
void car_go_straight(uint32_t counter);

void car_set_global_speed(void);


#endif
