#ifndef __PIXELS_RGBA_32_H__
#define __PIXELS_RGBA_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelRedShift FirstByteShift
#define PixelGreenShift SecondByteShift
#define PixelBlueShift ThirdByteShift
#define PixelAlphaShift FourthByteShift

#include "Construction32.h"
#include "Composition32.h"

#endif
