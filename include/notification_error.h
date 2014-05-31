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

#ifndef __NOTIFICATION_ERROR_H__
#define __NOTIFICATION_ERROR_H__

/**
 * @addtogroup NOTIFICATION_TYPE
 * @{
 */

/**
 * @breief Enumeration for notification error
 */
typedef enum _notification_error {
	NOTIFICATION_ERROR_NONE = 0,	/**< Success */
	NOTIFICATION_ERROR_INVALID_DATA = -1,	/**< Invalid parameter */
	NOTIFICATION_ERROR_NO_MEMORY = -2,	/**< No memory */
	NOTIFICATION_ERROR_FROM_DB = -3,	/**< Error from DB query */
	NOTIFICATION_ERROR_ALREADY_EXIST_ID = -4,	/**< Already exist private ID */
	NOTIFICATION_ERROR_FROM_DBUS = -5,	/**< Error from DBus */
	NOTIFICATION_ERROR_NOT_EXIST_ID = -6,	/**< Not exist private ID */
	NOTIFICATION_ERROR_IO = -7,	/**< disk i/o error */
	NOTIFICATION_ERROR_SERVICE_NOT_READY = -8,	/**< no reponse from master */
	NOTIFICATION_ERROR_PERMISSION_DENIED = -9, /**< Permission denied */
} notification_error_e;

/** 
 * @}
 */

#endif				/* __NOTIFICATION_ERROR_H__ */
