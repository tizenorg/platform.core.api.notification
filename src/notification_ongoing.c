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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dbus/dbus.h>

#include <notification_db.h>
#include <notification_debug.h>
#include <notification_ongoing.h>
#include <notification_private.h>

int notification_ongoing_update_progress(const char *caller_pkgname,
							  int priv_id,
							  double progress)
{
	DBusConnection *connection = NULL;
	DBusMessage *signal = NULL;
	DBusError err;
	dbus_bool_t ret;

	dbus_error_init(&err);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (!connection) {
		NOTIFICATION_ERR("Fail to dbus_bus_get");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	signal =
	    dbus_message_new_signal("/dbus/signal", "notification.ongoing",
				    "update_progress");
	if (!signal) {
		NOTIFICATION_ERR("Fail to dbus_message_new_signal");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	ret = dbus_message_append_args(signal,
				       DBUS_TYPE_STRING, &caller_pkgname,
				       DBUS_TYPE_INT32, &priv_id,
				       DBUS_TYPE_DOUBLE, &progress,
				       DBUS_TYPE_INVALID);

	if (ret) {
		ret = dbus_connection_send(connection, signal, NULL);
		if (ret) {
			dbus_connection_flush(connection);
		}
	}

	dbus_message_unref(signal);

	if (ret) {
		return NOTIFICATION_ERROR_NONE;
	}

	return NOTIFICATION_ERROR_FROM_DBUS;
}

int notification_ongoing_update_size(const char *caller_pkgname,
						      int priv_id, double size)
{
	DBusConnection *connection = NULL;
	DBusMessage *signal = NULL;
	DBusError err;
	dbus_bool_t ret;

	dbus_error_init(&err);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (!connection) {
		NOTIFICATION_ERR("Fail to dbus_bus_get");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	signal =
	    dbus_message_new_signal("/dbus/signal", "notification.ongoing",
				    "update_size");
	if (!signal) {
		NOTIFICATION_ERR("Fail to dbus_message_new_signal");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	ret = dbus_message_append_args(signal,
				       DBUS_TYPE_STRING, &caller_pkgname,
				       DBUS_TYPE_INT32, &priv_id,
				       DBUS_TYPE_DOUBLE, &size,
				       DBUS_TYPE_INVALID);
	if (ret) {
		ret = dbus_connection_send(connection, signal, NULL);

		if (ret) {
			dbus_connection_flush(connection);
		}
	}

	dbus_message_unref(signal);

	if (ret) {
		return NOTIFICATION_ERROR_NONE;
	}

	return NOTIFICATION_ERROR_FROM_DBUS;
}

int notification_ongoing_update_content(const char *caller_pkgname,
						      int priv_id, const char *content)
{
	DBusConnection *connection = NULL;
	DBusMessage *signal = NULL;
	DBusError err;
	dbus_bool_t ret;

	dbus_error_init(&err);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (!connection) {
		NOTIFICATION_ERR("Fail to dbus_bus_get");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	signal =
	    dbus_message_new_signal("/dbus/signal", "notification.ongoing",
				    "update_content");
	if (!signal) {
		NOTIFICATION_ERR("Fail to dbus_message_new_signal");
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	if(content == NULL) {
		ret = dbus_message_append_args(signal,
						   DBUS_TYPE_STRING, &caller_pkgname,
						   DBUS_TYPE_INT32, &priv_id,
						   DBUS_TYPE_INVALID);
	} else {
		ret = dbus_message_append_args(signal,
						   DBUS_TYPE_STRING, &caller_pkgname,
						   DBUS_TYPE_INT32, &priv_id,
						   DBUS_TYPE_STRING, &content,
						   DBUS_TYPE_INVALID);
	}
	if (ret) {
		ret = dbus_connection_send(connection, signal, NULL);

		if (ret) {
			dbus_connection_flush(connection);
		}
	}

	dbus_message_unref(signal);

	if (ret) {
		return NOTIFICATION_ERROR_NONE;
	}

	return NOTIFICATION_ERROR_FROM_DBUS;
}

