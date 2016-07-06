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

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>
#include <db-util.h>
#include <tizen.h>

#include <notification.h>
#include <notification_db.h>
#include <notification_error.h>
#include <notification_debug.h>
#include <notification_private.h>
#include <notification_setting.h>
#include <notification_setting_internal.h>
#include <notification_setting_service.h>


static int _get_table_field_data_int(char  **table, int *buf, int index)
{
	if ((table == NULL) || (buf == NULL) || (index < 0))  {
		/* LCOV_EXCL_START */
		NOTIFICATION_ERR("table[%p], buf[%p], index[%d]", table, buf, index);
		return false;
		/* LCOV_EXCL_STOP */
	}

	if (table[index] != NULL) {
		*buf = atoi(table[index]);
		return true;
	}

	/* LCOV_EXCL_START */
	*buf = 0;
	return false;
	/* LCOV_EXCL_STOP */
}

static int _get_table_field_data_string(char **table, char **buf, int ucs2, int index)
{
	int ret = false;

	if ((table == NULL) || (buf == NULL) || (index < 0))  {
		/* LCOV_EXCL_START */
		NOTIFICATION_ERR("table[%p], buf[%p], index[%d]", table, buf, index);
		return false;
		/* LCOV_EXCL_STOP */
	}

	char *pTemp = table[index];
	int sLen = 0;
	if (pTemp == NULL) {
		*buf = NULL; /* LCOV_EXCL_LINE */
	} else {
		sLen = strlen(pTemp);
		if (sLen) {
			*buf = (char *) malloc(sLen + 1);
			if (*buf == NULL) {
				NOTIFICATION_ERR("malloc is failed"); /* LCOV_EXCL_LINE */
				goto out;
			}
			memset(*buf, 0, sLen + 1);
			strncpy(*buf, pTemp, sLen);
		} else {
			*buf = NULL; /* LCOV_EXCL_LINE */
		}
	}

	ret = true;
out:

	return ret;
}

static int _get_current_time(struct tm *date)
{
	time_t now;

	if (date == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	time(&now);
	localtime_r(&now, date);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API
int noti_setting_service_get_setting_by_package_name(const char *package_name, notification_setting_h *setting, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *local_db_handle = NULL;
	char *sql_query = NULL;
	char **query_result = NULL;
	int sql_return;
	int row_count = 0;
	int column_count = 0;
	int i = 0;
	int col_index = 0;
	notification_setting_h result_setting_array = NULL;

	if (package_name == NULL || setting == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER"); /* LCOV_EXCL_LINE */
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT package_name, allow_to_notify, do_not_disturb_except, visibility_class "
			"FROM %s "
			"WHERE package_name = %Q AND uid = %d", NOTIFICATION_SETTING_DB_TABLE, package_name, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query"); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("sqlite3_get_table failed [%d][%s]", sql_return, sql_query); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	if (!row_count) {
		NOTIFICATION_DBG("No setting found for [%s]", package_name); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);

	row_count = 1;

	if (!(result_setting_array = (struct notification_setting *)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed..."); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	col_index = column_count;

	_get_table_field_data_string(query_result, &(result_setting_array[i].package_name), 1, col_index++);
	_get_table_field_data_int(query_result, (int *)&(result_setting_array[i].allow_to_notify), col_index++);
	_get_table_field_data_int(query_result, (int *)&(result_setting_array[i].do_not_disturb_except), col_index++);
	_get_table_field_data_int(query_result, &(result_setting_array[i].visibility_class), col_index++);

	*setting = result_setting_array;

out:
	if (query_result)
		sqlite3_free_table(query_result);

	if (sql_query)
		sqlite3_free(sql_query);

	if (local_db_handle) {
		sql_return = db_util_close(local_db_handle);
		if (sql_return != SQLITE_OK)
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return); /* LCOV_EXCL_LINE */
	}

	return err;
}

EXPORT_API
int noti_setting_get_setting_array(notification_setting_h *setting_array, int *count, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *local_db_handle = NULL;
	char *sql_query = NULL;
	char **query_result = NULL;
	int sql_return;
	int row_count = 0;
	int column_count = 0;
	int i = 0;
	int col_index = 0;
	notification_setting_h result_setting_array = NULL;

	if (setting_array == NULL || count == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER"); /* LCOV_EXCL_LINE */
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT package_name, allow_to_notify, do_not_disturb_except, visibility_class "
			"FROM %s WHERE uid = %d "
			"ORDER BY package_name", NOTIFICATION_SETTING_DB_TABLE, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query"); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_FROM_DB failed [%d][%s]", sql_return, sql_query);	/* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	if (!row_count) {
		NOTIFICATION_DBG("No setting found..."); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);
	if (!(result_setting_array = (struct notification_setting *)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed..."); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	col_index = column_count;

	for (i = 0; i < row_count; i++) {
		_get_table_field_data_string(query_result, &(result_setting_array[i].package_name), 1, col_index++);
		_get_table_field_data_int(query_result, (int *)&(result_setting_array[i].allow_to_notify), col_index++);
		_get_table_field_data_int(query_result, (int *)&(result_setting_array[i].do_not_disturb_except), col_index++);
		_get_table_field_data_int(query_result, &(result_setting_array[i].visibility_class), col_index++);
	}

	*setting_array = result_setting_array;
	*count = row_count;

out:
	if (query_result)
			sqlite3_free_table(query_result);

	if (sql_query)
		sqlite3_free(sql_query);

	if (local_db_handle) {
		sql_return = db_util_close(local_db_handle);
		if (sql_return != SQLITE_OK)
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return); /* LCOV_EXCL_LINE */
	}

	return err;
}


EXPORT_API
int noti_system_setting_load_system_setting(notification_system_setting_h *system_setting, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *local_db_handle = NULL;
	char *sql_query = NULL;
	char **query_result = NULL;
	int sql_return;
	int row_count = 0;
	int column_count = 0;
	int col_index = 0;
	notification_system_setting_h result_system_setting = NULL;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER"); /* LCOV_EXCL_LINE */
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT do_not_disturb, visibility_class, "
			"dnd_schedule_enabled, dnd_schedule_day, "
			"dnd_start_hour, dnd_start_min, dnd_end_hour, dnd_end_min "
			"FROM %s WHERE uid = %d", NOTIFICATION_SYSTEM_SETTING_DB_TABLE, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query"); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("sqlite3_get_table failed [%d][%s]", sql_return, sql_query); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);
	if (!(result_system_setting = (struct notification_system_setting *)malloc(sizeof(struct notification_system_setting)))) {
		NOTIFICATION_ERR("malloc failed..."); /* LCOV_EXCL_LINE */
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	/* no system setting record. allow everyting */
	if (!row_count) {
		NOTIFICATION_DBG("No setting found..."); /* LCOV_EXCL_LINE */
		result_system_setting->do_not_disturb = 0;
		result_system_setting->visibility_class = 0;
		result_system_setting->dnd_schedule_enabled = 0;
		result_system_setting->dnd_schedule_day = 0;
		result_system_setting->dnd_start_hour = 0;
		result_system_setting->dnd_start_min = 0;
		result_system_setting->dnd_end_hour = 0;
		result_system_setting->dnd_end_min = 0;
	} else {
		/* LCOV_EXCL_START */
		col_index = column_count;
		_get_table_field_data_int(query_result, (int *)&(result_system_setting->do_not_disturb), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->visibility_class), col_index++);
		_get_table_field_data_int(query_result, (int *)&(result_system_setting->dnd_schedule_enabled), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->dnd_schedule_day), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->dnd_start_hour), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->dnd_start_min), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->dnd_end_hour), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->dnd_end_min), col_index++);
		/* LCOV_EXCL_STOP */
	}

	*system_setting = result_system_setting;
out:
	if (query_result)
			sqlite3_free_table(query_result);

	if (sql_query)
		sqlite3_free(sql_query);

	if (local_db_handle) {
		sql_return = db_util_close(local_db_handle);
		if (sql_return != SQLITE_OK)
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return); /* LCOV_EXCL_LINE */
	}

	return err;
}

EXPORT_API
int notification_setting_db_update(const char *package_name, int allow_to_notify, int do_not_disturb_except, int visibility_class, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	char *sqlbuf = NULL;
	int sqlret;

	if (package_name == NULL || strlen(package_name) == 0)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	sqlret = db_util_open(DBPATH, &db, 0);
	if (sqlret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d]", DBPATH, sqlret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	sqlbuf = sqlite3_mprintf("UPDATE %s SET allow_to_notify = %d, do_not_disturb_except = %d, visibility_class = %d " \
			"WHERE package_name = %Q AND uid = %d",
			NOTIFICATION_SETTING_DB_TABLE, allow_to_notify, do_not_disturb_except, visibility_class, package_name, uid);
	if (!sqlbuf) {
		NOTIFICATION_ERR("fail to alloc query");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto return_close_db;
	}

	err = notification_db_exec(db, sqlbuf, NULL);

return_close_db:
	if (sqlbuf)
		sqlite3_free(sqlbuf);

	sqlret = db_util_close(db);
	if (sqlret != SQLITE_OK)
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);

	return err;
}

EXPORT_API
int notification_setting_db_update_system_setting(int do_not_disturb, int visibility_class,
		int dnd_schedule_enabled, int dnd_schedule_day,
		int dnd_start_hour, int dnd_start_min,
		int dnd_end_hour, int dnd_end_min, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	int sqlret;
	int field_index = 1;
	sqlite3 *db = NULL;
	sqlite3_stmt *db_statement = NULL;

	sqlret = db_util_open(DBPATH, &db, 0);

	if (sqlret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d][%s]", DBPATH, sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlite3_exec(db, "BEGIN immediate;", NULL, NULL, NULL);

	sqlret = sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO notification_system_setting (uid, do_not_disturb, visibility_class, dnd_schedule_enabled, dnd_schedule_day, dnd_start_hour, dnd_start_min, dnd_end_hour, dnd_end_min) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?) ", -1, &db_statement, NULL);

	if (sqlret != SQLITE_OK) {
		NOTIFICATION_ERR("sqlite3_prepare_v2 failed [%d][%s]", sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlite3_bind_int(db_statement, field_index++, uid);
	sqlite3_bind_int(db_statement, field_index++, do_not_disturb);
	sqlite3_bind_int(db_statement, field_index++, visibility_class);
	sqlite3_bind_int(db_statement, field_index++, dnd_schedule_enabled);
	sqlite3_bind_int(db_statement, field_index++, dnd_schedule_day);
	sqlite3_bind_int(db_statement, field_index++, dnd_start_hour);
	sqlite3_bind_int(db_statement, field_index++, dnd_start_min);
	sqlite3_bind_int(db_statement, field_index++, dnd_end_hour);
	sqlite3_bind_int(db_statement, field_index++, dnd_end_min);

	sqlret = sqlite3_step(db_statement);
	if (sqlret != SQLITE_OK && sqlret != SQLITE_DONE) {
		NOTIFICATION_ERR("sqlite3_step failed [%d][%s]", sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlret = sqlite3_changes(db);

	if (sqlret == 0)
		NOTIFICATION_WARN("No changes on DB");

return_close_db:
	if (db_statement)
		sqlite3_finalize(db_statement);


	if (db) {
		if (err == NOTIFICATION_ERROR_NONE)
			sqlite3_exec(db, "END;", NULL, NULL, NULL);
		else
			sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);

		sqlret = db_util_close(db);
	}

	if (sqlret != SQLITE_OK)
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);

	return err;
}

EXPORT_API
int notification_setting_db_update_do_not_disturb(int do_not_disturb, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	int sqlret;
	sqlite3 *db = NULL;
	char *sqlbuf = NULL;

	sqlret = db_util_open(DBPATH, &db, 0);
	if (sqlret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d]", DBPATH, sqlret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	sqlbuf = sqlite3_mprintf("UPDATE notification_system_setting SET do_not_disturb = %d WHERE uid = %d", do_not_disturb, uid);
	if (!sqlbuf) {
		NOTIFICATION_ERR("fail to alloc query");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto return_close_db;
	}

	err = notification_db_exec(db, sqlbuf, NULL);

return_close_db:
	if (sqlbuf)
		sqlite3_free(sqlbuf);

	sqlret = db_util_close(db);
	if (sqlret != SQLITE_OK)
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);

	return err;
}

EXPORT_API
int noti_system_setting_set_alarm(int week_flag, int hour, int min, alarm_cb_t handler, alarm_id_t *dnd_schedule_alarm_id)
{
	int err = NOTIFICATION_ERROR_NONE;
	struct tm struct_time;
	alarm_entry_t *alarm_info = NULL;
	alarm_date_t alarm_time;
	alarm_id_t alarm_id = -1;

	err = alarmmgr_init("notification");
	if (err < 0) {
		NOTIFICATION_ERR("alarmmgr_init failed (%d)", err);
		goto out;
	}

	err = alarmmgr_set_cb(handler, NULL);
	if (err < 0) {
		NOTIFICATION_ERR("alarmmgr_set_cb failed (%d)", err);
		goto out;
	}

	err = _get_current_time(&struct_time);
	if (err != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("get_current_time failed");
		goto out;
	}

	alarm_info = alarmmgr_create_alarm();
	if (alarm_info == NULL) {
		NOTIFICATION_ERR("alarmmgr_create_alarm failed");
		goto out;
	}

	alarm_time.year = struct_time.tm_year + 1900;
	alarm_time.month = struct_time.tm_mon + 1;
	alarm_time.day = struct_time.tm_mday;
	alarm_time.hour = hour;
	alarm_time.min = min;
	alarm_time.sec = 0;

	err = alarmmgr_set_time(alarm_info, alarm_time);
	if (err != ALARMMGR_RESULT_SUCCESS) {
		NOTIFICATION_ERR("alarmmgr_set_time failed (%d)", err);
		goto out;
	}

	if (week_flag) {
		err = alarmmgr_set_repeat_mode(alarm_info, ALARM_REPEAT_MODE_WEEKLY, week_flag);
		if (err != ALARMMGR_RESULT_SUCCESS) {
			NOTIFICATION_ERR("alarmmgr_set_repeat_mode failed (%d)", err);
			goto out;
		}
	}

	err = alarmmgr_set_type(alarm_info, ALARM_TYPE_VOLATILE);
	if (err != ALARMMGR_RESULT_SUCCESS) {
		NOTIFICATION_ERR("alarmmgr_set_type failed (%d)", err);
		goto out;
	}

	err = alarmmgr_add_alarm_with_localtime(alarm_info, NULL, &alarm_id);
	if (err != ALARMMGR_RESULT_SUCCESS) {
		NOTIFICATION_ERR("alarmmgr_add_alarm_with_localtime failed (%d)", err);
		goto out;
	}

	*dnd_schedule_alarm_id = alarm_id;

	NOTIFICATION_DBG("alarm_id [%d]", *dnd_schedule_alarm_id);

out:
	if (alarm_info)
		alarmmgr_free_alarm(alarm_info);

	return err;
}
