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
#include <tzplatform_config.h>

#include <notification_error.h>
#include <notification_debug.h>
#include <notification_db.h>

#define NOTIFICATION_DB_NAME ".notification.db"
#define CREATE_NOTIFICATION_TABLE " \
PRAGMA journal_mode = PERSIST; \
create	table if not exists noti_list ( \
			type INTEGER NOT NULL, \
			layout INTEGER NOT NULL default 0, \
			caller_pkgname TEXT NOT NULL, \
			launch_pkgname TEXT, \
			image_path TEXT, \
			group_id INTEGER default 0,  \
			internal_group_id INTEGER default 0,  \
			priv_id INTERGER NOT NULL,  \
			title_key TEXT, \
			b_text TEXT, \
			b_key TEXT, \
			tag TEXT, \
			b_format_args TEXT, \
			num_format_args INTEGER default 0, \
			text_domain TEXT, \
			text_dir TEXT, \
			time INTEGER default 0, \
			insert_time INTEGER default 0, \
			args TEXT, \
			group_args TEXT, \
			b_execute_option TEXT, \
			b_service_responding TEXT, \
			b_service_single_launch TEXT, \
			b_service_multi_launch TEXT, \
			b_event_handler_click_on_button_1 TEXT, \
			b_event_handler_click_on_button_2 TEXT, \
			b_event_handler_click_on_button_3 TEXT, \
			b_event_handler_click_on_button_4 TEXT, \
			b_event_handler_click_on_button_5 TEXT, \
			b_event_handler_click_on_button_6 TEXT, \
			b_event_handler_click_on_icon TEXT, \
			b_event_handler_click_on_thumbnail TEXT, \
			sound_type INTEGER default 0, \
			sound_path TEXT, \
			vibration_type INTEGER default 0, \
			vibration_path TEXT, \
			led_operation INTEGER default 0, \
			led_argb INTEGER default 0, \
			led_on_ms INTEGER default -1, \
			led_off_ms INTEGER default -1, \
			flags_for_property INTEGER default 0, \
			flag_simmode INTEGER default 0, \
			display_applist INTEGER, \
			progress_size DOUBLE default 0, \
			progress_percentage DOUBLE default 0, \
			rowid INTEGER PRIMARY KEY AUTOINCREMENT, \
			ongoing_flag INTEGER default 0, \
			auto_remove INTEGER default 1, \
			UNIQUE (caller_pkgname, priv_id)  \
		); \
		create table if not exists noti_group_data ( \
			caller_pkgname TEXT NOT NULL, \
			group_id INTEGER default 0, \
			badge INTEGER default 0, \
			title TEXT, \
			content TEXT, \
			loc_title TEXT, \
			loc_content TEXT, \
			count_display_title INTEGER, \
			count_display_content INTEGER, \
			rowid INTEGER PRIMARY KEY AUTOINCREMENT, \
			UNIQUE (caller_pkgname, group_id) \
		); \
		create table if not exists ongoing_list ( \
			caller_pkgname TEXT NOT NULL, \
			launch_pkgname TEXT, \
			icon_path TEXT, \
			group_id INTEGER default 0, \
			internal_group_id INTEGER default 0, \
			priv_id INTERGER NOT NULL, \
			title TEXT, \
			content TEXT, \
			default_content TEXT, \
			loc_title TEXT, \
			loc_content TEXT, \
			loc_default_content TEXT, \
			text_domain TEXT, \
			text_dir TEXT, \
			args TEXT, \
			group_args TEXT, \
			flag INTEGER default 0, \
			progress_size DOUBLE default 0, \
			progress_percentage DOUBLE default 0, \
			rowid INTEGER PRIMARY KEY AUTOINCREMENT, \
			UNIQUE (caller_pkgname, priv_id) \
		); \
		CREATE TABLE IF NOT EXISTS notification_setting ( \
			priv_id INTEGER PRIMARY KEY AUTOINCREMENT, \
			package_name TEXT NOT NULL, \
			allow_to_notify INTEGER DEFAULT 1, \
			do_not_disturb_except INTEGER DEFAULT 0, \
			visibility_class INTEGER DEFAULT 0, \
			UNIQUE (priv_id, package_name) \
		); \
		CREATE TABLE IF NOT EXISTS notification_system_setting ( \
			priv_id INTERGER PRIMARY KEY, \
			do_not_disturb INTEGER DEFAULT 0, \
			visibility_class INTEGER DEFAULT 0 \
		); \
		INSERT INTO notification_system_setting (priv_id, do_not_disturb, visibility_class) VALUES (0, 0, 0); \
		CREATE UNIQUE INDEX package_name_idx1 ON notification_setting (package_name);"

EXPORT_API int notification_db_init()
{
	int r;
	sqlite3 *db = NULL;
	char *errmsg = NULL;
	char defname[FILENAME_MAX];
	const char *db_path = tzplatform_getenv(TZ_SYS_DB);
	if (db_path == NULL) {
		NOTIFICATION_ERR("fail to get db_path");
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}
	snprintf(defname, sizeof(defname), "%s/%s", db_path, NOTIFICATION_DB_NAME);

	NOTIFICATION_DBG("db path : %s", defname);
	r = sqlite3_open_v2(defname, &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
	if (r) {
		db_util_close(db);
		NOTIFICATION_ERR("fail to open notification db %d", r);
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	r = sqlite3_exec(db, CREATE_NOTIFICATION_TABLE, NULL, NULL, &errmsg);
	if (r != SQLITE_OK) {
		NOTIFICATION_ERR("query error(%d)(%s)", r, errmsg);
		sqlite3_free(errmsg);
		db_util_close(db);
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	db_util_close(db);
	return NOTIFICATION_ERROR_NONE;
}

sqlite3 *notification_db_open(const char *dbfile)
{
	int ret = 0;
	sqlite3 *db = 0;

	ret = db_util_open(dbfile, &db, 0);
	if (ret != SQLITE_OK) {
		if (ret == SQLITE_PERM)
			set_last_result(NOTIFICATION_ERROR_PERMISSION_DENIED);
		else
			set_last_result(NOTIFICATION_ERROR_FROM_DB);

		return NULL;
	}

	return db;
}

int notification_db_close(sqlite3 **db)
{
	int ret = 0;

	if (db == NULL || *db == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = db_util_close(*db);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("DB close error(%d)", ret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	*db = NULL;

	return NOTIFICATION_ERROR_NONE;
}

int notification_db_exec(sqlite3 *db, const char *query, int *num_changes)
{
	int ret = 0;
	sqlite3_stmt *stmt = NULL;

	if (db == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	if (query == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	if (stmt != NULL) {
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_OK || ret == SQLITE_DONE) {
			if (num_changes != NULL)
				*num_changes = sqlite3_changes(db);

			sqlite3_finalize(stmt);
		} else {
			NOTIFICATION_ERR("DB err(%d) : %s", ret,
					 sqlite3_errmsg(db));
			sqlite3_finalize(stmt);
			return NOTIFICATION_ERROR_FROM_DB;
		}
	} else {
			return NOTIFICATION_ERROR_FROM_DB;
	}

	return NOTIFICATION_ERROR_NONE;
}

char *notification_db_column_text(sqlite3_stmt * stmt, int col)
{
	const unsigned char *col_text = NULL;

	col_text = sqlite3_column_text(stmt, col);
	if (col_text == NULL || col_text[0] == '\0')
		return NULL;

	return strdup((char *)col_text);
}

bundle *notification_db_column_bundle(sqlite3_stmt * stmt, int col)
{
	const unsigned char *col_bundle = NULL;

	col_bundle = sqlite3_column_text(stmt, col);
	if (col_bundle == NULL || col_bundle[0] == '\0')
		return NULL;

	return bundle_decode(col_bundle, strlen((char *)col_bundle));
}

