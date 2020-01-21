#include "SimpleAllocator.h"

#include <assert.h>

void InitializeSimpleAllocator(SimpleAllocator *self,size_t membersize,size_t maxmembers,void *storage)
{
	self->nummembers=0;
	self->maxmembers=maxmembers;
	self->membersize=membersize;

	self->queue=storage;
	self->members=(uint8_t *)storage+2*maxmembers;

	for(int i=0;i<maxmembers;i++) self->queue[i]=maxmembers-1-i;
}

void FreeSimpleAllocator(SimpleAllocator *self)
{
	assert(self->nummembers==0);
	free(self);
}

void *AllocMember(SimpleAllocator *self)
{
	if(IsSimpleAllocatorFull(self)) return NULL;

	self->nummembers++;
	int n=self->queue[self->maxmembers-self->nummembers];

	return &self->members[n*self->membersize];
}

void FreeMember(SimpleAllocator *self,void *member)
{
	assert(self->nummembers>0);

	uintptr_t n=((uintptr_t)member-(uintptr_t)self->members)/self->membersize;

	self->queue[self->maxmembers-self->nummembers]=n;
	self->nummembers--;
}

