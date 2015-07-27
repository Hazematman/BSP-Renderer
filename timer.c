#include <SDL2/SDL.h>
#include "timer.h"

void resetTimer(Timer *timer){
	timer->ticks = SDL_GetTicks();
}

float getElapsedTime(Timer *timer){
	return (float)(SDL_GetTicks() - timer->ticks) / 1000.0f;
}

uint32_t getElapsedTicks(Timer *timer){
	return (SDL_GetTicks() - timer->ticks);
}

