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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libintl.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>

#include <app.h>
#include <app_control_internal.h>
#include <aul.h>
#include <appsvc.h>
#include <tizen.h>
#include <vconf-keys.h>
#include <vconf.h>

#include <notification.h>
#include <notification_list.h>
#include <notification_debug.h>
#include <notification_private.h>
#include <notification_noti.h>
#include <notification_ongoing.h>
#include <notification_group.h>
#include <notification_ipc.h>
#include <notification_internal.h>

typedef struct _notification_cb_list notification_cb_list_s;

typedef enum __notification_cb_type {
	NOTIFICATION_CB_NORMAL = 1,
	NOTIFICATION_CB_DETAILED,
} _notification_cb_type_e;

struct _notification_cb_list {
	notification_cb_list_s *prev;
	notification_cb_list_s *next;

	_notification_cb_type_e cb_type;
	void (*changed_cb) (void *data, notification_type_e type);
	void (*detailed_changed_cb) (void *data, notification_type_e type, notification_op *op_list, int num_op);
	void *data;
};

static notification_cb_list_s *g_notification_cb_list = NULL;

void notification_call_changed_cb(notification_op *op_list, int op_num)
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_type_e type = 0;

	if (g_notification_cb_list == NULL)
		return;

	noti_cb_list = g_notification_cb_list;

	while (noti_cb_list->prev != NULL)
		noti_cb_list = noti_cb_list->prev;


	if (op_list == NULL) {
		NOTIFICATION_ERR("invalid data");
		return ;
	}

	notification_get_type(op_list->noti, &type);

	while (noti_cb_list != NULL) {
		if (noti_cb_list->cb_type == NOTIFICATION_CB_NORMAL && noti_cb_list->changed_cb) {
			noti_cb_list->changed_cb(noti_cb_list->data,
					type);
		}
		if (noti_cb_list->cb_type == NOTIFICATION_CB_DETAILED && noti_cb_list->detailed_changed_cb) {
			noti_cb_list->detailed_changed_cb(noti_cb_list->data,
					type, op_list, op_num);
		}

		noti_cb_list = noti_cb_list->next;
	}
}

EXPORT_API int notification_add_deferred_task(
		void (*deferred_task_cb)(void *data), void *user_data)
{
	if (deferred_task_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	return notification_ipc_add_deffered_task(deferred_task_cb, user_data);
}

EXPORT_API int notification_del_deferred_task(
		void (*deferred_task_cb)(void *data))
{
	if (deferred_task_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	return notification_ipc_del_deffered_task(deferred_task_cb);
}


EXPORT_API int notification_resister_changed_cb_for_uid(
		void (*changed_cb)(void *data, notification_type_e type),
		void *user_data, uid_t uid)
{
	notification_cb_list_s *noti_cb_list_new = NULL;
	notification_cb_list_s *noti_cb_list = NULL;

	if (changed_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	noti_cb_list_new =
		(notification_cb_list_s *) malloc(sizeof(notification_cb_list_s));

	if (noti_cb_list_new == NULL) {
		NOTIFICATION_ERR("malloc failed");
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	noti_cb_list_new->next = NULL;
	noti_cb_list_new->prev = NULL;

	noti_cb_list_new->cb_type = NOTIFICATION_CB_NORMAL;
	noti_cb_list_new->changed_cb = changed_cb;
	noti_cb_list_new->detailed_changed_cb = NULL;
	noti_cb_list_new->data = user_data;

	if (g_notification_cb_list == NULL) {
		g_notification_cb_list = noti_cb_list_new;
	} else {
		noti_cb_list = g_notification_cb_list;

		while (noti_cb_list->next != NULL)
			noti_cb_list = noti_cb_list->next;


		noti_cb_list->next = noti_cb_list_new;
		noti_cb_list_new->prev = noti_cb_list;
	}

	if (notification_ipc_monitor_init(uid) != NOTIFICATION_ERROR_NONE) {
		notification_unresister_changed_cb(changed_cb);
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_resister_changed_cb(
		void (*changed_cb)(void *data, notification_type_e type),
		void *user_data)
{
	return notification_resister_changed_cb_for_uid(
			changed_cb, user_data, getuid());
}

EXPORT_API int notification_unresister_changed_cb(
		void (*changed_cb)(void *data, notification_type_e type))
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_cb_list_s *noti_cb_list_prev = NULL;
	notification_cb_list_s *noti_cb_list_next = NULL;

	noti_cb_list = g_notification_cb_list;

	if (changed_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti_cb_list == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	while (noti_cb_list->prev != NULL)
		noti_cb_list = noti_cb_list->prev;


	do {
		if (noti_cb_list->changed_cb == changed_cb) {
			noti_cb_list_prev = noti_cb_list->prev;
			noti_cb_list_next = noti_cb_list->next;

			if (noti_cb_list_prev == NULL)
				g_notification_cb_list = noti_cb_list_next;
			else
				noti_cb_list_prev->next = noti_cb_list_next;

			if (noti_cb_list_next == NULL) {
				if (noti_cb_list_prev != NULL)
					noti_cb_list_prev->next = NULL;

			} else {
				noti_cb_list_next->prev = noti_cb_list_prev;
			}

			free(noti_cb_list);

			if (g_notification_cb_list == NULL)
				notification_ipc_monitor_fini();

			return NOTIFICATION_ERROR_NONE;
		}
		noti_cb_list = noti_cb_list->next;
	} while (noti_cb_list != NULL);

	return NOTIFICATION_ERROR_INVALID_PARAMETER;
}

EXPORT_API int notification_update_progress(notification_h noti,
		int priv_id,
		double progress)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;
	double input_progress = 0.0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL)
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		else
			input_priv_id = noti->priv_id;

	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(noti->caller_pkgname);

	if (progress < 0.0)
		input_progress = 0.0;
	else if (progress > 1.0)
		input_progress = 1.0;
	else
		input_progress = progress;

	ret = notification_ongoing_update_progress(caller_pkgname, input_priv_id,
			input_progress);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}

EXPORT_API int notification_update_size(notification_h noti,
		int priv_id,
		double size)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;
	double input_size = 0.0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL)
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		else
			input_priv_id = noti->priv_id;
	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(noti->caller_pkgname);

	if (size < 0.0)
		input_size = 0.0;
	else
		input_size = size;

	ret = notification_ongoing_update_size(caller_pkgname, input_priv_id,
			input_size);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}

EXPORT_API int notification_update_content(notification_h noti,
		int priv_id,
		const char *content)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL)
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		else
			input_priv_id = noti->priv_id;

	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(noti->caller_pkgname);

	ret = notification_ongoing_update_content(caller_pkgname, input_priv_id,
			content);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}

/* notification_set_icon will be removed */
/* LCOV_EXCL_START */
EXPORT_API int notification_set_icon(notification_h noti,
		const char *icon_path)
{
	int ret_err = NOTIFICATION_ERROR_NONE;

	ret_err =
		notification_set_image(noti, NOTIFICATION_IMAGE_TYPE_ICON,
				icon_path);

	return ret_err;
}
/* LCOV_EXCL_STOP */

/* notification_get_icon will be removed */
/* LCOV_EXCL_START */
EXPORT_API int notification_get_icon(notification_h noti,
		char **icon_path)
{
	int ret_err = NOTIFICATION_ERROR_NONE;
	char *ret_image_path = NULL;

	ret_err =
		notification_get_image(noti, NOTIFICATION_IMAGE_TYPE_ICON,
				&ret_image_path);

	if (ret_err == NOTIFICATION_ERROR_NONE && icon_path != NULL)
		*icon_path = ret_image_path;

	return ret_err;
}
/* LCOV_EXCL_STOP */

EXPORT_API int notification_translate_localized_text(notification_h noti)
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *ret_text = NULL;
	char buf_key[32];
	char *bundle_val = NULL;
	char *new_text;
	bundle *b;
	notification_text_type_e type = NOTIFICATION_TEXT_TYPE_TITLE;

	for (; type < NOTIFICATION_TEXT_TYPE_MAX; type++) {
		noti_err = notification_get_text(noti, type, &ret_text);
		if (noti_err == NOTIFICATION_ERROR_NONE && ret_text) {
			if (noti->b_text == NULL) {
				noti->b_text = bundle_create();
				if (noti->b_text == NULL)
					return NOTIFICATION_ERROR_OUT_OF_MEMORY;
			}

			b = noti->b_text;

			new_text = strdup(ret_text);

			snprintf(buf_key, sizeof(buf_key), "%d", type);
			bundle_get_str(b, buf_key, &bundle_val);
			if (bundle_val != NULL)
				bundle_del(b, buf_key);

			bundle_add_str(b, buf_key, new_text);
			free(new_text);
			new_text = NULL;

			noti->num_format_args = 0;
			bundle_val = NULL;
		}
	}

	if (noti->b_key) {
		bundle_free(noti->b_key);
		noti->b_key = NULL;
	}

	return noti_err;
}

/* LCOV_EXCL_START */
EXPORT_API int notification_set_title(notification_h noti,
		const char *title,
		const char *loc_title)
{
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_set_text(noti, NOTIFICATION_TEXT_TYPE_TITLE,
			title, loc_title,
			NOTIFICATION_VARIABLE_TYPE_NONE);

	return noti_err;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_title(notification_h noti,
		char **title,
		char **loc_title)
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *ret_text = NULL;

	noti_err =
		notification_get_text(noti, NOTIFICATION_TEXT_TYPE_TITLE,
				&ret_text);

	if (title != NULL)
		*title = ret_text;

	if (loc_title != NULL)
		*loc_title = NULL;

	return noti_err;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_set_content(notification_h noti,
		const char *content,
		const char *loc_content)
{
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_set_text(noti, NOTIFICATION_TEXT_TYPE_CONTENT,
			content, loc_content,
			NOTIFICATION_VARIABLE_TYPE_NONE);

	return noti_err;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_content(notification_h noti,
		char **content,
		char **loc_content)
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *ret_text = NULL;

	noti_err =
		notification_get_text(noti, NOTIFICATION_TEXT_TYPE_CONTENT,
				&ret_text);

	if (content != NULL)
		*content = ret_text;

	if (loc_content != NULL)
		*loc_content = NULL;

	return noti_err;

#if 0
	ret =
		vconf_get_bool
		(VCONFKEY_SETAPPL_STATE_TICKER_NOTI_DISPLAY_CONTENT_BOOL, &boolval);

	if (ret == -1 || boolval == 0) {
		if (content != NULL && noti->default_content != NULL)
			*content = noti->default_content;

		if (loc_content != NULL && noti->loc_default_content != NULL)
			*loc_content = noti->loc_default_content;
	}
#endif
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_set_application(notification_h noti,
		const char *pkgname)
{
	if (noti == NULL || pkgname == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti->launch_pkgname)
		free(noti->launch_pkgname);

	noti->launch_pkgname = strdup(pkgname);

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_application(notification_h noti,
		char **pkgname)
{
	if (noti == NULL || pkgname == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti->launch_pkgname)
		*pkgname = noti->launch_pkgname;
	else
		*pkgname = noti->caller_pkgname;

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_set_args(notification_h noti, bundle *args,
		bundle *group_args)
{
	if (noti == NULL || args == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti->args)
		bundle_free(noti->args);

	noti->args = bundle_dup(args);

	if (noti->group_args) {
		bundle_free(noti->group_args);
		noti->group_args = NULL;
	}

	if (group_args != NULL)
		noti->group_args = bundle_dup(group_args);

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_args(notification_h noti,
		bundle **args,
		bundle **group_args)
{
	if (noti == NULL || args == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti->args)
		*args = noti->args;
	else
		*args = NULL;

	if (group_args != NULL && noti->group_args)
		*group_args = noti->group_args;

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
int notification_get_grouping_list_for_uid(notification_type_e type, int count,
		notification_list_h *list, uid_t uid)
{
	notification_list_h get_list = NULL;
	int ret = 0;

	if (list == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = notification_noti_get_grouping_list(type, count, &get_list, uid);
	if (ret != NOTIFICATION_ERROR_NONE)
		return ret;

	*list = get_list;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_grouping_list(notification_type_e type, int count,
		notification_list_h *list)
{
	return notification_get_grouping_list_for_uid(type, count, list, getuid());
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_delete_group_by_group_id(const char *pkgname,
		notification_type_e type,
		int group_id)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (pkgname == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(pkgname);

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname, getuid());

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
int notification_delete_group_by_priv_id_for_uid(const char *pkgname,
		notification_type_e type,
		int priv_id, uid_t uid)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (pkgname == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(pkgname);

	ret = notification_ipc_request_delete_single(type, caller_pkgname, priv_id, uid);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_delete_group_by_priv_id(const char *pkgname,
		notification_type_e type,
		int priv_id)
{
	return notification_delete_group_by_priv_id_for_uid(pkgname, type, priv_id, getuid());
}

int notification_get_count_for_uid(notification_type_e type,
		const char *pkgname,
		int group_id,
		int priv_id, int *count,
		uid_t uid)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (count == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (pkgname == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(pkgname);

	ret = notification_ipc_request_get_count(
			type,
			caller_pkgname,
			group_id,
			priv_id,
			count,
			uid);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_count(notification_type_e type,
		const char *pkgname,
		int group_id,
		int priv_id, int *count)
{
	return notification_get_count_for_uid(type, pkgname, group_id, priv_id, count, getuid());
}

int notification_clear_for_uid(notification_type_e type, uid_t uid)
{
	int ret = 0;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = notification_ipc_request_delete_multiple(type, NULL, uid);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_clear(notification_type_e type)
{
	return notification_clear_for_uid(type, getuid());
}

EXPORT_API int notification_op_get_data(notification_op *noti_op, notification_op_data_type_e type,
		void *data)
{
	if (noti_op == NULL || data == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	switch (type) {
	case NOTIFICATION_OP_DATA_TYPE:
		*((int *)data) = noti_op->type;
		break;
	case NOTIFICATION_OP_DATA_PRIV_ID:
		*((int *)data) = noti_op->priv_id;
		break;
	case NOTIFICATION_OP_DATA_NOTI:
		*((notification_h *)data) = noti_op->noti;
		break;
	case NOTIFICATION_OP_DATA_EXTRA_INFO_1:
		*((int *)data) = noti_op->extra_info_1;
		break;
	case NOTIFICATION_OP_DATA_EXTRA_INFO_2:
		*((int *)data) = noti_op->extra_info_2;
		break;
	default:
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
		break;
	}

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_set_pkgname(notification_h noti,
		const char *pkgname)
{
	/* check noti and pkgname are valid data */
	if (noti == NULL || pkgname == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Remove previous caller pkgname */
	if (noti->caller_pkgname) {
		free(noti->caller_pkgname);
		noti->caller_pkgname = NULL;
	}

	noti->caller_pkgname = strdup(pkgname);

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
int notification_delete_all_by_type_for_uid(const char *pkgname,
		notification_type_e type, uid_t uid)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (pkgname == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(pkgname);

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname, uid);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_delete_all_by_type(const char *pkgname,
		notification_type_e type)
{
	return notification_delete_all_by_type_for_uid(pkgname, type, getuid());
}

int notification_delete_by_priv_id_for_uid(const char *pkgname,
		notification_type_e type,
		int priv_id,
		uid_t uid)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (pkgname == NULL)
		caller_pkgname = notification_get_pkgname_by_pid();
	else
		caller_pkgname = strdup(pkgname);

	ret = notification_ipc_request_delete_single(type, caller_pkgname, priv_id, uid);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_delete_by_priv_id(const char *pkgname,
		notification_type_e type,
		int priv_id)
{
	return notification_delete_by_priv_id_for_uid(pkgname, type, priv_id, getuid());
}

EXPORT_API int notification_set_execute_option(notification_h noti,
		notification_execute_type_e type,
		const char *text,
		const char *key,
		bundle *service_handle)
{
	char buf_key[32] = { 0, };
	char *ret_val = NULL;
	bundle *b = NULL;

	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (type <= NOTIFICATION_EXECUTE_TYPE_NONE
			|| type >= NOTIFICATION_EXECUTE_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Create execute option bundle if does not exist */
	if (noti->b_execute_option == NULL)
		noti->b_execute_option = bundle_create();

	b = noti->b_execute_option;

	/* Save text */
	if (text != NULL) {
		/* Make text key */
		snprintf(buf_key, sizeof(buf_key), "text%d", type);

		/* Check text key exist */
		bundle_get_str(b, buf_key, &ret_val);
		if (ret_val != NULL)
			/* Remove previous data */
			bundle_del(b, buf_key);

		/* Add text data */
		bundle_add_str(b, buf_key, text);
	}

	/* Save key */
	if (key != NULL) {
		/* Make key key */
		snprintf(buf_key, sizeof(buf_key), "key%d", type);

		/* Check key key exist */
		bundle_get_str(b, buf_key, &ret_val);
		if (ret_val != NULL)
			/* Remove previous data */
			bundle_del(b, buf_key);

		/* Add text data */
		bundle_add_str(b, buf_key, key);
	}

	switch ((int)type) {
	case NOTIFICATION_EXECUTE_TYPE_RESPONDING:
		/* Remove previous data if exist */
		if (noti->b_service_responding != NULL) {
			bundle_free(noti->b_service_responding);
			noti->b_service_responding = NULL;
		}

		/* Save service handle */
		if (service_handle != NULL)
			noti->b_service_responding = bundle_dup(service_handle);

		break;
	case NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH:
		/* Remove previous data if exist */
		if (noti->b_service_single_launch != NULL) {
			bundle_free(noti->b_service_single_launch);
			noti->b_service_single_launch = NULL;
		}

		/* Save service handle */
		if (service_handle != NULL)
			noti->b_service_single_launch =
				bundle_dup(service_handle);

		break;
	case NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH:
		/* Remove previous data if exist */
		if (noti->b_service_multi_launch != NULL) {
			bundle_free(noti->b_service_multi_launch);
			noti->b_service_multi_launch = NULL;
		}

		/* Save service handle */
		if (service_handle != NULL)
			noti->b_service_multi_launch =
				bundle_dup(service_handle);

		break;
	}

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_id(notification_h noti,
		int *group_id, int *priv_id)
{
	/* check noti is valid data */
	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Check group_id is valid data */
	if (group_id) {
		/* Set group id */
		if (noti->group_id < NOTIFICATION_GROUP_ID_NONE)
			*group_id = NOTIFICATION_GROUP_ID_NONE;
		else
			*group_id = noti->group_id;
	}

	/* Check priv_id is valid data */
	if (priv_id)
		/* Set priv_id */
		*priv_id = noti->priv_id;

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
notification_h notification_load_for_uid(char *pkgname,
		int priv_id, uid_t uid)
{
	int ret = 0;
	notification_h noti = NULL;

	noti = (notification_h)calloc(1, sizeof(struct _notification));
	if (noti == NULL) {
		NOTIFICATION_ERR("NO MEMORY : noti == NULL");
		return NULL;
	}

	ret = notification_ipc_request_load_noti_by_priv_id(noti, pkgname, priv_id, uid);
	if (ret != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return NULL;
	}

	return noti;
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API notification_h notification_load(char *pkgname,
		int priv_id)
{
	return notification_load_for_uid(pkgname, priv_id, getuid());
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API notification_h notification_new(notification_type_e type,
		int group_id, int priv_id)
{
	return notification_create(type);
}

static void _notification_get_text_domain(notification_h noti)
{
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_get_execute_option(notification_h noti,
		notification_execute_type_e type,
		const char **text,
		bundle **service_handle)
{
	char buf_key[32] = { 0, };
	char *ret_val = NULL;
	char *get_str = NULL;
	bundle *b = NULL;

	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (type <= NOTIFICATION_EXECUTE_TYPE_NONE
			|| type >= NOTIFICATION_EXECUTE_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;


	switch (type) {
	case NOTIFICATION_EXECUTE_TYPE_RESPONDING:
		b = noti->b_service_responding;
		break;
	case NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH:
		b = noti->b_service_single_launch;
		break;
	case NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH:
		b = noti->b_service_multi_launch;
		break;
	default:
		break;
	}

	if (b != NULL) {
		/* Return text */
		if (text != NULL) {
			/*  Get text domain and dir */
			if (noti->domain == NULL || noti->dir == NULL)
				_notification_get_text_domain(noti);

			/* Make key */
			snprintf(buf_key, sizeof(buf_key), "key%d", type);

			/* Check key key exist */
			bundle_get_str(b, buf_key, &ret_val);
			if (ret_val != NULL && noti->domain != NULL
					&& noti->dir != NULL) {
				/* Get application string */
				bindtextdomain(noti->domain, noti->dir);

				get_str = dgettext(noti->domain, ret_val);

				*text = get_str;
			} else if (ret_val != NULL) {
				/* Get system string */
				get_str = dgettext("sys_string", ret_val);

				*text = get_str;
			} else {
				/* Get basic text */
				snprintf(buf_key, sizeof(buf_key), "text%d",
						type);

				bundle_get_str(b, buf_key, &ret_val);

				*text = ret_val;
			}
		}
	}

	if (service_handle != NULL)
		*service_handle = b;

	return NOTIFICATION_ERROR_NONE;
}
/* LCOV_EXCL_STOP */

EXPORT_API int notification_insert_for_uid(notification_h noti,
		int *priv_id, uid_t uid)
{
	int ret = 0;
	int id = 0;

	/* Check noti is vaild data */
	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Check noti type is valid type */
	if (noti->type <= NOTIFICATION_TYPE_NONE
			|| noti->type >= NOTIFICATION_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	noti->uid = uid;
	/* Save insert time */
	noti->insert_time = time(NULL);
	ret = notification_ipc_request_insert(noti, &id);
	if (ret != NOTIFICATION_ERROR_NONE)
		return ret;

	noti->priv_id = id;
	NOTIFICATION_DBG("from master:%d", id);

	/* If priv_id is valid data, set priv_id */
	if (priv_id != NULL)
		*priv_id = noti->priv_id;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_insert(notification_h noti,
		int *priv_id)
{
	return notification_insert_for_uid(noti, priv_id, getuid());
}

EXPORT_API int notification_update_async_for_uid(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data, uid_t uid)
{
	int ret = 0;
	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	noti->uid = uid;
	/* Update insert time ? */
	noti->insert_time = time(NULL);
	ret = notification_ipc_request_update_async(noti, result_cb, user_data);

	return ret;
}

EXPORT_API int notification_update_async(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data)
{
	return notification_update_async_for_uid(noti, result_cb, user_data, getuid());
}

EXPORT_API int notification_register_detailed_changed_cb_for_uid(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data, uid_t uid)
{
	notification_cb_list_s *noti_cb_list_new = NULL;
	notification_cb_list_s *noti_cb_list = NULL;

	if (detailed_changed_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (notification_ipc_monitor_init(uid) != NOTIFICATION_ERROR_NONE)
		return NOTIFICATION_ERROR_IO_ERROR;

	noti_cb_list_new =
		(notification_cb_list_s *) malloc(sizeof(notification_cb_list_s));

	if (noti_cb_list_new == NULL) {
		NOTIFICATION_ERR("malloc failed");
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	noti_cb_list_new->next = NULL;
	noti_cb_list_new->prev = NULL;

	noti_cb_list_new->cb_type = NOTIFICATION_CB_DETAILED;
	noti_cb_list_new->changed_cb = NULL;
	noti_cb_list_new->detailed_changed_cb = detailed_changed_cb;
	noti_cb_list_new->data = user_data;

	if (g_notification_cb_list == NULL) {
		g_notification_cb_list = noti_cb_list_new;
	} else {
		noti_cb_list = g_notification_cb_list;

		while (noti_cb_list->next != NULL)
			noti_cb_list = noti_cb_list->next;


		noti_cb_list->next = noti_cb_list_new;
		noti_cb_list_new->prev = noti_cb_list;
	}
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_register_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data)
{
	return notification_register_detailed_changed_cb_for_uid(detailed_changed_cb, user_data, getuid());
}

EXPORT_API int notification_unregister_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data)
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_cb_list_s *noti_cb_list_prev = NULL;
	notification_cb_list_s *noti_cb_list_next = NULL;

	noti_cb_list = g_notification_cb_list;

	if (detailed_changed_cb == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (noti_cb_list == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;


	while (noti_cb_list->prev != NULL)
		noti_cb_list = noti_cb_list->prev;


	do {
		if (noti_cb_list->detailed_changed_cb == detailed_changed_cb) {
			noti_cb_list_prev = noti_cb_list->prev;
			noti_cb_list_next = noti_cb_list->next;

			if (noti_cb_list_prev == NULL)
				g_notification_cb_list = noti_cb_list_next;
			else
				noti_cb_list_prev->next = noti_cb_list_next;

			if (noti_cb_list_next == NULL) {
				if (noti_cb_list_prev != NULL)
					noti_cb_list_prev->next = NULL;

			} else {
				noti_cb_list_next->prev = noti_cb_list_prev;
			}

			free(noti_cb_list);

			if (g_notification_cb_list == NULL)
				notification_ipc_monitor_fini();

			return NOTIFICATION_ERROR_NONE;
		}
		noti_cb_list = noti_cb_list->next;
	} while (noti_cb_list != NULL);

	return NOTIFICATION_ERROR_INVALID_PARAMETER;
}

/* LCOV_EXCL_START */
EXPORT_API int notification_is_service_ready(void)
{
	return notification_ipc_is_master_ready();
}
/* LCOV_EXCL_STOP */

EXPORT_API int notification_set_uid(notification_h noti,
		uid_t uid)
{
	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	noti->uid = uid;
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_uid(notification_h noti,
		uid_t *uid)
{
	if (noti == NULL || uid == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	*uid = noti->uid;
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_post_for_uid(notification_h noti, uid_t uid)
{
	int ret = 0;
	int id = 0;

	/* Check noti is vaild data */
	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Check noti type is valid type */
	if (noti->type <= NOTIFICATION_TYPE_NONE
			|| noti->type >= NOTIFICATION_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	/* Save insert time */
	noti->insert_time = time(NULL);
	noti->uid = uid;

	ret = notification_ipc_request_insert(noti, &id);
	if (ret != NOTIFICATION_ERROR_NONE)
		return ret;

	noti->priv_id = id;
	NOTIFICATION_DBG("from master:%d", id);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_update_for_uid(notification_h noti, uid_t uid)
{
	int ret;

	/* Check noti is valid data */
	if (noti != NULL) {
		noti->uid = uid;
		/* Update insert time ? */
		noti->insert_time = time(NULL);
		ret = notification_ipc_request_update(noti);
	} else {
		notification_ipc_request_refresh(uid);
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

EXPORT_API int notification_delete_for_uid(notification_h noti, uid_t uid)
{
	int ret = 0;

	if (noti == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = notification_ipc_request_delete_single(NOTIFICATION_TYPE_NONE, noti->caller_pkgname, noti->priv_id, uid);

	return ret;
}

EXPORT_API int notification_delete_all_for_uid(notification_type_e type, uid_t uid)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	caller_pkgname = notification_get_pkgname_by_pid();

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname, uid);

	if (caller_pkgname)
		free(caller_pkgname);

	return ret;
}

EXPORT_API notification_h notification_load_by_tag_for_uid(const char *tag, uid_t uid)
{
	int ret = 0;
	notification_h noti = NULL;
	char *caller_pkgname = NULL;

	if (tag == NULL) {
		NOTIFICATION_ERR("Invalid parameter");
		set_last_result(NOTIFICATION_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	caller_pkgname = notification_get_pkgname_by_pid();
	if (!caller_pkgname) {
		/* LCOV_EXCL_START */
		NOTIFICATION_ERR("Failed to get a package name");
		set_last_result(NOTIFICATION_ERROR_OUT_OF_MEMORY);
		return NULL;
		/* LCOV_EXCL_STOP */
	}

	noti = (notification_h)calloc(1, sizeof(struct _notification));
	if (noti == NULL) {
		/* LCOV_EXCL_START */
		NOTIFICATION_ERR("Failed to alloc a new notification");
		set_last_result(NOTIFICATION_ERROR_OUT_OF_MEMORY);
		free(caller_pkgname);

		return NULL;
		/* LCOV_EXCL_STOP */
	}

	ret = notification_ipc_request_load_noti_by_tag(noti, caller_pkgname, (char *)tag, uid);
	free(caller_pkgname);

	set_last_result(ret);
	if (ret != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return NULL;
	}

	return noti;
}
