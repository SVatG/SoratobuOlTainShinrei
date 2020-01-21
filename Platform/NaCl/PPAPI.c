#include "PPAPI.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>




void _InitializeScreen();
void _InitializeInput();

void Instance_DidChangeView(PP_Instance instance,const struct PP_Rect *position,const struct PP_Rect *clip);
void Instance_DidChangeFocus(PP_Instance instance,PP_Bool has_focus);
PP_Bool InputEvent_HandleInputEvent(PP_Instance instance,PP_Resource inputevent);

void EntryPoint();




struct PPB_Instance *ppb_instance=NULL;
struct PPB_Core *ppb_core=NULL;
struct PPB_Var *ppb_var=NULL;
struct PPB_Messaging *ppb_messaging=NULL;
struct PPB_ImageData *ppb_imagedata=NULL;
struct PPB_Graphics2D *ppb_graphics2d=NULL;
struct PPB_InputEvent *ppb_inputevent=NULL;
struct PPB_KeyboardInputEvent *ppb_keyboardinputevent=NULL;
struct PPB_URLRequestInfo *ppb_urlrequestinfo=NULL;
struct PPB_URLLoader *ppb_urlloader=NULL;
PP_Module module_id=0,instance_id=0;

static pthread_t workerthread; 




static PP_Bool Instance_DidCreate(PP_Instance instance,uint32_t argc,const char *argn[],const char *argv[])
{
	instance_id=instance;

	_InitializeScreen();
	_InitializeInput();

	pthread_create(&workerthread,NULL,(void *)EntryPoint,NULL);

	return PP_TRUE;
}

static void Instance_DidDestroy(PP_Instance instance)
{
}

static PP_Bool Instance_HandleDocumentLoad(PP_Instance instance,PP_Resource url_loader)
{
	return PP_FALSE;
}



static void Messaging_HandleMessage(PP_Instance instance,struct PP_Var var_message)
{
}



PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,PPB_GetInterface get_browser)
{
	module_id=module;

	ppb_instance=(struct PPB_Instance *)get_browser(PPB_INSTANCE_INTERFACE);
	ppb_core=(struct PPB_Core *)get_browser(PPB_CORE_INTERFACE);
	ppb_var=(struct PPB_Var *)get_browser(PPB_VAR_INTERFACE);
	ppb_messaging=(struct PPB_Messaging *)get_browser(PPB_MESSAGING_INTERFACE);
	ppb_imagedata=(struct PPB_ImageData *)get_browser(PPB_IMAGEDATA_INTERFACE);
	ppb_graphics2d=(struct PPB_Graphics2D *)get_browser(PPB_GRAPHICS_2D_INTERFACE);
	ppb_inputevent=(struct PPB_InputEvent *)get_browser(PPB_INPUT_EVENT_INTERFACE);
	ppb_keyboardinputevent=(struct PPB_KeyboardInputEvent *)get_browser(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
	ppb_urlrequestinfo=(struct PPB_URLRequestInfo *)get_browser(PPB_URLREQUESTINFO_INTERFACE);
	ppb_urlloader=(struct PPB_URLLoader *)get_browser(PPB_URLLOADER_INTERFACE);

	return PP_OK;
}


PP_EXPORT const void *PPP_GetInterface(const char *interface_name)
{
	if(strcmp(interface_name,PPP_INSTANCE_INTERFACE)==0)
	{
		static struct PPP_Instance instance_interface=
		{
			&Instance_DidCreate,
			&Instance_DidDestroy,
			&Instance_DidChangeView,
			&Instance_DidChangeFocus,
			&Instance_HandleDocumentLoad,
		};
 		return &instance_interface;
	}
	else if(strcmp(interface_name,PPP_INPUT_EVENT_INTERFACE)==0)
	{
		static struct PPP_InputEvent inputevent_interface=
		{
			&InputEvent_HandleInputEvent
		};
		return &inputevent_interface;
	}
	else if(strcmp(interface_name,PPP_MESSAGING_INTERFACE)==0)
	{
		static struct PPP_Messaging messaging_interface=
		{
			&Messaging_HandleMessage
		};
		return &messaging_interface;
	}
	else return NULL;
}

PP_EXPORT void PPP_ShutdownModule()
{
}

