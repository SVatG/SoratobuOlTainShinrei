#ifndef __DECORATIONS_H__
#define __DECORATIONS_H__

#include "Sprite.h"

typedef struct Animation
{
	Sprite sprite;
	int firstframe,numframes;
	int ttl;
	bool loop;
} Animation;

Animation *NewAnimation(int32_t x,int32_t y,const char *filename,
int firstframe,int numframes,int ttl,bool loop);

Animation *NewSplatExplosion(int32_t x,int32_t y,int colour);

Animation *NewPlayerBulletFlash(int32_t x,int32_t y);

#endif
