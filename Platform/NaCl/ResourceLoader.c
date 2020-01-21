#include "../ResourceLoader.h"
#include "../Shared/ResourceLoaderCore.h"
#include "PPAPI.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

static ResourceBundle *AddRequest(ResourceLoader *self,const char *path);
static void StartRequests(void *userdata,int32_t result);

struct LoadRequest
{
	ResourceLoader *self;
	PP_Resource loader;
	uint8_t *bytes;
	size_t size;
	char path[1024];
};

struct ResourceLoader
{
	ResourceLoaderCore core;

	int numrequests,finishedrequests;
	struct LoadRequest requests[32];

	sem_t progresslock;
	int progress;
};

ResourceLoader *AllocResourceLoader()
{
	ResourceLoader *self=malloc(sizeof(ResourceLoader));

	InitializeResourceLoaderCore(&self->core);
	self->numrequests=0;
	self->finishedrequests=0;

	sem_init(&self->progresslock,0,1);

	return self;
}

void FreeResourceLoader(ResourceLoader *self)
{
	if(!self) return;
	if(self->numrequests) return; // TODO: Handle this gracefully rather than leaking!

	CleanupResourceLoaderCore(&self->core);

	sem_destroy(&self->progresslock);

	free(self);
}

void AddResourceParser(ResourceLoader *self,
ResourceIdentificationFunction *identifyfunc,ResourceParsingFunction *parsefunc)
{
	AddResourceParserCore(&self->core,identifyfunc,parsefunc);
}

void StartRequestingResourceBundles(ResourceLoader *self)
{
	StartRequestingResourceBundlesCore(&self->core);
}

void RequestResourceBundle(ResourceLoader *self,const char *path)
{
	RequestResourceBundleCore(&self->core,path,AddRequest);
}

void EndRequestingResourceBundles(ResourceLoader *self)
{
	EndRequestingResourceBundlesCore(&self->core);

	self->finishedrequests=0;
	self->progress=0;

	if(self->numrequests)
	ppb_core->CallOnMainThread(0,PP_MakeCompletionCallback(StartRequests,self),0);
}

int ResourceLoadingProgress(ResourceLoader *self)
{
	if(!self->numrequests) return 100;

	return self->progress;
}




ResourceBundle *ResourceBundleList(ResourceLoader *self)
{
	if(self->numrequests) return NULL;
	return ResourceBundleListCore(&self->core);
}




static void StartLoadingURL(void *userdata,int32_t result);
static void FinishLoadingURL(void *userdata,int32_t result);
static void CheckDownloadProgress(void *userdata,int32_t result);

static ResourceBundle *AddRequest(ResourceLoader *self,const char *path)
{
	if(self->numrequests>=sizeof(self->requests)/sizeof(self->requests[0]))
	{
		// TODO: Error handling?
		return NULL;
	}

	self->requests[self->numrequests].self=self;
	self->requests[self->numrequests].loader=0;
	strcpy(self->requests[self->numrequests].path,path);

	self->numrequests++;

	return NULL;
}

#include <time.h>
static void StartRequests(void *userdata,int32_t result)
{
	ResourceLoader *self=userdata;

	// Start downloads for all requests.
	for(int i=0;i<self->numrequests;i++)
	{
		size_t length=strlen(self->requests[i].path);
		char archivepath[length+7+16];

		strcpy(archivepath,self->requests[i].path);
		strcat(archivepath,".laser");
sprintf(archivepath+strlen(archivepath),"?%d",time(NULL));

		struct PP_Var url=ppb_var->VarFromUtf8(module_id,archivepath,strlen(archivepath));
		struct PP_Var method=ppb_var->VarFromUtf8(module_id,"GET",3);

		PP_Resource request=ppb_urlrequestinfo->Create(instance_id);
		ppb_urlrequestinfo->SetProperty(request,PP_URLREQUESTPROPERTY_URL,url);
		ppb_urlrequestinfo->SetProperty(request,PP_URLREQUESTPROPERTY_METHOD,method);
		ppb_urlrequestinfo->SetProperty(request,PP_URLREQUESTPROPERTY_RECORDDOWNLOADPROGRESS,PP_MakeBool(true));

		ppb_var->Release(url);
		ppb_var->Release(method);

		PP_Resource loader=ppb_urlloader->Create(instance_id);
		self->requests[i].loader=loader;

		ppb_urlloader->Open(loader,request,PP_MakeCompletionCallback(StartLoadingURL,&self->requests[i]));

		ppb_core->ReleaseResource(request);
	}

	// Start progress updating callback.
	ppb_core->CallOnMainThread(0,PP_MakeCompletionCallback(CheckDownloadProgress,self),0);
}

static void StartLoadingURL(void *userdata,int32_t result)
{
	struct LoadRequest *request=userdata;
	ResourceLoader *self=request->self;

	int64_t received,size;
	if(ppb_urlloader->GetDownloadProgress(request->loader,&received,&size))
	{
		if(size<0)
		{
			fprintf(stderr,"Web server did not return a file size, cannot load resource \"%s\"!\n",request->path);
			return; // TODO: more error handling
		}

		request->bytes=malloc(size);
		request->size=size;

		if(request->bytes)
		{
			ppb_urlloader->ReadResponseBody(request->loader,request->bytes,
			size,PP_MakeCompletionCallback(FinishLoadingURL,request));
		}
		else
		{
			fprintf(stderr,"Internal error: Couldn't allocate %lld bytes for resource \"%s\".\n",size,request->path);
			return; // TODO: more error handling
		}
	}
	else
	{
		fprintf(stderr,"Internal error: Couldn't get download progress for resource \"%s\".\n",request->path);
		return; // TODO: more error handling
	}
}

static void FinishLoadingURL(void *userdata,int32_t result)
{
	struct LoadRequest *request=userdata;
	ResourceLoader *self=request->self;

	// Check if we got a partial read, and start a new read in that case.
	int64_t received,size;
	ppb_urlloader->GetDownloadProgress(request->loader,&received,&size);
	if(received!=size)
	{
		ppb_urlloader->ReadResponseBody(request->loader,&request->bytes[received],
		size-received,PP_MakeCompletionCallback(FinishLoadingURL,request));
		return;
	}

	// We have all the data.
	ResourceBundle *bundle=(ResourceBundle *)request->bytes;

	if(FixUpResourceBundle(bundle,request->path))
	{
		bundle->head.next=self->core.currentresources;
		self->core.currentresources=bundle;
	}
	else
	{
		fprintf(stderr,"Couldn't parse file format of resource \"%s\".\n",request->path);
		free(request->bytes);
		return; // TODO: more error handling
	}

	//free(request->bytes);

	self->finishedrequests++;
	if(self->finishedrequests==self->numrequests)
	{
		for(int i=0;i<self->numrequests;i++)
		{
			ppb_core->ReleaseResource(self->requests[i].loader);
		}

		self->numrequests=0;
	}
}

static void CheckDownloadProgress(void *userdata,int32_t result)
{
	ResourceLoader *self=userdata;

	if(!self->numrequests) return;

	int64_t totalreceived=0,totalsize=0;
	for(int i=0;i<self->numrequests;i++)
	{
		if(!self->requests[i].loader) goto giveup;

		int64_t received,size;
		if(!ppb_urlloader->GetDownloadProgress(self->requests[i].loader,
		&received,&size)) goto giveup;

		if(size<0) goto giveup;

		totalreceived+=received;
		totalsize+=size;
	}

	self->progress=(99*totalreceived)/totalsize;

	giveup: 0;

	ppb_core->CallOnMainThread(0,PP_MakeCompletionCallback(CheckDownloadProgress,self),0);
}
