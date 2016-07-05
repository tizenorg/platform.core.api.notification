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
#include <string.h>
#include <stdlib.h>
#include <db-util.h>
#include <package_manager.h>
#include <pkgmgr-info.h>
#include <tizen_type.h>
#include <tzplatform_config.h>

#include <notification.h>
#include <notification_db.h>
#include <notification_list.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_ipc.h>
#include <notification_private.h>
#include <notification_setting.h>
#include <notification_setting_internal.h>

#define NOTIFICATION_PRIVILEGE "http://tizen.org/privilege/notification"

typedef struct {
	uid_t uid;
	sqlite3 *db;
} setting_local_info;

EXPORT_API int notification_setting_get_setting_array_for_uid(notification_setting_h *setting_array, int *count, uid_t uid)
{
	int ret = NOTIFICATION_ERROR_NONE;
	if (setting_array == NULL || count == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	ret = notification_ipc_request_get_setting_array(setting_array, count, uid);
	return ret;
}

EXPORT_API int notification_setting_get_setting_array(notification_setting_h *setting_array, int *count)
{
	return notification_setting_get_setting_array_for_uid(setting_array, count, getuid());
}

EXPORT_API int notification_setting_get_setting_by_package_name_for_uid(const char *package_name, notification_setting_h *setting, uid_t uid)
{
	int ret = NOTIFICATION_ERROR_NONE;
	if (package_name == NULL || setting == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	ret = notification_ipc_request_get_setting_by_package_name(package_name, setting, uid);
	return ret;
}

/* LCOV_EXCL_START */
EXPORT_API int notification_setting_get_setting_by_package_name(const char *package_name, notification_setting_h *setting)
{
	return notification_setting_get_setting_by_package_name_for_uid(package_name, setting, getuid());
}
/* LCOV_EXCL_STOP */

/* LCOV_EXCL_START */
EXPORT_API int notification_setting_get_setting(notification_setting_h *setting)
{
	int ret;
	char *package_name = NULL;

	package_name = notification_get_pkgname_by_pid();

	if (package_name == NULL)
		return NOTIFICATION_ERROR_NOT_EXIST_ID;

	ret = notification_setting_get_setting_by_package_name(package_name, setting);

	free(package_name);

	return ret;
}
/* LCOV_EXCL_STOP */

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

	if (setting->package_name != NULL)
		free(setting->package_name);

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

EXPORT_API int notification_setting_update_setting_for_uid(notification_setting_h setting, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	err = notification_ipc_update_setting(setting, uid);
	if (err != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_setting_update_setting returns[%d]\n", err);
		goto out;
	}

out:
	return err;
}

EXPORT_API int notification_setting_update_setting(notification_setting_h setting)
{
	return notification_setting_update_setting_for_uid(setting, getuid());
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

static bool _is_package_in_setting_table(sqlite3 *db, const char *package_name, uid_t uid)
{
	sqlite3_stmt *db_statement = NULL;
	int sqlite3_ret = SQLITE_OK;
	bool err = true;
	int field_index = 1;

	sqlite3_ret = sqlite3_prepare_v2(db, "SELECT package_name FROM notification_setting WHERE uid = ? AND package_name = ?", -1, &db_statement, NULL);

	if (sqlite3_ret != SQLITE_OK) {
		NOTIFICATION_ERR("sqlite3_prepare_v2 failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = false;
		goto out;
	}

	sqlite3_bind_int(db_statement, field_index++, uid);
	sqlite3_bind_text(db_statement, field_index++, package_name, -1, SQLITE_TRANSIENT);

	sqlite3_ret = sqlite3_step(db_statement);

	if (sqlite3_ret == SQLITE_DONE) {
		NOTIFICATION_INFO("no matched package_name found[%s][%d]", package_name, sqlite3_ret);
		err = false;
		goto out;
	}

	if (sqlite3_ret != SQLITE_OK && sqlite3_ret != SQLITE_ROW) {
		NOTIFICATION_ERR("sqlite3_step failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = false;
		goto out;
	}
out:
	if (db_statement)
		sqlite3_finalize(db_statement);

	return err;
}

static int foreach_package_info_callback(const pkgmgrinfo_pkginfo_h package_info, void *user_data)
{
	setting_local_info *info = (setting_local_info *)user_data;
	sqlite3 *db = info->db;
	sqlite3_stmt *db_statement = NULL;
	char *package_name = NULL;
	int pkgmgr_ret = PACKAGE_MANAGER_ERROR_NONE;
	int sqlite3_ret = SQLITE_OK;
	int field_index = 1;
	int err = true;

	if ((pkgmgr_ret = pkgmgrinfo_pkginfo_get_pkgname(package_info, &package_name)) != PACKAGE_MANAGER_ERROR_NONE) {
		NOTIFICATION_ERR("package_info_get_package failed [%d]", pkgmgr_ret);
		err = false;
		goto out;
	}

	if (_is_package_in_setting_table(db, package_name, info->uid) == true) {
		NOTIFICATION_INFO("uid %d [%s] is exist", info->uid, package_name);
		goto out;
	}

	NOTIFICATION_INFO("uid %d [%s] will be inserted", info->uid, package_name);
	sqlite3_ret = sqlite3_prepare_v2(db, "INSERT INTO notification_setting (uid, package_name) VALUES (?, ?) ", -1, &db_statement, NULL);

	if (sqlite3_ret != SQLITE_OK) {
		NOTIFICATION_ERR("sqlite3_prepare_v2 failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = false;
		goto out;
	}

	sqlite3_bind_int(db_statement, field_index++, info->uid);
	sqlite3_bind_text(db_statement, field_index++, package_name, -1, SQLITE_TRANSIENT);

	sqlite3_ret = sqlite3_step(db_statement);

	NOTIFICATION_INFO("sqlite3_step returns[%d]", sqlite3_ret);

	if (sqlite3_ret != SQLITE_OK && sqlite3_ret != SQLITE_DONE) {
		NOTIFICATION_ERR("sqlite3_step failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = false;
	}

out:
	if (db_statement)
		sqlite3_finalize(db_statement);

	NOTIFICATION_INFO("foreach_package_info_callback returns[%d]", err);
	return err;
}

EXPORT_API int notification_setting_refresh_setting_table(uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	sqlite3 *db = NULL;
	int sqlite3_ret = SQLITE_OK;
	int pkgmgr_ret = PACKAGE_MANAGER_ERROR_NONE;
	pkgmgrinfo_pkginfo_filter_h filter;
	setting_local_info info;

	NOTIFICATION_ERR("refresh seeting table [%d]", uid);
	sqlite3_ret = sqlite3_open_v2(DBPATH, &db, SQLITE_OPEN_READWRITE, NULL);
	if (sqlite3_ret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d]", DBPATH, sqlite3_ret);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sqlite3_exec(db, "BEGIN immediate;", NULL, NULL, NULL);

	pkgmgr_ret = pkgmgrinfo_pkginfo_filter_create(&filter);
	if (pkgmgr_ret != PMINFO_R_OK) {
		NOTIFICATION_ERR("pkgmgrinfo_pkginfo_filter_create failed [%d]", pkgmgr_ret);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	pkgmgr_ret = pkgmgrinfo_pkginfo_filter_add_string(filter, PMINFO_PKGINFO_PROP_PACKAGE_PRIVILEGE, NOTIFICATION_PRIVILEGE);
	if (pkgmgr_ret != PMINFO_R_OK) {
		NOTIFICATION_ERR("pkgmgrinfo_pkginfo_filter_add_string failed [%d]", pkgmgr_ret);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	info.db = db;
	info.uid = uid;
	pkgmgr_ret = pkgmgrinfo_pkginfo_usr_filter_foreach_pkginfo(filter, foreach_package_info_callback, &info, uid);
	if (pkgmgr_ret != PMINFO_R_OK) {
		NOTIFICATION_ERR("pkgmgrinfo_pkginfo_usr_filter_foreach_pkginfo failed [%d]", pkgmgr_ret);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	pkgmgrinfo_pkginfo_filter_destroy(filter);


out:

	if (db) {
		if (err == NOTIFICATION_ERROR_NONE)
			sqlite3_exec(db, "END;", NULL, NULL, NULL);
		else
			sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);

		if ((sqlite3_ret = db_util_close(db)) != SQLITE_OK)
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlite3_ret);
	}

	NOTIFICATION_INFO("notification_setting_refresh_setting_table returns [%08X]", err);

	return err;
}

typedef enum {
	OPERATION_TYPE_INSERT_RECORD = 0,
	OPERATION_TYPE_DELETE_RECORD = 1,
} notification_setting_operation_type;

static int _notification_setting_alter_package_list(notification_setting_operation_type operation_type, const char *package_name, uid_t uid)
{
	sqlite3 *db = NULL;
	sqlite3_stmt *db_statement = NULL;
	int sqlite3_ret = SQLITE_OK;
	int field_index = 1;
	bool is_package_in_setting_table = false;
	int err = NOTIFICATION_ERROR_NONE;

	sqlite3_ret = db_util_open(DBPATH, &db, 0);

	if (sqlite3_ret != SQLITE_OK || db == NULL) {
		NOTIFICATION_ERR("db_util_open failed [%s][%d]", DBPATH, sqlite3_ret);
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sqlite3_exec(db, "BEGIN immediate;", NULL, NULL, NULL);

	is_package_in_setting_table = _is_package_in_setting_table(db, package_name, uid);

	switch (operation_type) {
	case OPERATION_TYPE_INSERT_RECORD:
		if (is_package_in_setting_table == true) {
			NOTIFICATION_INFO("[%s] is already exist", package_name);
			goto out;
		}
		NOTIFICATION_INFO("[%s] will be inserted", package_name);
		sqlite3_ret = sqlite3_prepare_v2(db, "INSERT INTO notification_setting (uid, package_name) VALUES (?, ?) ", -1, &db_statement, NULL);
		break;

	case OPERATION_TYPE_DELETE_RECORD:
		if (is_package_in_setting_table == false) {
			NOTIFICATION_INFO("[%s] is not exist", package_name);
			goto out;
		}
		NOTIFICATION_INFO("[%s] will be removed", package_name);
		sqlite3_ret = sqlite3_prepare_v2(db, "DELETE FROM notification_setting WHERE uid = ? AND package_name = ? ", -1, &db_statement, NULL);
		break;
	default:
		break;
	}

	if (sqlite3_ret != SQLITE_OK) {
		NOTIFICATION_ERR("sqlite3_prepare_v2 failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = NOTIFICATION_ERROR_FROM_DB;
		goto out;
	}

	sqlite3_bind_int(db_statement, field_index++, uid);
	sqlite3_bind_text(db_statement, field_index++, package_name, -1, SQLITE_TRANSIENT);

	sqlite3_ret = sqlite3_step(db_statement);

	if (sqlite3_ret != SQLITE_OK && sqlite3_ret != SQLITE_DONE) {
		NOTIFICATION_ERR("sqlite3_step failed [%d][%s]", sqlite3_ret, sqlite3_errmsg(db));
		err = NOTIFICATION_ERROR_FROM_DB;
	}

out:
	if (db_statement)
		sqlite3_finalize(db_statement);

	if (db) {
		NOTIFICATION_INFO("err [%d]", err);
		if (err == NOTIFICATION_ERROR_NONE)
			sqlite3_exec(db, "END;", NULL, NULL, NULL);
		else
			sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);


		if ((sqlite3_ret = db_util_close(db)) != SQLITE_OK)
			NOTIFICATION_WARN("fail to db_util_close - [%d]", sqlite3_ret);

	}

	return err;
}

/* LCOV_EXCL_START */
bool privilege_info_cb(const char *privilege_name, void *user_data)
{
	bool *found = user_data;

	if (privilege_name && strcmp(NOTIFICATION_PRIVILEGE, privilege_name) == 0) {
		*found = true;
		return false;
	}

	return true;
}
/* LCOV_EXCL_STOP */

EXPORT_API int notification_setting_insert_package_for_uid(const char *package_id, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;
	err = _notification_setting_alter_package_list(OPERATION_TYPE_INSERT_RECORD, package_id, uid);

	return err;
}

EXPORT_API int notification_setting_delete_package_for_uid(const char *package_id, uid_t uid)
{
	return _notification_setting_alter_package_list(OPERATION_TYPE_DELETE_RECORD, package_id, uid);
}

/* system setting --------------------------------*/

EXPORT_API int notification_system_setting_load_system_setting_for_uid(notification_system_setting_h *system_setting, uid_t uid)
{
	int ret = NOTIFICATION_ERROR_NONE;
	if (system_setting == NULL) {
		NOTIFICATION_ERR("NOTIFICATION_ERROR_INVALID_PARAMETER");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	ret = notification_ipc_request_load_system_setting(system_setting, uid);

	return ret;
}

EXPORT_API int notification_system_setting_load_system_setting(notification_system_setting_h *system_setting)
{
	return notification_system_setting_load_system_setting_for_uid(system_setting, getuid());
}

EXPORT_API int notification_system_setting_update_system_setting_for_uid(notification_system_setting_h system_setting, uid_t uid)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	err = notification_ipc_update_system_setting(system_setting, uid);
	if (err != NOTIFICATION_ERROR_NONE) {
		NOTIFICATION_ERR("notification_ipc_update_system_setting returns[%d]\n", err);
		goto out;
	}

out:
	return err;
}

EXPORT_API int notification_system_setting_update_system_setting(notification_system_setting_h system_setting)
{
	return notification_system_setting_update_system_setting_for_uid(system_setting, getuid());
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

EXPORT_API int notification_system_setting_dnd_schedule_get_enabled(notification_system_setting_h system_setting, bool *enabled)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || enabled == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*enabled = system_setting->dnd_schedule_enabled;
out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_set_enabled(notification_system_setting_h system_setting, bool enabled)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->dnd_schedule_enabled = enabled;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_get_day(notification_system_setting_h system_setting, int *day)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || day == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*day = system_setting->dnd_schedule_day;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_set_day(notification_system_setting_h system_setting, int day)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->dnd_schedule_day = day;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_get_start_time(notification_system_setting_h system_setting, int *hour, int *min)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || hour == NULL || min == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*hour = system_setting->dnd_start_hour;
	*min = system_setting->dnd_start_min;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_set_start_time(notification_system_setting_h system_setting, int hour, int min)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->dnd_start_hour = hour;
	system_setting->dnd_start_min = min;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_get_end_time(notification_system_setting_h system_setting, int *hour, int *min)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL || hour == NULL || min == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	*hour = system_setting->dnd_end_hour;
	*min = system_setting->dnd_end_min;

out:
	return err;
}

EXPORT_API int notification_system_setting_dnd_schedule_set_end_time(notification_system_setting_h system_setting, int hour, int min)
{
	int err = NOTIFICATION_ERROR_NONE;

	if (system_setting == NULL) {
		NOTIFICATION_ERR("Invalid parameter\n");
		err = NOTIFICATION_ERROR_INVALID_PARAMETER;
		goto out;
	}

	system_setting->dnd_end_hour = hour;
	system_setting->dnd_end_min = min;

out:
	return err;
}
