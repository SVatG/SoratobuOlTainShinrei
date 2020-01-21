#include "../Input.h"
#include "Dingoo.h"

bool HandleInput()
{
	_sys_judge_event();
	return true;
}

uint32_t GetButtonState()
{
	uint32_t keystate=_kbd_get_key();
	uint32_t out=0;

	if(keystate&CONTROL_BUTTON_A) out|=Fire1Button;
	if(keystate&CONTROL_BUTTON_B) out|=Fire2Button;
	if(keystate&CONTROL_BUTTON_X) out|=Fire3Button;
	if(keystate&CONTROL_BUTTON_Y) out|=Fire4Button;
	if(keystate&CONTROL_BUTTON_START) out|=StartButton;
	if(keystate&CONTROL_BUTTON_SELECT) out|=SelectButton;

	if(keystate&CONTROL_TRIGGER_LEFT) out|=Fire4Button;
	if(keystate&CONTROL_TRIGGER_RIGHT) out|=Fire4Button;

	if(keystate&CONTROL_DPAD_UP) out|=UpButton;
	if(keystate&CONTROL_DPAD_DOWN) out|=DownButton;
	if(keystate&CONTROL_DPAD_LEFT) out|=LeftButton;
	if(keystate&CONTROL_DPAD_RIGHT) out|=RightButton;

	if(keystate&CONTROL_POWER) out|=QuitButton;

	return out;
}
