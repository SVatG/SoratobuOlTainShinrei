#ifndef __PARALLAX_BACKGROUND_H__
#define __PARALLAX_BACKGROUND_H__

#include "../Graphics/RLEBitmap.h"

typedef struct ParallaxBackground
{
	RLEBitmap *layers[4];
	int startheight[4];
	int endheight[4];
} ParallaxBackground;

void InitializeParallaxBackground(ParallaxBackground *self,
const char *file1,const char *file2,const char *file3,const char *file4,
int start1,int start2,int start3,int start4,int end1,int end2,int end3,int end4);

void DrawParallaxBackground(ParallaxBackground *self,Bitmap *screen,int x);

#endif
