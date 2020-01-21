#ifndef __GAME_H__
#define __GAME_H__

#include "SimpleAllocator.h"
#include "Object.h"
#include "Player.h"
#include "ParallaxBackground.h"
#include "Utils.h"
#include "../Graphics/Bitmap.h"
#include "../Platform/ResourceLoader.h"

#include <stdbool.h>

#define MaximumObjectSize 256
#define MaximumNumberOfObjects 1024

typedef struct Game
{
	ResourceLoader *loader;
	ResourceBundle *resources;

	Player player;

	SimpleAllocator allocator;
	uint32_t objectcounter;

	ObjectList controllers;
	ObjectList enemies;
	ObjectList enemybullets,playerbullets;
	ObjectList items,decorations,newdecorations;

	ParallaxBackground background;

	RLEBitmap **star,**font;

	uint32_t xmask;

	int timer;
	int32_t x0,y0;
	int32_t viewx;

	int score;

	uint8_t allocatorstorage[SizeOfSimpleAllocatorStorage(MaximumObjectSize,MaximumNumberOfObjects)];
} Game;

extern Game game;

bool InitializeGame();
void CleanupGame();

void StartGame();

bool IsGameOver();

void RunGame(bool left,bool right,bool up,bool down,bool fire,bool focus,bool bomb);
void DrawGame(Bitmap *screen);

static inline bool IsGameRunning(Game *self) { return true; }


static inline int ObjectLocalTime(Object *self) { return game.timer-self->birthtime; }

static inline int32_t XCoordDifference(int32_t a,int32_t b) { return ((a-b+game.xmask/2)&game.xmask)-game.xmask/2; }
static inline int32_t YCoordDifference(int32_t a,int32_t b) { return a-b; }

static inline int WrappedXCoordForWorldCoord(int32_t worldx) { return XCoordDifference(worldx,Fix(game.x0)); }
static inline int WrappedYCoordForWorldCoord(int32_t worldy) { return worldy; }

static inline int ScreenXCoordForWorldCoord(int32_t worldx) { return FixedToInt(WrappedXCoordForWorldCoord(worldx)); }
static inline int ScreenYCoordForWorldCoord(int32_t worldy) { return FixedToInt(WrappedYCoordForWorldCoord(worldy)); }

#endif
