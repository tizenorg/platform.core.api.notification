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
* @brief Called when new message is posted
* @param[in] message The message posted
* @param[in] data user data
* @pre notification_status_monitor_message_cb_set() used to register this callback
* @see notification_status_monitor_message_cb_set()
*/
typedef void (*notification_status_message_cb)(const char *message, void *data);

/**
* @brief register callback for receiving message
* @param[in] callback The callback function
* @param[in] data user_data
*/
int notification_status_monitor_message_cb_set(notification_status_message_cb callback, void *user_data);

/**
* @brief unregister callback for receiving message
* @param[in] callback The callback function
* @param[in] data user_data
*/
int notification_status_monitor_message_cb_unset(void);

/**
 * @addtogroup NOTIFICATION_STATUS
 * @{
 */

/**
 * @brief This function send string(message) to notification status monitor
 * @details Send the message to notification status monitor.
 * @param[in] message messages you want to post
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
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
