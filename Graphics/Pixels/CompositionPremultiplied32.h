#ifndef __PIXELS_COMPOSITION_PREMULTIPLIED_32_H__
#define __PIXELS_COMPOSITION_PREMULTIPLIED_32_H__

#include "Packing32.h"
#include "Multiply8.h"

#include <stdint.h>

static Pixel SourceOverCompositionMode(Pixel below,Pixel above)
{
	int r1=ExtractRed(below);
	int g1=ExtractGreen(below);
	int b1=ExtractBlue(below);
	int a1=ExtractAlpha(below);
	int r2=ExtractRed(above);
	int g2=ExtractGreen(above);
	int b2=ExtractBlue(above);
	int a2=ExtractAlpha(above);

	int r=r2+PixelMul8(255-a2,r1);
	int g=g2+PixelMul8(255-a2,g1);
	int b=b2+PixelMul8(255-a2,b1);
	int a=a2+PixelMul8(255-a2,a1);

	return AssemblePixel(r,g,b,a);
}

static Pixel FastTransparencyCompositionMode(Pixel below,Pixel above)
{
	if(above&(0x80<<PixelAlphaShift)) return above;
	else return below;
}

static Pixel FastHalfTransparentCompositionMode(Pixel below,Pixel above)
{
	if(above&(0x80<<PixelAlphaShift))
	{
		Pixel halfabove=(above>>1)&0x7f7f7f7f;
		Pixel halfbelow=(below>>1)&0x7f7f7f7f;
		Pixel carry=(above&below&0x01010101);

		return halfabove+halfbelow+carry;
	}
	else return below;
}

static Pixel DummyCompositionMode(Pixel below,Pixel above)
{
	return above;
}

#endif
