#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Sprite.h"

typedef struct Enemy
{
	Sprite sprite;
	int colour,score;
	int hitend;
} Enemy;

Enemy *NewEnemy(size_t objsize,int32_t x,int32_t y,const char *filename,int colour,int score,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc);

bool HitEnemy(Object *obj,Object *other,int power);
void DrawEnemy(Object *obj,Bitmap *screen);




Object *NewEctoplasm(int32_t x,int32_t y,int dir);

Object *NewOrb(int32_t x,int32_t y,int size);

Object *NewFlame(int32_t x,int32_t y,int dir);

#endif
