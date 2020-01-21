#ifndef __PIXELS_BGRA_32_H__
#define __PIXELS_BGRA_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelBlueShift FirstByteShift
#define PixelGreenShift SecondByteShift
#define PixelRedShift ThirdByteShift
#define PixelAlphaShift FourthByteShift

#include "Construction32.h"
#include "Composition32.h"

#endif
