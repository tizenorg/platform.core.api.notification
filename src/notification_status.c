/*
 *  libnotification
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seungtaek Chung <seungtaek.chung@samsung.com>, Mi-Ju Lee <miju52.lee@samsung.com>, Xi Zhichan <zhichan.xi@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <vconf.h>
#include <E_DBus.h>
#include <Ecore.h>
#include <Elementary.h>
#include <Eina.h>

#include <notification.h>
#include <notification_db.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_private.h>
#include <notification_status.h>
#include <notification_status_internal.h>

#define PATH_NAME    "/Org/Tizen/System/Notification/Status_message"
#define INTERFACE_NAME "org.tizen.system.notification.status_message"
#define MEMBER_NAME	"status_message"

static Eina_List *toast_list;
static Eina_List *toast_popup;

struct _message_cb_data {
	notification_status_message_cb callback;
	void *data;
	E_DBus_Connection *dbus_connection;
	E_DBus_Signal_Handler *dbus_hdlr;
};

static struct _message_cb_data md;

int _post_toast_message(char *message);

static void popup_timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	char *msg = NULL;
	int count = 0;

	evas_object_del(toast_popup);
	toast_popup = NULL;
	evas_object_del((Evas_Object *)data);

	count = eina_list_count(toast_list);

	if (count == 1){
		msg = (char *)eina_list_data_get(toast_list);
		free(msg);

		eina_list_free(toast_list);
		toast_list = NULL;
	} else if (count > 1) {
		msg = (char *)eina_list_data_get(toast_list);
		toast_list = eina_list_remove(toast_list, msg);
		free(msg);
		_post_toast_message((char *)eina_list_data_get(toast_list));
	}
}

int _post_toast_message(char *message)
{
	Evas_Object *toast_window;
	Evas *e;
	Ecore_Evas *ee;
	double scale = elm_config_scale_get();


	toast_window = elm_win_add(NULL, "toast", ELM_WIN_BASIC);

	elm_win_alpha_set(toast_window, EINA_TRUE);
	elm_win_title_set(toast_window, "toast");

	elm_win_indicator_mode_set(toast_window, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_type_set(toast_window,ELM_WIN_INDICATOR_TYPE_1);

	//elm_win_autodel_set(toast_win, EINA_TRUE);
	if (elm_win_wm_rotation_supported_get(toast_window)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(toast_window, (const int*)(&rots), 4);
	}

	e = evas_object_evas_get(toast_window);
	ee = ecore_evas_ecore_evas_get(e);
	ecore_evas_name_class_set(ee, "TOAST_POPUP", "SYSTEM_POPUP");

	evas_object_resize(toast_window, (480 * scale), (650 * scale));
	ecore_x_window_shape_input_rectangle_set(elm_win_xwindow_get(toast_window), 0, 0, (480 * scale), (650 * scale));

	toast_popup = elm_popup_add(toast_window);

	elm_object_style_set(toast_popup, "toast");
	evas_object_size_hint_weight_set(toast_popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	elm_object_text_set(toast_popup,message);

	if (eina_list_count(toast_list) != 1) {
		elm_popup_timeout_set(toast_popup, 1.0);
	}
	else {
		elm_popup_timeout_set(toast_popup, 3.0);
	}
	evas_object_smart_callback_add(toast_popup, "timeout", popup_timeout_cb, (void *)toast_window);

	elm_win_prop_focus_skip_set(toast_window, EINA_TRUE);

	evas_object_show(toast_window);
	evas_object_show(toast_popup);

	return 0;
}

int notification_noti_post_toast_message(const char *message)
{
	int let = 0;
	char *msg = NULL;
	int count = 0;

	msg = (char *)calloc(strlen(message) + 1, sizeof(char));
	strcpy(msg, message);

	count = eina_list_count(toast_list);
	if (count == 0) {
		toast_list = eina_list_append(toast_list, msg);
		let = _post_toast_message(msg);
	}
	else if (count == 1) {
		if (strcmp(msg, (char *)eina_list_nth(toast_list, count - 1)) == 0) {
			elm_popup_timeout_set(toast_popup, 3.0);
		}
		else {
			toast_list = eina_list_append(toast_list, msg);
			elm_popup_timeout_set(toast_popup, 1.0);
		}
	}
	else if (count >= 2) {
		if (strcmp(msg, (char *)eina_list_nth(toast_list, count - 1)) == 0) {
			free(msg);
			return 0;
		}
		else {
			toast_list = eina_list_append(toast_list, msg);
		}
	}

	return 0;
}

static void __notification_status_message_dbus_callback(void *data, DBusMessage *msg)
{
	int ret = 0;
	DBusError err;
	char *message = NULL;

	if(data==NULL||msg==NULL)
	{
		NOTIFICATION_ERR("message is NULL");
		return;
	}

	dbus_error_init(&err);
	ret = dbus_message_get_args(msg, &err,
			DBUS_TYPE_STRING, &message,
			DBUS_TYPE_INVALID);
	if(ret == 0)
	{
		NOTIFICATION_ERR("dbus_message_get_args error");
		return;
	}

	if (dbus_error_is_set(&err)) {
		NOTIFICATION_ERR("Dbus err: %s", err.message);
		dbus_error_free(&err);
		return;
	}
	/*if (!md.callback)
		return;

	if (strlen(message) <= 0){
		NOTIFICATION_ERR("message has only NULL");
		return;
	}

	md.callback(message, md.data);*/

	notification_noti_post_toast_message(message);
}

EXPORT_API
int notification_status_message_post(const char *message)
{
	DBusConnection *connection = NULL;
	DBusMessage *signal = NULL;
	DBusError err;
	dbus_bool_t ret;

	if (!message) {
		NOTIFICATION_ERR("message is NULL");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (strlen(message) <= 0) {
		NOTIFICATION_ERR("message has only NULL");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	dbus_error_init(&err);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (!connection) {
		NOTIFICATION_ERR("Fail to dbus_bus_get");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	signal =
	    dbus_message_new_signal(PATH_NAME, INTERFACE_NAME,
				    MEMBER_NAME);
	if (!signal) {
		NOTIFICATION_ERR("Fail to dbus_message_new_signal");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	ret = dbus_message_append_args(signal,
					   DBUS_TYPE_STRING, &message,
					   DBUS_TYPE_INVALID);
	if (ret) {
		ret = dbus_connection_send(connection, signal, NULL);

		if (ret) {
			dbus_connection_flush(connection);
		}
	}

	dbus_message_unref(signal);
	dbus_connection_unref(connection);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int notification_status_monitor_message_cb_set(notification_status_message_cb callback, void *user_data)
{
	if (!callback)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	E_DBus_Connection *dbus_connection;
	E_DBus_Signal_Handler *dbus_handler_size = NULL;

	e_dbus_init();
	dbus_connection = e_dbus_bus_get(DBUS_BUS_SYSTEM);
	if (dbus_connection == NULL) {
		NOTIFICATION_ERR("noti register : failed to get dbus bus");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}
	dbus_handler_size =
		e_dbus_signal_handler_add(dbus_connection, NULL,
			PATH_NAME,
			INTERFACE_NAME, MEMBER_NAME,
			__notification_status_message_dbus_callback,
			user_data);
	if (dbus_handler_size == NULL)
	{
		NOTIFICATION_ERR("fail to add size signal");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}


	md.callback = callback;
	md.data = user_data;
	md.dbus_connection = dbus_connection;
	md.dbus_hdlr = dbus_handler_size;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int notification_status_monitor_message_cb_unset(void)
{
	if (md.dbus_hdlr != NULL) {
			e_dbus_signal_handler_del(md.dbus_connection,
					md.dbus_hdlr);
			md.dbus_hdlr = NULL;
	}
	if (md.dbus_connection != NULL) {
		e_dbus_connection_close(md.dbus_connection);
		md.dbus_connection = NULL;
		e_dbus_shutdown();
	}

	md.callback = NULL;
	md.data = NULL;

	return NOTIFICATION_ERROR_NONE;
}
