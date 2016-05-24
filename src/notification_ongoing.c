/*
 * Copyright (c) 2000 - 2016 Samsung Electronics Co., Ltd. All rights reserved.
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
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>
#include <gio/gio.h>

#include <notification_db.h>
#include <notification_debug.h>
#include <notification_ongoing.h>
#include <notification_private.h>

#define PATH_NAME	"/dbus/signal"
#define INTERFACE_NAME	"notification.ongoing"
#define MEMBER_PROGRESS	"update_progress"
#define MEMBER_SIZE	"update_size"
#define MEMBER_CONTENT	"update_content"

struct _ongoing_update_cb_data {
	notification_ongoing_update_cb callback;
	void *data;
	GDBusConnection *conn;
	guint subscribe_id;
};

static struct _ongoing_update_cb_data od;

static void __notification_ongoing_update_callback(GDBusConnection *connection,
					const gchar *sender_name,
					const gchar *object_path,
					const gchar *interface_name,
					const gchar *signal_name,
					GVariant *parameters,
					gpointer user_data)
{
	struct ongoing_info_s *info;
	char *pkgname = NULL;
	int priv_id = 0;
	double progress = 0;
	double size = 0;
	char *content = NULL;

	info = calloc(1, sizeof(struct ongoing_info_s));
	if (info == NULL) {
		NOTIFICATION_ERR("Out of memory");
		return;
	}

	if (g_strcmp0(signal_name, MEMBER_PROGRESS) == 0) {
		g_variant_get(parameters, "(sid)", &pkgname, &priv_id, &progress);
		info->type = ONGOING_TYPE_PROGRESS;
	}

	if (g_strcmp0(signal_name, MEMBER_SIZE) == 0) {
		g_variant_get(parameters, "(sid)", &pkgname, &priv_id, &size);
		info->type = ONGOING_TYPE_SIZE;

	}

	if (g_strcmp0(signal_name, MEMBER_CONTENT) == 0) {
		g_variant_get(parameters, "(sis)", &pkgname, &priv_id, &content);
		info->type = ONGOING_TYPE_CONTENT;
	}

	if (pkgname == NULL) {
		NOTIFICATION_ERR("pkgname is null");
		free(info);
		return;
	}

	info->pkgname = strdup(pkgname);
	info->priv_id = priv_id;
	info->progress = progress;
	info->size = size;
	info->content = content;

	od.callback(info, od.data);
}

static int __send_ongoing_update_signal(const char *signal_name, GVariant *param)
{
	GError *err = NULL;
	GDBusConnection *conn;
	int ret = NOTIFICATION_ERROR_NONE;

	if (signal_name == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &err);
	if (conn == NULL) {
		NOTIFICATION_ERR("g_bus_get_sync() failed: %s", err->message);
		ret = NOTIFICATION_ERROR_FROM_DBUS;
		goto end;
	}

	if (g_dbus_connection_emit_signal(conn,
					NULL,
					PATH_NAME,
					INTERFACE_NAME,
					signal_name,
					param,
					&err) == FALSE) {
		NOTIFICATION_ERR("g_dbus_connection_emit_signal() failed: %s",
					err->message);
		ret = NOTIFICATION_ERROR_FROM_DBUS;
		goto end;
	}

	if (g_dbus_connection_flush_sync(conn, NULL, &err) == FALSE) {
		NOTIFICATION_ERR("g_dbus_connection_flush_sync() failed: %s",
					err->message);
		ret = NOTIFICATION_ERROR_FROM_DBUS;
		goto end;
	}


end:
	if (err)
		g_error_free(err);

	if (conn)
		g_object_unref(conn);


	return ret;
}

EXPORT_API
int notification_ongoing_update_cb_set(notification_ongoing_update_cb callback,
					void *user_data)
{
	GError *error = NULL;

	if (!callback)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	od.conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
	if (od.conn == NULL) {
		NOTIFICATION_ERR("Failed to connect to the D-BUS Daemon: %s",
					error->message);
		g_error_free(error);
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	od.subscribe_id = g_dbus_connection_signal_subscribe(od.conn,
					NULL,
					INTERFACE_NAME,
					NULL,
					PATH_NAME,
					NULL,
					G_DBUS_SIGNAL_FLAGS_NONE,
					__notification_ongoing_update_callback,
					NULL,
					NULL);
	if (od.subscribe_id == 0) {
		NOTIFICATION_ERR("g_dbus_connection_signal_subscribe() failed");
		g_object_unref(od.conn);
		return NOTIFICATION_ERROR_FROM_DBUS;
	}

	od.callback = callback;
	od.data = user_data;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int notification_ongoing_update_cb_unset(void)
{
	if (od.subscribe_id) {
		g_dbus_connection_signal_unsubscribe(od.conn, od.subscribe_id);
		od.subscribe_id = 0;
	}

	if (od.conn) {
		g_object_unref(od.conn);
		od.conn = NULL;
	}

	od.callback = NULL;
	od.data = NULL;

	return NOTIFICATION_ERROR_NONE;
}

int notification_ongoing_update_progress(const char *caller_pkgname,
					int priv_id, double progress)
{
	GVariant *param;
	int ret;

	param = g_variant_new("(sid)", caller_pkgname, priv_id, progress);

	ret = __send_ongoing_update_signal(MEMBER_PROGRESS, param);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_ongoing_update_progress failed %d",
					ret);
		return ret;
	}

	return NOTIFICATION_ERROR_NONE;
}

int notification_ongoing_update_size(const char *caller_pkgname,
					int priv_id, double size)
{
	GVariant *param;
	int ret;

	param = g_variant_new("(sid)", caller_pkgname, priv_id, size);

	ret = __send_ongoing_update_signal(MEMBER_SIZE, param);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_ongoing_update_size failed %d",
					ret);
		return ret;
	}

	return NOTIFICATION_ERROR_NONE;
}

int notification_ongoing_update_content(const char *caller_pkgname,
					int priv_id, const char *content)
{
	GVariant *param;
	int ret;

	param = g_variant_new("(sis)", caller_pkgname, priv_id, content);

	ret = __send_ongoing_update_signal(MEMBER_CONTENT, param);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_ongoing_update_content failed %d",
					ret);
		return ret;
	}
	return NOTIFICATION_ERROR_NONE;
}
