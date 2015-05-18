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
#include <db-util.h>

#include <notification.h>
#include <notification_db.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_ipc.h>
#include <notification_private.h>
#include <notification_setting.h>
#include <notification_setting_internal.h>


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
	if (pTemp == NULL)
		*buf = NULL;
	else {
		sLen = strlen(pTemp);
		if(sLen) {
			*buf = (char *) malloc(sLen + 1);
			if (*buf == NULL) {
				NOTIFICATION_ERR("malloc is failed");
				goto out;
			}
			memset(*buf, 0, sLen + 1);
			strncpy(*buf, pTemp, sLen);
		}
		else
			*buf = NULL;
	}

	ret = true;
out:

	return ret;
}



EXPORT_API int notification_setting_get_setting_array(notification_setting_h *setting_array, int *count)
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
	notification_setting_h result_setting_array= NULL;

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
			"FROM %s "
			"ORDER BY package_name", NOTIFICATION_SETTING_DB_TABLE);

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
		NOTIFICATION_DBG ("No setting found...");
		err= NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG ("row_count [%d] column_count [%d]", row_count, column_count);
	if (!(result_setting_array = (struct notification_setting*)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed...");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	col_index = column_count;

	for (i = 0; i < row_count; i++) {
		_get_table_field_data_string(query_result, &(result_setting_array[i].package_name), 1, col_index++);
		_get_table_field_data_int(query_result, (int*)&(result_setting_array[i].allow_to_notify), col_index++);
		_get_table_field_data_int(query_result, (int*)&(result_setting_array[i].do_not_disturb_except), col_index++);
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
		if (sql_return != SQLITE_OK) {
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
		}
	}

	return err;
}

EXPORT_API int notification_setting_get_setting_by_package_name(const char *package_name, notification_setting_h *setting)
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
	notification_setting_h result_setting_array= NULL;

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
			"WHERE package_name = %Q ", NOTIFICATION_SETTING_DB_TABLE, package_name);

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
		NOTIFICATION_DBG ("No setting found...");
		err= NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG ("row_count [%d] column_count [%d]", row_count, column_count);

	row_count = 1;

	if (!(result_setting_array = (struct notification_setting*)malloc(sizeof(struct notification_setting) * row_count))) {
		NOTIFICATION_ERR("malloc failed...");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	col_index = column_count;

	_get_table_field_data_string(query_result, &(result_setting_array[i].package_name), 1, col_index++);
	_get_table_field_data_int(query_result, (int*)&(result_setting_array[i].allow_to_notify), col_index++);
	_get_table_field_data_int(query_result, (int*)&(result_setting_array[i].do_not_disturb_except), col_index++);
	_get_table_field_data_int(query_result, &(result_setting_array[i].visibility_class), col_index++);

	*setting = result_setting_array;

out:
	if (query_result)
			sqlite3_free_table(query_result);

	if (sql_query)
		sqlite3_free(sql_query);

	if (local_db_handle) {
		sql_return = db_util_close(local_db_handle);
		if (sql_return != SQLITE_OK) {
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
		}
	}

	return err;
}

EXPORT_API int notification_setting_get_setting(notification_setting_h *setting)
{
	char *package_name = NULL;

	package_name = notification_get_pkgname_by_pid();

	if (package_name == NULL)
		return NOTIFICATION_ERROR_NOT_EXIST_ID;

	return notification_setting_get_setting_by_package_name(package_name, setting);
}

EXPORT_API int notification_setting_get_package_name(notification_setting_h setting, char **value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	if (setting->package_name == NULL) {
		NOTIFICATION_ERR("setting->package_name is null\n");
		err = NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	*value = SAFE_STRDUP(setting->package_name);

out:

	return err;
}

EXPORT_API int notification_setting_set_package_name(notification_setting_h setting, char *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	if (setting->package_name != NULL) {
		free(setting->package_name);
	}

	setting->package_name = SAFE_STRDUP(value);

out:

	return err;
}

EXPORT_API int notification_setting_get_allow_to_notify(notification_setting_h setting, bool *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*value = setting->allow_to_notify;

out:

	return err;
}

EXPORT_API int notification_setting_set_allow_to_notify(notification_setting_h setting, bool value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	setting->allow_to_notify = value;

out:

	return err;
}

EXPORT_API int notification_setting_get_do_not_disturb_except(notification_setting_h setting, bool *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*value = setting->do_not_disturb_except;

out:

	return err;
}

EXPORT_API int notification_setting_set_do_not_disturb_except(notification_setting_h setting, bool value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	setting->do_not_disturb_except = value;

out:

	return err;
}

EXPORT_API int notification_setting_get_visibility_class(notification_setting_h setting, int *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*value = setting->visibility_class;

out:

	return err;
}

EXPORT_API int notification_setting_set_visibility_class(notification_setting_h setting, int value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	setting->visibility_class = value;

out:

	return err;
}

EXPORT_API int notification_setting_update_setting(notification_setting_h setting)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	err = notification_ipc_update_setting(setting);
	if (err != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_setting_update_setting returns[%d]\n", err);
		goto out;
	}

out:
	return err;
}

EXPORT_API int notification_setting_free_notification(notification_setting_h setting)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
			NOTIFICATION_ERR("Invalid parameter\n");
			err = NOTIFICATION_ERROR_INVALID_PARAMETER;
			goto out;
		}

	SAFE_FREE(setting->package_name);


	/* add codes to free all properties */

	SAFE_FREE(setting);
out:

	return err;
}

EXPORT_API int notification_setting_db_update(const char *package_name, int allow_to_notify, int do_not_disturb_except, int visibility_class)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	char *sqlbuf = NULL;
	int sqlret;

	if (package_name == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	sqlret = db_util_open(DBPATH, &db, 0);
	if (sqlret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d]", DBPATH, sqlret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	sqlbuf = sqlite3_mprintf("UPDATE %s SET allow_to_notify = %d, do_not_disturb_except = %d, visibility_class = %d " \
			"WHERE package_name = %Q",
			NOTIFICATION_SETTING_DB_TABLE, allow_to_notify, do_not_disturb_except, visibility_class, package_name);
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
	if (sqlret != SQLITE_OK) {
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);
	}

	return err;
}

/* system setting --------------------------------*/

EXPORT_API int notification_system_setting_load_system_setting(notification_system_setting_h *system_setting)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *local_db_handle = NULL;
	char *sql_query = NULL;
	char **query_result = NULL;
	int sql_return;
	int row_count = 0;
	int column_count = 0;
	int col_index = 0;
	notification_system_setting_h result_system_setting= NULL;

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
			"FROM %s ", NOTIFICATION_SYSTEM_SETTING_DB_TABLE);

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
		NOTIFICATION_DBG ("No setting found...");
		err= NOTIFICATION_ERROR_NOT_EXIST_ID;
		goto out;
	}

	NOTIFICATION_DBG ("row_count [%d] column_count [%d]", row_count, column_count);

	row_count = 1;

	if (!(result_system_setting = (struct notification_system_setting*)malloc(sizeof(struct notification_system_setting)))) {
		NOTIFICATION_ERR("malloc failed...");
		err = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto out;
	}

	col_index = column_count;

	_get_table_field_data_int(query_result, (int*)&(result_system_setting->do_not_disturb), col_index++);
	_get_table_field_data_int(query_result, &(result_system_setting->visibility_class), col_index++);

	*system_setting = result_system_setting;

out:
	if (query_result)
			sqlite3_free_table(query_result);

	if (sql_query)
		sqlite3_free(sql_query);

	if (local_db_handle) {
		sql_return = db_util_close(local_db_handle);
		if (sql_return != SQLITE_OK) {
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sql_return);
		}
	}

	return err;
}

EXPORT_API int notification_system_setting_update_system_setting(notification_system_setting_h system_setting)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	err = notification_ipc_update_system_setting(system_setting);
	if (err != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_ipc_update_system_setting returns[%d]\n", err);
		goto out;
	}

out:
	return err;
}

EXPORT_API int notification_system_setting_free_system_setting(notification_system_setting_h system_setting)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
			NOTIFICATION_ERR("Invalid parameter\n");
			err = NOTIFICATION_ERROR_INVALID_PARAMETER;
			goto out;
		}

	/* add codes to free all properties */

	SAFE_FREE(system_setting);

out:

	return err;
}

EXPORT_API int notification_system_setting_get_do_not_disturb(notification_system_setting_h system_setting, bool *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*value = system_setting->do_not_disturb;

out:

	return err;
}

EXPORT_API int notification_system_setting_set_do_not_disturb(notification_system_setting_h system_setting, bool value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->do_not_disturb = value;

out:

	return err;
}

EXPORT_API int notification_system_setting_get_visibility_class(notification_system_setting_h system_setting, int *value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || value == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*value = system_setting->visibility_class;

out:

	return err;
}

EXPORT_API int notification_system_setting_set_visibility_class(notification_system_setting_h system_setting, int value)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->visibility_class = value;

out:

	return err;
}


EXPORT_API int notification_setting_db_update_system_setting(int do_not_disturb, int visibility_class)
{
	int err = NOTIFICATION_ERROR_NONE;
	int sqlret;
	int field_index = 0;
	sqlite3 *db = NULL;
	sqlite3_stmt *db_statement = NULL;

	sqlret = db_util_open(DBPATH, &db, 0);

	if (sqlret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d][%s]", DBPATH, sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlite3_exec(db, "BEGIN immediate;", NULL, NULL, NULL);

	sqlret = sqlite3_prepare_v2(db, "UPDATE notification_system_setting SET do_not_disturb = ?, visibility_class = ?;", -1, &db_statement, NULL);

	if (sqlret != SQLITE_OK) {
		NOTIFICATION_ERR("sqlite3_prepare_v2 failed [%d][%s]", sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlite3_bind_int(db_statement, field_index++, do_not_disturb);
	sqlite3_bind_int(db_statement, field_index++, visibility_class);

	sqlret = sqlite3_step(db_statement);

	if (sqlret != SQLITE_OK && sqlret != SQLITE_DONE) {
		NOTIFICATION_ERR("sqlite3_step failed [%d][%s]", sqlret, sqlite3_errmsg(db));
		err =  NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlret = sqlite3_changes(db);

	if (sqlret == 0) {
		NOTIFICATION_WARN("No changes on DB");
	}

return_close_db:
	if (db_statement) {
		sqlite3_finalize(db_statement);
	}

	if (db) {
		if (err == NOTIFICATION_ERROR_NONE) {
			sqlite3_exec(db, "END;", NULL, NULL, NULL);
		}
		else {
			sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
		}
		sqlret = db_util_close(db);
	}

	if (sqlret != SQLITE_OK) {
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);
	}

	return err;
}

/* OLD IMPLEMENTATION ----------------------------*/
#define NOTIFICATION_SETTING_DB "notification_setting"
#define NOTIFICATION_SETTING_DB_PATH "/opt/usr/dbspace/.notification_parser.db"

struct _notification_setting_h {
	char *appid;
	char *notification;
	char *sounds;
	char *contents;
	char *badge;
	char *pkgid;
};

struct prop_table {
	const char *property;
	const char *column;
	const char *default_value;
};

static struct prop_table g_prop_table[] = {
	{
		.property = "OPT_NOTIFICATION",
		.column = "notification",
		.default_value = "ON",
	},
	{
		.property = "OPT_SOUNDS",
		.column = "sounds",
		.default_value = "ON",
	},
	{
		.property = "OPT_CONTENTS",
		.column = "contents",
		.default_value = "ON",
	},
	{
		.property = "OPT_BADGE",
		.column = "badge",
		.default_value = "ON",
	},
	{
		.property = NULL,
		.column = NULL,
		.default_value = NULL,
	}
};

static const char *_get_prop_column(const char *property)
{
	int i;

	for (i = 0; g_prop_table[i].property; i++) {
		if (strcmp(g_prop_table[i].property, property))
			continue;

		return g_prop_table[i].column;
	}

	return NULL;
}

#ifdef TBD
static const char *_get_prop_default_value(const char *property)
{
	int i;

	for (i = 0; g_prop_table[i].property; i++) {
		if (strcmp(g_prop_table[i].property, property))
			continue;

		return g_prop_table[i].default_value;
	}

	return NULL;
}
#endif

static int _is_record_exist(const char *pkgname, sqlite3 *db)
{
	sqlite3_stmt *stmt = NULL;
	int count = 0;
	int result = NOTIFICATION_ERROR_NONE;
	char *sqlbuf = NULL;
	int sqlret;

	if (!pkgname)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!db)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	sqlbuf = sqlite3_mprintf("SELECT count(*) FROM %s WHERE " \
			"appid = %Q",
			NOTIFICATION_SETTING_DB, pkgname);

	if (!sqlbuf) {
		NOTIFICATION_ERR("fail to alloc sql query");
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	sqlret = sqlite3_prepare_v2(db, sqlbuf, -1, &stmt, NULL);
	if (sqlret != SQLITE_OK) {
		NOTIFICATION_ERR("DB err [%s]", sqlite3_errmsg(db));
		NOTIFICATION_ERR("query[%s]", sqlbuf);
		result = NOTIFICATION_ERROR_FROM_DB;
		goto free_and_return;
	}

	sqlret = sqlite3_step(stmt);
	if (sqlret == SQLITE_ROW)
		count = sqlite3_column_int(stmt, 0);
	else
		count = 0;

	if (count > 0)
		result = NOTIFICATION_ERROR_ALREADY_EXIST_ID;
	else
		result = NOTIFICATION_ERROR_NOT_EXIST_ID;

	free_and_return:
	if (sqlbuf)
		sqlite3_free(sqlbuf);

	if (stmt)
		sqlite3_finalize(stmt);

	return result;
}

EXPORT_API int notification_setting_db_set(const char *pkgname, const char *property, const char *value)
{
	int ret = NOTIFICATION_ERROR_NONE;
	int result = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	char *sqlbuf = NULL;
	int sqlret;
	const char *column = NULL;

	if (!pkgname)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!property)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!value)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	column = _get_prop_column(property);
	if (!column)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	sqlret = db_util_open(NOTIFICATION_SETTING_DB_PATH, &db, 0);
	if (sqlret != SQLITE_OK || !db) {
		NOTIFICATION_ERR("fail to db_util_open - [%d]", sqlret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = _is_record_exist(pkgname, db);
	if (ret != NOTIFICATION_ERROR_ALREADY_EXIST_ID) {
		result = ret;
		goto return_close_db;
	}

	sqlbuf = sqlite3_mprintf("UPDATE %s SET %s = %Q " \
			"WHERE appid = %Q",
			NOTIFICATION_SETTING_DB, column, value, pkgname);
	if (!sqlbuf) {
		NOTIFICATION_ERR("fail to alloc query");
		result = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto return_close_db;
	}

	result = notification_db_exec(db, sqlbuf, NULL);

	return_close_db:
	if (sqlbuf)
		sqlite3_free(sqlbuf);

	sqlret = db_util_close(db);
	if (sqlret != SQLITE_OK) {
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);
	}

	return result;
}

EXPORT_API int notification_setting_db_get(const char *pkgname, const char *property, char **value)
{
	int ret = NOTIFICATION_ERROR_NONE;
	int result = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	char *sqlbuf = NULL;
	sqlite3_stmt *stmt = NULL;
	int sqlret;
	const char *column = NULL;

	if (!pkgname)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!property)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!value)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	column = _get_prop_column(property);
	if (!column)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	sqlret = db_util_open(NOTIFICATION_SETTING_DB_PATH, &db, 0);
	if (sqlret != SQLITE_OK || !db) {
		NOTIFICATION_ERR("fail to db_util_open - [%d]", sqlret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = _is_record_exist(pkgname, db);
	if (ret != NOTIFICATION_ERROR_ALREADY_EXIST_ID) {
		result = ret;
		goto return_close_db;
	}

	sqlbuf = sqlite3_mprintf("SELECT %s FROM %s " \
			"WHERE appid = %Q",
			column, NOTIFICATION_SETTING_DB, pkgname);
	if (!sqlbuf) {
		NOTIFICATION_ERR("fail to alloc query");
		result = NOTIFICATION_ERROR_OUT_OF_MEMORY;
		goto return_close_db;
	}

	sqlret = sqlite3_prepare_v2(db, sqlbuf, -1, &stmt, NULL);
	if (sqlret != SQLITE_OK) {
		NOTIFICATION_ERR("fail to prepare %s - [%s]",
				sqlbuf, sqlite3_errmsg(db));
		result = NOTIFICATION_ERROR_FROM_DB;
		goto return_close_db;
	}

	sqlret = sqlite3_step(stmt);
	if (sqlret == SQLITE_ROW) {
		int get_bytes = sqlite3_column_bytes(stmt, 0);
		char *get_data = (char *)calloc(get_bytes + 1, sizeof(char));
		if (get_data != NULL) {
			memcpy(get_data, sqlite3_column_text(stmt, 0),
					get_bytes * sizeof(char));
			get_data[get_bytes] = '\0';
			*value = get_data;
		} else {
			NOTIFICATION_ERR("fail to alloc query");
			result = NOTIFICATION_ERROR_OUT_OF_MEMORY;
			goto return_close_db;
		}
	}

	return_close_db:
	if (sqlbuf)
		sqlite3_free(sqlbuf);

	if (stmt)
		sqlite3_finalize(stmt);

	sqlret = db_util_close(db);
	if (sqlret != SQLITE_OK)
		NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlret);

	return result;
}

EXPORT_API int notification_setting_property_set(const char *pkgname, const char *property, const char *value)
{
	int ret = 0;

	if (!pkgname)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!property)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!value)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = notification_ipc_noti_setting_property_set(pkgname, property, value);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_setting_property_get(const char *pkgname, const char *property, char **value)
{
	int ret = 0;

	if (!pkgname)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!property)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (!value)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = notification_ipc_noti_setting_property_get(pkgname, property, value);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	return NOTIFICATION_ERROR_NONE;
}

