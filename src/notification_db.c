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

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sqlite3.h>
#include <db-util.h>
#include <tizen.h>

#include <notification_error.h>
#include <notification_debug.h>
#include <notification_db.h>

sqlite3 * notification_db_open(const char *dbfile)
{
	int ret = 0;
	sqlite3 *db =0;

	ret = db_util_open(dbfile, &db, 0);
	if (ret != SQLITE_OK) {
		if (ret == SQLITE_PERM) {
			set_last_result(NOTIFICATION_ERROR_PERMISSION_DENIED);
		}
		else {
			set_last_result(NOTIFICATION_ERROR_FROM_DB);
		}
		return NULL;
	}

	return db;
}

int notification_db_close(sqlite3 ** db)
{
	int ret = 0;

	if (db == NULL || *db == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = db_util_close(*db);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("DB close error(%d)", ret);
		return NOTIFICATION_ERROR_FROM_DB;
	}

	*db = NULL;

	return NOTIFICATION_ERROR_NONE;
}

int notification_db_exec(sqlite3 * db, const char *query, int *num_changes)
{
	int ret = 0;
	sqlite3_stmt *stmt = NULL;

	if (db == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (query == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("DB err(%d) : %s", ret,
				 sqlite3_errmsg(db));
		return NOTIFICATION_ERROR_FROM_DB;
	}

	if (stmt != NULL) {
		ret = sqlite3_step(stmt);
		if (ret == SQLITE_OK || ret == SQLITE_DONE) {
			if (num_changes != NULL) {
				*num_changes = sqlite3_changes(db);
			}
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
	if (col_text == NULL || col_text[0] == '\0') {
		return NULL;
	}

	return strdup((char *)col_text);
}

bundle *notification_db_column_bundle(sqlite3_stmt * stmt, int col)
{
	const unsigned char *col_bundle = NULL;

	col_bundle = sqlite3_column_text(stmt, col);
	if (col_bundle == NULL || col_bundle[0] == '\0') {
		return NULL;
	}

	return bundle_decode(col_bundle, strlen((char *)col_bundle));
}
