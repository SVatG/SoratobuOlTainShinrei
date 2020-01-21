#include "RLEBitmap.h"
#include "Drawing.h"

#include <stdbool.h>
#include <string.h>

static inline int MaximumRunLength()
{
	if(sizeof(Pixel)==4) return 65535;
	else return 255;
}

static inline Pixel PackRunLengths(int empty,int filled)
{
	if(sizeof(Pixel)==4) return (empty<<16)|filled;
	else return (empty<<8)|filled;
}

size_t SizeOfRLEBitmapFromBitmap(Bitmap *bitmap)
{
	return SizeOfRLEBitmapFromPartialBitmap(bitmap,0,0,bitmap->width,bitmap->height);
}

size_t SizeOfRLEBitmapFromPartialBitmap(Bitmap *bitmap,int x0,int y0,int width,int height)
{
	int maxlen=MaximumRunLength();

	size_t numpixels=0;

	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			int emptystart=x;
			while(x<width && x-emptystart<maxlen)
			{
				Pixel p=ReadPixel(bitmap,x+x0,y+y0);
				if(!IsPixelTransparent(p)) break;
				x++;
			}
			//int empty=x-emptystart;

			int filledstart=x;
			while(x<width && x-filledstart<maxlen)
			{
				Pixel p=ReadPixel(bitmap,x+x0,y+y0);
				if(IsPixelTransparent(p)) break;
				x++;
			}
			int filled=x-filledstart;

			numpixels+=1+filled;
		}
	}

	return sizeof(RLEBitmap)+numpixels*sizeof(Pixel);
}

void InitializeRLEBitmapFromBitmap(RLEBitmap *self,Bitmap *bitmap)
{
	InitializeRLEBitmapFromPartialBitmap(self,bitmap,0,0,bitmap->width,bitmap->height);
}

void InitializeRLEBitmapFromPartialBitmap(RLEBitmap *self,Bitmap *bitmap,int x0,int y0,int width,int height)
{
	int maxlen=MaximumRunLength();

	self->width=width;
	self->height=height;
	Pixel *ptr=self->rows;

	for(int y=0;y<height;y++)
	{
		int x=0;
		while(x<width)
		{
			int emptystart=x;
			while(x<width && x-emptystart<maxlen)
			{
				Pixel p=ReadPixel(bitmap,x+x0,y+y0);
				if(!IsPixelTransparent(p)) break;
				x++;
			}
			int empty=x-emptystart;

			int filledstart=x;
			while(x<width && x-filledstart<maxlen)
			{
				Pixel p=ReadPixel(bitmap,x+x0,y+y0);
				if(IsPixelTransparent(p)) break;
				x++;
			}
			int filled=x-filledstart;

			*ptr++=PackRunLengths(empty,filled);
			memcpy(ptr,BitmapPixelPointer(bitmap,filledstart+x0,y+y0),filled*sizeof(Pixel));
			ptr+=filled;
		}
	}
}

