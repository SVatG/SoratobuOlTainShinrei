#include <stdbool.h>
#include <stdio.h>

#include "Game.h"
#include "FrameRateManager.h"
#include "../Platform/Input.h"
#include "../Platform/Screen.h"

void EntryPoint()
{
	FrameRateManager framerate;
	InitializeFrameRateManager(&framerate,60,0);

	InitializeGame();

	for(;;)
	{
		StartGame();

		bool canquit=false;

		for(;;)
		{
			if(!HandleInput()) return;
			uint32_t state=GetButtonState();
			if(state&QuitButton) return;

			if(canquit)
			if(state&Fire1Button) break;

			if(IsGameOver())
			if(!(state&Fire1Button)) canquit=true;
		
			BeginFrame(&framerate);
			SleepUntilFrameStart(&framerate);

			RunGame(IsLeftPressed(state),IsRightPressed(state),
			IsUpPressed(state),IsDownPressed(state),IsFire1Pressed(state),
			IsFire4Pressed(state),IsFire2Pressed(state));

			//if(!ShouldSkipThisFrame(&framerate))
			{
				Bitmap *screen=StartScreenUpdates();
				DrawGame(screen);
				EndScreenUpdates();
			}
		}
	}

	return;
}

