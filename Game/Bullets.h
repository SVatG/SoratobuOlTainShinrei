#ifndef __BULLETS_H__
#define __BULLETS_H__

#include "Sprite.h"

typedef struct Bullet
{
	Sprite sprite;
	int ttl,colour;
} Bullet;

Bullet *NewRoundBullet(size_t objsize,ObjectRunFunction runfunc,
int32_t x,int32_t y,int ttl,int colour,int size);

Bullet *NewSimpleBulletAtAngle(int32_t x,int32_t y,int32_t v,int a,
int ttl,int colour,int size);
Bullet *NewSimpleBulletTowardsPlayer(int32_t x,int32_t y,int32_t v,
int ttl,int colour,int size);

Bullet *NewPlayerBullet(int32_t x,int32_t y,int32_t vx,int32_t vy,
int ttl,int power,int size,int dir);

#endif
