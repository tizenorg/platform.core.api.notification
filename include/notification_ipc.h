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

#ifndef __NOTIFICATION_IPC_H__
#define __NOTIFICATION_IPC_H__

#include <notification.h>

#define NOTIFICATION_ADDR "/tmp/.notification.service"
#define NOTIFICATION_DEL_PACKET_UNIT 10

#ifdef __cplusplus
extern "C" {
#endif

struct packet;

notification_error_e notification_ipc_monitor_init(void);
notification_error_e notification_ipc_monitor_fini(void);

notification_error_e notification_ipc_make_noti_from_packet(notification_h noti, const struct packet *packet);
struct packet *notification_ipc_make_packet_from_noti(notification_h noti, const char *command, int packet_type);

notification_error_e notification_ipc_request_insert(notification_h noti, int *priv_id);
notification_error_e notification_ipc_request_update(notification_h noti);
notification_error_e notification_ipc_request_refresh(void);
notification_error_e notification_ipc_request_delete_single(notification_type_e type, char *pkgname, int priv_id);
notification_error_e notification_ipc_request_delete_multiple(notification_type_e type, char *pkgname);

notification_error_e notification_ipc_add_deffered_task(void (*deffered_task_cb)(void *data), void *user_data);
notification_error_e notification_ipc_del_deffered_task(void (*deffered_task_cb)(void *data));

#ifdef __cplusplus
}
#endif
#endif
