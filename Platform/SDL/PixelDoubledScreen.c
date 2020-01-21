#include "../Screen.h"
#include "../Shared/PixelDoubling.h"

#include <SDL/SDL.h>

static SDL_Surface *screensurface;
static Bitmap screenbitmap;
static Bitmap lowresbitmap;

void _InitializeScreen()
{
	screensurface=SDL_SetVideoMode(ScreenWidth()*2,ScreenHeight()*2,32,SDL_HWSURFACE);

	InitializeBitmap(&lowresbitmap,
	ScreenWidth(),ScreenHeight(),
	BytesPerRowForWidth(ScreenWidth()),
	malloc(ScreenHeight()*BytesPerRowForWidth(ScreenWidth())));
}

int ScreenWidth() { return 320; }

int ScreenHeight() { return 240; }

Bitmap *StartScreenUpdates()
{
	return &lowresbitmap;
}

void EndScreenUpdates()
{
	SDL_LockSurface(screensurface);
	InitializeBitmap(&screenbitmap,screensurface->w,screensurface->h,screensurface->pitch,screensurface->pixels);

	CopyDoubledPixels(&screenbitmap,&lowresbitmap);

	SDL_UnlockSurface(screensurface);

	SDL_UpdateRect(screensurface,0,0,screensurface->w,screensurface->h);
}
