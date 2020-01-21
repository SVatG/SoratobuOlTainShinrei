#include "Sprite.h"
#include "../Graphics/Drawing.h"

Sprite *NewSprite(ObjectList *list,size_t objsize,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc)
{
	Sprite *self=(Sprite *)NewObject(list,objsize,runfunc,hitfunc,drawfunc);

	self->frames=NULL;
	self->hotx=0;
	self->hoty=0;

	self->frame=0;

	return self;
}

void DrawSprite(Object *obj,Bitmap *screen)
{
	Sprite *self=(Sprite *)obj;

	DrawRLEBitmap(screen,self->frames[self->frame],
	ScreenXCoordForWorldCoord(obj->x)-self->hotx,
	ScreenYCoordForWorldCoord(obj->y)-self->hoty);
}

void CompositeSprite(Object *obj,Bitmap *screen,CompositionMode mode)
{
	Sprite *self=(Sprite *)obj;

	CompositeRLEBitmap(screen,self->frames[self->frame],
	ScreenXCoordForWorldCoord(obj->x)-self->hotx,
	ScreenYCoordForWorldCoord(obj->y)-self->hoty,
	mode);
}

void FillSprite(Object *obj,Bitmap *screen,Pixel c)
{
	Sprite *self=(Sprite *)obj;

	DrawFilledRLEBitmap(screen,self->frames[self->frame],
	ScreenXCoordForWorldCoord(obj->x)-self->hotx,
	ScreenYCoordForWorldCoord(obj->y)-self->hoty,
	c);
}
