#ifndef __PIXELS_RGB_16_H__
#define __PIXELS_RGB_16_H__

typedef uint16_t Pixel;

#define RedPixelBits 5
#define GreenPixelBits 6
#define BluePixelBits 5
#define AlphaPixelBits 0

#define TransparentPixel 0xf81f

static inline Pixel AssemblePixel(int r,int g,int b)
{
	return ((r&0xf8)<<8)|((g&0xfc)<<3)|((b&0xf8)>>3);
}

static inline int ExtractRed(Pixel c) { return ((c&0xf800)*0x21)>>13; }
static inline int ExtractGreen(Pixel c) { return ((c&0x07e0)*0x41)>>9; }
static inline int ExtractBlue(Pixel c) { return ((c&0x001f)*0x21)>>2; }
static inline int ExtractAlpha(Pixel c) { return c==TransparentPixel?0:0xff; }

static inline int IsPixelTransparent(Pixel c) { return c==TransparentPixel; }

static inline Pixel RGBA(int r,int g,int b,int a)
{
	if(a<0x80) return TransparentPixel;
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b);
}

static inline Pixel RGB(int r,int g,int b)
{
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b);
}

static Pixel SourceOverCompositionMode(Pixel below,Pixel above)
{
	if(above!=TransparentPixel) return above;
	else return below;
}

static Pixel FastTransparencyCompositionMode(Pixel below,Pixel above)
{
	if(above!=TransparentPixel) return above;
	else return below;
}

static Pixel FastHalfTransparentCompositionMode(Pixel below,Pixel above)
{
	if(above!=TransparentPixel)
	{
		Pixel halfabove=(above>>1)&0x7bef;
		Pixel halfbelow=(below>>1)&0x7bef;
		Pixel carry=(above&below&0x0821);

		return halfabove+halfbelow+carry;
	}
	else return below;
}

static Pixel DummyCompositionMode(Pixel below,Pixel above)
{
	return above;
}

#endif

