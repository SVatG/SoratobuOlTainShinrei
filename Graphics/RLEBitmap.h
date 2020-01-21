#ifndef __RLE_BITMAP_H__
#define __RLE_BITMAP_H__

#include "Pixels.h"
#include "Bitmap.h"
#include "Drawing.h"

#include <stdlib.h>

typedef struct RLEBitmap
{
	int width,height;
	Pixel rows[0];
} RLEBitmap;

size_t BytesNeededForRLEBitmap(Bitmap *self);

size_t SizeOfRLEBitmapFromBitmap(Bitmap *bitmap);
size_t SizeOfRLEBitmapFromPartialBitmap(Bitmap *bitmap,int x,int y,int width,int height);

void InitializeRLEBitmapFromBitmap(RLEBitmap *self,Bitmap *bitmap);
void InitializeRLEBitmapFromPartialBitmap(RLEBitmap *self,Bitmap *bitmap,int x,int y,int width,int height);

void DrawRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y);
void DrawRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y);
void CompositeRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,CompositionMode mode);
void CompositeRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,CompositionMode mode);

void DrawFilledRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c);
void DrawFilledRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c);
void CompositeFilledRLEBitmap(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c,CompositionMode mode);
void CompositeFilledRLEBitmapNoClip(Bitmap *bitmap,RLEBitmap *src,int x,int y,Pixel c,CompositionMode mode);

#endif
