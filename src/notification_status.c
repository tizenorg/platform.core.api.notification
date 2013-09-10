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

#include <notification.h>
#include <notification_db.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_internal.h>
#include <notification_status.h>

#define NOTIFICATION_STATUS_MESSAGE_KEY "memory/private/libstatus/message"

struct _message_cb_data {
	notification_status_message_cb callback;
	void *data;
};

static struct _message_cb_data md;

static void __notification_status_message_change_cb(keynode_t *node, void *user_data)
{
	int type;
	char *message = NULL;

	if (!node) {
		NOTIFICATION_ERR("key node is NULL");
		return;
	}

	if (!md.callback)
		return;

	type = vconf_keynode_get_type(node);
	if (type != VCONF_TYPE_STRING) {
		NOTIFICATION_ERR("invail node type : %d", type);
		return;
	}

	message = vconf_keynode_get_str(node);
	if (!message) {
		NOTIFICATION_ERR("fail to get message");
		return;
	}

	if (strlen(message) <= 0){
		NOTIFICATION_DBG("message has only NULL");
		return;
	}

	NOTIFICATION_DBG("message : %s", message);
	md.callback(message, md.data);
}

EXPORT_API
int notification_status_message_post(const char *message)
{
	int ret = 0;

	if (!message) {
		NOTIFICATION_ERR("message is NULL");
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	if (strlen(message) <= 0) {
		NOTIFICATION_ERR("message has only NULL");
		return NOTIFICATION_ERROR_INVALID_DATA;
	}


	ret = vconf_set_str(NOTIFICATION_STATUS_MESSAGE_KEY, message);
	if (ret) {
		NOTIFICATION_ERR("fail to set message [%s]", message);
		return NOTIFICATION_ERROR_IO;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int notification_status_monitor_message_cb_set(notification_status_message_cb callback, void *user_data)
{
	int ret = 0;
	if (!callback)
		return NOTIFICATION_ERROR_INVALID_DATA;

	ret = vconf_notify_key_changed(NOTIFICATION_STATUS_MESSAGE_KEY,
			__notification_status_message_change_cb, NULL);
	if (ret && errno != EALREADY) {
		NOTIFICATION_ERR("fail to set message cb");
		return NOTIFICATION_ERROR_IO;
	}

	md.callback = callback;
	md.data = user_data;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int notification_status_monitor_message_cb_unset(void)
{
	vconf_ignore_key_changed(NOTIFICATION_STATUS_MESSAGE_KEY,
			__notification_status_message_change_cb);
	md.callback = NULL;
	md.data = NULL;

	return NOTIFICATION_ERROR_NONE;
}
