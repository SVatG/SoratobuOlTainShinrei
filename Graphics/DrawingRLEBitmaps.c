#include "RLEBitmap.h"
#include "Clipping.h"

#include <string.h>
#include <strings.h>


static inline void AdjustRLERows(Pixel **rows,int width,int *height,int *y,int maxheight);

static void DrawRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0);
static void DrawRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0);

void DrawRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y)
{
	if(IsCompletelyOutside(x,y,src->width,src->height,bitmap->width,bitmap->height)) return;

	Pixel *rows=src->rows;
	int height=src->height;
	AdjustRLERows(&rows,src->width,&height,&y,bitmap->height);

	if(IsSpanCompletelyInside(x,src->width,bitmap->width)) DrawRLEBitmapRowsNoClip(bitmap,rows,src->width,height,x,y);
	else DrawRLEBitmapRows(bitmap,rows,src->width,height,x,y);
}

void DrawRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y)
{
	DrawRLEBitmapRows(bitmap,src->rows,src->width,src->height,x,y);
}



static void CompositeRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,CompositionMode mode);
static void CompositeRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,CompositionMode mode);

void CompositeRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,CompositionMode mode)
{
	if(IsCompletelyOutside(x,y,src->width,src->height,bitmap->width,bitmap->height)) return;

	Pixel *rows=src->rows;
	int height=src->height;
	AdjustRLERows(&rows,src->width,&height,&y,bitmap->height);

	if(IsSpanCompletelyInside(x,src->width,bitmap->width)) CompositeRLEBitmapRows(bitmap,rows,src->width,height,x,y,mode);
	else CompositeRLEBitmapRowsNoClip(bitmap,rows,src->width,height,x,y,mode);
}

void CompositeRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,CompositionMode mode)
{
	CompositeRLEBitmapRows(bitmap,src->rows,src->width,src->height,x,y,mode);
}

static void DrawFilledRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c);
static void DrawFilledRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c);

void DrawFilledRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c)
{
	if(IsCompletelyOutside(x,y,src->width,src->height,bitmap->width,bitmap->height)) return;

	Pixel *rows=src->rows;
	int height=src->height;
	AdjustRLERows(&rows,src->width,&height,&y,bitmap->height);

	if(IsSpanCompletelyInside(x,src->width,bitmap->width)) DrawFilledRLEBitmapRowsNoClip(bitmap,rows,src->width,height,x,y,c);
	else DrawFilledRLEBitmapRows(bitmap,rows,src->width,height,x,y,c);
}

void DrawFilledRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c)
{
	DrawFilledRLEBitmapRows(bitmap,src->rows,src->width,src->height,x,y,c);
}

static void CompositeFilledRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c,CompositionMode mode);
static void CompositeFilledRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c,CompositionMode mode);

void CompositeFilledRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c,CompositionMode mode)
{
	if(IsCompletelyOutside(x,y,src->width,src->height,bitmap->width,bitmap->height)) return;

	Pixel *rows=src->rows;
	int height=src->height;
	AdjustRLERows(&rows,src->width,&height,&y,bitmap->height);

	if(IsSpanCompletelyInside(x,src->width,bitmap->width)) CompositeFilledRLEBitmapRows(bitmap,rows,src->width,height,x,y,c,mode);
	else CompositeFilledRLEBitmapRowsNoClip(bitmap,rows,src->width,height,x,y,c,mode);
}

void CompositeFilledRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c,CompositionMode mode)
{
	CompositeFilledRLEBitmapRows(bitmap,src->rows,src->width,src->height,x,y,c,mode);
}





static inline int EmptySpanLength(Pixel spans)
{
	if(sizeof(Pixel)==4) return spans>>16;
	else return spans>>8;
}

static inline int FilledSpanLength(Pixel spans)
{
	if(sizeof(Pixel)==4) return spans&0xffff;
	else return spans&0xff;
}

static inline Pixel *SkipRLERows(Pixel *rows,int width,int height)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			int empty=EmptySpanLength(spans);
			int filled=FilledSpanLength(spans);

			rows+=filled;

			x+=empty+filled;
		}
	}
	return rows;
}

static inline void AdjustRLERows(Pixel **rows,int width,int *height,int *y,int maxheight)
{
	if(*y+*height>maxheight) *height=maxheight-*y;

	if(*y<0)
	{
		int skiprows=-*y;
		*rows=SkipRLERows(*rows,width,skiprows);
		*height-=skiprows;
		*y=0;
	}
}




static void DrawRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0)
{
	for(int y=0;y<height;y++)
	{
		Pixel *dest=BitmapPixelPointer(bitmap,x0,y+y0);
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			int empty=EmptySpanLength(spans);
			int filled=FilledSpanLength(spans);

			dest+=empty;
			x+=empty;

			int start=0;
			int num=filled;

			if(x+x0+num>bitmap->width) num=bitmap->width-(x+x0);

			if(x+x0<0)
			{
				int skippixels=-(x+x0);
				start=skippixels;
				num-=skippixels;
			}

			if(num>0) memcpy(&dest[start],&rows[start],num*sizeof(Pixel));

			dest+=filled;
			rows+=filled;
			x+=filled;
		}
	}
}

static void DrawRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0)
{
	for(int y=0;y<height;y++)
	{
		Pixel *dest=BitmapPixelPointer(bitmap,x0,y+y0);
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			int empty=EmptySpanLength(spans);
			int filled=FilledSpanLength(spans);

			dest+=empty;
			x+=empty;

			memcpy(dest,rows,filled*sizeof(Pixel));

			dest+=filled;
			rows+=filled;
			x+=filled;
		}
	}
}

static void CompositeRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,CompositionMode mode)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			// TODO: Is it worth checking the span and using NoClip?
			int filled=FilledSpanLength(spans);
			for(int i=0;i<filled;i++) CompositePixel(bitmap,x+x0+i,y+y0,rows[i],mode);

			rows+=filled;
			x+=filled;
		}
	}
}

static void CompositeRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,CompositionMode mode)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			int filled=FilledSpanLength(spans);
			for(int i=0;i<filled;i++) CompositePixelNoClip(bitmap,x+x0+i,y+y0,rows[i],mode);

			rows+=filled;
			x+=filled;
		}
	}
}

static void DrawFilledRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			int filled=FilledSpanLength(spans);
			DrawHorizontalLine(bitmap,x+x0,y+y0,filled,c);

			rows+=filled;
			x+=filled;
		}
	}
}

static void DrawFilledRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			int filled=FilledSpanLength(spans);
			DrawHorizontalLineNoClip(bitmap,x+x0,y+y0,filled,c);

			rows+=filled;
			x+=filled;
		}
	}
}

static void CompositeFilledRLEBitmapRows(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c,CompositionMode mode)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			int filled=FilledSpanLength(spans);
			CompositeHorizontalLine(bitmap,x+x0,y+y0,filled,c,mode);

			rows+=filled;
			x+=filled;
		}
	}
}

static void CompositeFilledRLEBitmapRowsNoClip(Bitmap *bitmap,Pixel *rows,int width,int height,int x0,int y0,Pixel c,CompositionMode mode)
{
	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			Pixel spans=*rows++;
			x+=EmptySpanLength(spans);

			int filled=FilledSpanLength(spans);
			CompositeHorizontalLineNoClip(bitmap,x+x0,y+y0,filled,c,mode);

			rows+=filled;
			x+=filled;
		}
	}
}
