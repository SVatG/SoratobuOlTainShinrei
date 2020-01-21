#ifndef __SCREEN_H__
#define __SCREEN_H__

#include "../Graphics/Bitmap.h"

int ScreenWidth();
int ScreenHeight();

Bitmap *StartScreenUpdates();
void EndScreenUpdates();

#endif
