#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Object.h"
#include "Game.h"
#include "../Graphics/Drawing.h"
#include "../Graphics/RLEBitmap.h"

typedef struct Sprite
{
	Object obj;
	RLEBitmap **frames;
	int hotx,hoty;

	int frame;
} Sprite;


Sprite *NewSprite(ObjectList *list,size_t objsize,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc);

static inline void SetSpriteSheet(Sprite *self,const char *name)
{
	self->frames=DataForResourceWithName(game.resources,name,NULL);
	self->hotx=self->frames[0]->width/2;
	self->hoty=self->frames[0]->height/2;
}

static inline void SetSpriteHotSpot(Sprite *self,int hotx,int hoty)
{
	self->hotx=hotx;
	self->hoty=hoty;
}

static inline void SetSpriteFrame(Sprite *self,int frame)
{
	self->frame=frame;
}

void DrawSprite(Object *obj,Bitmap *screen);
void CompositeSprite(Object *obj,Bitmap *screen,CompositionMode mode);
void FillSprite(Object *obj,Bitmap *screen,Pixel c);

#endif

