/*
 * Copyright (c) 2015 - 2016 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef __NOTIFICATION_SETTING_INTERNAL_H__
#define __NOTIFICATION_SETTING_INTERNAL_H__

#include <sys/types.h>
#include <stdbool.h>
#include "notification.h"
#include "notification_setting.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Application setting */
struct notification_setting {
	char *package_name;
	bool  allow_to_notify;
	bool  do_not_disturb_except;
	int   visibility_class;
};

int notification_setting_get_setting_array(notification_setting_h *setting_array, int *count);
int notification_setting_get_setting_array_for_uid(notification_setting_h *setting_array, int *count, uid_t uid);

int notification_setting_get_setting_by_package_name(const char *package_name, notification_setting_h *setting);
int notification_setting_get_setting_by_package_name_for_uid(const char *package_name, notification_setting_h *setting, uid_t uid);

int notification_setting_get_package_name(notification_setting_h setting, char **value);
int notification_setting_set_package_name(notification_setting_h setting, char *value);

int notification_setting_get_allow_to_notify(notification_setting_h setting, bool *value);
int notification_setting_set_allow_to_notify(notification_setting_h setting, bool value);

int notification_setting_get_do_not_disturb_except(notification_setting_h setting, bool *value);
int notification_setting_set_do_not_disturb_except(notification_setting_h setting, bool value);

int notification_setting_insert_package_for_uid(const char *package_id, uid_t uid);
int notification_setting_delete_package_for_uid(const char *package_id, uid_t uid);

int notification_setting_update_setting_for_uid(notification_setting_h setting, uid_t uid);

/* System setting */
typedef struct notification_system_setting *notification_system_setting_h;

int notification_system_setting_load_system_setting(notification_system_setting_h *system_setting);
int notification_system_setting_load_system_setting_for_uid(notification_system_setting_h *system_setting, uid_t uid);

int notification_system_setting_update_system_setting(notification_system_setting_h system_setting);
int notification_system_setting_update_system_setting_for_uid(notification_system_setting_h system_setting, uid_t uid);

int notification_system_setting_free_system_setting(notification_system_setting_h system_setting);

int notification_system_setting_get_do_not_disturb(notification_system_setting_h system_setting, bool *value);
int notification_system_setting_set_do_not_disturb(notification_system_setting_h system_setting, bool value);

int notification_system_setting_get_visibility_class(notification_system_setting_h system_setting, int *value);
int notification_system_setting_set_visibility_class(notification_system_setting_h system_setting, int value);

/* OLD IMPLEMENTATION */
int notification_setting_property_set(const char *pkgname, const char *property, const char *value) NOTIFICATION_DEPRECATED_API;
int notification_setting_property_get(const char *pkgname, const char *property, char **value) NOTIFICATION_DEPRECATED_API;

#ifdef __cplusplus
}
#endif
#endif /* __NOTIFICATION_SETTING_INTERNAL_H__ */

