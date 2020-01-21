#include "Time.h"

#include <SDL/SDL.h>

int TicksPerSecond()
{
	return 1000;
}

uint64_t GetTicks()
{
	return SDL_GetTicks();
}

void SleepForTicks(uint64_t ticks)
{
	SDL_Delay((uint32_t)ticks);
}

