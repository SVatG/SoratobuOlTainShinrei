#include "../Input.h"
#include "PPAPI.h"

static uint32_t state;

void _InitializeInput()
{
	state=0;

	ppb_inputevent->RequestInputEvents(instance_id,
	PP_INPUTEVENT_CLASS_KEYBOARD|PP_INPUTEVENT_CLASS_MOUSE);
}

bool HandleInput()
{
	return true;
}

uint32_t GetButtonState()
{
	return state;
}

void Instance_DidChangeFocus(PP_Instance instance,PP_Bool hasfocus)
{
	if(!hasfocus) state=0;
}

PP_Bool InputEvent_HandleInputEvent(PP_Instance instance,PP_Resource event)
{
	switch(ppb_inputevent->GetType(event))
	{
		case PP_INPUTEVENT_TYPE_KEYDOWN:
		{
			uint32_t code=ppb_keyboardinputevent->GetKeyCode(event);
			switch(code)
			{
				case 37: state|=LeftButton; break;
				case 38: state|=UpButton; break;
				case 39: state|=RightButton; break;
				case 40: state|=DownButton; break;
				case 'Z': state|=Fire1Button; break;
				case 'X': state|=Fire2Button; break;
				case 'C': state|=Fire3Button; break;
				case 16: state|=Fire4Button; break;
				case 13: state|=StartButton; break;
				case ' ': state|=SelectButton; break;
				case 27: state|=QuitButton; break;
			}
		}
		break;

		case PP_INPUTEVENT_TYPE_KEYUP:
		{
			uint32_t code=ppb_keyboardinputevent->GetKeyCode(event);
			switch(code)
			{
				case 37: state&=~LeftButton; break;
				case 38: state&=~UpButton; break;
				case 39: state&=~RightButton; break;
				case 40: state&=~DownButton; break;
				case 'Z': state&=~Fire1Button; break;
				case 'X': state&=~Fire2Button; break;
				case 'C': state&=~Fire3Button; break;
				case 16: state&=~Fire4Button; break;
				case 13: state&=~StartButton; break;
				case ' ': state&=~SelectButton; break;
				case 27: state&=~QuitButton; break;
			}
		}
		break;
	}

	return true;
}
