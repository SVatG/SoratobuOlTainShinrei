#include "FrameRateManager.h"
#include "../Platform/Time.h"



void InitializeFrameRateManager(FrameRateManager *self,int fps,int maxframeskip)
{
	self->fps=fps;
	self->timerrate=TicksPerSecond();

	self->maxframeskip=maxframeskip;
	self->skippableframesleft=maxframeskip;

	self->currtime=0;
	self->nexttime=0;

	self->timelost=0;

	self->firstframe=true;
}




static inline uint64_t TimerTicksToInternalTime(FrameRateManager *self,uint64_t ticks)
{
	return ticks*self->fps;
}

static inline uint64_t InternalTimeToTimerTicks(FrameRateManager *self,uint64_t time)
{
	return time/self->fps;
}

static inline uint64_t FramesToInternalTime(FrameRateManager *self,int frames)
{
	return frames*self->timerrate;
}

static inline int InternalTimeToFrames(FrameRateManager *self,uint64_t time)
{
	return (int)(time/self->timerrate);
}

void BeginFrame(FrameRateManager *self)
{
	uint64_t ticks=GetTicks();
	uint64_t time=TimerTicksToInternalTime(self,ticks);

	if(self->firstframe)
	{
		self->nexttime=time;
		self->firstframe=false;
	}
	else
	{
		if(ShouldSkipThisFrame(self)) self->skippableframesleft--;
		else self->skippableframesleft=self->maxframeskip;

		self->nexttime+=FramesToInternalTime(self,1);

		if(time>self->nexttime+FramesToInternalTime(self,self->skippableframesleft))
		{
			uint64_t oldtime=self->nexttime;
			self->nexttime=time-FramesToInternalTime(self,self->skippableframesleft);
			self->timelost+=self->nexttime-oldtime;
		}
	}

	self->currtime=time;
}

void SleepUntilFrameStart(FrameRateManager *self)
{
	if(self->nexttime>self->currtime)
	{
		uint64_t ticks=InternalTimeToTimerTicks(self,self->nexttime-self->currtime);
		SleepForTicks(ticks);
	}
}

bool ShouldSkipThisFrame(FrameRateManager *self)
{
	return self->nexttime<self->currtime;
}



int TimeLost(FrameRateManager *self)
{
	return (int)InternalTimeToTimerTicks(self,self->timelost);
}

void ResetTimeLost(FrameRateManager *self)
{
	self->timelost=0;
}
