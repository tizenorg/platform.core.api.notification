/*
 *  libnotification
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jeonghoon Park <jh1979.park@samsung.com>, Youngjoo Park <yjoo93.park@samsung.com>
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
} notification_error_e;

/** 
 * @}
 */

#endif				/* __NOTIFICATION_ERROR_H__ */
