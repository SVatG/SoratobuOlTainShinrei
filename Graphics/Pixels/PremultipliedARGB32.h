#ifndef __PIXELS_PREMULTIPLIED_ARGB_32_H__
#define __PIXELS_PREMULTIPLIED_ARGB_32_H__

typedef uint32_t Pixel;

#include "Endianness.h"
#define PixelAlphaShift FirstByteShift
#define PixelRedShift SecondByteShift
#define PixelGreenShift ThirdByteShift
#define PixelBlueShift FourthByteShift

#include "ConstructionPremultiplied32.h"
#include "CompositionPremultiplied32.h"

#endif
