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

#ifndef __NOTIFICATION_SETTING_H__
#define __NOTIFICATION_SETTING_H__

#include <stdbool.h>
#include <sys/types.h>

#include "notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct notification_setting *notification_setting_h;

int notification_setting_get_setting(notification_setting_h *setting);

int notification_setting_get_visibility_class(notification_setting_h setting, int *value);
int notification_setting_set_visibility_class(notification_setting_h setting, int value);

int notification_setting_update_setting(notification_setting_h setting);

int notification_setting_free_notification(notification_setting_h setting);

int notification_setting_refresh_setting_table(uid_t uid);

#ifdef __cplusplus
}
#endif
#endif /* __NOTIFICATION_SETTING_H__ */

