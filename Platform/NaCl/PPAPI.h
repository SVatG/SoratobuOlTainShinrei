#ifndef __PPAPI_H__
#define __PPAPI_H__

#include "ppapi/c/pp_errors.h"
//#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_var.h"

//#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_url_request_info.h"
#include "ppapi/c/ppb_url_loader.h"

#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_input_event.h"
#include "ppapi/c/ppp_messaging.h"

extern struct PPB_Instance *ppb_instance;
extern struct PPB_Core *ppb_core;
extern struct PPB_Var *ppb_var;
extern struct PPB_Messaging *ppb_messaging;
extern struct PPB_ImageData *ppb_imagedata;
extern struct PPB_Graphics2D *ppb_graphics2d;
extern struct PPB_InputEvent *ppb_inputevent;
extern struct PPB_KeyboardInputEvent *ppb_keyboardinputevent;
extern struct PPB_URLRequestInfo *ppb_urlrequestinfo;
extern struct PPB_URLLoader *ppb_urlloader;
extern PP_Module module_id,instance_id;

#endif
