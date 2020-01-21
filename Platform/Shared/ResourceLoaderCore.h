#ifndef __RESOURCE_LOADER_CORE_H__
#define __RESOURCE_LOADER_CORE_H__

#include <string.h>

#include "../ResourceLoader.h"

static void FreeResourceBundleList(ResourceBundle *bundle);

typedef struct ResourceLoaderCore
{
	ResourceBundle *currentresources;
	ResourceBundle *oldresources;
	ResourceBundle **buildlink;

	int numparsers;
	ResourceIdentificationFunction *identifyfunctions[32];
	ResourceParsingFunction *parsefunctions[32];
} ResourceLoaderCore;

typedef ResourceBundle *ResourceLoadingFunction(struct ResourceLoader *loader,const char *path);

static void InitializeResourceLoaderCore(ResourceLoaderCore *self)
{
	self->currentresources=NULL;
	self->oldresources=NULL;
	self->buildlink=NULL;

	self->numparsers=0;
}

static void CleanupResourceLoaderCore(ResourceLoaderCore *self)
{
	FreeResourceBundleList(self->currentresources);
	FreeResourceBundleList(self->oldresources);
}

static void AddResourceParserCore(ResourceLoaderCore *self,
ResourceIdentificationFunction *identifyfunc,ResourceParsingFunction *parsefunc)
{
	if(self->numparsers>=sizeof(self->parsefunctions)/sizeof(self->parsefunctions[0])) return;

	self->identifyfunctions[self->numparsers]=identifyfunc;
	self->parsefunctions[self->numparsers]=parsefunc;
	self->numparsers++;
}

static void StartRequestingResourceBundlesCore(ResourceLoaderCore *self)
{
	self->oldresources=self->currentresources;
	self->currentresources=NULL;
	self->buildlink=&self->currentresources;
}

static void RequestResourceBundleCore(ResourceLoaderCore *self,const char *path,
ResourceLoadingFunction *loadfunc)
{
	// TODO: Error handling.

	ResourceBundle **link=&self->oldresources;
	ResourceBundle *bundle=self->oldresources;
	while(bundle)
	{
		if(strncmp(bundle->name,path,sizeof(bundle->name))==0)
		{
			*link=bundle->head.next;
			bundle->head.next=NULL;
			*self->buildlink=bundle;
			self->buildlink=&bundle->head.next;
			return;
		}

		link=&bundle->head.next;
		bundle=bundle->head.next;
	}

	ResourceBundle *newbundle=loadfunc((void *)self,path);
	if(!newbundle) return;

	*self->buildlink=newbundle;
	self->buildlink=&newbundle->head.next;
}

static void EndRequestingResourceBundlesCore(ResourceLoaderCore *self)
{
	FreeResourceBundleList(self->oldresources);
	self->oldresources=NULL;
}

static ResourceBundle *ResourceBundleListCore(ResourceLoaderCore *self)
{
	return self->currentresources;
}

static void FreeResourceBundleList(ResourceBundle *bundle)
{
	while(bundle)
	{
		ResourceBundle *next=bundle->head.next;
		free(bundle);
		bundle=next;
	}
}

#endif
