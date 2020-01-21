#ifndef __PIXELS_PACKING_32_H__
#define __PIXELS_PACKING_32_H__

#include <stdint.h>
#include <stdbool.h>

#define RedPixelBits 8
#define GreenPixelBits 8
#define BluePixelBits 8
#define AlphaPixelBits 8

static inline Pixel AssemblePixel(int r,int g,int b,int a)
{
	return (r<<PixelRedShift)|(g<<PixelGreenShift)|(b<<PixelBlueShift)|(a<<PixelAlphaShift);
}

static inline int ExtractRed(Pixel c) { return (c>>PixelRedShift)&0xff; }
static inline int ExtractGreen(Pixel c) { return (c>>PixelGreenShift)&0xff; }
static inline int ExtractBlue(Pixel c) { return (c>>PixelBlueShift)&0xff; }
static inline int ExtractAlpha(Pixel c) { return (c>>PixelAlphaShift)&0xff; }

static inline int IsPixelTransparent(Pixel c) { return (c&(0x80<<PixelAlphaShift))?false:true; }

#endif
