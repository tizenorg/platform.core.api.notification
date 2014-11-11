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

#include <tizen.h>

/**
 * @file notification_error.h
 */

/**
 * @addtogroup NOTIFICATION_MODULE
 * @{
 */

/**
 * @brief Enumeration for notification errors.
 * @since_tizen 2.3
 */
typedef enum _notification_error {
	NOTIFICATION_ERROR_NONE = TIZEN_ERROR_NONE,	/**< Success */
	NOTIFICATION_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
	NOTIFICATION_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY,	/**< out of memory */
	NOTIFICATION_ERROR_IO_ERROR= TIZEN_ERROR_IO_ERROR,	/**< I/O error */
	NOTIFICATION_ERROR_PERMISSION_DENIED = TIZEN_ERROR_PERMISSION_DENIED, /**< Permission denied */
	NOTIFICATION_ERROR_FROM_DB = TIZEN_ERROR_NOTIFICATION | 0x01,	/**< Error from DB query */
	NOTIFICATION_ERROR_ALREADY_EXIST_ID = TIZEN_ERROR_NOTIFICATION | 0x02,	/**< Already exist private ID */
	NOTIFICATION_ERROR_FROM_DBUS = TIZEN_ERROR_NOTIFICATION | 0x03,	/**< Error from DBus */
	NOTIFICATION_ERROR_NOT_EXIST_ID = TIZEN_ERROR_NOTIFICATION | 0x04,	/**< Not exist private ID */
	NOTIFICATION_ERROR_SERVICE_NOT_READY = TIZEN_ERROR_NOTIFICATION | 0x05,	/**< No reponse from notification service */
} notification_error_e;

/** 
 * @}
 */

#endif				/* __NOTIFICATION_ERROR_H__ */
