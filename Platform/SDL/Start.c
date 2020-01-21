#include <SDL/SDL.h>
#include <stdbool.h>

#ifdef __APPLE__
#include <limits.h>
#include <Carbon/Carbon.h>
#endif

void _InitializeScreen();
void EntryPoint();

int main(int argc,char **argv)
{
	#ifdef __APPLE__
	char resourcedir[PATH_MAX];
	CFURLRef url=CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	CFURLGetFileSystemRepresentation(url,1,(UInt8 *)resourcedir,PATH_MAX);
	chdir(resourcedir);
	CFRelease(url);
	#endif

	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)<0) return 1;

	_InitializeScreen();

	EntryPoint();

	SDL_Quit();

	return 0;
}
