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

#ifndef __NOTIFICATION_NOTI_H__
#define __NOTIFICATION_NOTI_H__

#include <notification.h>
#include <notification_list.h>

#define TAG_TIME "TIME"
#define TAG_TYPE_INVALID -1
#define TAG_TYPE_TIME 1

int notification_noti_set_tag(const char *tag, char *value, char *buf, int buf_len);

char *notification_noti_strip_tag(const char *tagged_str);

int notification_noti_get_tag_type(const char *tagged_str);

int notification_noti_insert(notification_h noti);

int notification_noti_update(notification_h noti);

int notification_noti_delete_all(notification_type_e type, const char *pkgname, int *num_deleted, int **list_deleted_rowid);

int notification_noti_delete_group_by_group_id(const char *pkgname,
					       int group_id, int *num_deleted, int **list_deleted_rowid);

int notification_noti_delete_group_by_priv_id(const char *pkgname, int priv_id);

int notification_noti_get_by_priv_id(notification_h noti, char *pkgname, int priv_id);
int notification_noti_get_by_tag(notification_h noti, char *pkgname, char* tag);

int notification_noti_delete_by_priv_id(const char *pkgname, int priv_id);
int notification_noti_delete_by_priv_id_get_changes(const char *pkgname, int priv_id, int *num_changes);

int notification_noti_get_count(notification_type_e type,
						 const char *pkgname,
						 int group_id, int priv_id,
						 int *count);

int notification_noti_get_grouping_list(notification_type_e type,
							 int count,
							 notification_list_h *list);

int notification_noti_get_detail_list(const char *pkgname,
						       int group_id,
						       int priv_id, int count,
						       notification_list_h *list);

int notification_noti_check_tag(notification_h noti);

#endif				/* __NOTIFICATION_NOTI_H__ */
