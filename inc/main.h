#ifndef __LBS_MAPS_H__
#define __LBS_MAPS_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#include <tizen.h>
#include <locations.h>


typedef struct appdata {
	Evas_Object *win;
	Evas_Object *layout;
	Evas_Object *conform;
	Evas_Object *nf;
	location_manager_s *manager;
	Ecore_Thread *thread;
} appdata_s;

#endif /* __basicui_H__ */
