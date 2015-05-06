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
#include <notification_setting.h>
#include <notification_setting_internal.h>

#define NOTIFICATION_ADDR "/tmp/.notification.service"
#define NOTIFICATION_DEL_PACKET_UNIT 10

#ifdef __cplusplus
extern "C" {
#endif

struct packet;

int notification_ipc_monitor_init(void);
int notification_ipc_monitor_fini(void);

int notification_ipc_make_noti_from_packet(notification_h noti, const struct packet *packet);
struct packet *notification_ipc_make_packet_from_noti(notification_h noti, const char *command, int packet_type);
struct packet *notification_ipc_make_reply_packet_from_noti(notification_h noti, struct packet *packet);


int notification_ipc_request_insert(notification_h noti, int *priv_id);
int notification_ipc_request_update(notification_h noti);
int notification_ipc_request_update_async(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data);
int notification_ipc_request_refresh(void);
int notification_ipc_request_delete_single(notification_type_e type, char *pkgname, int priv_id);
int notification_ipc_request_delete_multiple(notification_type_e type, char *pkgname);

int notification_ipc_is_master_ready(void);
int notification_ipc_add_deffered_task(void (*deferred_task_cb)(void *data), void *user_data);
int notification_ipc_del_deffered_task(void (*deferred_task_cb)(void *data));

int notification_ipc_update_setting(notification_setting_h setting);
int notification_ipc_update_system_setting(notification_system_setting_h system_setting);
int notification_ipc_noti_setting_property_set(const char *pkgname, const char *property, const char *value);
int notification_ipc_noti_setting_property_get(const char *pkgname, const char *property, char **value);

int notification_ipc_request_load_noti_by_tag(notification_h noti, const char *pkgname, const char *tag);

#ifdef __cplusplus
}
#endif
#endif
