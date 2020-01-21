#ifndef __PIXELS_ABGR_32_H__
#define __PIXELS_ABGR_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelAlphaShift FirstByteShift
#define PixelBlueShift SecondByteShift
#define PixelGreenShift ThirdByteShift
#define PixelRedShift FourthByteShift

#include "Construction32.h"
#include "Composition32.h"

#endif
