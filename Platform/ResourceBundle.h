#ifndef __RESOURCE_BUNDLE_H__
#define __RESOURCE_BUNDLE_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct ResourceBundleEntry
{
	uint32_t offset,size,type;
	char name[32];
} ResourceBundleEntry;

typedef struct ResourceBundle
{
	union
	{
		struct ResourceBundle *next;
		uint8_t magic[8];
	} head;

	uint32_t numentries;
	char name[128];

	ResourceBundleEntry entries[0];
} ResourceBundle;

bool FixUpResourceBundle(ResourceBundle *self,const char *filename);

void *DataForResourceWithName(ResourceBundle *self,const char *name,size_t *sizeptr);

#endif
