#ifndef __PIXELS_ARGB_32_H__
#define __PIXELS_ARGB_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelAlphaShift FirstByteShift
#define PixelRedShift SecondByteShift
#define PixelGreenShift ThirdByteShift
#define PixelBlueShift FourthByteShift

#include "Construction32.h"
#include "Composition32.h"

#endif
