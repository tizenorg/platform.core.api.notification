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

typedef enum _notification_setting_property {
	NOTIFICATION_SETTING_PROPERTY_ALLOW_TO_NOTIFY = 1,
	NOTIFICATION_SETTING_PROPERTY_DO_NOT_DISTURB = 2,
	NOTIFICATION_SETTING_PROPERTY_VISIBILITY_CLASS = 3,
} notification_setting_property_e;



/**
 * @internal
 * @brief This function sets notification property for designated package.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege http://tizen.org/privilege/core/notification
 * @param[in] pkgname The name of designated package
 * @param[in] property name
 * @param[in] property value
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification_error_e
 */
int notification_setting_property_set(const char *pkgname, const char *property, const char *value);

/**
 * @internal
 * @brief This function gets notification property for designated package.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege http://tizen.org/privilege/core/notification
 * @param[in] pkgname The name of designated package
 * @param[in] property name
 * @param[in] pointer which can save the getting value
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification
 */
int notification_setting_property_get(const char *pkgname, const char *property, char **value);

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_SETTING_H__ */
