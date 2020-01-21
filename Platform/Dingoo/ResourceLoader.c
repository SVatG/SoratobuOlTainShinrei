#include "../ResourceLoader.h"
#include "../Shared/ResourceLoaderCore.h"

#include "Dingoo.h"
#include <stdlib.h>

static ResourceBundle *AllocAndLoadBundleAtPath(ResourceLoader *self,const char *path);




struct ResourceLoader
{
	ResourceLoaderCore core;
};

typedef struct ResourceBundleHeader
{
	uint32_t offset,size;
	char name[128];
} ResourceBundleHeader;

int numbundles=0;
ResourceBundleHeader *bundleheaders=NULL;
FILE *executablehandle=NULL;
int val1,val2;

void _InitializeResourceLoader(const uint16_t *executable)
{
	// Open executable file.
	executablehandle=fsys_fopenW(executable,(uint16_t[]){'r','b',0});
	if(!executablehandle) return;

	// Seek to end and check if there is a footer for attached bundles.
	fsys_fseek(executablehandle,-14,2);
	struct { uint32_t numbundles; char magic[10]; } footer;
	if(fsys_fread(&footer,1,14,executablehandle)!=14) { fsys_fclose(executablehandle); executablehandle=NULL; return; }

	if(memcmp(footer.magic,"MaGiCaNnOn",10)!=0) { fsys_fclose(executablehandle); executablehandle=NULL; return; }

	numbundles=footer.numbundles;

	// Allocate and read bundle directory.
	int dirsize=numbundles*sizeof(ResourceBundleHeader);
	bundleheaders=malloc(dirsize);
	if(!bundleheaders) { fsys_fclose(executablehandle); executablehandle=NULL; return; }

	fsys_fseek(executablehandle,-dirsize-14,2);
	if(fsys_fread(bundleheaders,1,dirsize,executablehandle)!=dirsize) { free(bundleheaders); fsys_fclose(executablehandle); executablehandle=NULL; return; }
}



ResourceLoader *AllocResourceLoader()
{
	ResourceLoader *self=malloc(sizeof(ResourceLoader));

	InitializeResourceLoaderCore(&self->core);

	return self;
}

void FreeResourceLoader(ResourceLoader *self)
{
	if(!self) return;

	CleanupResourceLoaderCore(&self->core);

	free(self);
}

void AddResourceParser(ResourceLoader *self,
ResourceIdentificationFunction *identifyfunc,ResourceParsingFunction *parsefunc)
{ AddResourceParserCore(&self->core,identifyfunc,parsefunc); }

void StartRequestingResourceBundles(ResourceLoader *self)
{ StartRequestingResourceBundlesCore(&self->core); }

void RequestResourceBundle(ResourceLoader *self,const char *path)
{ RequestResourceBundleCore(&self->core,path,AllocAndLoadBundleAtPath); }

void EndRequestingResourceBundles(ResourceLoader *self)
{ EndRequestingResourceBundlesCore(&self->core); }

int ResourceLoadingProgress(ResourceLoader *self)
{
	return 100;
}

ResourceBundle *ResourceBundleList(ResourceLoader *self)
{ return ResourceBundleListCore(&self->core); }

static ResourceBundle *AllocAndLoadBundleAtPath(ResourceLoader *self,const char *path)
{
	// If we failed to find attached bundles at initialization, give up.
	if(!executablehandle) return NULL;

	// Search through the directory for the requested bundle.
	for(int i=0;i<numbundles;i++)
	{
		if(strncmp(path,bundleheaders[i].name,sizeof(bundleheaders[i].name))==0)
		{
			fsys_fseek(executablehandle,bundleheaders[i].offset,0);

			// Allocate memory.
			uint32_t size=bundleheaders[i].size;
			void *bytes=malloc(size);
			if(!bytes) return NULL;

			if(fsys_fread(bytes,1,size,executablehandle)!=size) { free(bytes); return NULL; }

			ResourceBundle *bundle=bytes;
			if(!FixUpResourceBundle(bundle,path)) { free(bytes); return NULL; }

			return bundle;
		}
	}

	return NULL;
}
