#include "Enemy.h"
#include "Bullets.h"
#include "Decorations.h"
#include "Random.h"


Enemy *NewEnemy(size_t objsize,int32_t x,int32_t y,const char *filename,int colour,int score,
ObjectRunFunction runfunc,ObjectHitFunction hitfunc,ObjectDrawFunction drawfunc)
{
	Enemy *self=(Enemy *)NewSprite(&game.enemies,objsize,
	runfunc,hitfunc,drawfunc);

	SetObjectPosition((Object *)self,x,y);
	SetSpriteSheet((Sprite *)self,filename);

	self->colour=colour;
	self->score=score;
	self->hitend=0;

	return self;
}

bool HitEnemy(Object *obj,Object *other,int power)
{
	Enemy *self=(Enemy *)obj;

	obj->power-=power;
	self->hitend=game.timer+10;

	if(obj->power<=0)
	{
		NewSplatExplosion(obj->x,obj->y,self->colour);
		game.score+=self->score;
	}

	return obj->power>0;
}

void DrawEnemy(Object *obj,Bitmap *screen)
{
	Enemy *self=(Enemy *)obj;

	if(game.timer<self->hitend) FillSprite(obj,screen,RGB(0xff,0xff,0xff));
	else DrawSprite(obj,screen);
}





typedef struct Ectoplasm
{
	Enemy enemy;
	int tx,ty;
	int state;
	int startcharge;
} Ectoplasm;

static bool RunEctoplasm(Object *obj);

Object *NewEctoplasm(int32_t x,int32_t y,int dir)
{
	Ectoplasm *self=(Ectoplasm *)NewEnemy(sizeof(Ectoplasm),x,y,
	"Graphics/Sprites/Ectoplasm",7,20,
	RunEctoplasm,HitEnemy,DrawEnemy);

	SetObjectVelocity((Object *)self,0,0);
	SetObjectHitRadius((Object *)self,Fix(12));
	SetObjectPower((Object *)self,25);
	SetSpriteFrame((Sprite *)self,0);

	self->tx=x;
	self->ty=y;
	self->state=0;

	return (Object *)self;
}

static bool RunEctoplasm(Object *obj)
{
	Ectoplasm *self=(Ectoplasm *)obj;

	int forcex=0,forcey=0;
	if(self->state==0)
	{
		int dx=XCoordDifference(game.player.x,obj->x);
		int dy=YCoordDifference(game.player.y,obj->y);
		int r=isqrt(isq(dx)+isq(dy));

		forcex=idiv(dx,r);
		forcey=idiv(dy,r);

		if(r<Fix(64))
		{
			self->state=1;
		}
	}
	else if(self->state==1)
	{
		if(((Sprite *)self)->frame==4)
		{
			self->startcharge=game.timer;
			self->state=2;
		}
	}
	else if(self->state==2)
	{
		if(game.timer<self->startcharge+30)
		{
		}
		else
		{
			int a=RandomInteger()&4095;
			int numbullets=12;
			for(int i=0;i<numbullets;i++)
			{
				int angle=i*4096/numbullets+a;
				NewSimpleBulletAtAngle(obj->x,obj->y,Fix(3),angle,120,self->enemy.colour,1);
			}

			int playerdx=XCoordDifference(game.player.x,obj->x);
			int escapedir=playerdx>=0?-1:1;

			self->tx=game.player.x+Fix(escapedir*RandomIntegerInRange(150,300));
			self->ty=Fix(RandomIntegerInRange(10,229));

			self->state=3;
		}
	}
	else if(self->state==3)
	{
		int dx=XCoordDifference(self->tx,obj->x);
		int dy=YCoordDifference(self->ty,obj->y);
		int r=isqrt(isq(dx)+isq(dy));

		forcex=idiv(dx,r);
		forcey=idiv(dy,r);

		if(r<Fix(64))
		{
			self->state=4;
			self->startcharge=game.timer;
		}
	}
	else if(self->state==4)
	{
		if(game.timer>=self->startcharge+30)
		{
			self->state=0;
		}
	}

	obj->vx+=forcex/3;
	obj->vy+=forcey/3;
	obj->vx-=obj->vx/8;
	obj->vy-=obj->vy/8;

	if(self->state!=2)
	{
		static const int numframes=16;
		static const int frames[16]={0,1,2,3,4,5,6,7,8,7,6,5,4,3,2,1};

		int step=(ObjectLocalTime(obj)/6)%numframes;
		SetSpriteFrame((Sprite *)self,frames[step]);
	}
	else
	{
		static const int numframes=4;
		static const int frames[16]={9,4,10,4};

		int step=(ObjectLocalTime(obj)/3)%numframes;
		SetSpriteFrame((Sprite *)self,frames[step]);
	}

//	if((RandomInteger()&63)==0)
//	NewSimpleBulletTowardsPlayer(obj->x,obj->y,Fix(4),120,self->enemy.colour,self->enemy.colour);

	return true;
}




typedef struct Orb
{
	Enemy enemy;
	int size;
	int burstheight,offset;
	bool reverse;
} Orb;

static bool RunOrb(Object *obj);

Object *NewOrb(int32_t x,int32_t y,int size)
{
	Orb *self=(Orb *)NewEnemy(sizeof(Orb),x,y,
	"Graphics/Sprites/Orb",10,1,
	RunOrb,HitEnemy,DrawEnemy);

	static int radiuses[2]={Fix(4),Fix(6)};

	SetObjectVelocity((Object *)self,0,-Fix(0.5));
	SetObjectHitRadius((Object *)self,radiuses[size]);
	SetObjectPower((Object *)self,5);
	SetSpriteFrame((Sprite *)self,0);

	self->size=size;
	self->offset=RandomInteger()&4095;
	self->burstheight=Fix(RandomIntegerInRange(8,48));
	self->reverse=RandomInteger()&1;

	return (Object *)self;
}

static bool RunOrb(Object *obj)
{
	Orb *self=(Orb *)obj;

	if(obj->y<self->burstheight)
	{
		NewSplatExplosion(obj->x,obj->y,self->enemy.colour);
//		NewSimpleBulletTowardsPlayer(obj->x,obj->y,Fix(4),120,8,1);
		int a=RandomInteger()&4095;
		int numbullets=4*(self->size+2);
		for(int i=0;i<numbullets;i++)
		{
			int angle=i*4096/numbullets+a;
			NewSimpleBulletAtAngle(obj->x,obj->y,Fix(3),angle,120,self->enemy.colour,1);
		}
		return false;
	}

	int time=ObjectLocalTime(obj);

	int frame=(time/2)%4;
	if(self->reverse) frame^=3;
	if(obj->y<self->burstheight+Fix(48))
	{
		if(time%5>=3) frame=4;
	}

	SetSpriteFrame((Sprite *)self,frame+self->size*5);

	obj->vx=isin(time*5+self->offset)/3;

	return true;
}




typedef struct Flame
{
	Enemy enemy;
	int a,da;
} Flame;

static bool RunFlame(Object *obj);

Object *NewFlame(int32_t x,int32_t y,int dir)
{
	Flame *self=(Flame *)NewEnemy(sizeof(Flame),x,y,
	"Graphics/Sprites/Flame",1,10,
	RunFlame,HitEnemy,DrawEnemy);

	SetObjectVelocity((Object *)self,Fix(dir),0);
	SetObjectHitRadius((Object *)self,Fix(12));
	SetObjectPower((Object *)self,25);
	SetSpriteFrame((Sprite *)self,0);

	self->a=(dir==1)?0:2048;
	self->da=(RandomInteger()%1)*2-1;

	return (Object *)self;
}

static bool RunFlame(Object *obj)
{
	Flame *self=(Flame *)obj;

	int frame=(ObjectLocalTime(obj)/4)%4;
	SetSpriteFrame((Sprite *)self,frame);

	if((RandomInteger()&127)==0)
	{
		int a=self->a&4095;

		if(obj->y<Fix(120))
		{
			if(a<2048) self->da=(RandomInteger()%1)*2-1;
			else if(a<3072) self->da=-1;
			else self->da=1;
		}
		else
		{
			if(a>2048) self->da=(RandomInteger()%1)*2-1;
			else if(a>1024) self->da=1;
			else self->da=-1;
		}
	}

	if(obj->y<0&&(self->a&4095)>2048) self->a=-self->a;
	if(obj->y>Fix(240)&&(self->a&4095)<2048) self->a=-self->a;

	self->a+=self->da*10;

	SetObjectVelocityAndDirection(obj,Fix(1),self->a);

	if((RandomInteger()&127)==0)
	NewSimpleBulletTowardsPlayer(obj->x,obj->y,Fix(2.5),240,self->enemy.colour,self->enemy.colour);

	return true;
}
