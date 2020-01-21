#ifndef __SIMPLE_ALLOCATOR_H__
#define __SIMPLE_ALLOCATOR_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct SimpleAllocator
{
	size_t nummembers,maxmembers,membersize;
	uint8_t *members;
	uint16_t *queue;
} SimpleAllocator;

#define SizeOfSimpleAllocatorStorage(membersize,maxmembers) ((membersize)*(maxmembers)+2*(maxmembers))

void InitializeSimpleAllocator(SimpleAllocator *self,size_t membersize,size_t maxmembers,void *storage);

void *AllocMember(SimpleAllocator *self);
void FreeMember(SimpleAllocator *self,void *member);

static inline bool IsSimpleAllocatorFull(SimpleAllocator *self)
{
	return self->nummembers>=self->maxmembers;
}

static inline size_t AllocatorMemberSize(SimpleAllocator *self)
{
	return self->membersize;
}

#endif

