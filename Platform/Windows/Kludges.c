#include <stdlib.h>

size_t strnlen(const char *string,size_t maxlen)
{
	for(int len=0;len<maxlen;len++) if(string[len]==0) return len;
	return maxlen;
}
