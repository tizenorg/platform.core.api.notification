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
#include <notification_list.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_private.h>
#include <notification_status.h>
#include <notification_status_internal.h>

#define PATH_NAME    "/Org/Tizen/System/Notification/Status_message"
#define INTERFACE_NAME "org.tizen.system.notification.status_message"
#define MEMBER_NAME	"status_message"

struct _message_cb_data {
	notification_status_message_cb callback;
	void *data;
	E_DBus_Connection *dbus_connection;
	E_DBus_Signal_Handler *dbus_hdlr;
};

static struct _message_cb_data md;

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
	if (!md.callback)
		return;

	if (strlen(message) <= 0){
		NOTIFICATION_ERR("message has only NULL");
		return;
	}

	md.callback(message, md.data);
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


