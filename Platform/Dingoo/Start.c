#include <stdint.h>

uint16_t *g_pGameDecodeBuf;
extern char _gp[];

void _InitializeResourceLoader(const uint16_t *executable);
void EntryPoint();

int GameMain(const uint16_t *executable)
{
	// The startup code should do this, but it is out of date and
	// no source is available, so do it here. gp needs to be set to
	// point to the middle of the small data section.
	__asm volatile("move $gp,%0"::"r" (_gp));

	_InitializeResourceLoader(executable);

	EntryPoint();
}

