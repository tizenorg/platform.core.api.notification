/*
 *  libnotification
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seungtaek Chung <seungtaek.chung@samsung.com>, Mi-Ju Lee <miju52.lee@samsung.com>, Xi Zhichan <zhichan.xi@samsung.com>, Youngsub Ko <ys4610.ko@samsung.com>
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

#include <vconf.h>

#include <notification.h>
#include <notification_db.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_internal.h>

#define NOTI_BURST_DELETE_UNIT 10

static int _notification_noti_bind_query_text(sqlite3_stmt * stmt, const char *name,
					 const char *str)
{
	int ret = 0;
	int index = 0;

	index = sqlite3_bind_parameter_index(stmt, name);
	if (index == 0) {
		NOTIFICATION_ERR("Insert : invalid column name");
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret =
	    sqlite3_bind_text(stmt, index, NOTIFICATION_CHECK_STR(str), -1,
			      SQLITE_STATIC);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert text : %s",
				 NOTIFICATION_CHECK_STR(str));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	return NOTIFICATION_ERROR_NONE;
}

static int _notification_noti_bind_query_double(sqlite3_stmt * stmt, const char *name,
					 double val)
{
	int ret = 0;
	int index = 0;

	index = sqlite3_bind_parameter_index(stmt, name);
	if (index == 0) {
		NOTIFICATION_ERR("Insert : invalid column name");
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_bind_double(stmt, index, val);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert double : %f", val);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	return NOTIFICATION_ERROR_NONE;
}

static int _notification_noti_check_priv_id(notification_h noti, sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;

	/* Make query to check priv_id exist */
	snprintf(query, sizeof(query),
		 "select count(*) from noti_list where caller_pkgname = '%s' and priv_id = %d",
		 noti->caller_pkgname, noti->priv_id);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Get count DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	} else {
		result = 0;
	}

	sqlite3_finalize(stmt);

	/* If result > 0, there is priv_id in DB */
	if (result > 0) {
		return NOTIFICATION_ERROR_ALREADY_EXIST_ID;
	}

	return NOTIFICATION_ERROR_NONE;
}

static int _notification_noti_get_priv_id(notification_h noti, sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;

	/* Make query to get max priv_id */
	snprintf(query, sizeof(query),
		 "select max(priv_id) from noti_list where caller_pkgname = '%s'",
		 noti->caller_pkgname);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Get count DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	} else {
		result = 0;
	}

	sqlite3_finalize(stmt);

	if (result < 0) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Increase result(max priv_id value) for next priv_id */
	noti->priv_id = result + 1;

	return NOTIFICATION_ERROR_NONE;
}

static int _notification_noti_get_internal_group_id_by_priv_id(const char *pkgname,
							       int priv_id,
							       sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;

	snprintf(query, sizeof(query),
		 "select internal_group_id from noti_list where caller_pkgname = '%s' and priv_id = %d",
		 pkgname, priv_id);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Get count DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	} else {
		result = 0;
	}

	sqlite3_finalize(stmt);

	return result;
}

static int _notification_noti_get_max_internal_group_id(notification_h noti,
							sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;

	/* Get max internal group id */
	snprintf(query, sizeof(query),
		 "select max(internal_group_id) from noti_list");

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Get count DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	} else {
		result = 0;
	}

	sqlite3_finalize(stmt);

	return result;
}

static int _notification_noti_get_internal_group_id(notification_h noti,
						    sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;
	const char *ret_title = NULL;
	char buf_key[32] = { 0, };

	if (noti->group_id == NOTIFICATION_GROUP_ID_NONE) {
		/* If Group ID is NONE Get max internal group ID */
		result = _notification_noti_get_max_internal_group_id(noti, db);
		if (result < 0) {
			return NOTIFICATION_ERROR_FROM_DB;
		}

		/* Internal Group ID is max internal group ID + 1 */
		noti->internal_group_id = result + 1;

		return NOTIFICATION_ERROR_NONE;
	} else if (noti->group_id == NOTIFICATION_GROUP_ID_DEFAULT) {
		/* If Group ID is DEFAULT, Get internal group id if it exist */
		if (noti->b_key != NULL) {
			snprintf(buf_key, sizeof(buf_key), "%d",
				 NOTIFICATION_TEXT_TYPE_TITLE);

			ret_title = bundle_get_val(noti->b_key, buf_key);
		}

		if (ret_title == NULL && noti->b_text != NULL) {
			snprintf(buf_key, sizeof(buf_key), "%d",
				 NOTIFICATION_TEXT_TYPE_TITLE);

			ret_title = bundle_get_val(noti->b_text, buf_key);
		}

		if (ret_title == NULL) {
			ret_title = noti->caller_pkgname;
		}

		snprintf(query, sizeof(query),
			 "select internal_group_id from noti_list where title_key = $title_key and group_id = %d",
			 NOTIFICATION_GROUP_ID_DEFAULT);
	} else {
		/* If Group ID is > DEFAULT, Get internal group id if it exit */
		snprintf(query, sizeof(query),
			 "select internal_group_id from noti_list where caller_pkgname = '%s' and group_id = %d",
			 NOTIFICATION_CHECK_STR(noti->caller_pkgname),
			 noti->group_id);
	}

	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Bind query */
	if (ret_title != NULL) {
		ret =
		    _notification_noti_bind_query_text(stmt, "$title_key",
						  NOTIFICATION_CHECK_STR
						  (ret_title));
		if (ret != NOTIFICATION_ERROR_NONE) {
			NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
			if (stmt) {
				sqlite3_finalize(stmt);
			}
			return ret;
		}
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		result = sqlite3_column_int(stmt, 0);
	} else {
		/* If there is not internal_group_id, create new one */
		result = _notification_noti_get_max_internal_group_id(noti, db);
		result++;
	}

	sqlite3_finalize(stmt);

	noti->internal_group_id = result;

	return NOTIFICATION_ERROR_NONE;
}

static int _notification_noti_make_query(notification_h noti, char *query,
					 int query_size)
{
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

	/* Make query */
	snprintf(query, query_size, "insert into noti_list ("
		 "type, "
		 "layout, "
		 "caller_pkgname, launch_pkgname, "
		 "image_path, "
		 "group_id, internal_group_id, priv_id, "
		 "title_key, "
		 "b_text, b_key, b_format_args, num_format_args, "
		 "text_domain, text_dir, "
		 "time, insert_time, "
		 "args, group_args, "
		 "b_execute_option, "
		 "b_service_responding, b_service_single_launch, b_service_multi_launch, "
		 "sound_type, sound_path, vibration_type, vibration_path, led_operation, led_argb, led_on_ms, led_off_ms, "
		 "flags_for_property, flag_simmode, display_applist, "
		 "progress_size, progress_percentage) values ("
		 "%d, "
		 "%d, "
		 "'%s', '%s', "
		 "'%s', "
		 "%d, %d, %d, "
		 "$title_key, "
		 "'%s', '%s', '%s', %d, "
		 "'%s', '%s', "
		 "%d, %d, "
		 "'%s', '%s', "
		 "'%s', "
		 "'%s', '%s', '%s', "
		 "%d, '%s', %d, '%s', %d, %d, %d, %d,"
		 "%d, %d, %d, "
		 "$progress_size, $progress_percentage)",
		 noti->type,
		 noti->layout,
		 NOTIFICATION_CHECK_STR(noti->caller_pkgname),
		 NOTIFICATION_CHECK_STR(noti->launch_pkgname),
		 NOTIFICATION_CHECK_STR(b_image_path), noti->group_id,
		 noti->internal_group_id, noti->priv_id,
		 NOTIFICATION_CHECK_STR(b_text), NOTIFICATION_CHECK_STR(b_key),
		 NOTIFICATION_CHECK_STR(b_format_args), noti->num_format_args,
		 NOTIFICATION_CHECK_STR(noti->domain),
		 NOTIFICATION_CHECK_STR(noti->dir), (int)noti->time,
		 (int)noti->insert_time, NOTIFICATION_CHECK_STR(args),
		 NOTIFICATION_CHECK_STR(group_args),
		 NOTIFICATION_CHECK_STR(b_execute_option),
		 NOTIFICATION_CHECK_STR(b_service_responding),
		 NOTIFICATION_CHECK_STR(b_service_single_launch),
		 NOTIFICATION_CHECK_STR(b_service_multi_launch),
		 noti->sound_type, NOTIFICATION_CHECK_STR(noti->sound_path),
		 noti->vibration_type,
		 NOTIFICATION_CHECK_STR(noti->vibration_path),
		 noti->led_operation,
		 noti->led_argb,
		 noti->led_on_ms,
		 noti->led_off_ms,
		 noti->flags_for_property, flag_simmode, noti->display_applist);

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

	return NOTIFICATION_ERROR_NONE;
}


static int _notification_noti_make_update_query(notification_h noti, char *query,
					 int query_size)
{
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

	/* Decode bundle to update DB */
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

	/* Make query */
	snprintf(query, query_size, "update noti_list set "
		 "type = %d, "
		 "layout = %d, "
		 "launch_pkgname = '%s', "
		 "image_path = '%s', "
		 "b_text = '%s', b_key = '%s', "
		 "b_format_args = '%s', num_format_args = %d, "
		 "text_domain = '%s', text_dir = '%s', "
		 "time = %d, insert_time = %d, "
		 "args = '%s', group_args = '%s', "
		 "b_execute_option = '%s', "
		 "b_service_responding = '%s', "
		 "b_service_single_launch = '%s', "
		 "b_service_multi_launch = '%s', "
		 "sound_type = %d, sound_path = '%s', "
		 "vibration_type = %d, vibration_path = '%s', "
		 "led_operation = %d, led_argb = %d, "
		 "led_on_ms = %d, led_off_ms = %d, "
		 "flags_for_property = %d, flag_simmode = %d, "
		 "display_applist = %d, "
		 "progress_size = $progress_size, progress_percentage = $progress_percentage "
		 "where priv_id = %d ",
		 noti->type,
		 noti->layout,
		 NOTIFICATION_CHECK_STR(noti->launch_pkgname),
		 NOTIFICATION_CHECK_STR(b_image_path),
		 NOTIFICATION_CHECK_STR(b_text), NOTIFICATION_CHECK_STR(b_key),
		 NOTIFICATION_CHECK_STR(b_format_args), noti->num_format_args,
		 NOTIFICATION_CHECK_STR(noti->domain),
		 NOTIFICATION_CHECK_STR(noti->dir),
		 (int)noti->time, (int)noti->insert_time,
		 NOTIFICATION_CHECK_STR(args), NOTIFICATION_CHECK_STR(group_args),
		 NOTIFICATION_CHECK_STR(b_execute_option),
		 NOTIFICATION_CHECK_STR(b_service_responding),
		 NOTIFICATION_CHECK_STR(b_service_single_launch),
		 NOTIFICATION_CHECK_STR(b_service_multi_launch),
		 noti->sound_type, NOTIFICATION_CHECK_STR(noti->sound_path),
		 noti->vibration_type,
		 NOTIFICATION_CHECK_STR(noti->vibration_path),
		 noti->led_operation,
		 noti->led_argb,
		 noti->led_on_ms,
		 noti->led_off_ms,
		 noti->flags_for_property, flag_simmode, noti->display_applist,
		 noti->priv_id);

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

	return NOTIFICATION_ERROR_NONE;
}

static void _notification_noti_populate_from_stmt(sqlite3_stmt * stmt, notification_h noti) {
	int col = 0;

	if (stmt == NULL || noti == NULL) {
		return ;
	}

	noti->type = sqlite3_column_int(stmt, col++);
	noti->layout = sqlite3_column_int(stmt, col++);
	noti->caller_pkgname = notification_db_column_text(stmt, col++);
	noti->launch_pkgname = notification_db_column_text(stmt, col++);
	noti->b_image_path = notification_db_column_bundle(stmt, col++);
	noti->group_id = sqlite3_column_int(stmt, col++);
	noti->internal_group_id = 0;
	noti->priv_id = sqlite3_column_int(stmt, col++);

	noti->b_text = notification_db_column_bundle(stmt, col++);
	noti->b_key = notification_db_column_bundle(stmt, col++);
	noti->b_format_args = notification_db_column_bundle(stmt, col++);
	noti->num_format_args = sqlite3_column_int(stmt, col++);

	noti->domain = notification_db_column_text(stmt, col++);
	noti->dir = notification_db_column_text(stmt, col++);
	noti->time = sqlite3_column_int(stmt, col++);
	noti->insert_time = sqlite3_column_int(stmt, col++);
	noti->args = notification_db_column_bundle(stmt, col++);
	noti->group_args = notification_db_column_bundle(stmt, col++);

	noti->b_execute_option = notification_db_column_bundle(stmt, col++);
	noti->b_service_responding = notification_db_column_bundle(stmt, col++);
	noti->b_service_single_launch =
	    notification_db_column_bundle(stmt, col++);
	noti->b_service_multi_launch =
	    notification_db_column_bundle(stmt, col++);

	noti->sound_type = sqlite3_column_int(stmt, col++);
	noti->sound_path = notification_db_column_text(stmt, col++);
	noti->vibration_type = sqlite3_column_int(stmt, col++);
	noti->vibration_path = notification_db_column_text(stmt, col++);
	noti->led_operation = sqlite3_column_int(stmt, col++);
	noti->led_argb = sqlite3_column_int(stmt, col++);
	noti->led_on_ms = sqlite3_column_int(stmt, col++);
	noti->led_off_ms = sqlite3_column_int(stmt, col++);

	noti->flags_for_property = sqlite3_column_int(stmt, col++);
	noti->display_applist = sqlite3_column_int(stmt, col++);
	noti->progress_size = sqlite3_column_double(stmt, col++);
	noti->progress_percentage = sqlite3_column_double(stmt, col++);

	noti->app_icon_path = NULL;
	noti->app_name = NULL;
	noti->temp_title = NULL;
	noti->temp_content = NULL;
}

static notification_h _notification_noti_get_item(sqlite3_stmt * stmt)
{
	notification_h noti = NULL;

	noti = malloc(sizeof(struct _notification));
	if (noti == NULL) {
		return NULL;
	}

	_notification_noti_populate_from_stmt(stmt, noti);

	return noti;
}

int notification_noti_set_tag(const char *tag, char *value, char *buf, int buf_len)
{
	int len_total = 0;

	len_total += (strlen(tag) * 2) + 5 + strlen(value) + 1;

	if (buf_len <= len_total)
		return NOTIFICATION_ERROR_INVALID_DATA;

	snprintf(buf, buf_len, "<%s>%s</%s>", tag, value, tag);

	return NOTIFICATION_ERROR_NONE;
}

char *notification_noti_strip_tag(const char *tagged_str)
{
	if (tagged_str == NULL)
		return NULL;

	int len_total = strlen(tagged_str);

	if (len_total == 0)
		return NULL;

	char *b_f_e = strstr(tagged_str, ">");
	char *b_e_s = strstr(tagged_str, "</");

	if (b_f_e == NULL || b_e_s == NULL || (b_e_s - b_f_e - 1) <= 0)
		return NULL;

	return strndup(b_f_e + 1, b_e_s - b_f_e - 1);
}

int notification_noti_get_tag_type(const char *tagged_str)
{
	if (tagged_str == NULL)
		return TAG_TYPE_INVALID;

	if (strlen(tagged_str)== 0)
		return TAG_TYPE_INVALID;

	char *b_f_s = strstr(tagged_str, "<");
	char *b_f_e = strstr(tagged_str, ">");

	if (b_f_s == NULL || b_f_e == NULL || (b_f_e - b_f_s - 1) <= 0)
		return TAG_TYPE_INVALID;

	char *start = b_f_s + 1;
	int len_tag = b_f_e - b_f_s - 1;

	if (strncmp(start,TAG_TIME,len_tag) == 0) {
		return TAG_TYPE_TIME;
	}

	return TAG_TYPE_INVALID;
}

static int _notification_noti_update_priv_id(sqlite3 * db, int rowid)
{
	char query[128] = { 0, };

	if (db == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	snprintf(query, sizeof(query), "update noti_list set "
			"priv_id = %d, internal_group_id = %d where rowid = %d",
			rowid, rowid, rowid);

	return notification_db_exec(db, query, NULL);
}

EXPORT_API int notification_noti_insert(notification_h noti)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	char buf_key[32] = { 0, };
	const char *title_key = NULL;

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Initialize private ID */
	noti->priv_id = NOTIFICATION_PRIV_ID_NONE;
	noti->group_id = NOTIFICATION_GROUP_ID_NONE;
	noti->internal_group_id = NOTIFICATION_GROUP_ID_NONE;

	/* make query */
	ret = _notification_noti_make_query(noti, query, sizeof(query));
	if (ret != NOTIFICATION_ERROR_NONE) {
		goto err;
	}

	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert Query : %s", query);
		NOTIFICATION_ERR("Insert DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	/* Get title key */
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

	/* Bind query */
	ret = _notification_noti_bind_query_text(stmt, "$title_key", title_key);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		goto err;
	}
	ret = _notification_noti_bind_query_double(stmt, "$progress_size",noti->progress_size);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		return ret;
	}
	ret = _notification_noti_bind_query_double(stmt, "$progress_percentage",noti->progress_percentage);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_OK || ret == SQLITE_DONE) {
		noti->priv_id = (int)sqlite3_last_insert_rowid(db);
		if (_notification_noti_update_priv_id(db, noti->priv_id) == 0) {
			ret = NOTIFICATION_ERROR_NONE;
		} else {
			ret = NOTIFICATION_ERROR_FROM_DB;
		}
	} else {
		ret = NOTIFICATION_ERROR_FROM_DB;
	}
err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

int notification_noti_get_by_priv_id(notification_h noti, char *pkgname, int priv_id)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;

	if (priv_id < 0 || noti == NULL) {
		ret = NOTIFICATION_ERROR_INVALID_DATA;
		goto err;
	}

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	char *base_query = "select "
			 "type, layout, caller_pkgname, launch_pkgname, image_path, group_id, priv_id, "
			 "b_text, b_key, b_format_args, num_format_args, "
			 "text_domain, text_dir, time, insert_time, args, group_args, "
			 "b_execute_option, b_service_responding, b_service_single_launch, b_service_multi_launch, "
			 "sound_type, sound_path, vibration_type, vibration_path, led_operation, led_argb, led_on_ms, led_off_ms, "
			 "flags_for_property, display_applist, progress_size, progress_percentage "
			 "from noti_list ";

	if (pkgname != NULL) {
		snprintf(query, sizeof(query), "%s where caller_pkgname = '%s' and priv_id = %d",
				base_query ,pkgname, priv_id);
	} else {
		snprintf(query, sizeof(query), "%s where priv_id = %d", base_query,  priv_id);
	}

	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("select Query : %s", query);
		NOTIFICATION_ERR("select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		_notification_noti_populate_from_stmt(stmt, noti);
		ret = NOTIFICATION_ERROR_NONE;
	} else {
		ret = NOTIFICATION_ERROR_FROM_DB;
	}
err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db != NULL) {
		notification_db_close(&db);
	}

	return ret;
}

EXPORT_API int notification_noti_update(notification_h noti)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Check private ID is exist */
	ret = _notification_noti_check_priv_id(noti, db);
	if (ret != NOTIFICATION_ERROR_ALREADY_EXIST_ID) {
		ret = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto err;
	}

	/* make update query */
	ret = _notification_noti_make_update_query(noti, query, sizeof(query));
	if (ret != NOTIFICATION_ERROR_NONE) {
		goto err;
	}

	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert Query : %s", query);
		NOTIFICATION_ERR("Insert DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	ret = _notification_noti_bind_query_double(stmt, "$progress_size",noti->progress_size);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		goto err;
	}
	ret = _notification_noti_bind_query_double(stmt, "$progress_percentage",noti->progress_percentage);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		goto err;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_OK || ret == SQLITE_DONE) {
		ret = NOTIFICATION_ERROR_NONE;
	} else {
		ret = NOTIFICATION_ERROR_FROM_DB;
	}
err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

EXPORT_API int notification_noti_delete_all(notification_type_e type, const char *pkgname, int *num_deleted, int **list_deleted_rowid)
{
	int ret = NOTIFICATION_ERROR_NONE;
	int ret_tmp = NOTIFICATION_ERROR_NONE;
	int i = 0, data_cnt = 0;
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char buf[128] = { 0, };
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_base[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where[NOTIFICATION_QUERY_MAX] = { 0, };

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	if (pkgname == NULL) {
		if (type != NOTIFICATION_TYPE_NONE) {
			snprintf(query_where, sizeof(query_where),
				 "where type = %d ", type);
		}
	} else {
		if (type == NOTIFICATION_TYPE_NONE) {
			snprintf(query_where, sizeof(query_where),
				 "where caller_pkgname = '%s' ", pkgname);
		} else {
			snprintf(query_where, sizeof(query_where),
				 "where caller_pkgname = '%s' and type = %d ",
				 pkgname, type);
		}
	}

	if (num_deleted != NULL) {
		*num_deleted = 0;
	}
	if (list_deleted_rowid != NULL) {
		*list_deleted_rowid = NULL;
		snprintf(query, sizeof(query),
				"select priv_id from noti_list %s ", query_where);

		ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			NOTIFICATION_ERR("Select Query : %s", query);
			NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
					 sqlite3_errmsg(db));

			ret = NOTIFICATION_ERROR_FROM_DB;
			goto err;
		}

		while(sqlite3_step(stmt) == SQLITE_ROW) {
			if (data_cnt % 8 == 0) {
				int *tmp;

				tmp = (int *)realloc(*list_deleted_rowid, sizeof(int) * (data_cnt + 8 + 1));
				if (tmp) {
					*list_deleted_rowid = tmp;
				} else {
					NOTIFICATION_ERR("Heap: %s\n", strerror(errno));
					/*!
					 * \TODO
					 * How can I handle this?
					 */
					free(*list_deleted_rowid);
					*list_deleted_rowid = NULL;
					ret = NOTIFICATION_ERROR_NO_MEMORY;
					goto err;
				}
			}
			*((*list_deleted_rowid) + data_cnt) = sqlite3_column_int(stmt, 0);
			data_cnt++;
		}

		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}

		if (data_cnt > 0) {
			query_where[0] = '\0';
			snprintf(query_base, sizeof(query_base) - 1, "delete from noti_list");
			for (i = 0; i < data_cnt ; i++) {
				if (i % NOTI_BURST_DELETE_UNIT == 0 && i != 0) {
					snprintf(query, sizeof(query) - 1, "%s where priv_id in (%s)", query_base, query_where);
					ret_tmp = notification_db_exec(db, query, NULL);
					query_where[0] = '\0';
					if (ret == NOTIFICATION_ERROR_NONE) {
						ret = ret_tmp;
					}
				}
				snprintf(buf, sizeof(buf) - 1, "%s%d", (i % NOTI_BURST_DELETE_UNIT == 0) ? "" : ",", *((*list_deleted_rowid) + i));
				strncat(query_where, buf,sizeof(query_where) - strlen(query_where) - 1);
			}
			if ((i <= NOTI_BURST_DELETE_UNIT) || ((i % NOTI_BURST_DELETE_UNIT) > 0) ) {
				snprintf(query, sizeof(query) - 1, "%s where priv_id in (%s)", query_base, query_where);
				ret_tmp = notification_db_exec(db, query, NULL);
				if (ret == NOTIFICATION_ERROR_NONE) {
					ret = ret_tmp;
				}
			}
		} else {
			free(*list_deleted_rowid);
			*list_deleted_rowid = NULL;
		}

		if (num_deleted != NULL) {
			*num_deleted = data_cnt;
		}
	} else {
		/* Make main query */
		snprintf(query_base, sizeof(query_base), "delete from noti_list ");
		snprintf(query, sizeof(query), "%s %s", query_base, query_where);

		ret = notification_db_exec(db, query, NULL);

		if (num_deleted != NULL) {
			*num_deleted = sqlite3_changes(db);
		}
	}

err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}
	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

int notification_noti_delete_group_by_group_id(const char *pkgname,
					       int group_id, int *num_deleted, int **list_deleted_rowid)
{
	int ret = NOTIFICATION_ERROR_NONE;
	int ret_tmp = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	int i = 0, data_cnt = 0;
	sqlite3_stmt *stmt = NULL;
	char buf[128] = { 0, };
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_base[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where[NOTIFICATION_QUERY_MAX] = { 0, };

	/* Check pkgname is valid */
	if (pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	snprintf(query_where, sizeof(query_where),
			"where caller_pkgname = '%s' and group_id = %d", pkgname, group_id);

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	if (num_deleted != NULL) {
		*num_deleted = 0;
	}
	if (list_deleted_rowid != NULL) {
		*list_deleted_rowid = NULL;
		snprintf(query, sizeof(query),
				"select priv_id from noti_list %s ", query_where);

		ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_OK) {
			NOTIFICATION_ERR("Select Query : %s", query);
			NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
					 sqlite3_errmsg(db));

			ret = NOTIFICATION_ERROR_FROM_DB;
			goto err;
		}

		while(sqlite3_step(stmt) == SQLITE_ROW) {
			if (data_cnt % 8 == 0) {
				int *tmp;
				tmp = (int *)realloc(*list_deleted_rowid, sizeof(int) * (data_cnt + 8 + 1));
				if (tmp) {
					*list_deleted_rowid = tmp;
				} else {
					free(*list_deleted_rowid);
					*list_deleted_rowid = NULL;
					ret = NOTIFICATION_ERROR_NO_MEMORY;
					goto err;
				}
			}
			*((*list_deleted_rowid) + data_cnt) = sqlite3_column_int(stmt, 0);
			data_cnt++;
		}

		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}

		if (data_cnt > 0) {
			query_where[0] = '\0';
			snprintf(query_base, sizeof(query_base) - 1, "delete from noti_list");
			for (i = 0; i < data_cnt ; i++) {
				if (i % NOTI_BURST_DELETE_UNIT == 0 && i != 0) {
					snprintf(query, sizeof(query) - 1, "%s where priv_id in (%s)", query_base, query_where);
					ret_tmp = notification_db_exec(db, query, NULL);
					query_where[0] = '\0';
					if (ret == NOTIFICATION_ERROR_NONE) {
						ret = ret_tmp;
					}
				}
				snprintf(buf, sizeof(buf) - 1, "%s%d", (i % NOTI_BURST_DELETE_UNIT == 0) ? "" : ",", *((*list_deleted_rowid) + i));
				strncat(query_where, buf,sizeof(query_where) - strlen(query_where) - 1);
			}
			if ((i <= NOTI_BURST_DELETE_UNIT) || ((i % NOTI_BURST_DELETE_UNIT) > 0) ) {
				snprintf(query, sizeof(query) - 1, "%s where priv_id in (%s)", query_base, query_where);
				ret_tmp = notification_db_exec(db, query, NULL);
				if (ret == NOTIFICATION_ERROR_NONE) {
					ret = ret_tmp;
				}
			}
		} else {
			free(*list_deleted_rowid);
			*list_deleted_rowid = NULL;
		}

		if (num_deleted != NULL) {
			*num_deleted = data_cnt;
		}
	} else {
		/* Make query */
		snprintf(query, sizeof(query), "delete from noti_list %s", query_where);

		/* execute DB */
		ret = notification_db_exec(db, query, NULL);
	}

err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}
	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

int notification_noti_delete_group_by_priv_id(const char *pkgname, int priv_id)
{
	sqlite3 *db = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int internal_group_id = 0;
	int ret;

	/* Check pkgname is valid */
	if (pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Get internal group id using priv id */
	internal_group_id =
	    _notification_noti_get_internal_group_id_by_priv_id(pkgname,
								priv_id, db);

	/* Make query */
	snprintf(query, sizeof(query), "delete from noti_list "
		 "where caller_pkgname = '%s' and internal_group_id = %d",
		 pkgname, internal_group_id);

	/* execute DB */
	ret = notification_db_exec(db, query, NULL);

	/* Close DB */
	notification_db_close(&db);

	return ret;
}

EXPORT_API int notification_noti_delete_by_priv_id(const char *pkgname, int priv_id)
{
	sqlite3 *db = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret;

	/* Check pkgname is valid */
	if (pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Make query */
	snprintf(query, sizeof(query), "delete from noti_list "
		 "where caller_pkgname = '%s' and priv_id = %d", pkgname,
		 priv_id);

	/* execute DB */
	ret = notification_db_exec(db, query, NULL);

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

EXPORT_API int notification_noti_delete_by_priv_id_get_changes(const char *pkgname, int priv_id, int *num_changes)
{
	sqlite3 *db = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret;

	/* Check pkgname is valid */
	if (pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_DATA;
	}

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Make query */
	snprintf(query, sizeof(query), "delete from noti_list "
		 "where caller_pkgname = '%s' and priv_id = %d", pkgname,
		 priv_id);

	/* execute DB */
	ret = notification_db_exec(db, query, num_changes);

	if (num_changes != NULL) {
		NOTIFICATION_DBG("deleted num:%d", *num_changes);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return ret;
}

notification_error_e notification_noti_get_count(notification_type_e type,
						 const char *pkgname,
						 int group_id, int priv_id,
						 int *count)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_base[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where_more[NOTIFICATION_QUERY_MAX] = { 0, };

	int ret = 0, get_count = 0, internal_group_id = 0;
	int status = VCONFKEY_TELEPHONY_SIM_UNKNOWN;
	int flag_where = 0;
	int flag_where_more = 0;
	int ret_vconf = 0;

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Check current sim status */
	ret_vconf = vconf_get_int(VCONFKEY_TELEPHONY_SIM_SLOT, &status);

	/* Make query */
	snprintf(query_base, sizeof(query_base),
		 "select count(*) from noti_list ");

	if (pkgname != NULL) {
		if (group_id == NOTIFICATION_GROUP_ID_NONE) {
			if (priv_id == NOTIFICATION_PRIV_ID_NONE) {
				snprintf(query_where, sizeof(query_where),
					 "where caller_pkgname = '%s' ",
					 pkgname);
				flag_where = 1;
			} else {
				internal_group_id =
				    _notification_noti_get_internal_group_id_by_priv_id
				    (pkgname, priv_id, db);
				snprintf(query_where, sizeof(query_where),
					 "where caller_pkgname = '%s' and internal_group_id = %d ",
					 pkgname, internal_group_id);
				flag_where = 1;
			}
		} else {
			if (priv_id == NOTIFICATION_PRIV_ID_NONE) {
				snprintf(query_where, sizeof(query_where),
					 "where caller_pkgname = '%s' and group_id = %d ",
					 pkgname, group_id);
				flag_where = 1;
			} else {
				internal_group_id =
				    _notification_noti_get_internal_group_id_by_priv_id
				    (pkgname, priv_id, db);
				snprintf(query_where, sizeof(query_where),
					 "where caller_pkgname = '%s' and internal_group_id = %d ",
					 pkgname, internal_group_id);
				flag_where = 1;
			}
		}

	}

	if (ret_vconf == 0 && status == VCONFKEY_TELEPHONY_SIM_INSERTED) {
		if (type != NOTIFICATION_TYPE_NONE) {
			snprintf(query_where_more, sizeof(query_where_more),
				 "type = %d ", type);
			flag_where_more = 1;
		}
	} else {
		if (type != NOTIFICATION_TYPE_NONE) {
			snprintf(query_where_more, sizeof(query_where_more),
				 "type = %d and flag_simmode = 0 ", type);
			flag_where_more = 1;
		} else {
			snprintf(query_where_more, sizeof(query_where_more),
				 "flag_simmode = 0 ");
			flag_where_more = 1;
		}
	}

	if (flag_where == 1) {
		if (flag_where_more == 1) {
			snprintf(query, sizeof(query), "%s %s and %s",
				 query_base, query_where, query_where_more);
		} else {
			snprintf(query, sizeof(query), "%s %s", query_base,
				 query_where);
		}

	} else {
		if (flag_where_more == 1) {
			snprintf(query, sizeof(query), "%s where %s",
				 query_base, query_where_more);
		} else {
			snprintf(query, sizeof(query), "%s", query_base);
		}
	}

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		get_count = sqlite3_column_int(stmt, 0);
	}

	ret = NOTIFICATION_ERROR_NONE;

err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	*count = get_count;

	return ret;
}

notification_error_e notification_noti_get_grouping_list(notification_type_e type,
							 int count,
							 notification_list_h *
							 list)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_base[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where[NOTIFICATION_QUERY_MAX] = { 0, };

	int ret = 0;
	notification_list_h get_list = NULL;
	notification_h noti = NULL;
	int internal_count = 0;
	int status;

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Check current sim status */
	ret = vconf_get_int(VCONFKEY_TELEPHONY_SIM_SLOT, &status);

	/* Make query */
	snprintf(query_base, sizeof(query_base), "select "
		 "type, layout, caller_pkgname, launch_pkgname, image_path, group_id, priv_id, "
		 "b_text, b_key, b_format_args, num_format_args, "
		 "text_domain, text_dir, time, insert_time, args, group_args, "
		 "b_execute_option, b_service_responding, b_service_single_launch, b_service_multi_launch, "
		 "sound_type, sound_path, vibration_type, vibration_path, led_operation, led_argb, led_on_ms, led_off_ms, "
		 "flags_for_property, display_applist, progress_size, progress_percentage "
		 "from noti_list ");

	if (status == VCONFKEY_TELEPHONY_SIM_INSERTED) {
		if (type != NOTIFICATION_TYPE_NONE) {
			snprintf(query_where, sizeof(query_where),
				 "where type = %d ", type);
		}
	} else {
		if (type != NOTIFICATION_TYPE_NONE) {
			snprintf(query_where, sizeof(query_where),
				 "where type = %d and flag_simmode = 0 ", type);
		} else {
			snprintf(query_where, sizeof(query_where),
				 "where flag_simmode = 0 ");
		}
	}

	snprintf(query, sizeof(query),
		 "%s %s "
		 "group by internal_group_id "
		 "order by rowid desc, time desc", query_base, query_where);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		/* Make notification list */
		noti = _notification_noti_get_item(stmt);
		if (noti != NULL) {
			internal_count++;

			get_list = notification_list_append(get_list, noti);

			if (count != -1 && internal_count >= count) {
				NOTIFICATION_INFO
				    ("internal count %d >= count %d",
				     internal_count, count);
				break;
			}
		}
	}

	ret = NOTIFICATION_ERROR_NONE;

err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	if (get_list != NULL) {
		*list = notification_list_get_head(get_list);
	}

	return ret;
}

notification_error_e notification_noti_get_detail_list(const char *pkgname,
						       int group_id,
						       int priv_id, int count,
						       notification_list_h *list)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char query_base[NOTIFICATION_QUERY_MAX] = { 0, };
	char query_where[NOTIFICATION_QUERY_MAX] = { 0, };

	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	notification_list_h get_list = NULL;
	notification_h noti = NULL;
	int internal_count = 0;
	int internal_group_id = 0;
	int status = 0; /* If the vconf_get_int failed, the status will be the garbage value */

	/* Open DB */
	db = notification_db_open(DBPATH);
	if (!db) {
		return NOTIFICATION_ERROR_FROM_DB;
	}

	/* Check current sim status */
	ret = vconf_get_int(VCONFKEY_TELEPHONY_SIM_SLOT, &status);

	/* Make query */
	snprintf(query_base, sizeof(query_base), "select "
		 "type, layout, caller_pkgname, launch_pkgname, image_path, group_id, priv_id, "
		 "b_text, b_key, b_format_args, num_format_args, "
		 "text_domain, text_dir, time, insert_time, args, group_args, "
		 "b_execute_option, b_service_responding, b_service_single_launch, b_service_multi_launch, "
		 "sound_type, sound_path, vibration_type, vibration_path, led_operation, led_argb, led_on_ms, led_off_ms, "
		 "flags_for_property, display_applist, progress_size, progress_percentage "
		 "from noti_list ");

	if (priv_id == NOTIFICATION_PRIV_ID_NONE && group_id == NOTIFICATION_GROUP_ID_NONE) {
		if (status == VCONFKEY_TELEPHONY_SIM_INSERTED) {
			snprintf(query_where, sizeof(query_where),
				 "where  caller_pkgname = '%s' ",
				 pkgname, internal_group_id);
		} else {
			snprintf(query_where, sizeof(query_where),
				 "where  caller_pkgname = '%s' and flag_simmode = 0 ",
				 pkgname, internal_group_id);
		}
	} else {
		internal_group_id =
		    _notification_noti_get_internal_group_id_by_priv_id(pkgname,
									priv_id, db);

		if (status == VCONFKEY_TELEPHONY_SIM_INSERTED) {
			snprintf(query_where, sizeof(query_where),
				 "where  caller_pkgname = '%s' and internal_group_id = %d ",
				 pkgname, internal_group_id);
		} else {
			snprintf(query_where, sizeof(query_where),
				 "where  caller_pkgname = '%s' and internal_group_id = %d and flag_simmode = 0 ",
				 pkgname, internal_group_id);
		}
	}

	snprintf(query, sizeof(query),
		 "%s %s "
		 "order by rowid desc, time desc", query_base, query_where);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		ret = NOTIFICATION_ERROR_FROM_DB;
		goto err;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		/* Make notification list */
		noti = _notification_noti_get_item(stmt);
		if (noti != NULL) {
			internal_count++;

			get_list = notification_list_append(get_list, noti);

			if (count != -1 && internal_count >= count) {
				NOTIFICATION_INFO
				    ("internal count %d >= count %d",
				     internal_count, count);
				break;
			}
		}
	}

	ret = NOTIFICATION_ERROR_NONE;

err:
	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	if (get_list != NULL) {
		*list = notification_list_get_head(get_list);
	}

	return ret;
}
