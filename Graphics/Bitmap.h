#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "Pixels.h"

#include <stdlib.h>
#include <stdint.h>

typedef struct Bitmap
{
	int16_t width,height;
	int32_t bytesperrow;
	Pixel *pixels;
} Bitmap;

void InitializeBitmap(Bitmap *self,int width,int height,int bytesperrow,Pixel *pixels);

void ClearBitmap(Bitmap *self);
void FillBitmap(Bitmap *self,uint32_t c);

//void ScrollBitmap(Bitmap *self,int dx,int dy);
//void ScrollRepeatingBitmap(Bitmap *self,int dx,int dy);

static inline int BytesPerRowForWidth(int width)
{
	return (width*sizeof(Pixel)+3)&~3;
}

static inline size_t SizeOfBitmapDataWithWidthAndHeight(int width,int height)
{
	return BytesPerRowForWidth(width)*height;
}

static inline Pixel *BitmapRowPointer(Bitmap *self,int y)
{
	Pixel *row=(Pixel *)((char *)self->pixels+self->bytesperrow*y);
	return row;
}

static inline Pixel *BitmapPixelPointer(Bitmap *self,int x,int y)
{
	Pixel *row=(Pixel *)((char *)self->pixels+self->bytesperrow*y);
	return &row[x];
}

#endif

