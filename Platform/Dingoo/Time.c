#include "../Time.h"

#include "Dingoo.h"

int TicksPerSecond()
{
	return 100;
}

uint64_t GetTicks()
{
	return OSTimeGet();
}

void SleepForTicks(uint64_t ticks)
{
	OSTimeDly(ticks);
}
