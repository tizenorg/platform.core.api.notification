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

#ifndef __NOTIFICATION_LIST_H__
#define __NOTIFICATION_LIST_H__

#include <notification.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file notification.h
 * @brief This file contains the notification list APIs
 */

/**
 * @addtogroup NOTIFICATION_LIST
 * @{
 */

/**
 * @breief Notification list handle
 */
typedef struct _notification_list *notification_list_h;

/**
 * @brief This function get head pointer of the notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @return notification list handle if success, NULL if failure.
 * @retval notification list handle(#notification_list_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

	noti_list = notification_list_get_head(noti_list);
}
 * @endcode
 */
notification_list_h notification_list_get_head(notification_list_h list);

/**
 * @brief This function get tail pointer of the notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @return notification list handle if success, NULL if failure.
 * @retval notification list handle(#notification_list_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

	noti_list = notification_list_get_tail(noti_list);
}
 * @endcode
 */
notification_list_h notification_list_get_tail(notification_list_h list);

/**
 * @brief This function get previous pointer of the current notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @return notification list handle if success, NULL if failure.
 * @retval notification list handle(#notification_list_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

	noti_list = notification_list_get_prev(noti_list);
}
 * @endcode
 */
notification_list_h notification_list_get_prev(notification_list_h list);

/**
 * @brief This function get next pointer of the current notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @return notification list handle if success, NULL if failure.
 * @retval notification list handle(#notification_list_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

	noti_list = notification_list_get_next(noti_list);
}
 * @endcode
 */
notification_list_h notification_list_get_next(notification_list_h list);

/**
 * @brief This function get notification handle that list has.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @return notification handle if success, NULL if failure.
 * @retval notification handle(#notification_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

	noti = notification_list_get_data(noti_list);
}
 * @endcode
 */
notification_h notification_list_get_data(notification_list_h list);

/**
 * @brief This function append notification data in notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @param[in] noti notification handle
 * @return notification handle if success, NULL if failure.
 * @retval notification handle(#notification_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_list = notification_list_append(noti_list, noti);
}
 * @endcode
 */
notification_list_h notification_list_append(notification_list_h list,
					     notification_h noti);

/**
 * @brief This function remove notification data from notification list.
 * @details 
 * @remarks
 * @param[in] list notification list handle
 * @param[in] noti notification handle
 * @return notification handle if success, NULL if failure.
 * @retval notification handle(#notification_h) - success
 * @retval NULL - failure
 * @pre 
 * @post
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	
	...
	
	noti_list = notification_list_remove(noti_list, noti);
}
 * @endcode
 */
notification_list_h notification_list_remove(notification_list_h list,
					     notification_h noti);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_LIST_H__ */
