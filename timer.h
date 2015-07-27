#ifndef TIMER_H
#define TIMER_H

#include "types.h"

typedef struct {
	uint32_t ticks;
} Timer;

void resetTimer(Timer *timer);
float getElapsedTime(Timer *timer);
uint32_t getElapsedTicks(Timer *timer);

#endif
