#ifndef __PIXEL_DOUBLING_H__
#define __PIXEL_DOUBLING_H__

#include "../../Graphics/Bitmap.h"

static void CopyDoubledPixels(Bitmap *dest,Bitmap *src)
{
	if(src->width*2>dest->width) return;
	if(src->height*2>dest->height) return;

	for(int y=0;y<src->height;y++)
	{
		uint32_t *srcrow=BitmapRowPointer(src,y);
		uint64_t *destrow1=(uint64_t *)BitmapRowPointer(dest,2*y+0);
		uint64_t *destrow2=(uint64_t *)BitmapRowPointer(dest,2*y+1);

		for(int x=0;x<src->width;x++)
		{
			uint32_t pixel=srcrow[x];
			uint64_t doublepixel=((uint64_t)pixel<<32)|pixel;

			destrow1[x]=doublepixel;
			destrow2[x]=doublepixel;
		}
	}
}

#endif
