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

#include <Ecore.h>
#include <Eina.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <packet.h>
#include <com-core.h>
#include <com-core_packet.h>

#include <notification_ipc.h>
#include <notification_db.h>
#include <notification_type.h>
#include <notification_internal.h>
#include <notification_debug.h>

#define NOTIFICATION_IPC_TIMEOUT 1.0

static struct info {
	int server_fd;
	int client_fd;
	const char *socket_file;
	struct {
		int (*request_cb)(const char *appid, const char *name, int type, const char *content, const char *icon, pid_t pid, double period, int allow_duplicate, void *data);
		void *data;
	} server_cb;
	int initialized;
	int is_started_cb_set_svc;
} s_info = {
	.server_fd = -1,
	.client_fd = -1,
	.socket_file = NOTIFICATION_ADDR,
	.initialized = 0,
	.is_started_cb_set_svc = 0,
};

typedef struct _task_list task_list;
struct _task_list {
	task_list *prev;
	task_list *next;

	void (*task_cb) (void *data);
	void *data;
};

static task_list *g_task_list;

static Eina_Bool _do_deffered_task(void *data);

/*
 * The concept of having to deffer a task till the "master" was
 * started is now defunct since we use systemd to auto activate
 * the service, with clients blissfully unaware of the state
 * of the service at the time they attempt to use the socket.
 *
 * To enable backwards compatability we now setup an idle handler
 * to cleanup all the deffered task.  This keeps the call behavior
 * as non-blocking and is true to the original spirit (i.e. run
 * the task when the service is available)
 */

notification_error_e
notification_ipc_add_deffered_task(
		void (*deffered_task_cb)(void *data),
		void *user_data)
{
	task_list *list = NULL;
	task_list *list_new = NULL;

	list_new =
	    (task_list *) malloc(sizeof(task_list));

	if (list_new == NULL) {
		return NOTIFICATION_ERROR_NO_MEMORY;
	}

	list_new->next = NULL;
	list_new->prev = NULL;

	list_new->task_cb = deffered_task_cb;
	list_new->data = user_data;

	if (g_task_list == NULL) {
		g_task_list = list_new;
	} else {
		list = g_task_list;

		while (list->next != NULL) {
			list = list->next;
		}

		list->next = list_new;
		list_new->prev = list;
	}

	ecore_idler_add(_do_deffered_task, NULL);

	return NOTIFICATION_ERROR_NONE;
}

notification_error_e
notification_ipc_del_deffered_task(
		void (*deffered_task_cb)(void *data))
{
	task_list *list_del = NULL;
	task_list *list_prev = NULL;
	task_list *list_next = NULL;

	list_del = g_task_list;

	if (list_del == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	while (list_del->prev != NULL) {
		list_del = list_del->prev;
	}

	do {
		if (list_del->task_cb == deffered_task_cb) {
			list_prev = list_del->prev;
			list_next = list_del->next;

			if (list_prev == NULL) {
				g_task_list = list_next;
			} else {
				list_prev->next = list_next;
			}

			if (list_next == NULL) {
				if (list_prev != NULL) {
					list_prev->next = NULL;
				}
			} else {
				list_next->prev = list_prev;
			}

			free(list_del);

			if (g_task_list == NULL) {
				ecore_idler_del(_do_deffered_task);
			}

			return NOTIFICATION_ERROR_NONE;
		}
		list_del = list_del->next;
	} while (list_del != NULL);

	return NOTIFICATION_ERROR_INVALID_DATA;
}

static Eina_Bool _do_deffered_task(void *data) {
	task_list *list_do = NULL;
	task_list *list_temp = NULL;

	if (g_task_list) {
        list_do = g_task_list;
        g_task_list = NULL;

        while (list_do->prev != NULL) {
            list_do = list_do->prev;
        }

        while (list_do != NULL) {
            if (list_do->task_cb != NULL) {
                list_do->task_cb(list_do->data);
                NOTIFICATION_DBG("called:%p", list_do->task_cb);
            }
            list_temp = list_do->next;
            free(list_do);
            list_do = list_temp;
        }
    }

    return ECORE_CALLBACK_CANCEL;
}

/*!
 * functions to create operation list
 */
notification_op *notification_ipc_create_op(notification_op_type_e type, int num_op, int *list_priv_id, int num_priv_id, notification_h *noti_list)
{
	int i = 0;
	notification_op *op_list = NULL;

	if (num_op <= 0) {
		return NULL;
	}

	op_list = (notification_op *)malloc(sizeof(notification_op) * num_op);
	memset(op_list, 0x0, sizeof(notification_op) * num_op);

	for (i = 0; i < num_op; i++) {
		(op_list + i)->type = type;
		if (list_priv_id != NULL) {
			(op_list + i)->priv_id = *(list_priv_id + i);
		}
		if (noti_list != NULL) {
			(op_list + i)->noti = *(noti_list + i);
		}
	}

	return op_list;
}

/*!
 * utility functions creating notification packet
 */
static inline char *_dup_string(const char *string)
{
	char *ret;

	if (string == NULL) {
		return NULL;
	}
	if (string[0] == '\0') {
		return NULL;
	}

	ret = strdup(string);
	if (!ret)
		NOTIFICATION_ERR("Error: %s\n", strerror(errno));

	return ret;
}

static inline bundle *_create_bundle_from_string(unsigned char *string)
{
	if (string == NULL) {
		return NULL;
	}
	if (string[0] == '\0') {
		return NULL;
	}

	return bundle_decode(string, strlen((char *)string));
}

/*!
 * functions creating notification packet
 */
EXPORT_API notification_error_e notification_ipc_make_noti_from_packet(notification_h noti, const struct packet *packet)
{
	int ret = 0;
	int type;
	int layout;
	int group_id;
	int internal_group_id;
	int priv_id;
	char *caller_pkgname = NULL;
	char *launch_pkgname = NULL;
	unsigned char *args = NULL;
	unsigned char *group_args = NULL;
	unsigned char *b_execute_option = NULL;
	unsigned char *b_service_responding = NULL;
	unsigned char *b_service_single_launch = NULL;
	unsigned char *b_service_multi_launch = NULL;
	char *domain = NULL;
	char *dir = NULL;
	unsigned char *b_text = NULL;
	unsigned char *b_key = NULL;
	unsigned char *b_format_args = NULL;
	int num_format_args;
	unsigned char *b_image_path = NULL;
	int sound_type;
	char *sound_path = NULL;
	int vibration_type;
	char *vibration_path = NULL;
	int led_operation;
	int led_argb;
	int led_on_ms;
	int led_off_ms;
	time_t time;
	time_t insert_time;
	int flags_for_property;
	int display_applist;
	double progress_size;
	double progress_percentage;
	char *app_icon_path = NULL;
	char *app_name = NULL;
	char *temp_title = NULL;
	char *temp_content = NULL;

	if (noti == NULL) {
		NOTIFICATION_ERR("invalid data");
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	ret = packet_get(packet,
			"iiiiisssssssssssssisisisiiiiiiiiddssss",
			&type,
			&layout,
			&group_id,
			&internal_group_id,
			&priv_id,
			&caller_pkgname,
			&launch_pkgname,
			&args,
			&group_args,
			&b_execute_option,
			&b_service_responding,
			&b_service_single_launch,
			&b_service_multi_launch,
			&domain,
			&dir,
			&b_text,
			&b_key,
			&b_format_args,
			&num_format_args,
			&b_image_path,
			&sound_type,
			&sound_path,
			&vibration_type,
			&vibration_path,
			&led_operation,
			&led_argb,
			&led_on_ms,
			&led_off_ms,
			&time,
			&insert_time,
			&flags_for_property,
			&display_applist,
			&progress_size,
			&progress_percentage,
			&app_icon_path,
			&app_name,
			&temp_title,
			&temp_content);

	if (ret != 38) {
		NOTIFICATION_ERR("failed to create a noti from packet");
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	/*!
	 * This is already allocated from the notification_create function.
	 * Before reallocate string to here.
	 * We have to release old one first.
	 */
	free(noti->caller_pkgname);
	noti->caller_pkgname = _dup_string(caller_pkgname);
	noti->launch_pkgname = _dup_string(launch_pkgname);
	noti->args = _create_bundle_from_string(args);
	noti->group_args = _create_bundle_from_string(group_args);
	noti->b_execute_option = _create_bundle_from_string(b_execute_option);
	noti->b_service_responding = _create_bundle_from_string(b_service_responding);
	noti->b_service_single_launch = _create_bundle_from_string(b_service_single_launch);
	noti->b_service_multi_launch = _create_bundle_from_string(b_service_multi_launch);
	noti->domain = _dup_string(domain);
	noti->dir = _dup_string(dir);
	noti->b_text = _create_bundle_from_string(b_text);
	noti->b_key = _create_bundle_from_string(b_key);
	noti->b_format_args = _create_bundle_from_string(b_format_args);
	noti->b_image_path = _create_bundle_from_string(b_image_path);
	noti->sound_path = _dup_string(sound_path);
	noti->vibration_path = _dup_string(vibration_path);
	noti->app_icon_path = _dup_string(app_icon_path);
	noti->app_name = _dup_string(app_name);
	noti->temp_title = _dup_string(temp_title);
	noti->temp_content = _dup_string(temp_content);

	noti->type = type;
	noti->layout = layout;
	noti->group_id = group_id;
	noti->internal_group_id = internal_group_id;
	noti->priv_id = priv_id;
	noti->num_format_args = num_format_args;
	noti->sound_type = sound_type;
	noti->vibration_type = vibration_type;
	noti->led_operation = led_operation;
	noti->led_argb = led_argb;
	noti->led_on_ms = led_on_ms;
	noti->led_off_ms = led_off_ms;
	noti->time = time;
	noti->insert_time = insert_time;
	noti->flags_for_property = flags_for_property;
	noti->display_applist = display_applist;
	noti->progress_size = progress_size;
	noti->progress_percentage = progress_percentage;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API struct packet *notification_ipc_make_packet_from_noti(notification_h noti, const char *command, int packet_type)
{
	struct packet *result = NULL;
	char *args = NULL;
	char *group_args = NULL;
	char *b_image_path = NULL;
	char *b_execute_option = NULL;
	char *b_service_responding = NULL;
	char *b_service_single_launch = NULL;
	char *b_service_multi_launch = NULL;
	char *b_text = NULL;
	char *b_key = NULL;
	char *b_format_args = NULL;
	int flag_simmode = 0;
	struct packet *(*func_to_create_packet)(const char *command, const char *fmt, ...);
	const char *title_key = NULL;
	char buf_key[32] = { 0, };

	/* Decode bundle to insert DB */
	if (noti->args) {
		bundle_encode(noti->args, (bundle_raw **) & args, NULL);
	}
	if (noti->group_args) {
		bundle_encode(noti->group_args, (bundle_raw **) & group_args,
			      NULL);
	}

	if (noti->b_execute_option) {
		bundle_encode(noti->b_execute_option,
			      (bundle_raw **) & b_execute_option, NULL);
	}
	if (noti->b_service_responding) {
		bundle_encode(noti->b_service_responding,
			      (bundle_raw **) & b_service_responding, NULL);
	}
	if (noti->b_service_single_launch) {
		bundle_encode(noti->b_service_single_launch,
			      (bundle_raw **) & b_service_single_launch, NULL);
	}
	if (noti->b_service_multi_launch) {
		bundle_encode(noti->b_service_multi_launch,
			      (bundle_raw **) & b_service_multi_launch, NULL);
	}

	if (noti->b_text) {
		bundle_encode(noti->b_text, (bundle_raw **) & b_text, NULL);
	}
	if (noti->b_key) {
		bundle_encode(noti->b_key, (bundle_raw **) & b_key, NULL);
	}
	if (noti->b_format_args) {
		bundle_encode(noti->b_format_args,
			      (bundle_raw **) & b_format_args, NULL);
	}

	if (noti->b_image_path) {
		bundle_encode(noti->b_image_path,
			      (bundle_raw **) & b_image_path, NULL);
	}

	/* Check only simmode property is enable */
	if (noti->flags_for_property & NOTIFICATION_PROP_DISPLAY_ONLY_SIMMODE) {
		flag_simmode = 1;
	}

	if (noti->b_key != NULL) {
		snprintf(buf_key, sizeof(buf_key), "%d",
			 NOTIFICATION_TEXT_TYPE_TITLE);

		title_key = bundle_get_val(noti->b_key, buf_key);
	}

	if (title_key == NULL && noti->b_text != NULL) {
		snprintf(buf_key, sizeof(buf_key), "%d",
			 NOTIFICATION_TEXT_TYPE_TITLE);

		title_key = bundle_get_val(noti->b_text, buf_key);
	}

	if (title_key == NULL) {
		title_key = noti->caller_pkgname;
	}

	if (packet_type == 1)
		func_to_create_packet = packet_create;
	else if (packet_type == 2)
		func_to_create_packet = packet_create_noack;
	else {
		goto out;
	}

	result = func_to_create_packet(command,
			"iiiiisssssssssssssisisisiiiiiiiiddssss",
			noti->type,
			noti->layout,
			noti->group_id,
			noti->internal_group_id,
			noti->priv_id,
			NOTIFICATION_CHECK_STR(noti->caller_pkgname),
			NOTIFICATION_CHECK_STR(noti->launch_pkgname),
			NOTIFICATION_CHECK_STR(args),
			NOTIFICATION_CHECK_STR(group_args),
			NOTIFICATION_CHECK_STR(b_execute_option),
			NOTIFICATION_CHECK_STR(b_service_responding),
			NOTIFICATION_CHECK_STR(b_service_single_launch),
			NOTIFICATION_CHECK_STR(b_service_multi_launch),
			NOTIFICATION_CHECK_STR(noti->domain),
			NOTIFICATION_CHECK_STR(noti->dir),
			NOTIFICATION_CHECK_STR(b_text),
			NOTIFICATION_CHECK_STR(b_key),
			NOTIFICATION_CHECK_STR(b_format_args),
			noti->num_format_args,
			NOTIFICATION_CHECK_STR(b_image_path),
			noti->sound_type,
			NOTIFICATION_CHECK_STR(noti->sound_path),
			noti->vibration_type,
			NOTIFICATION_CHECK_STR(noti->vibration_path),
			noti->led_operation,
			noti->led_argb,
			noti->led_on_ms,
			noti->led_off_ms,
			noti->time,
			noti->insert_time,
			noti->flags_for_property,
			noti->display_applist,
			noti->progress_size,
			noti->progress_percentage,
			NOTIFICATION_CHECK_STR(noti->app_icon_path),
			NOTIFICATION_CHECK_STR(noti->app_name),
			NOTIFICATION_CHECK_STR(noti->temp_title),
			NOTIFICATION_CHECK_STR(noti->temp_content));

out:
	/* Free decoded data */
	if (args) {
		free(args);
	}
	if (group_args) {
		free(group_args);
	}

	if (b_execute_option) {
		free(b_execute_option);
	}
	if (b_service_responding) {
		free(b_service_responding);
	}
	if (b_service_single_launch) {
		free(b_service_single_launch);
	}
	if (b_service_multi_launch) {
		free(b_service_multi_launch);
	}

	if (b_text) {
		free(b_text);
	}
	if (b_key) {
		free(b_key);
	}
	if (b_format_args) {
		free(b_format_args);
	}

	if (b_image_path) {
		free(b_image_path);
	}

	return result;
}

/*!
 * functions to handler services
 */
static struct packet *_handler_insert(pid_t pid, int handle, const struct packet *packet)
{
	notification_h noti = NULL;

	if (!packet) {
		NOTIFICATION_ERR("a packet is null");
		return NULL;
	}
	noti = notification_create(NOTIFICATION_TYPE_NOTI);
	if (!noti) {
		NOTIFICATION_ERR("failed to create a notification");
		return NULL;
	}
	notification_ipc_make_noti_from_packet(noti, packet);

	if (noti->flags_for_property
		& NOTIFICATION_PROP_DISABLE_UPDATE_ON_INSERT) {
		/* Disable changed cb */
	} else {
		/* Enable changed cb */
		notification_op *noti_op = notification_ipc_create_op(NOTIFICATION_OP_INSERT, 1, &(noti->priv_id), 1, &noti);
		if (noti_op != NULL) {
			notification_call_changed_cb(noti_op, 1);
			free(noti_op);
		}
	}
	notification_free(noti);

	return NULL;
}

static struct packet *_handler_update(pid_t pid, int handle, const struct packet *packet)
{
	notification_h noti = NULL;

	if (!packet) {
		NOTIFICATION_ERR("a packet is null");
		return NULL;
	}

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
	if (!noti) {
		NOTIFICATION_ERR("failed to create a notification");
		return NULL;
	}

	notification_ipc_make_noti_from_packet(noti, packet);

	notification_op *noti_op = notification_ipc_create_op(NOTIFICATION_OP_UPDATE, 1, &(noti->priv_id), 1, &noti);
	if (noti_op != NULL) {
		notification_call_changed_cb(noti_op, 1);
		free(noti_op);
	}

	notification_free(noti);

	return NULL;
}

static struct packet *_handler_refresh(pid_t pid, int handle, const struct packet *packet)
{
	if (!packet) {
		NOTIFICATION_ERR("a packet is null");
		return NULL;
	}
	notification_op *noti_op = notification_ipc_create_op(NOTIFICATION_OP_REFRESH, 1, NULL, 0, NULL);
	if (noti_op != NULL) {
		notification_call_changed_cb(noti_op, 1);
		free(noti_op);
	}

	return NULL;
}

static struct packet *_handler_delete_single(pid_t pid, int handle, const struct packet *packet)
{
	int num_deleted = 0;
	int priv_id = NOTIFICATION_PRIV_ID_NONE;

	if (!packet) {
		NOTIFICATION_ERR("a packet is null");
		return NULL;
	}
	if (packet_get(packet, "ii", &num_deleted, &priv_id) == 2) {
		notification_op *noti_op = notification_ipc_create_op(NOTIFICATION_OP_DELETE, 1, &priv_id, 1, NULL);
		if (noti_op != NULL) {
			notification_call_changed_cb(noti_op, 1);
			free(noti_op);
		}
	}

	return NULL;
}

static struct packet *_handler_delete_multiple(pid_t pid, int handle, const struct packet *packet)
{
	int ret = 0;
	int buf[10] = {0,};
	int num_deleted = 0;

	NOTIFICATION_ERR("delete_noti_multiple");

	if (!packet) {
		NOTIFICATION_ERR("a packet is null");
		return NULL;
	}
	ret = packet_get(packet, "iiiiiiiiiii", &num_deleted,
			&(buf[0]),
			&(buf[1]),
			&(buf[2]),
			&(buf[3]),
			&(buf[4]),
			&(buf[5]),
			&(buf[6]),
			&(buf[7]),
			&(buf[8]),
			&(buf[9]));

	NOTIFICATION_ERR("packet data count:%d", ret);
	NOTIFICATION_ERR("packet data num deleted:%d", num_deleted);

	int i = 0;
	for (i = 0 ; i < 10 ; i++) {
		NOTIFICATION_ERR("packet data[%d]:%d",i, buf[i]);
	}

	if (ret == 11) {
		notification_op *noti_op = notification_ipc_create_op(
				NOTIFICATION_OP_DELETE, num_deleted, buf, num_deleted, NULL);
		if (noti_op != NULL) {
			notification_call_changed_cb(noti_op, num_deleted);
			free(noti_op);
		}
	}

	return NULL;
}

static int _handler_service_register(pid_t pid, int handle, const struct packet *packet, void *data)
{
	int ret;

	if (!packet) {
		NOTIFICATION_ERR("Packet is not valid\n");
		ret = NOTIFICATION_ERROR_INVALID_DATA;
	} else if (packet_get(packet, "i", &ret) != 1) {
		NOTIFICATION_ERR("Packet is not valid\n");
		ret = NOTIFICATION_ERROR_INVALID_DATA;
	} else {
		if (ret == 0) {
			notification_op *noti_op = notification_ipc_create_op(NOTIFICATION_OP_SERVICE_READY, 1, NULL, 1, NULL);
			if (noti_op != NULL) {
				notification_call_changed_cb(noti_op, 1);
				free(noti_op);
			}
		}
	}
	return ret;
}

notification_error_e notification_ipc_monitor_init(void)
{
	int ret;
	struct packet *packet;
	static struct method service_table[] = {
		{
			.cmd = "add_noti",
			.handler = _handler_insert,
		},
		{
			.cmd = "update_noti",
			.handler = _handler_update,
		},
		{
			.cmd = "refresh_noti",
			.handler = _handler_refresh,
		},
		{
			.cmd = "del_noti_single",
			.handler = _handler_delete_single,
		},
		{
			.cmd = "del_noti_multiple",
			.handler = _handler_delete_multiple,
		},
		{
			.cmd = NULL,
			.handler = NULL,
		},
	};

	if (s_info.initialized == 1) {
		return NOTIFICATION_ERROR_NONE;
	} else {
		s_info.initialized = 1;
	}

	NOTIFICATION_ERR("register a service\n");

	s_info.server_fd = com_core_packet_client_init(s_info.socket_file, 0, service_table);
	if (s_info.server_fd < 0) {
		NOTIFICATION_ERR("Failed to make a connection to the master\n");
		return NOTIFICATION_ERROR_IO;
	}

	packet = packet_create("service_register", "");
	if (!packet) {
		NOTIFICATION_ERR("Failed to build a packet\n");
		return NOTIFICATION_ERROR_IO;
	}

	ret = com_core_packet_async_send(s_info.server_fd, packet, 1.0, _handler_service_register, NULL);
	NOTIFICATION_DBG("Service register sent: %d\n", ret);
	packet_destroy(packet);
	if (ret != 0) {
		com_core_packet_client_fini(s_info.server_fd);
		s_info.server_fd = NOTIFICATION_ERROR_INVALID_DATA;
		ret = NOTIFICATION_ERROR_IO;
	} else {
		ret = NOTIFICATION_ERROR_NONE;
	}

	NOTIFICATION_DBG("Server FD: %d\n", s_info.server_fd);
	return ret;
}

notification_error_e notification_ipc_monitor_fini(void)
{
	if (s_info.initialized == 0) {
		return NOTIFICATION_ERROR_NONE;
	}

	com_core_packet_client_fini(s_info.server_fd);
	s_info.server_fd = NOTIFICATION_ERROR_INVALID_DATA;

	s_info.initialized = 0;

	return NOTIFICATION_ERROR_NONE;
}

/*!
 * functions to request the service
 */
notification_error_e notification_ipc_request_insert(notification_h noti, int *priv_id)
{
	int status = 0;
	int id = NOTIFICATION_PRIV_ID_NONE;
	struct packet *packet;
	struct packet *result;

	/* Initialize private ID */
	noti->priv_id = NOTIFICATION_PRIV_ID_NONE;
	noti->group_id = NOTIFICATION_GROUP_ID_NONE;
	noti->internal_group_id = NOTIFICATION_GROUP_ID_NONE;

	packet = notification_ipc_make_packet_from_noti(noti, "add_noti", 1);
	result = com_core_packet_oneshot_send(NOTIFICATION_ADDR,
			packet,
			NOTIFICATION_IPC_TIMEOUT);
	packet_destroy(packet);

	if (result != NULL) {
		if (packet_get(result, "ii", &status, &id) != 2) {
			NOTIFICATION_ERR("Failed to get a result packet");
			packet_unref(result);
			return NOTIFICATION_ERROR_IO;
		}

		if (status != NOTIFICATION_ERROR_NONE) {
			packet_unref(result);
			return status;
		}
		packet_unref(result);
	} else {
		return NOTIFICATION_ERROR_SERVICE_NOT_READY;
	}

	if (priv_id != NULL) {
		*priv_id = id;
	}

	return NOTIFICATION_ERROR_NONE;
}

notification_error_e notification_ipc_request_delete_single(notification_type_e type, char *pkgname, int priv_id)
{
	int status = 0;
	int id = NOTIFICATION_PRIV_ID_NONE;
	struct packet *packet;
	struct packet *result;

	packet = packet_create("del_noti_single", "si", pkgname, priv_id);
	result = com_core_packet_oneshot_send(NOTIFICATION_ADDR,
			packet,
			NOTIFICATION_IPC_TIMEOUT);
	packet_destroy(packet);

	if (result != NULL) {
		if (packet_get(result, "ii", &status, &id) != 2) {
			NOTIFICATION_ERR("Failed to get a result packet");
			packet_unref(result);
			return NOTIFICATION_ERROR_IO;
		}
		packet_unref(result);
	} else {
		return NOTIFICATION_ERROR_SERVICE_NOT_READY;
	}

	return status;
}

notification_error_e notification_ipc_request_delete_multiple(notification_type_e type, char *pkgname)
{
	int status = 0;
	int num_deleted = 0;
	struct packet *packet;
	struct packet *result;

	packet = packet_create("del_noti_multiple", "si", pkgname, type);
	result = com_core_packet_oneshot_send(NOTIFICATION_ADDR,
			packet,
			NOTIFICATION_IPC_TIMEOUT);
	packet_destroy(packet);

	if (result != NULL) {
		if (packet_get(result, "ii", &status, &num_deleted) != 2) {
			NOTIFICATION_ERR("Failed to get a result packet");
			packet_unref(result);
			return NOTIFICATION_ERROR_IO;
		}
		NOTIFICATION_ERR("num deleted:%d", num_deleted);
		packet_unref(result);
	} else {
		return NOTIFICATION_ERROR_SERVICE_NOT_READY;
	}

	return status;
}

notification_error_e notification_ipc_request_update(notification_h noti)
{
	int status = 0;
	int id = NOTIFICATION_PRIV_ID_NONE;
	struct packet *packet;
	struct packet *result;

	packet = notification_ipc_make_packet_from_noti(noti, "update_noti", 1);
	result = com_core_packet_oneshot_send(NOTIFICATION_ADDR,
			packet,
			NOTIFICATION_IPC_TIMEOUT);
	packet_destroy(packet);

	if (result != NULL) {
		if (packet_get(result, "ii", &status, &id) != 2) {
			NOTIFICATION_ERR("Failed to get a result packet");
			packet_unref(result);
			return NOTIFICATION_ERROR_IO;
		}
		packet_unref(result);
	} else {
		return NOTIFICATION_ERROR_SERVICE_NOT_READY;
	}

	return status;
}

notification_error_e notification_ipc_request_refresh(void)
{
	int status = 0;
	struct packet *packet;
	struct packet *result;

	packet = packet_create("refresh_noti", "i", NOTIFICATION_OP_REFRESH);
	result = com_core_packet_oneshot_send(NOTIFICATION_ADDR,
			packet,
			NOTIFICATION_IPC_TIMEOUT);
	packet_destroy(packet);

	if (result != NULL) {
		if (packet_get(result, "i", &status) != 1) {
			NOTIFICATION_ERR("Failed to get a result packet");
			packet_unref(result);
			return NOTIFICATION_ERROR_IO;
		}
		packet_unref(result);
	} else {
		return NOTIFICATION_ERROR_SERVICE_NOT_READY;
	}

	return status;
}
