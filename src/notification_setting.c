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

#define NOTIFICATION_SETTING_DB "notification_setting"
#define NOTIFICATION_SETTING_DB_PATH "/opt/usr/dbspace/.notification_parser.db"

typedef struct _notification_setting_h notification_setting_h;

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
