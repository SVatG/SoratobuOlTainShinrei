#include "Drawing.h"
#include "Clipping.h"

typedef void LineDrawingFunction(Bitmap *bitmap,int x,int y,int len,Pixel c,CompositionMode comp);

static inline void DrawHorizontalLineWrapped(Bitmap *bitmap,int x,int y,int len,Pixel c,CompositionMode comp) { DrawHorizontalLine(bitmap,x,y,len,c); }
static inline void DrawHorizontalLineNoClipWrapped(Bitmap *bitmap,int x,int y,int len,Pixel c,CompositionMode comp) { DrawHorizontalLineNoClip(bitmap,x,y,len,c); }
static inline void DrawVerticalLineWrapped(Bitmap *bitmap,int x,int y,int len,Pixel c,CompositionMode comp) { DrawVerticalLine(bitmap,x,y,len,c); }
static inline void DrawVerticalLineNoClipWrapped(Bitmap *bitmap,int x,int y,int len,Pixel c,CompositionMode comp) { DrawVerticalLineNoClip(bitmap,x,y,len,c); }

static inline void DrawRectangleImplementation(Bitmap *bitmap,int x,int y,int w,int h,
Pixel c,CompositionMode comp,LineDrawingFunction *horizontal,LineDrawingFunction *vertical)
{
	if(w==1) vertical(bitmap,x,y,h,c,comp);
	else if(h==1) horizontal(bitmap,x,y,w,c,comp);
	else
	{
		horizontal(bitmap,x,y,w,c,comp);
		horizontal(bitmap,x,y+h-1,w,c,comp);
		if(h<2) return;
		vertical(bitmap,x,y+1,h-2,c,comp);
		vertical(bitmap,x+w-1,y+1,h-2,c,comp);
	}
}

void DrawRectangle(Bitmap *bitmap,int x,int y,int w,int h,Pixel c)
{
	DrawRectangleImplementation(bitmap,x,y,w,h,c,DummyCompositionMode,DrawHorizontalLineWrapped,DrawVerticalLineWrapped);
}

void DrawRectangleNoClip(Bitmap *bitmap,int x,int y,int w,int h,Pixel c)
{
	DrawRectangleImplementation(bitmap,x,y,w,h,c,DummyCompositionMode,DrawHorizontalLineNoClipWrapped,DrawVerticalLineNoClipWrapped);
}

void CompositeRectangle(Bitmap *bitmap,int x,int y,int w,int h,Pixel c,CompositionMode comp)
{
	DrawRectangleImplementation(bitmap,x,y,w,h,c,comp,CompositeHorizontalLine,CompositeVerticalLine);
}

void CompositeRectangleNoClip(Bitmap *bitmap,int x,int y,int w,int h,Pixel c,CompositionMode comp)
{
	DrawRectangleImplementation(bitmap,x,y,w,h,c,comp,CompositeHorizontalLineNoClip,CompositeVerticalLineNoClip);
}




void DrawFilledRectangle(Bitmap *bitmap,int x,int y,int w,int h,Pixel c)
{
	if(ClipRectangle(&x,&y,&w,&h,bitmap->width,bitmap->height))
	DrawFilledRectangleNoClip(bitmap,x,y,w,h,c);
}

void DrawFilledRectangleNoClip(Bitmap *bitmap,int x,int y,int w,int h,Pixel c)
{
	for(int i=0;i<h;i++) DrawHorizontalLineNoClip(bitmap,x,y+i,w,c);
}

void CompositeFilledRectangle(Bitmap *bitmap,int x,int y,int w,int h,Pixel c,CompositionMode comp)
{
	if(ClipRectangle(&x,&y,&w,&h,bitmap->width,bitmap->height))
	CompositeFilledRectangleNoClip(bitmap,x,y,w,h,c,comp);
}

void CompositeFilledRectangleNoClip(Bitmap *bitmap,int x,int y,int w,int h,Pixel c,CompositionMode comp)
{
	for(int i=0;i<h;i++) CompositeHorizontalLineNoClip(bitmap,x,y+i,w,c,comp);
}

