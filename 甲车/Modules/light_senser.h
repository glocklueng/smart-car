#ifndef __LIGHT_SENSER_H__
#define __LIGHT_SENSER_H__

#include "bitband.h"

extern uint8_t running_state;
extern uint8_t pocess_state;

#define LIGHT_L1		PeInBit(0)
#define LIGHT_L2		PeInBit(7)
#define LIGHT_L3		PeInBit(8)
#define LIGHT_L4		PeInBit(9)
#define LIGHT_L5		PeInBit(10)
#define LIGHT_SENTER	PeInBit(11)
#define LIGHT_R5		PeInBit(11)
#define LIGHT_R4		PeInBit(12)
#define LIGHT_R3		PeInBit(13)
#define LIGHT_R2		PeInBit(14)
#define LIGHT_R1		PeInBit(15)

#define STATE_START	0
#define STATE1	1
#define STATE2	2
#define STATE3	3
#define STATE4	4
#define STATE5	5
#define STATE6	6
#define STATE7	7
#define STATE8	8
#define STATE9	9
#define STATE_IDEL	10
#define STATE_STOP	11

void light_senser_init(void);
int8_t light_senser_read(void);


#endif

