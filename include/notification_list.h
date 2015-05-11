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
 * @file notification_list.h
 * @brief This file contains the notification list APIs.
 */

/**
 * @internal
 * @addtogroup NOTIFICATION_LIST
 * @{
 */

/**
 * @brief Notification list handle.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 */
typedef struct _notification_list *notification_list_h;

/**
 * @internal
 * @brief Gets the head pointer of the notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @return Notification list handle on success, NULL on failure
 * @retval #notification_list_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Gets the tail pointer to the notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @return Notification list handle on success, NULL on failure
 * @retval #notification_list_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Gets the previous pointer of the current notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @return Notification list handle on success, NULL on failure
 * @retval #notification_list_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Gets the next pointer of the current notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @return Notification list handle on success, NULL on failure
 * @retval #notification_list_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Gets the notification handle that the list has.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @return Notification handle on success, NULL on failure
 * @retval #notification_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Appends notification data to the notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list Notification list handle
 * @param[in] noti Notification handle
 * @return Notification handle on success, NULL on failure
 * @retval #notification_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @exception #NOTIFICATION_ERROR_OUT_OF_MEMORY Out of memory
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

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
 * @internal
 * @brief Removes notification data from the notification list.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] list The notification list handle
 * @param[in] noti The notification handle
 * @return Notification handle on success,
 *         otherwise @c NULL on failure
 * @retval #notification_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_list_h
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	
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
