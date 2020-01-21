#ifndef __PIXELS_PREMULTIPLIED_RGBA_32_H__
#define __PIXELS_PREMULTIPLIED_RGBA_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelRedShift FirstByteShift
#define PixelGreenShift SecondByteShift
#define PixelBlueShift ThirdByteShift
#define PixelAlphaShift FourthByteShift

#include "ConstructionPremultiplied32.h"
#include "CompositionPremultiplied32.h"

#endif
