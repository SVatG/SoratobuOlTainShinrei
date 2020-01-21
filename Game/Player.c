#include "Player.h"
#include "Game.h"
#include "Bullets.h"
#include "Decorations.h"
#include "Random.h"
#include "../Graphics/Drawing.h"

#define HotX 12
#define HotY 14
#define DeathWait 180
#define ImmortalTime (DeathWait+120)

void InitializePlayer(Player *self)
{
	self->x=100<<12;
	self->y=100<<12;
	self->vx=0;
	self->vy=0;
	self->dir=1;

	self->nextbullet=0;

	self->timeofdeath=-DeathWait;

	self->lives=3;

	self->frames=DataForResourceWithName(game.resources,"Graphics/Sprites/OL",NULL);
}

bool IsPlayerOutOfLives(Player *self) { return self->lives==0; }
bool IsPlayerDead(Player *self) { return game.timer<self->timeofdeath+DeathWait||IsPlayerOutOfLives(self); }
bool IsPlayerExploding(Player *self) { return game.timer<self->timeofdeath+DeathWait/2; }
bool IsPlayerImmortal(Player *self) { return game.timer<self->timeofdeath+ImmortalTime||IsPlayerOutOfLives(self); }

void RunPlayer(Player *self,bool left,bool right,bool up,bool down,bool fire,bool focus,bool bomb)
{
	int speed=1<<12;

	if(IsPlayerDead(self))
	{
		if(IsPlayerExploding(self))
		{
			if((game.timer-self->timeofdeath)%4==1)
			NewSplatExplosion(
			self->x+Fix(RandomIntegerInRange(-20,20)),
			self->y+Fix(RandomIntegerInRange(-20,20)),
			RandomInteger()%13);
		}

		return;
	}

	if(left) self->vx-=speed;
	if(right) self->vx+=speed;
	if(up) self->vy-=speed;
	if(down) self->vy+=speed;

	self->x+=self->vx;
	self->y+=self->vy;

	if(focus)
	{
		self->vx-=self->vx/2;
		self->vy-=self->vy/2;
	}
	else
	{
		self->vx-=self->vx/4;
		self->vy-=self->vy/4;
	}

	if(!focus)
	{
		if(left) self->dir=-1;
		if(right) self->dir=1;
	}

	if(fire)
	{
		int x=self->x+Fix(4+(self->dir==-1?1:0))*self->dir;
		int y=self->y;
		if(game.timer>=self->nextbullet)
		if(focus)
		{
			NewPlayerBullet(x,y,
			Fix(12)*self->dir,
			0,
			50,5,2,self->dir);

			self->nextbullet=game.timer+3;
		}
		else
		{
			NewPlayerBullet(x,y,
			Fix(12)*self->dir,
			0,
			50,2,1,self->dir);

			NewPlayerBullet(x,y,
			Fix(12)*self->dir,
			Fix(3),
			50,1,0,self->dir);

			NewPlayerBullet(x,y,
			Fix(12)*self->dir,
			Fix(-3),
			50,1,0,self->dir);

			self->nextbullet=game.timer+3;
		}
	}

	self->frame=4;

	if(left) self->frame-=self->dir;
	if(right) self->frame+=self->dir;

	if(up) self->frame-=3;

	if(fire) self->frame+=6;

	if(self->dir==-1) self->frame+=12;
}

void CollidePlayer(Player *self)
{
	if(IsPlayerImmortal(self)) return;

	if(CollideObjectListAgainstSquareWithPower(&game.enemybullets,
	game.player.x,game.player.y,Fix(3),1)<=0||
	CollideObjectListAgainstSquareWithPower(&game.enemies,
	game.player.x,game.player.y,Fix(3),1)<=0)
	{
		self->vx=0;
		self->vy=0;
		self->lives--;

		NewSplatExplosion(self->x,self->y,RandomInteger()%13);
		self->timeofdeath=game.timer;
	}
}

void DrawPlayer(Player *self,Bitmap *screen)
{
	if(IsPlayerDead(self)) return;

	int32_t bob=isinf(game.timer*60);

	int sx=ScreenXCoordForWorldCoord(self->x)-HotX;
	int sy=ScreenYCoordForWorldCoord(self->y+bob)-HotY;

	if(IsPlayerImmortal(self)&&(game.timer&0x07)>0x02) CompositeRLEBitmap(screen,self->frames[self->frame],sx,sy,FastHalfTransparentCompositionMode);
	else DrawRLEBitmap(screen,self->frames[self->frame],sx,sy);
}
