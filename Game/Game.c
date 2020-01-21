#include "Game.h"
#include "Player.h"
#include "Controllers.h"
#include "Utils.h"
#include "SimpleAllocator.h"
#include "../Graphics/Drawing.h"
#include "../Platform/Screen.h"

static void DisplayLoadingProgress();

Game game;

bool InitializeGame()
{
	game.loader=AllocResourceLoader();

	StartRequestingResourceBundles(game.loader);
	RequestResourceBundle(game.loader,"Graphics/Sprites");
	RequestResourceBundle(game.loader,"Graphics/Backgrounds/NightSky");
	EndRequestingResourceBundles(game.loader);

	DisplayLoadingProgress();

	game.resources=ResourceBundleList(game.loader);

//	Bitmap *bullets=DataForResourceWithName(resources,"Graphics/Sprites/Bullets.png",NULL);
//	Bitmap *splat=DataForResourceWithName(resources,"Graphics/Sprites/Splat.png",NULL);

	InitializeSimpleAllocator(&game.allocator,MaximumObjectSize,MaximumNumberOfObjects,game.allocatorstorage);
	game.objectcounter=0;

	InitializeObjectList(&game.controllers);
	InitializeObjectList(&game.enemies);
	InitializeObjectList(&game.enemybullets);
	InitializeObjectList(&game.playerbullets);
	InitializeObjectList(&game.items);
	InitializeObjectList(&game.decorations);
	InitializeObjectList(&game.newdecorations);

	InitializeParallaxBackground(&game.background,
	"Graphics/Backgrounds/NightSky/NightSky1",
	"Graphics/Backgrounds/NightSky/NightSky2",
	"Graphics/Backgrounds/NightSky/NightSky3",
	"Graphics/Backgrounds/NightSky/NightSky4",
	0,73,118,165,131,182,223,239);

	game.star=DataForResourceWithName(game.resources,"Graphics/Sprites/Star",NULL);
	game.font=DataForResourceWithName(game.resources,"Graphics/Sprites/Font",NULL);

	return true;
}

void CleanupGame()
{
	CleanupObjectList(&game.controllers);
	CleanupObjectList(&game.enemies);
	CleanupObjectList(&game.enemybullets);
	CleanupObjectList(&game.playerbullets);
	CleanupObjectList(&game.items);
	CleanupObjectList(&game.decorations);
	CleanupObjectList(&game.newdecorations);

	//FreeSimpleAllocator(game.allocator);

	FreeResourceLoader(game.loader);
}

void StartGame()
{
	CleanupObjectList(&game.controllers);
	CleanupObjectList(&game.enemies);
	CleanupObjectList(&game.enemybullets);
	CleanupObjectList(&game.playerbullets);
	CleanupObjectList(&game.items);
	CleanupObjectList(&game.decorations);
	CleanupObjectList(&game.newdecorations);

	InitializePlayer(&game.player);

	NewMainController();

	game.xmask=Fix(2048)-1;

	game.x0=0;
	game.y0=0;
	game.viewx=0;

	game.score=0;
}

bool IsGameOver() { return IsPlayerOutOfLives(&game.player); }

void RunGame(bool left,bool right,bool up,bool down,bool fire,bool focus,bool bomb)
{
	int oldscore=game.score;

	RunPlayer(&game.player,left,right,up,down,fire,focus,bomb);

	int aimx;
	if(IsPlayerDead(&game.player))
	{
		aimx=game.player.x+Fix(game.player.dir*80);
	}
	else if(focus)
	{
		aimx=game.player.x+Fix(game.player.dir*120);
	}
	else
	{
		aimx=game.player.x+Fix(game.player.dir*80);
	}


	int force=(aimx-game.viewx)/16;
	int maxforce=Fix(4);

	if(force>maxforce) force=maxforce;
	if(force<-maxforce) force=-maxforce;
	game.viewx+=force+game.player.vx;

	game.x0=FixedToInt(game.viewx)-160;

	RunObjectList(&game.controllers);
	RunAndMoveObjectList(&game.enemies);
	RunAndMoveObjectList(&game.enemybullets);
	RunAndMoveObjectList(&game.playerbullets);
	RunAndMoveObjectList(&game.items);

	MergeObjectLists(&game.decorations,&game.newdecorations);
	RunAndMoveObjectList(&game.decorations);

	ClipObjectListAgainstScreen(&game.playerbullets,-32,ScreenWidth()+32,
	-32,ScreenHeight()+32);

	CalculateExtentsForObjectList(&game.enemies);
	CalculateExtentsForObjectList(&game.enemybullets);
	CalculateExtentsForObjectList(&game.playerbullets);
	CalculateExtentsForObjectList(&game.items);
	CalculateExtentsForObjectList(&game.decorations);
/*	CalculateExactExtentsForObjectList(&game.enemies);
	CalculateExactExtentsForObjectList(&game.enemybullets);
	CalculateExactExtentsForObjectList(&game.playerbullets);
	CalculateExactExtentsForObjectList(&game.items);
	CalculateExactExtentsForObjectList(&game.decorations);*/

	// TODO: Insertion sort is not yet implemented properly.
	InsertionSortObjectList(&game.enemies);
	InsertionSortObjectList(&game.playerbullets);
	InsertionSortObjectList(&game.enemybullets);

	CollideSortedObjectLists(&game.enemies,&game.playerbullets);
	//CollideSortedObjectListsExactly(&game.enemybullets,&game.playerbullets);

	CollidePlayer(&game.player);

	//self->x0=self->player.r.x+self->player.lookat.x-Fix(GameWidth)/2;
	//self->y0=imul(self->player.r.y+self->player.lookat.y,FixInvSqrt2)-Fix(GameHeight)/2;

	int oldstep=oldscore/500;
	int newstep=game.score/500;

	game.player.lives+=newstep-oldstep;

	game.timer++;
}

void DrawGame(Bitmap *screen)
{
//	FillBitmap(screen,RGB(0,0,64));
	DrawParallaxBackground(&game.background,screen,game.x0);

	DrawPlayer(&game.player,screen);

	DrawObjectList(&game.enemies,screen);
	DrawObjectList(&game.decorations,screen);
	DrawObjectList(&game.items,screen);
	DrawObjectList(&game.enemybullets,screen);
	DrawObjectList(&game.playerbullets,screen);

	int maxlives=3;
	if(game.player.lives>maxlives) maxlives=game.player.lives;
	for(int i=1;i<maxlives;i++)
	{
		int x=320-8-16*i;
		int y=240-16;

		if(i<game.player.lives) DrawRLEBitmap(screen,game.star[1],x,y);
		else DrawRLEBitmap(screen,game.star[0],x,y);
	}

	int x=320-8-12*7;
	int y=4;

	DrawRLEBitmap(screen,game.font['S'],x+0*7,y);
	DrawRLEBitmap(screen,game.font['c'],x+1*7,y);
	DrawRLEBitmap(screen,game.font['o'],x+2*7,y);
	DrawRLEBitmap(screen,game.font['r'],x+3*7,y);
	DrawRLEBitmap(screen,game.font['e'],x+4*7,y);
	DrawRLEBitmap(screen,game.font[':'],x+5*7,y);
	DrawRLEBitmap(screen,game.font[' '],x+6*7,y);
	DrawRLEBitmap(screen,game.font['0'+(game.score/10000)%10],x+7*7,y);
	DrawRLEBitmap(screen,game.font['0'+(game.score/1000)%10],x+8*7,y);
	DrawRLEBitmap(screen,game.font['0'+(game.score/100)%10],x+9*7,y);
	DrawRLEBitmap(screen,game.font['0'+(game.score/10)%10],x+10*7,y);
	DrawRLEBitmap(screen,game.font['0'+(game.score/1)%10],x+11*7,y);

	if(IsPlayerOutOfLives(&game.player))
	{
		int x=159-7*9/2;
		int y=119-3;
		DrawRLEBitmap(screen,game.font['G'],x+0*7,y);
		DrawRLEBitmap(screen,game.font['a'],x+1*7,y);
		DrawRLEBitmap(screen,game.font['m'],x+2*7,y);
		DrawRLEBitmap(screen,game.font['e'],x+3*7,y);
		DrawRLEBitmap(screen,game.font[' '],x+4*7,y);
		DrawRLEBitmap(screen,game.font['O'],x+5*7,y);
		DrawRLEBitmap(screen,game.font['v'],x+6*7,y);
		DrawRLEBitmap(screen,game.font['e'],x+7*7,y);
		DrawRLEBitmap(screen,game.font['r'],x+8*7,y);
	}
}



static void DisplayLoadingProgress()
{
	int progress;
	while((progress=ResourceLoadingProgress(game.loader))!=100)
	{
		Bitmap *screen=StartScreenUpdates();

		FillBitmap(screen,RGB(0,64,0));

		int w=ScreenWidth();
		int h=ScreenHeight();
		DrawRectangle(screen,32,h/2-16,w-64,32,RGB(224,255,224));
		DrawFilledRectangle(screen,34,h/2-14,((w-68)*progress)/100,28,RGB(224,255,224));

		EndScreenUpdates();
	}
}
