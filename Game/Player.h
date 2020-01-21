#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdbool.h>
#include <stdint.h>

#include "../Graphics/RLEBitmap.h"

typedef struct Player
{
	int32_t x,y;
	int32_t vx,vy;
	int32_t frame,dir;

	int nextbullet;

	int timeofdeath;

	int lives;

	RLEBitmap **frames;
} Player;

void InitializePlayer(Player *self);

bool IsPlayerOutOfLives(Player *self);
bool IsPlayerDead(Player *self);
bool IsPlayerExploding(Player *self);
bool IsPlayerImmortal(Player *self);

void RunPlayer(Player *self,bool left,bool right,bool up,bool down,bool fire,bool focus,bool bomb);
void CollidePlayer(Player *self);
void DrawPlayer(Player *self,Bitmap *screen);

#endif
