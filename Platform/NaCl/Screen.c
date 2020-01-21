#include "../Screen.h"
#include "PPAPI.h"

#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>

Bitmap screenbitmap;
PP_Resource context=0;
PP_Resource pixels=0;
sem_t screenlock;

void _InitializeScreen()
{
	struct PP_Size size=PP_MakeSize(ScreenWidth(),ScreenHeight());
	pixels=ppb_imagedata->Create(instance_id,PP_IMAGEDATAFORMAT_BGRA_PREMUL,&size,true);

	struct PP_ImageDataDesc desc;
	ppb_imagedata->Describe(pixels,&desc);		

	void *bytes=ppb_imagedata->Map(pixels);

	InitializeBitmap(&screenbitmap,desc.size.width,desc.size.height,desc.stride,bytes);

	sem_init(&screenlock,0,1);
}

int ScreenWidth() { return 640; }

int ScreenHeight() { return 480; }

Bitmap *StartScreenUpdates()
{
	sem_wait(&screenlock);

	return &screenbitmap;
}

static void StartFlush(void *userdata,int32_t result);
static void EndFlush(void *userdata,int32_t result);

void EndScreenUpdates()
{
	if(!context) return;

	ppb_core->CallOnMainThread(0,PP_MakeCompletionCallback(StartFlush,NULL),0);
}

static void StartFlush(void *userdata,int32_t result)
{
	struct PP_Point point=PP_MakePoint(0,0);
	ppb_graphics2d->PaintImageData(context,pixels,&point,NULL);

	int res=ppb_graphics2d->Flush(context,PP_MakeCompletionCallback(EndFlush,NULL));
	if(res!=PP_OK_COMPLETIONPENDING) sem_post(&screenlock);
}

static void EndFlush(void *userdata,int32_t result)
{
	sem_post(&screenlock);
}



void Instance_DidChangeView(PP_Instance instance,const struct PP_Rect *position,const struct PP_Rect *clip)
{
	if(context) ppb_core->ReleaseResource(context);

	context=ppb_graphics2d->Create(instance,&position->size,false);
	ppb_instance->BindGraphics(instance,context);
}

