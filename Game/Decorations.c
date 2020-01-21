#include "Decorations.h"
#include "Game.h"

static bool RunAnimation(Object *obj);

Animation *NewAnimation(int32_t x,int32_t y,const char *filename,
int firstframe,int numframes,int ttl,bool loop)
{
	Animation *self=(Animation *)NewSprite(&game.newdecorations,sizeof(Animation),
	RunAnimation,NULL,DrawSprite);

	SetObjectPosition((Object *)self,x,y);
	SetSpriteSheet((Sprite *)self,filename);
	SetSpriteFrame((Sprite *)self,firstframe);

	self->firstframe=firstframe;
	self->numframes=numframes;
	self->ttl=ttl;
	self->loop=loop;

	return self;
}

static bool RunAnimation(Object *obj)
{
	Animation *self=(Animation *)obj;
	int time=ObjectLocalTime(obj);

	if(self->loop)
	{
		time=time%self->ttl;
	}
	else 
	{
		if(time>=self->ttl) return false;
	}

	int frame=self->firstframe+(self->numframes*time)/self->ttl;
	SetSpriteFrame((Sprite *)self,frame);

	return true;
}



Animation *NewSplatExplosion(int32_t x,int32_t y,int colour)
{
	return NewAnimation(x,y,"Graphics/Sprites/Splat",colour*8,8,32,false);
}

Animation *NewPlayerBulletFlash(int32_t x,int32_t y)
{
	return NewAnimation(x,y,"Graphics/Sprites/PlayerBulletFlash",0,4,8,false);
}

