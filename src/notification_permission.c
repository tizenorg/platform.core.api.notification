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

#define NOTIFICATION_DB_ACCESS_READ 0
#define NOTIFICATION_DB_ACCESS_WRITE 1

#if 0
int notification_permission_check_by_pid(const char *noti_pkgname, int pid, int access) {
	int ret = 0;
	char pkgname[512 + 1] = { 0, };
	bool preload = false;
	package_manager_compare_result_type_e compare_result;

	//get pkgname by pid
	const char *pkgname = aul_app_get_pkgname_bypid(pid);
	ret = aul_app_get_pkgname_bypid(pid, pkgname, sizeof(pkgname));
	if (ret == AUL_R_OK) {
		if (strcmp(pkgname, noti_pkgname) == 0) {
			return NOTIFICATION_ERROR_NONE;
		}

		package_manager_is_preload_package_by_app_id(pkgname, &preload);
		if (preload == true) {
			return NOTIFICATION_ERROR_NONE;
		}

		package_manager_compare_package_cert_info(noti_pkgname, &compare_result);
		if (compare_result == PACKAGE_MANAGER_COMPARE_MATCH ) {
			return NOTIFICATION_ERROR_NONE;
		}
	}

	return NOTIFICATION_ERROR_PERMISSION_DENIED;
}
#endif