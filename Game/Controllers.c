#include "Controllers.h"
#include "Game.h"
#include "Enemy.h"
#include "Random.h"



static Object *NewOrbController(int numorbs,int orbrate);
static Object *NewEctoplasmController(int numplasms,bool left);
static Object *NewFlameController(int numflames,bool left);


typedef struct MainController
{
	Object obj;
	int state;

	int waittime;

	int numwait;
	Object *waitobjects[8];
	uint32_t waitdogtags[8];
} MainController;

static bool RunMainController(Object *obj);
static void WaitForObject(MainController *self,Object *obj);

Object *NewMainController()
{
	MainController *self=(MainController *)NewObject(&game.controllers,
	sizeof(MainController),RunMainController,NULL,NULL);

	self->state=0;
	self->waittime=0;
	self->numwait=0;

	return (Object *)self;
}

static bool RunMainController(Object *obj)
{
	MainController *self=(MainController *)obj;

	// Check if we are waiting on a timer.
	if(self->waittime && game.timer<self->waittime) return true;

	// Check if we are waiting for objects to die.
	if(self->numwait)
	{
		for(int i=0;i<self->numwait;i++)
		{
			if(!IsObjectDead(self->waitobjects[i],self->waitdogtags[i])) return true;
		}
		self->numwait=0;
	}

	switch(self->state)
	{
		case 0:
			WaitForObject(self,NewFlameController(4,RandomInteger()&1));
			self->state++;
		break;

		case 1:
			WaitForObject(self,NewOrbController(32,10));
			self->state++;
		break;

		case 2:
			WaitForObject(self,NewFlameController(6,RandomInteger()&1));
			self->state++;
		break;

		case 3:
			WaitForObject(self,NewEctoplasmController(4,RandomInteger()&1));
			self->state++;
		break;

		case 4:
			WaitForObject(self,NewOrbController(64,10));
			self->state++;
		break;

		case 5:
			WaitForObject(self,NewEctoplasmController(4,RandomInteger()&1));
			self->state++;
		break;

		case 6:
		{
			bool left=RandomInteger()&1;
			WaitForObject(self,NewEctoplasmController(4,left));
			WaitForObject(self,NewFlameController(6,left));
			self->state++;
		}
		break;

		case 7:
			WaitForObject(self,NewFlameController(4,RandomInteger()&1));
			WaitForObject(self,NewOrbController(128,10));
			self->state++;
		break;

		case 8:
			WaitForObject(self,NewEctoplasmController(8,RandomInteger()&1));
			self->state++;
		break;

		case 9:
			WaitForObject(self,NewFlameController(12,RandomInteger()&1));
			self->state++;
		break;

		case 10:
		{
			bool left=RandomInteger()&1;
			WaitForObject(self,NewFlameController(8,left));
			WaitForObject(self,NewEctoplasmController(6,!left));
			self->state++;
		}
		break;

		case 11:
			WaitForObject(self,NewOrbController(128,10));
			WaitForObject(self,NewFlameController(8,RandomInteger()&1));
			self->state++;
		break;

		case 12:
			WaitForObject(self,NewOrbController(128,10));
			WaitForObject(self,NewEctoplasmController(12,RandomInteger()&1));
			self->state++;
		break;

		case 13:
		{
			bool left=RandomInteger()&1;
			WaitForObject(self,NewFlameController(6,left));
			WaitForObject(self,NewFlameController(6,!left));
			self->state++;
		}
		break;

		case 14:
		{
			bool left=RandomInteger()&1;
			WaitForObject(self,NewEctoplasmController(8,left));
			WaitForObject(self,NewEctoplasmController(8,!left));
			self->state++;
		}
		break;

		case 15:
		{
			bool left=RandomInteger()&1;
			WaitForObject(self,NewFlameController(12,left));
			WaitForObject(self,NewEctoplasmController(12,!left));
			WaitForObject(self,NewOrbController(256,10));
			self->state++;
		}
		break;

		case 16:
			self->state=0;
		break;
	}

	return true;
}

static void WaitForObject(MainController *self,Object *obj)
{
	assert(self->numwait<sizeof(self->waitobjects)/sizeof(self->waitobjects[0]));

	self->waitobjects[self->numwait]=obj;
	self->waitdogtags[self->numwait]=ObjectDogTag(obj);
	self->numwait++;
}





typedef struct OrbController
{
	Object obj;
	int numleft,rate,next;
} OrbController;

static bool RunOrbController(Object *obj);

static Object *NewOrbController(int numorbs,int orbrate)
{
	OrbController *self=(OrbController *)NewObject(&game.controllers,
	sizeof(OrbController),RunOrbController,NULL,NULL);

	self->numleft=numorbs;
	self->rate=orbrate;
	self->next=game.timer+orbrate;

	return (Object *)self;
}

static bool RunOrbController(Object *obj)
{
	OrbController *self=(OrbController *)obj;

	if(self->numleft)
	if(game.timer>self->next)
	{
		NewOrb(Fix(RandomIntegerInRange(10,309)+game.x0),Fix(248),RandomInteger()&1);
		self->next+=self->rate;
		self->numleft--;
	}

	return self->numleft || !IsObjectListEmpty(&game.enemies);
}




typedef struct EctoplasmController
{
	Object obj;
	int numleft,next;
	bool left;
} EctoplasmController;

static bool RunEctoplasmController(Object *obj);

static Object *NewEctoplasmController(int numplasms,bool left)
{
	EctoplasmController *self=(EctoplasmController *)NewObject(&game.controllers,
	sizeof(EctoplasmController),RunEctoplasmController,NULL,NULL);

	self->numleft=numplasms;
	self->next=game.timer+60;
	self->left=left;

	return (Object *)self;
}

static bool RunEctoplasmController(Object *obj)
{
	EctoplasmController *self=(EctoplasmController *)obj;

	if(self->numleft)
	if(game.timer>self->next)
	{
		int y=Fix(RandomIntegerInRange(10,229));

		if(self->left) NewEctoplasm(Fix(-8+game.x0),y,1);
		else NewEctoplasm(Fix(328+game.x0),y,-1);

		self->next+=60;
		self->numleft--;
	}

	return self->numleft || !IsObjectListEmpty(&game.enemies);
}



typedef struct FlameController
{
	Object obj;
	int numleft,next;
	bool left;
} FlameController;

static bool RunFlameController(Object *obj);

static Object *NewFlameController(int numflames,bool left)
{
	FlameController *self=(FlameController *)NewObject(&game.controllers,
	sizeof(FlameController),RunFlameController,NULL,NULL);

	self->numleft=numflames;
	self->next=game.timer+60;
	self->left=left;

	return (Object *)self;
}

static bool RunFlameController(Object *obj)
{
	FlameController *self=(FlameController *)obj;

	if(self->numleft)
	if(game.timer>self->next)
	{
		int y=Fix(RandomIntegerInRange(10,229));

		if(self->left) NewFlame(Fix(-8+game.x0),y,1);
		else NewFlame(Fix(328+game.x0),y,-1);

		self->next+=60;
		self->numleft--;
	}

	return self->numleft || !IsObjectListEmpty(&game.enemies);
}




/*static int nextcrow=60;
if(game.timer>nextcrow)
{
	nextcrow+=120;
	if(RandomInteger()&1) NewEctoplasm(Fix(-8+game.x0),Fix(RandomIntegerInRange(10,229)),1);
	else NewFlame(Fix(328+game.x0),Fix(RandomIntegerInRange(10,229)),-1);
}*/
