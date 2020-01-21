#ifndef __PIXELS_ARGB_16_H__
#define __PIXELS_ARGB_16_H__

typedef uint16_t Pixel;

#define RedPixelBits 5
#define GreenPixelBits 5
#define BluePixelBits 5
#define AlphaPixelBits 1

static inline Pixel AssemblePixel(int r,int g,int b,int a)
{
	return ((a&0x80)<<8)|((r&0xf8)<<7)|((g&0xf8)<<2)|((b&0xf8)>>2);
}

static inline int ExtractRed(Pixel c) { return ((c&0x7c00)*0x21)>>12; }
static inline int ExtractGreen(Pixel c) { return ((c&0x3e0)*0x21)>>7; }
static inline int ExtractBlue(Pixel c) { return ((c&0x1f)*0x21)>>2; }
static inline int ExtractAlpha(Pixel c) { return ((c>>15)&0x01)*0xff; }

static inline int IsPixelTransparent(Pixel c) { return (c&0x8000)?false:true; }

static inline Pixel RGBA(int r,int g,int b,int a)
{
	if(a<0) a=0; if(a>0xff) a=0xff;
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b,a);
}

static inline Pixel RGB(int r,int g,int b)
{
	if(r<0) r=0; if(r>0xff) r=0xff;
	if(g<0) g=0; if(g>0xff) g=0xff;
	if(b<0) b=0; if(b>0xff) b=0xff;
	return AssemblePixel(r,g,b,0xff);
}

static Pixel SourceOverCompositionMode(Pixel below,Pixel above)
{
	if(above&0x8000) return above;
	else return below;
}

static Pixel FastTransparencyCompositionMode(Pixel below,Pixel above)
{
	if(above&0x8000) return above;
	else return below;
}

static Pixel FastHalfTransparentCompositionMode(Pixel below,Pixel above)
{
	if(above&0x8000)
	{
		Pixel halfabove=(above>>1)&0x3def;
		Pixel halfbelow=(below>>1)&0x3def;
		Pixel carry=(above&below&0x8421);

		return halfabove+halfbelow+carry;
	}
	else return below;
}

static Pixel DummyCompositionMode(Pixel below,Pixel above)
{
	return above;
}

#endif
