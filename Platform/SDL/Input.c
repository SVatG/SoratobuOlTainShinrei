#include "../Input.h"

#include <SDL/SDL.h>

bool HandleInput()
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				return false;
		}
	}

	return true;
}

uint32_t GetButtonState()
{
	uint8_t *key=SDL_GetKeyState(NULL);

	uint32_t state=0;
	if(key[SDLK_LEFT]) state|=LeftButton;
	if(key[SDLK_RIGHT]) state|=RightButton;
	if(key[SDLK_UP]) state|=UpButton;
	if(key[SDLK_DOWN]) state|=DownButton;
	if(key[SDLK_z]) state|=Fire1Button;
	if(key[SDLK_x]) state|=Fire2Button;
	if(key[SDLK_y]) state|=Fire3Button;
	if(key[SDLK_LSHIFT]) state|=Fire4Button;
	if(key[SDLK_RETURN]) state|=StartButton;
	if(key[SDLK_SPACE]) state|=SelectButton;
	if(key[SDLK_ESCAPE]) state|=QuitButton;
//		if(key[SDLK_RCTRL]);

	return state;
}
