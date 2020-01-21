#include "Bullets.h"
#include "Game.h"
#include "Decorations.h"

static bool RunSimpleBullet(Object *obj);
static bool HitBullet(Object *obj,Object *other,int power);
static bool HitPlayerBullet(Object *obj,Object *other,int power);




Bullet *NewRoundBullet(size_t objsize,ObjectRunFunction runfunc,
int32_t x,int32_t y,int ttl,int colour,int size)
{
	static const int32_t radiuses[3]={Fix(1),Fix(2),Fix(3)};

	Bullet *self=(Bullet *)NewSprite(&game.enemybullets,objsize,
	runfunc,HitBullet,DrawSprite);

	SetObjectPosition((Object *)self,x,y);
	SetObjectPower((Object *)self,1);
	SetObjectHitRadius((Object *)self,radiuses[size]);
	SetSpriteSheet((Sprite *)self,"Graphics/Sprites/Bullets");
	SetSpriteFrame((Sprite *)self,colour*3+size);

	self->ttl=ttl;
	self->colour=colour;

	return self;
}

Bullet *NewSimpleBulletAtAngle(int32_t x,int32_t y,int32_t v,int a,
int ttl,int colour,int size)
{
	Bullet *self=NewRoundBullet(sizeof(Bullet),RunSimpleBullet,
	x,y,ttl,colour,size);

	SetObjectVelocityAndDirection((Object *)self,v,a);

	return self;
}

Bullet *NewSimpleBulletTowardsPlayer(int32_t x,int32_t y,int32_t v,
int ttl,int colour,int size)
{
	Bullet *self=NewRoundBullet(sizeof(Bullet),RunSimpleBullet,
	x,y,ttl,colour,size);

	AimObjectAtPosition((Object *)self,game.player.x,game.player.y,v);

	return self;
}




Bullet *NewPlayerBullet(int32_t x,int32_t y,int32_t vx,int32_t vy,
int ttl,int power,int size,int dir)
{
	static const int32_t radiuses[3]={Fix(4),Fix(6),Fix(8)};

	Bullet *self=(Bullet *)NewSprite(&game.playerbullets,sizeof(Bullet),
	RunSimpleBullet,HitPlayerBullet,DrawSprite);

	SetObjectPosition((Object *)self,x,y);
	SetObjectVelocity((Object *)self,vx,vy);
	SetObjectPower((Object *)self,power);
	SetObjectHitRadius((Object *)self,radiuses[size]);
	SetSpriteSheet((Sprite *)self,"Graphics/Sprites/PlayerBullets2");
	SetSpriteFrame((Sprite *)self,size+(dir==-1?3:0));

	self->ttl=ttl;
	self->colour=4;

	return self;
}

static bool RunSimpleBullet(Object *obj)
{
	Bullet *self=(Bullet *)obj;

	if(ObjectLocalTime(obj)>self->ttl) return false;

	return true;
/*	int origsize=self->sprite.size;
	for(int i=0;i<SimpleBulletBurnoutTime;i++)
	{
		int t=idiv(i,SimpleBulletBurnoutTime);
		self->sprite.size=imul(origsize,Fix(1)-RandomInteger()%(t+1));
		Wait(obj,game,1);
	}*/
}

static bool HitBullet(Object *obj,Object *other,int power)
{
	return false;
}

static bool HitPlayerBullet(Object *obj,Object *other,int power)
{
	NewPlayerBulletFlash(obj->x,obj->y);
	return false;
}

