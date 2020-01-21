#include "../Screen.h"

#include <SDL/SDL.h>

static SDL_Surface *screensurface;
static Bitmap lowresbitmap;

static void ExpandAndDoublePixels(void *pixels,int bytesperrow,Bitmap *src);
static uint32_t ExpandPixel(Pixel p);

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

	ExpandAndDoublePixels(screensurface->pixels,screensurface->pitch,&lowresbitmap);

	SDL_UnlockSurface(screensurface);

	SDL_UpdateRect(screensurface,0,0,screensurface->w,screensurface->h);
}

static void ExpandAndDoublePixels(void *pixels,int bytesperrow,Bitmap *src)
{
	for(int y=0;y<src->height;y++)
	{
		Pixel *srcrow=BitmapRowPointer(src,y);
		uint64_t *destrow1=(uint64_t *)((uint8_t *)pixels+bytesperrow*(2*y+0));
		uint64_t *destrow2=(uint64_t *)((uint8_t *)pixels+bytesperrow*(2*y+1));

		for(int x=0;x<src->width;x++)
		{
			Pixel pixel=srcrow[x];
			uint32_t expanded=ExpandPixel(pixel);
			uint64_t doublepixel=((uint64_t)expanded<<32)|expanded;

			destrow1[x]=doublepixel;
			destrow2[x]=doublepixel;
		}
	}
}

static uint32_t ExpandPixel(Pixel p)
{
	int r=ExtractRed(p);
	int g=ExtractGreen(p);
	int b=ExtractBlue(p);
	return (b<<24)|(g<<16)|(r<<8)|0xff;
}
