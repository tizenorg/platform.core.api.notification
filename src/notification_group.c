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
#include <stdlib.h>
#include <string.h>

#include <notification_debug.h>
#include <notification_group.h>
#include <notification_db.h>

static int _notification_group_bind_query(sqlite3_stmt * stmt, const char *name,
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

static int _notification_group_check_data_inserted(const char *pkgname,
						   int group_id, sqlite3 * db)
{
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = NOTIFICATION_ERROR_NONE, result = 0;

	snprintf(query, sizeof(query),
		 "select count(*) from noti_group_data where caller_pkgname = '%s' and group_id = %d",
		 pkgname, group_id);

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

	NOTIFICATION_INFO("Check Data Inserted : query[%s], result : [%d]",
			  query, result);

	sqlite3_finalize(stmt);

	if (result > 0) {
		return NOTIFICATION_ERROR_ALREADY_EXIST_ID;
	}

	return NOTIFICATION_ERROR_NONE;
}

notification_error_e notification_group_set_title(const char *pkgname,
						  int group_id,
						  const char *title,
						  const char *loc_title,
						  notification_count_display_type_e count_display)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int result = NOTIFICATION_ERROR_NONE;

	// db open
	db = notification_db_open(DBPATH);

	// Check pkgname & group_id
	ret = _notification_group_check_data_inserted(pkgname, group_id, db);
	if (ret == NOTIFICATION_ERROR_NONE) {
		// not exist -> insert
		snprintf(query, sizeof(query), "insert into noti_group_data ("
			 "caller_pkgname, group_id, unread_count, title, loc_title, count_display_title) values ("
			 "'%s', %d, 0, $title, $loc_title, %d)",
			 pkgname, group_id, count_display);

	} else {
		// exist -> update
		// not exist -> insert
		snprintf(query, sizeof(query), "update noti_group_data "
			 "set title = $title, loc_title = $loc_title, count_display_title = %d "
			 "where caller_pkgname = '%s' and group_id = %d",
			 count_display, pkgname, group_id);
	}

	// insert
	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert Query : %s", query);
		NOTIFICATION_ERR("Insert DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}

		if (db) {
			notification_db_close(&db);
		}
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = _notification_group_bind_query(stmt, "$title", title);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		return ret;
	}

	ret = _notification_group_bind_query(stmt, "$loc_title", loc_title);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_OK || ret == SQLITE_DONE) {
		result = NOTIFICATION_ERROR_NONE;
	} else {
		result = NOTIFICATION_ERROR_FROM_DB;
	}

	if (stmt) {
		sqlite3_finalize(stmt);
	}

	if (db) {
		notification_db_close(&db);
	}

	return result;
}

notification_error_e notification_group_get_title(const char *pkgname,
						  int group_id,
						  char **ret_title,
						  char **ret_loc_title,
						  notification_count_display_type_e *count_display)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int col = 0;

	// db open
	db = notification_db_open(DBPATH);

	snprintf(query, sizeof(query), "select "
		 "title, loc_title, count_display_title "
		 "from noti_group_data "
		 "where caller_pkgname = '%s' and group_id = %d",
		 pkgname, group_id);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		*ret_title = notification_db_column_text(stmt, col++);
		*ret_loc_title = notification_db_column_text(stmt, col++);
		*count_display = sqlite3_column_int(stmt, col++);
	}

	sqlite3_finalize(stmt);

	// db close
	if (db) {
		notification_db_close(&db);
	}

	return NOTIFICATION_ERROR_NONE;
}

notification_error_e notification_group_set_content(const char *pkgname,
						    int group_id,
						    const char *content,
						    const char *loc_content,
						    notification_count_display_type_e count_display)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int result = NOTIFICATION_ERROR_NONE;

	// db open
	db = notification_db_open(DBPATH);

	// Check pkgname & group_id
	ret = _notification_group_check_data_inserted(pkgname, group_id, db);
	if (ret == NOTIFICATION_ERROR_NONE) {
		// not exist -> insert
		snprintf(query, sizeof(query), "insert into noti_group_data ("
			 "caller_pkgname, group_id, unread_count, content, loc_content, count_display_content) values ("
			 "'%s', %d, 0, $content, $loc_content, %d)",
			 pkgname, group_id, count_display);
//              NOTIFICATION_INFO("Insert Query : %s", query);
	} else {
		// exist -> update
		// not exist -> insert
		snprintf(query, sizeof(query), "update noti_group_data "
			 "set content = $content, loc_content = $loc_content, count_display_content = %d "
			 "where caller_pkgname = '%s' and group_id = %d",
			 count_display, pkgname, group_id);
//              NOTIFICATION_INFO("Insert Query : %s", query);
	}

	// insert
	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert Query : %s", query);
		NOTIFICATION_ERR("Insert DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}

		if (db) {
			notification_db_close(&db);
		}
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = _notification_group_bind_query(stmt, "$content", content);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		return ret;
	}

	ret = _notification_group_bind_query(stmt, "$loc_content", loc_content);
	if (ret != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("Bind error : %s", sqlite3_errmsg(db));
		return ret;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_OK || ret == SQLITE_DONE) {
		result = NOTIFICATION_ERROR_NONE;
	} else {
		result = NOTIFICATION_ERROR_FROM_DB;
	}

	if (stmt) {
		sqlite3_finalize(stmt);
	}

	if (db) {
		notification_db_close(&db);
	}

	return result;
}

notification_error_e notification_group_get_content(const char *pkgname,
						    int group_id,
						    char **ret_content,
						    char **ret_loc_content,
						    notification_count_display_type_e *count_display)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int col = 0;

	// db open
	db = notification_db_open(DBPATH);

	snprintf(query, sizeof(query), "select "
		 "content, loc_content, count_display_content "
		 "from noti_group_data "
		 "where caller_pkgname = '%s' and group_id = %d",
		 pkgname, group_id);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		*ret_content = notification_db_column_text(stmt, col++);
		*ret_loc_content = notification_db_column_text(stmt, col++);
		*count_display = sqlite3_column_int(stmt, col++);
	}

	sqlite3_finalize(stmt);

	// db close
	if (db) {
		notification_db_close(&db);
	}

	return NOTIFICATION_ERROR_NONE;
}

notification_error_e notification_group_set_badge(const char *pkgname,
						  int group_id, int count)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int result = NOTIFICATION_ERROR_NONE;

	/* Open DB */
	db = notification_db_open(DBPATH);

	/* Check pkgname & group_id */
	ret = _notification_group_check_data_inserted(pkgname, group_id, db);

	/* Make query */
	if (ret == NOTIFICATION_ERROR_NONE) {
		/* Insert if does not exist */
		snprintf(query, sizeof(query), "insert into noti_group_data ("
			 "caller_pkgname, group_id, badge, content, loc_content) values ("
			 "'%s', %d, %d, '', '')", pkgname, group_id, count);

	} else {
		/* Update if exist */
		snprintf(query, sizeof(query), "update noti_group_data "
			 "set badge = %d "
			 "where caller_pkgname = '%s' and group_id = %d",
			 count, pkgname, group_id);
	}

	ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Insert Query : %s", query);
		NOTIFICATION_ERR("Insert DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));
		if (stmt) {
			sqlite3_finalize(stmt);
		}

		if (db) {
			notification_db_close(&db);
		}
		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_OK || ret == SQLITE_DONE) {
		result = NOTIFICATION_ERROR_NONE;
	} else {
		result = NOTIFICATION_ERROR_FROM_DB;
	}

	if (stmt) {
		sqlite3_finalize(stmt);
	}

	/* Close DB */
	if (db) {
		notification_db_close(&db);
	}

	return result;
}

notification_error_e notification_group_get_badge(const char *pkgname,
						  int group_id, int *count)
{
	sqlite3 *db;
	sqlite3_stmt *stmt = NULL;
	char query[NOTIFICATION_QUERY_MAX] = { 0, };
	int ret = 0;
	int col = 0;

	/* Open DB */
	db = notification_db_open(DBPATH);

	/* Make query */
	if (group_id == NOTIFICATION_GROUP_ID_NONE) {
		/* Check Group id None is exist */
		ret =
		    _notification_group_check_data_inserted(pkgname, group_id,
							    db);

		if (ret == NOTIFICATION_ERROR_NONE) {
			/* Get all of pkgname count if none group id is not exist */
			snprintf(query, sizeof(query),
				 "select sum(badge) "
				 "from noti_group_data "
				 "where caller_pkgname = '%s'", pkgname);
		} else {
			/* Get none group id count */
			snprintf(query, sizeof(query),
				 "select badge "
				 "from noti_group_data "
				 "where caller_pkgname = '%s' and group_id = %d",
				 pkgname, group_id);
		}
	} else {
		snprintf(query, sizeof(query),
			 "select badge "
			 "from noti_group_data "
			 "where caller_pkgname = '%s' and group_id = %d",
			 pkgname, group_id);
	}

	NOTIFICATION_INFO("Get badge : query[%s]", query);

	ret = sqlite3_prepare(db, query, strlen(query), &stmt, NULL);
	if (ret != SQLITE_OK) {
		NOTIFICATION_ERR("Select Query : %s", query);
		NOTIFICATION_ERR("Select DB error(%d) : %s", ret,
				 sqlite3_errmsg(db));

		return NOTIFICATION_ERROR_FROM_DB;
	}

	ret = sqlite3_step(stmt);
	if (ret == SQLITE_ROW) {
		*count = sqlite3_column_int(stmt, col++);
	}

	sqlite3_finalize(stmt);

	// db close
	if (db) {
		notification_db_close(&db);
	}

	return NOTIFICATION_ERROR_NONE;
}
