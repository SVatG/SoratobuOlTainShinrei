#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>

int TicksPerSecond();
uint64_t GetTicks();
void SleepForTicks(uint64_t ticks);

#endif

