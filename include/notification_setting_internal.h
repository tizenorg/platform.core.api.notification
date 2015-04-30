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

#ifndef INCLUDE_NOTIFICATION_SETTING_INTERNAL_H_
#define INCLUDE_NOTIFICATION_SETTING_INTERNAL_H_

#include <stdbool.h>
#include "notification.h"

#ifdef __cplusplus
extern "C" {
#endif

struct notification_setting {
	char *package_name;
	bool  allow_to_notify;
	bool  do_not_disturb_except;
	int   visibility_class;
};

int notification_setting_get_setting_array(notification_setting_h *setting_array, int *count);
int notification_setting_get_setting_by_package_name(const char *package_name, notification_setting_h *setting);

int notification_setting_get_package_name(notification_setting_h setting, char **value);
int notification_setting_set_package_name(notification_setting_h setting, char *value);

int notification_setting_get_allow_to_notify(notification_setting_h setting, bool *value);
int notification_setting_set_allow_to_notify(notification_setting_h setting, bool value);

int notification_setting_get_do_not_disturb_except(notification_setting_h setting, bool *value);
int notification_setting_set_do_not_disturb_except(notification_setting_h setting, bool value);


/* OLD IMPLEMENTATION */
int notification_setting_property_set(const char *pkgname, const char *property, const char *value) NOTIFICATION_DEPRECATED_API;
int notification_setting_property_get(const char *pkgname, const char *property, char **value) NOTIFICATION_DEPRECATED_API;

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_NOTIFICATION_SETTING_INTERNAL_H_ */
