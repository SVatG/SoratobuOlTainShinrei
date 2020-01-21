#ifndef __FRAME_RATE_MANAGER_H__
#define __FRAME_RATE_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct FrameRateManager
{
	int fps,timerrate;
	int maxframeskip,skippableframesleft;

	uint64_t currtime,nexttime;
	uint64_t timelost;

	bool firstframe;
} FrameRateManager;


void InitializeFrameRateManager(FrameRateManager *self,int fps,int maxframeskip);

void BeginFrame(FrameRateManager *self);
void SleepUntilFrameStart(FrameRateManager *self);
bool ShouldSkipThisFrame(FrameRateManager *self);

int TimeLost(FrameRateManager *self);
void ResetTimeLost(FrameRateManager *self);

#endif
