#include <SDL/SDL.h>

#include "../Screen.h"

static SDL_Surface *screensurface;
static Bitmap screenbitmap;

void _InitializeScreen()
{
	screensurface=SDL_SetVideoMode(ScreenWidth(),ScreenHeight(),32,SDL_HWSURFACE);
}

int ScreenWidth() { return 640; }

int ScreenHeight() { return 480; }

Bitmap *StartScreenUpdates()
{
	SDL_LockSurface(screensurface);
	InitializeBitmap(&screenbitmap,screensurface->w,screensurface->h,screensurface->pitch,screensurface->pixels);
	return &screenbitmap;
}

void EndScreenUpdates()
{
	SDL_UnlockSurface(screensurface);
	SDL_UpdateRect(screensurface,0,0,screensurface->w,screensurface->h);
}
