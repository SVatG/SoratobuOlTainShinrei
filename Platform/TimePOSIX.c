#include "Time.h"

#include <time.h>
#include <sys/time.h>

int TicksPerSecond()
{
	return 1000000;
}

uint64_t GetTicks()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000000+tv.tv_usec;
}

void SleepForTicks(uint64_t ticks)
{
	struct timespec spec=
	{
		.tv_sec=ticks/1000000,
		.tv_nsec=(ticks%1000000)*1000,
	};
	nanosleep(&spec,NULL);
}

