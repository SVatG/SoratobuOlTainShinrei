#include "ResourceLoader.h"
#include "Shared/ResourceLoaderCore.h"

#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>

static ResourceBundle *AllocAndLoadBundleAtPath(ResourceLoader *self,const char *path);

struct ResourceLoader
{
	ResourceLoaderCore core;
};

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
	// Add ".laser" extension to path.
	size_t length=strlen(path);
	char archivepath[length+7];
	memcpy(&archivepath[0],path,length);
	memcpy(&archivepath[length],".laser",7);

	// Find file size.
	struct stat st;
	if(stat(archivepath,&st)!=0) return NULL;
	if((st.st_mode&S_IFMT)!=S_IFREG) return NULL;

	// Allocate memory.
	void *bytes=malloc(st.st_size);
	if(!bytes) return NULL;

	// Open file and read contents.
	FILE *fh=fopen(archivepath,"rb");
	if(!fh) { free(bytes); return NULL; }

	if(fread(bytes,1,st.st_size,fh)!=st.st_size) { free(bytes); fclose(fh); return NULL; }
	fclose(fh);

	ResourceBundle *bundle=bytes;

	if(!FixUpResourceBundle(bundle,path)) { free(bytes); return NULL; }

	return bundle;
}
