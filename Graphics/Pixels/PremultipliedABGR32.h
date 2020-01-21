#ifndef __PIXELS_PREMULTIPLIED_ABGR_32_H__
#define __PIXELS_PREMULTIPLIED_ABGR_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelAlphaShift FirstByteShift
#define PixelBlueShift SecondByteShift
#define PixelGreenShift ThirdByteShift
#define PixelRedShift FourthByteShift

#include "ConstructionPremultiplied32.h"
#include "CompositionPremultiplied32.h"

#endif
