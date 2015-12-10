/*
 *  libnotification
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyuho Jo <kyuho.jo@samsung.com>
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

#define _GNU_SOURCE /* for asprintf */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libintl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/un.h>

#include <notification.h>
#include <notification_internal.h>

EXPORT_API int notification_wait_response(notification_h noti,
							   int timeout,
							   int *respi,
							   char **respc)
{
	bundle *b, *bc = NULL;
	pid_t tid;
	const char *tid_c;
	int sock_fd, msg_fd;
	char *sock_path;
	struct sockaddr_un sock_addr;
	char msg_buffer[1024];
	ssize_t msg_size;
	struct timeval timeout_tv;
	char *resp;

	memset(msg_buffer, 0, sizeof(msg_buffer));

    /* a response packet *must* have an execute option TYPE_RESPONDING
	    with an associated bundle.
	    If its bundle does not already contain a "tid" hint (which
	    complex applications such as xwalk may overwrite), we will
	    try to find the TID and set it in the bundle ourselves. */
	notification_get_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_RESPONDING,
	NULL, &b);

	if (b == NULL)
		return NOTIFICATION_ERROR_INVALID_PARAMETER;

	tid_c = bundle_get_val(b, "tid");
	if (tid_c == NULL) {
		tid = syscall(SYS_gettid);
		asprintf((char **)&tid_c, "%d", tid);
		bc = bundle_dup(b);
		bundle_add(bc, "tid", tid_c);
		notification_set_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_RESPONDING,
						 NULL, NULL, bc);
		bundle_free(bc);
		notification_update(noti);
	}

	sock_fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (sock_fd == -1)
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;

	sock_addr.sun_family = AF_UNIX;
	asprintf(&sock_path, "/tmp/.notification-%s", tid_c);
	strncpy(sock_addr.sun_path, sock_path, sizeof(sock_addr.sun_path) - 1);
	if (bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
		close(sock_fd);
		free(sock_path);
		free((char *)tid_c);
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	if (timeout > 0) {
		timeout_tv.tv_sec = timeout;
		timeout_tv.tv_usec = 0;
		setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_tv, sizeof(timeout_tv));
		setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout_tv, sizeof(timeout_tv));
	}

	listen(sock_fd, 1);
	msg_fd = accept(sock_fd, NULL, 0);
	do {
		msg_size = read(msg_fd, msg_buffer, 1024);
	} while (msg_size > 0);

	resp = strtok(msg_buffer, "\n");
	if (resp) {
		*respi = atoi(resp);
		if (respc != NULL) {
			resp = strtok(NULL, "\n");
			if (resp)
				*respc = resp;
			else
				*respc = NULL;
		}
	} else {
		*respi = 0;
		if (respc != NULL)
			*respc = NULL;
	}

	close(sock_fd);
	unlink(sock_path);
	free(sock_path);
	free((char *)tid_c);

	return NOTIFICATION_ERROR_NONE;
}
