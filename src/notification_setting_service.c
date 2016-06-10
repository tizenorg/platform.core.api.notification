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
		NOTIFICATION_ERR("table[%p], buf[%p], index[%d]", table, buf, index);
		return false;
	}

	if (table[index] != NULL) {
		*buf = atoi(table[index]);
		return true;
	}

	*buf = 0;
	return false;
}

static int _get_table_field_data_string(char **table, char **buf, int ucs2, int index)
{
	int ret = false;

	if ((table == NULL) || (buf == NULL) || (index < 0))  {
		NOTIFICATION_ERR("table[%p], buf[%p], index[%d]", table, buf, index);
		return false;
	}

	char *pTemp = table[index];
	int sLen = 0;
	if (pTemp == NULL) {
		*buf = NULL;
	} else {
		sLen = strlen(pTemp);
		if (sLen) {
			*buf = (char *) malloc(sLen + 1);
			if (*buf == NULL) {
				NOTIFICATION_ERR("malloc is failed");
				goto out;
			}
			memset(*buf, 0, sLen + 1);
			strncpy(*buf, pTemp, sLen);
		} else {
			*buf = NULL;
		}
	}

	ret = true;
out:

	return ret;
}

EXPORT_API int noti_setting_service_get_setting_by_package_name(const char *package_name, notification_setting_h *setting, uid_t uid)
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
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT package_name, allow_to_notify, do_not_disturb_except, visibility_class "
			"FROM %s "
			"WHERE package_name = %Q AND uid = %d", NOTIFICATION_SETTING_DB_TABLE, package_name, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("sqlite3_get_table failed [%d][%s]", sql_return, sql_query);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	if (!row_count) {
		NOTIFICATION_DBG("No setting found for [%s]", package_name);
		err = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);

	row_count = 1;

	if (!(result_setting_array = (struct notification_setting *)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed...");
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
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
	}

	return err;
}



EXPORT_API int noti_setting_get_setting_array(notification_setting_h *setting_array, int *count, uid_t uid)
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
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT package_name, allow_to_notify, do_not_disturb_except, visibility_class "
			"FROM %s WHERE uid = %d "
			"ORDER BY package_name", NOTIFICATION_SETTING_DB_TABLE, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_FROM_DB failed [%d][%s]", sql_return, sql_query);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	if (!row_count) {
		NOTIFICATION_DBG("No setting found...");
		err = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);
	if (!(result_setting_array = (struct notification_setting *)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed...");
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
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
	}

	return err;
}


EXPORT_API int noti_system_setting_load_system_setting(notification_system_setting_h *system_setting, uid_t uid)
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
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		err =  NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	sql_return = db_util_open(DBPATH, &local_db_handle, 0);

	if (sql_return != SQLITE_OK || local_db_handle == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%d]", sql_return);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sql_query = sqlite3_mprintf("SELECT do_not_disturb, visibility_class "
			"FROM %s WHERE uid = %d", NOTIFICATION_SYSTEM_SETTING_DB_TABLE, uid);

	if (!sql_query) {
		NOTIFICATION_ERR("fail to alloc query");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	sql_return = sqlite3_get_table(local_db_handle, sql_query, &query_result, &row_count, &column_count, NULL);

	if (sql_return != SQLITE_OK && sql_return != -1) {
		NOTIFICATION_ERR("sqlite3_get_table failed [%d][%s]", sql_return, sql_query);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	NOTIFICATION_DBG("row_count [%d] column_count [%d]", row_count, column_count);
	if (!(result_system_setting = (struct notification_system_setting *)malloc(sizeof(struct notification_system_setting)))) {
		NOTIFICATION_ERR("malloc failed...");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	/* no system setting record. allow everyting */
	if (!row_count) {
		NOTIFICATION_DBG("No setting found...");
		result_system_setting->do_not_disturb = 0;
		result_system_setting->visibility_class = 0;
	} else {
		col_index = column_count;
		_get_table_field_data_int(query_result, (int *)&(result_system_setting->do_not_disturb), col_index++);
		_get_table_field_data_int(query_result, &(result_system_setting->visibility_class), col_index++);
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
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
	}

	return err;
}

