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

#ifndef __NOTIFICATION_SETTING_H__
#define __NOTIFICATION_SETTING_H__

#include <notification.h>

typedef struct _notification_setting_h notification_setting_h;

#ifdef __cplusplus
extern "C" {
#endif

notification_error_e notification_setting_property_set(const char *pkgname, const char *property, const char *value);
notification_error_e notification_setting_property_get(const char *pkgname, const char *property, char **value);

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_SETTING_H__ */
