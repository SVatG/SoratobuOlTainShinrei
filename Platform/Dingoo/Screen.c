#include "../Screen.h"
#include "Dingoo.h"

static Bitmap screenbitmap;

int ScreenWidth() { return 320; }

int ScreenHeight() { return 240; }

Bitmap *StartScreenUpdates()
{
	uint16_t *screen=_lcd_get_frame();
	InitializeBitmap(&screenbitmap,320,240,640,screen);
	return &screenbitmap;
}

void EndScreenUpdates()
{
	_lcd_set_frame();
}

