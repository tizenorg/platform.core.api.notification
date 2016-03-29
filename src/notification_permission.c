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

#include <notification.h>
#include <notification_db.h>
#include <notification_noti.h>
#include <notification_debug.h>
#include <notification_ipc.h>
#include <notification_private.h>
#include <notification_permission.h>
#include <cynara-client.h>

#define NOTIFICATION_DB_ACCESS_READ 0
#define NOTIFICATION_DB_ACCESS_WRITE 1
#define SMACK_LABEL_LEN 255

int notification_check_permission()
{
	cynara *p_cynara;

	int fd = 0;
	int ret = 0;
	char subject_label[SMACK_LABEL_LEN + 1] = "";
	char uid[10] = {0,};
	char *client_session = "";

	static bool checked_privilege = FALSE;

	if (checked_privilege)
		return NOTIFICATION_ERROR_NONE;

	ret = cynara_initialize(&p_cynara, NULL);
	if (ret != CYNARA_API_SUCCESS) {
		LOGE("cannot init cynara [%d] failed!", ret);
		ret = NOTIFICATION_ERROR_IO_ERROR;
		goto out;
	}

	fd = open("/proc/self/attr/current", O_RDONLY);
	if (fd < 0) {
		LOGE("open [%d] failed!", errno);
		ret = NOTIFICATION_ERROR_IO_ERROR;
		goto out;
	}

	ret = read(fd, subject_label, SMACK_LABEL_LEN);
	if (ret < 0) {
		LOGE("read [%d] failed!", errno);
		close(fd);
		ret = NOTIFICATION_ERROR_IO_ERROR;
		goto out;
	}
	close(fd);

	snprintf(uid, 10, "%d", getuid());
	ret = cynara_check(p_cynara, subject_label, client_session, uid,
			"http://tizen.org/privilege/notification");
	if (ret != CYNARA_API_ACCESS_ALLOWED) {
		LOGE("cynara access check [%d] failed!", ret);
		ret = NOTIFICATION_ERROR_PERMISSION_DENIED;
		goto out;
	}
	ret = NOTIFICATION_ERROR_NONE;
	checked_privilege = TRUE;
out:

	if (p_cynara)
		cynara_finish(p_cynara);

	return ret;
}

