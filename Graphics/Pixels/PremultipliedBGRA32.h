#ifndef __PIXELS_PREMULTIPLIED_BGRA_32_H__
#define __PIXELS_PREMULTIPLIED_BGRA_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelBlueShift FirstByteShift
#define PixelGreenShift SecondByteShift
#define PixelRedShift ThirdByteShift
#define PixelAlphaShift FourthByteShift

#include "ConstructionPremultiplied32.h"
#include "CompositionPremultiplied32.h"

#endif
