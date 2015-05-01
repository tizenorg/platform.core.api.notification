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

#ifndef __NOTIFICATION_STATUS_DEF_H__
#define __NOTIFICATION_STATUS_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @file notification_status.h
 */

/**
 * @addtogroup NOTIFICATION_STATUS
 * @{
 */

/**
 * @internal
 * @brief Called when a new message is posted.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @param[in] message The message posted
 * @param[in] data    The user data
 * @pre notification_status_monitor_message_cb_set() used to register this callback.
 * @see notification_status_monitor_message_cb_set()
*/
typedef void (*notification_status_message_cb)(const char *message, void *data);

/**
 * @internal
 * @brief Registers a callback to receive a message.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @param[in] callback The callback function
 * @param[in] data     The user_data
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_FROM_DBUS Error from DBus
 */
int notification_status_monitor_message_cb_set(notification_status_message_cb callback, void *user_data);

/**
 * @internal
 * @brief Unregisters a callback to receive a message.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @param[in] callback The callback function
 * @param[in] data     The user_data
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 */
int notification_status_monitor_message_cb_unset(void);

/**
 * @brief Sends a string (message) to the notification status monitor.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] message The messages to be posted

 * @return  #NOTIFICATION_ERROR_NONE on success,
 *          otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_FROM_DBUS Error from DBus
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see #notification_error_e
 */
int notification_status_message_post(const char *message);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __NOTIFICATION_STATUS_DEF_H__ */
