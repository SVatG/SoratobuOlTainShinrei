#include "ParallaxBackground.h"
#include "Game.h"
#include "../Graphics/Drawing.h"

void InitializeParallaxBackground(ParallaxBackground *self,
const char *file1,const char *file2,const char *file3,const char *file4,
int start1,int start2,int start3,int start4,int end1,int end2,int end3,int end4)
{
/*	self->layers[0]=DataForResourceWithName(game.resources,file1,NULL);
	self->layers[1]=DataForResourceWithName(game.resources,file2,NULL);
	self->layers[2]=DataForResourceWithName(game.resources,file3,NULL);
	self->layers[3]=DataForResourceWithName(game.resources,file4,NULL);*/
	self->layers[0]=*(RLEBitmap **)DataForResourceWithName(game.resources,file1,NULL);
	self->layers[1]=*(RLEBitmap **)DataForResourceWithName(game.resources,file2,NULL);
	self->layers[2]=*(RLEBitmap **)DataForResourceWithName(game.resources,file3,NULL);
	self->layers[3]=*(RLEBitmap **)DataForResourceWithName(game.resources,file4,NULL);

	self->startheight[0]=start1;
	self->startheight[1]=start2;
	self->startheight[2]=start3;
	self->startheight[3]=start4;

	self->endheight[0]=end1;
	self->endheight[1]=end2;
	self->endheight[2]=end3;
	self->endheight[3]=end4;
}

void DrawParallaxBackground(ParallaxBackground *self,Bitmap *screen,int x0)
{
	const int wrapwidth=512;
	const int wrapmask=wrapwidth-1;

	Bitmap clippedscreen=*screen;

	for(int i=0;i<4;i++)
	{
		if(!self->layers[i]) continue;

		int x1=-((x0/(4-i))&wrapmask);

		// Fun hack: Prevent RLE sprites from drawing all the way to the end.
		clippedscreen.height=self->endheight[i]+1;

		if(x1+wrapwidth<screen->width)
		{
			DrawRLEBitmap(&clippedscreen,self->layers[i],x1,0);
			DrawRLEBitmap(&clippedscreen,self->layers[i],x1+wrapwidth,0);
		}
		else
		{
			DrawRLEBitmap(&clippedscreen,self->layers[i],x1,0);
		}

/*		int x1=(x0/(4-i))&wrapmask;
		int x2=(x1+screen->width-1)&wrapmask;

		if(x2>x1)
		{
			CompositePartialBitmapNoClip(screen,self->layers[i],
			0,self->startheight[i],
			x1,self->startheight[i],
			screen->width,
			self->endheight[i]-self->startheight[i]+1,
			FastTransparencyCompositionMode);
		}
		else
		{
			int firstwidth=wrapwidth-x1;

			CompositePartialBitmapNoClip(screen,self->layers[i],
			0,self->startheight[i],
			x1,self->startheight[i],
			firstwidth,
			self->endheight[i]-self->startheight[i]+1,
			FastTransparencyCompositionMode);

			CompositePartialBitmapNoClip(screen,self->layers[i],
			firstwidth,self->startheight[i],
			0,self->startheight[i],
			screen->width-firstwidth,
			self->endheight[i]-self->startheight[i]+1,
			FastTransparencyCompositionMode);
		}*/
	}
}
