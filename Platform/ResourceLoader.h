#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include "ResourceBundle.h"

#include <stdbool.h>

typedef struct ResourceLoader ResourceLoader;

typedef size_t ResourceIdentificationFunction(const void *bytes,size_t length);
typedef bool ResourceParsingFunction(const void *bytes,size_t length,void *destination);

ResourceLoader *AllocResourceLoader();
void FreeResourceLoader(ResourceLoader *self);

void AddResourceParser(ResourceLoader *self,
ResourceIdentificationFunction *identifyfunc,ResourceParsingFunction *parsefunc);

void StartRequestingResourceBundles(ResourceLoader *self);
void RequestResourceBundle(ResourceLoader *self,const char *path);
void EndRequestingResourceBundles(ResourceLoader *self);

int ResourceLoadingProgress(ResourceLoader *self);

ResourceBundle *ResourceBundleList(ResourceLoader *self);

#endif
