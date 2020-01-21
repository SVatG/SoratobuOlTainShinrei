#ifndef __PIXELS_CONSTRUCTION_PREMULTIPLIED_32_H__
#define __PIXELS_CONSTRUCTION_PREMULTIPLIED_32_H__

#include "Packing32.h"
#include "Multiply8.h"

static inline Pixel RGBA(int r,int g,int b,int a)
{
	if(a<0) a=0; if(a>0xff) a=0xff;
	r=PixelMul8(r,a);
	g=PixelMul8(g,a);
	b=PixelMul8(b,a);
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b,a);
}

static inline Pixel RGB(int r,int g,int b)
{
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b,0xff);
}

#endif
