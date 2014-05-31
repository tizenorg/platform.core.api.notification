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

#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#include <time.h>
#include <bundle.h>

#include <notification_error.h>
#include <notification_type.h>
#include <notification_list.h>
#include <notification_status.h>
#include <notification_setting.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file notification.h
 * @brief This file contains the notification APIs
 */

/**
 * @addtogroup NOTIFICATION_MODULE
 * @{
 */

/**
 * @brief Set absolute path for image file to display on notification view
 * @details 
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification image type
 * @param[in] image_path image file full path
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre Notification handle should be created by notification_new()
 * @post
 * @see #notification_image_type_e
 * @see notification_new()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_image(noti, NOTIFICATION_IMAGE_TYPE_ICON, APP_IMAGE_FULL_PATH);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_image(notification_h noti,
					    notification_image_type_e type,
					    const char *image_path);

/**
 * @brief Get absolute path for image file
 * @details 
 * @remarks Do not free image_path. It will be freed when notification_free() or notification_free_list().
 * @param[in] noti notification handle
 * @param[in] type notification image type
 * @param[out] image_path image file full path
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre Notification handle should be created by notification_new()
 * @post
 * @see #notification_image_type_e
 * @see notification_new()
 * @par Sample code:
 * @code
 #include <notification.h>
 ...
 {
 	char *image_path = NULL;
 	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
 	
 	noti_err  = notification_get_image(noti, NOTIFICATION_IMAGE_TYPE_ICON, &image_path);
 	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
 	}
}
 * @endcode
 */
notification_error_e notification_get_image(notification_h noti,
					    notification_image_type_e type,
					    char **image_path);

/**
 * @brief Set a timestamp
 * @details If input_time is 0, time information is set by current time.
 * @remarks
 * @param[in] noti notification handle
 * @param[in] input_time input time
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre Notification handle should be created by notification_new()
 * @post
 * @see notification_new()
 * @par Sample code:
 * @code
#include <notification.h>
 ...
  {
	 notification_h noti = NULL;
	 notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	 
	 noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	 if(noti == NULL) {
		 return;
	 }
 
	 noti_err  = notification_set_time(noti, time(NULL));
	 if(noti_err != NOTIFICATION_ERROR_NONE) {
	 	notification_free(noti);
		 return;
	 }
 }
 * @endcode
 */
notification_error_e notification_set_time(notification_h noti,
					   time_t input_time);

/**
 * @brief Get a timestamp
 * @details If ret_time is 0, time information is not set before.
 * @remarks
 * @param[in] noti notification handle
 * @param[out] ret_time return time value
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre Notification handle should be created by notification_new()
 * @post
 * @see notification_new()
 * @par Sample code:
 * @code
 #include <notification.h>
 ...
 {
	time_t ret_time;
 	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
 	
 	noti_err  = notification_get_time(noti, &ret_time);
 	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
 	}
}
 * @endcode
 */
notification_error_e notification_get_time(notification_h noti,
					   time_t * ret_time);

/**
 * @brief Get timestamp that the notification is inserted
 * @details If ret_time is 0, this notification data is not inserted before.
 * @remarks
 * @param[in] noti notification handle
 * @param[out] ret_time return time value
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre 
 * @post
 * @see 
 * @par Sample code:
 * @code
 #include <notification.h>
  ...
  {
	 time_t ret_time;
	 notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	 
	 noti_err  = notification_get_insert_time(noti, &ret_time);
	 if(noti_err != NOTIFICATION_ERROR_NONE) {
		 return;
	 }
 }
 * @endcode
 */
notification_error_e notification_get_insert_time(notification_h noti,
						  time_t * ret_time);

/**
 * @brief Set text to display on the notification view
 * @details Set title, content string. If text is formated data(only support %d, %f, %s), type - value pair should be set.
 * If %d, type NOTIFICATION_VARIABLE_TYPE_INT and value is integer value.
 * If %f, type NOTIFICATION_VARIABLE_TYPE_DOUBLE and value is double value.
 * If %s, type NOTIFICATION_VARIABLE_TYPE_STRING and value is character string.
 * If type is NOTIFICATION_VARIABLE_TYPE_COUNT, notification count is displaying with text.
 * If value is NOTIFICATION_COUNT_POS_LEFT, count is displaying at the left of the text.
 * If value is NOTIFICATION_COUNT_POS_IN, count is displaying in the text that text has %d format.
 * If value is NOTIFICATION_COUNT_POS_RIGHT, count is displaying at the right of the text.
 * Variable parameter should be terminated NOTIFICATION_VARIABLE_TYPE_NONE.
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification text type
 * @param[in] text basic text
 * @param[in] key text key for localization
 * @param[in] args_type variable parameter that type - value pair.
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre notification handle should be created by notification_new().
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_text(noti, NOTIFICATION_TEXT_TYPE_TITLE, "I'm Title", "IDS_APP_BODY_IM_TITLE", NOTIFICATION_VARIABLE_TYPE_NONE);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_text(notification_h noti,
					   notification_text_type_e type,
					   const char *text,
					   const char *key,
					   int args_type, ...);

/**
 * @brief Get the text from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification text type.
 * @param[out] text text
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	char *text = NULL;

	noti_err  = notification_get_text(noti, NOTIFICATION_TEXT_TYPE_TITLE, &text);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_text(notification_h noti,
					   notification_text_type_e type,
					   char **text);

/**
 * @brief Set a timestamp to display on the notification view
 * @details the timestamp will be converted to formatted string and it will be displayed on the setted text area
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification text type
 * @param[in] time time stamp
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre notification handle should be created by notification_new().
 * @post
 * @see
 */
notification_error_e notification_set_time_to_text(notification_h noti, notification_text_type_e type,
								time_t time);

/**
 * @brief Get a timestamp from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification text type
 * @param[in] time pointer of time stamp
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre notification handle should be created by notification_new().
 * @post
 * @see
 */
notification_error_e notification_get_time_from_text(notification_h noti, notification_text_type_e type,
								time_t *time);

/**
 * @brief Set text domain to localize the notification
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] domain text domain
 * @param[in] dir text dir
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_text_domain(noti, PACKAGE, LOCALEDIR);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_text_domain(notification_h noti,
						  const char *domain,
						  const char *dir);

/**
 * @brief Get text domain from the notification handle
 * @details
 * @remarks Do not free returned domain and dir. These are freed when notification_free or notification_free_list.
 * @param[in] noti notification handle
 * @param[out] domain domain
 * @param[out] dir locale dir
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	char *domain = NULL;
	char *dir = NULL;

	noti_err  = notification_get_text_domain(noti, &domain, &dir);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_text_domain(notification_h noti,
						  char **domain,
						  char **dir);

/**
 * @brief Set sound option for the notification
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification sound type
 * @param[in] path user sound file path
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_sound(noti, NOTIFICATION_SOUND_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_sound(notification_h noti,
					    notification_sound_type_e type,
					    const char *path);

/**
 * @brief Get sound option from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] type notification sound type
 * @param[out] path user sound file path
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	notification_sound_type_e type = NOTIFICATION_SOUND_TYPE_NONE;

	noti_err  = notification_get_sound(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_sound(notification_h noti,
					    notification_sound_type_e *type,
					    const char **path);

/**
 * @brief Set vibrate option for the notification
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification vibration type
 * @param[in] path user vibration file path
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_vibration(noti, NOTIFICATION_VIBRATION_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_vibration(notification_h noti,
						notification_vibration_type_e type,
						const char *path);

/**
  * @brief Get vibrate option from the notification handle
  * @details
  * @remarks
  * @param[in] noti notification handle
  * @param[out] type notification sound type
  * @param[out] path user vibration file path
  * @return NOTIFICATION_ERROR_NONE if success, other value if failure
  * @retval NOTIFICATION_ERROR_NONE - success
  * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
  * @pre
  * @post
  * @see 
  * @par Sample code:
  * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	notification_vibration_type_e type = NOTIFICATION_VIBRATION_TYPE_NONE;

	noti_err  = notification_get_vibration(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
notification_error_e notification_get_vibration(notification_h noti,
						notification_vibration_type_e *type,
						const char **path);

/**
 * @brief Set option of displaying the LED
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] operation led notification operation
 * @param[in] led_argb notification led color
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_led(noti, NOTIFICATION_LED_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_led(notification_h noti,
						notification_led_op_e operation,
						int led_argb);

/**
  * @brief Get option of displaying the LED from the notification handle
  * @details
  * @remarks
  * @param[in] noti notification handle
  * @param[out] operation led notification operation
  * @param[out] led_argb notification led color
  * @return NOTIFICATION_ERROR_NONE if success, other value if failure
  * @retval NOTIFICATION_ERROR_NONE - success
  * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
  * @pre
  * @post
  * @see
  * @par Sample code:
  * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	notification_led_type_e type = NOTIFICATION_LED_TYPE_NONE;

	noti_err  = notification_get_led(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
notification_error_e notification_get_led(notification_h noti,
						notification_led_op_e *operation,
						int *led_argb);

/**
 * @brief Set time period of flashing the LED
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] on_ms time for turning on the LED
 * @param[in] off_ms time for turning on the LED
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_led_time_period(noti, 100, 100);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_led_time_period(notification_h noti,
						int on_ms, int off_ms);

/**
  * @brief Get time period of flashing the LED from the notification handle
  * @details
  * @remarks
  * @param[in] noti notification handle
  * @param[out] on_ms time for turning on the LED
  * @param[out] off_ms time for turning on the LED
  * @return NOTIFICATION_ERROR_NONE if success, other value if failure
  * @retval NOTIFICATION_ERROR_NONE - success
  * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
  * @pre
  * @post
  * @see
  * @par Sample code:
  * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	int led_on_ms = 0;
	int led_off_ms = 0;

	noti_err  = notification_get_led_time_period(noti, &led_on_ms, &led_off_ms);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
notification_error_e notification_get_led_time_period(notification_h noti,
						int *on_ms, int *off_ms);

/**
 * @brief Set execution option for a notification
 * @details When notification data selected in display application, application launched by appsvc_run_service with service_handle.
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification execute type
 * @param[in] text basic text for button
 * @param[in] key value for localizaed text
 * @param[in] service_handle appsvc bundle data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	...

	b = bundle_create();
	appsvc_set_operation(b, APPSVC_OPERATION_VIEW);
	appsvc_set_uri(b,"http://www.samsung.com");

	noti_err  = notification_set_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, b);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}

	bundle_free(b);
}
 * @endcode
 */
notification_error_e notification_set_execute_option(notification_h noti,
						     notification_execute_type_e type,
						     const char *text,
						     const char *key,
						     bundle *service_handle);

/**
 * @brief Get execution option from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type notification execute type
 * @param[out] text text for button
 * @param[out] service_handle appsvc bundle data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	...

	noti_err  = notification_get_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, &b);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_execute_option(notification_h noti,
						     notification_execute_type_e type,
						     const char **text,
						     bundle **service_handle);

/**
 * @brief Set the property of the notification
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[in] flags property with | operation
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_property(noti, NOTIFICATION_PROP_DISPLAY_ONLY_SIMMODE | NOTIFICATION_PROP_DISABLE_APP_LAUNCH);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_property(notification_h noti,
					       int flags);

/**
 * @brief Get the property of the notification from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] flags notification property
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	int flags = 0;

	noti_err  = notification_get_property(noti, &flags);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_property(notification_h noti,
					       int *flags);

/**
 * @brief Set applications to display the notification
 * @details All display application is enable(NOTIFICATION_DISPLAY_APP_ALL) if you are not call this API.
 * @remarks
 * @param[in] noti notification handle
 * @param[in] applist with | operation
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_display_applist(noti, NOTIFICATION_DISPLAY_APP_NOTIFICATION_TRAY | NOTIFICATION_DISPLAY_APP_TICKER);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
}
 * @endcode
 */
notification_error_e notification_set_display_applist(notification_h noti,
						      int applist);

/**
 * @brief Get application list to display the notification from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] applist display application list.
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	int applist = 0;

	noti_err  = notification_get_display_applist(noti, &applist);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_display_applist(notification_h noti,
						      int *applist);

/**
 * @brief Set initial size for ongoing type.
 * @details After notification_insert, it does not upate size. If you want to update size, please call notification_update_size().
 * @remarks
 * @param[in] noti notification handle
 * @param[in] size double type size.
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_size(noti, 0.0);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_size(notification_h noti,
					   double size);

/**
 * @brief Get progress size.
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] size progress size
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	double size = 0.0;

	noti_err  = notification_get_size(noti, &size);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_size(notification_h noti,
					   double *size);

/**
 * @brief Set initial progress for ongoing type.
 * @details After notification_insert, it does not upate progress. If you want to update progress, please call notification_update_progress().
 * @remarks
 * @param[in] noti notification handle
 * @param[in] percentage progress percentage
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_progress(noti, 0.0);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_progress(notification_h noti,
					       double percentage);

/**
 * @brief Get progress from the notification handle
 * @details
 * @remarks At the end of the operation, progress should be 1.0
 * @param[in] noti notification handle
 * @param[out] percentage progress percentage
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	double percentage = 0.0;

	noti_err  = notification_get_progress(noti, &percentage);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_progress(notification_h noti,
					       double *percentage);

/**
 * @brief Set the package name of caller
 * @details caller_pkgname is set automatically when notification_new. We are not recommend to use this API.
 * @remarks
 * @param[in] noti notification handle
 * @param[in] pkgname caller package name
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_pkgname(noti, "com.samsung.phone");
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
notification_error_e notification_set_pkgname(notification_h noti,
					      const char *pkgname);

/**
 * @brief Get the package name of caller from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] pkgname caller package name
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	char *pkgname = NULL;

	noti_err  = notification_get_pkgname(noti, &pkgname);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_pkgname(notification_h noti,
					      char **pkgname);

/**
 * @brief Set the layout of the notification view
 * @details caller can set displaying layout of notification
 * @remarks
 * @param[in] noti notification handle
 * @param[in] type of layout
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see #notification_ly_type_e
 */
notification_error_e notification_set_layout(notification_h noti,
		notification_ly_type_e layout);

/**
 * @brief Get the layout of the notification view from the notification handle
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] type of layout
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see #notification_ly_type_e
 */
notification_error_e notification_get_layout(notification_h noti,
		notification_ly_type_e *layout);

/**
 * @brief Get Group ID and Private ID
 * @details
 * @remarks ID is valid only after inserting the notification
 * @param[in] noti notification handle
 * @param[out] group_id Group ID
 * @param[out] priv_id Private ID
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre 
 * @post
 * @see 
 * @par Sample code:
 * @code
#include <notification.h>
 ...
  {
	 notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	 int group_id, priv_id;
 
	 noti_err  = notification_get_id(noti, &group_id, &priv_id);
	 if(noti_err != NOTIFICATION_ERROR_NONE) {
		 return;
	 }
 }
 * @endcode
 */
notification_error_e notification_get_id(notification_h noti,
					 int *group_id, int *priv_id);

/**
 * @brief Get the type of notification
 * @details
 * @remarks
 * @param[in] noti notification handle
 * @param[out] type notification type
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide parameter
 * @pre 
 * @post
 * @see 
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;
	notification_type_e type;

	noti_err  = notification_get_type(noti, &type);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_type(notification_h noti,
					   notification_type_e * type);

/**
 * @brief Insert a notification
 * @details A notification will be inserted to DB and then it will be appeared on the notification area
 * When notification_new() call, if priv_id is NOTIFICATION_PRIV_ID_NONE, priv_id is return internally set priv_id.
 * @remarks
 * @param[in] noti notification handle
 * @param[out] priv_id private ID
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre notification_new()
 * @post notification_free()
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_insert(noti, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_insert(notification_h noti,
					 int *priv_id);

/**
 * @brief Update notification data.
 * @details The updated notification will be appeared on the notification area
 * @remarks
 * @param[in] noti notification handle that is created by notification_new().
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @retval NOTIFICATION_ERROR_NOT_EXIST_ID - not exist priv id
 * @pre
 * @post
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update(NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_update(notification_h noti);

/**
 * @brief Update a notification
 * @details The updated notification will be appeared on the notification area
 * @remarks This function update a notification in async manner
 * @param[in] noti notification handle that is created by notification_new().
 * @param[in] result_cb callback called when update completed
 * @param[in] user_data user data which you want to use in callback
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @retval NOTIFICATION_ERROR_NOT_EXIST_ID - not exist priv id
 * @pre
 * @post
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_async(NULL, result_cb, data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_update_async(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data);

/**
 * @brief This function clear all notification of type.
 * @details Not recommand API. Only for notification tray's clear button operation.
 * @remarks
 * @param[in] type notification type
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see #notification_type_e
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notifiation_clear(NOTIFICATION_TYPE_NOTI);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notifiation_clear(notification_type_e type);

/**
 * @brief Delete all the notifications of the type.
 * @details If pkgname is NULL, caller_pkgname is set internally.
 * @remarks
 * @param[in] pkgname caller application package name or NULL
 * @param[in] type notification type
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_delete_all_by_type(NULL, NOTIFICATION_TYPE_NOTI);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_delete_all_by_type(const char *pkgname,
						     notification_type_e type);

/**
 * @brief Delete a notification by priv_id.
 * @details If pkgname is NULL, caller_pkgname is set internally.
 * @remarks
 * @param[in] pkgname caller application package name or NULL
 * @param[in] type notification type
 * @param[in] priv_id priv ID
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_delete_by_priv_id(NULL, NOTIFICATION_TYPE_NOTI, APP_PRIV_ID);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_delete_by_priv_id(const char *pkgname,
						    notification_type_e type,
						    int priv_id);

/**
 * @brief Delete a notification with the given handle
 * @details notification_delete() remove notification data from DB and notification_free release menory of notification data.
 * @remarks
 * @param[in] noti notification handle
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
 	notificaton_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	...

	noti_err  = notification_delete(noti);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

}
 * @endcode
 */
notification_error_e notification_delete(notification_h noti);

/**
 * @brief Update progress of inserted notification. Only for the ongoing notification(NOTIFICATION_TYPE_ONGOING)
 * @details notification view on notification area could be updated
 * @remarks
 * @param[in] noti notification handle or NULL if priv_id is valid
 * @param[in] priv_id private ID
 * @param[in] progress % value of progressive data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_progress(NULL, APP_NOTI_PRIV_ID, 0.6);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_update_progress(notification_h noti,
						  int priv_id,
						  double progress);

/**
 * @brief Update size of inserted notification data. Only for the ongoing notification(NOTIFICATION_TYPE_ONGOING)
 * @details notification view on notification area could be updated
 * @remarks
 * @param[in] noti notification handle or NULL if priv_id is valid
 * @param[in] priv_id private ID
 * @param[in] size bytes of progressive data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_size(NULL, APP_NOTI_PRIV_ID, 3000000);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_update_size(notification_h noti,
					      int priv_id, double size);

/**
 * @brief Update content of inserted notification data. Only for the ongoing notification(NOTIFICATION_TYPE_ONGOING)
 * @details notification view on notification area could be updated
 * @remarks
 * @param[in] noti notification handle or NULL if priv_id is valid
 * @param[in] priv_id private ID
 * @param[in] content text to update
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - Invalide input value
 * @pre
 * @post
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_content(NULL, APP_NOTI_PRIV_ID, "updated string");
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_update_content(notification_h noti,
							 int priv_id,
							 const char *content);

/**
 * @brief Create internal structure data and return notification handle.
 * @details Available type is #NOTIFICATION_TYPE_NOTI and #NOTIFICATION_TYPE_ONGOING.
 * #NOTIFICATION_TYPE_NOTI is remaining notification data evenif device is restarted.
 * #NOTIFICATION_TYPE_ONGOING can display progressive feather, but notification data is removed after device is restarted.
 * If group_id is #NOTIFICATION_GROUP_ID_NONE, notification data is not grouping. #NOTIFICATION_GROUP_ID_DEFAULT, 
 * notification data is grouping with same title. Positive number ( > 0 ) is grouping with same number.
 * If priv_id is #NOTIFICATION_PRIV_ID_NONE, priv_id is set internally and return it when notification_insert() call.
 * Positive number and zero ( >= 0 ) is application set private ID. These ID should have be unique each application package.
 * @remarks
 * @param[in] type notification type
 * @param[in] group_id Group ID
 * @param[in] priv_id Priv ID
 * @return notification handle(#notification_h) if success, NULL if failure.
 * @retval #notification_h - success
 * @retval NULL - failure
 * @pre
 * @post
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
notification_h notification_new(notification_type_e type, int group_id,
				int priv_id);

/**
 * @brief This function create internal structure data and return notification handle.
 * @details Available type is #NOTIFICATION_TYPE_NOTI and #NOTIFICATION_TYPE_ONGOING.
 * #NOTIFICATION_TYPE_NOTI is remaining notification data evenif device is restarted.
 * #NOTIFICATION_TYPE_ONGOING can display progressive feather, but notification data is removed after device is restarted.
 * @remarks
 * @param[in] type notification type
 * @return notification handle(#notification_h) if success, NULL if failure.
 * @retval #notification_h - success
 * @retval NULL - failure
 * @pre
 * @post
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
notification_h notification_create(notification_type_e type);

/**
 * @brief load a notification from the notification DB with private id
 * @details
 * @remarks
 * @param[in] type notification type
 * @param[in] group_id Group ID
 * @param[in] priv_id Priv ID
 * @return notification handle(#notification_h) if success, NULL if failure.
 * @retval #notification_h - success
 * @retval NULL - failure
 * @pre
 * @post
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_load("org.tizen.app", priv_id);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
notification_h notification_load(char *pkgname,
				int priv_id);

/**
 * @brief Create a notification clone
 * @details Newly created notification handle is returned.
 * @remarks This clone notification handle should be call notification_free().
 * @param[in] noti notification handle
 * @param[out] clone newly created notification handle that has same with input noti.
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre
 * @post
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	notification_h clone = NULL;

	notification_clone(noti, &clone);
	...
}
 * @endcode
 */
notification_error_e notification_clone(notification_h noti, notification_h *clone);

/**
 * @brief Free internal structure data of notification handle.
 * @details Internal data of notification handle is released. Notification data that inserted is not deleted.
 * @remarks
 * @param[in] noti notification handle
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre notification_new()
 * @post
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}
	...

	noti_err = notification_free(noti);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_free(notification_h noti);

/**
 * @}
 */

/**
 * @addtogroup NOTIFICATION_LIST
 * @{
 */

/**
 * @brief This function return notification list handle.
 * @details If count is -1, all of notification list is returned.
 * @remarks
 * @param[in] type notification type
 * @param[in] count returned notification data number
 * @param[out] NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
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

	noti_err = notification_get_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_list(notification_type_e type,
					   int count,
					   notification_list_h * list);

/**
 * @brief This function will be deprecated.
 * @see notification_get_grouping_list()
 *
 */
notification_error_e notification_get_grouping_list(notification_type_e type,
						    int count,
						    notification_list_h *list);

/**
 * @brief This function return notification detail list handle of grouping data.
 * @details If count is -1, all of notification list is returned.
 * @remarks
 * @param[in] pkgname caller application package name
 * @param[in] group_id group id
 * @param[in] priv_id private id
 * @param[in] count returned notification data number
 * @param[out] list notification list handle
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
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

	noti_err = notification_get_detail_list(pkgname, group_id, priv_id, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_get_detail_list(const char *pkgname,
						  int group_id,
						  int priv_id,
						  int count,
						  notification_list_h *list);

/**
 * @brief Free notification list
 * @details
 * @remarks
 * @param[in] list notification list handle
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre notification_get_grouping_list() or notification_get_detail_list ()
 * @post
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	notification_error_e noti_err = NOTIFICATION_ERROR_NONE;

	...

	noti_err = notification_free_list(noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e notification_free_list(notification_list_h list);

/**
 * @}
 */

/**
 * @brief Register a callback for all notification events
 * @details The registered callback could be called for all notification events
 * @remarks
 * @param[in] changed_cb callback function
 * @param[in] user_data user data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre notification_new()
 * @post
 * @see notification_unresister_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_resister_changed_cb(app_changed_cb, user_data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e
notification_resister_changed_cb(
	void (*changed_cb)(void *data, notification_type_e type),
	void *user_data);

/**
 * @brief Unregister a callback for all notification events
 * @details
 * @remarks
 * @param[in] changed_cb callback function
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre
 * @post
 * @see notification_resister_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_unresister_changed_cb(app_changed_cb);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e
notification_unresister_changed_cb(
	void (*changed_cb)(void *data, notification_type_e type));

/**
 * @brief Register a callback for all notification events
 * @details The registered callback could be called for all notification events
 * @remarks
 * @param[in] changed_cb callback function
 * @param[in] user_data user data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre
 * @post
 * @see notification_unregister_detailed_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_resister_changed_cb(app_changed_cb, user_data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e
notification_register_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data);

/**
 * @brief Unregister a callback for all notification events
 * @details
 * @remarks
 * @param[in] changed_cb callback function
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre
 * @post
 * @see notification_register_detailed_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_unresister_changed_cb(app_changed_cb);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
notification_error_e
notification_unregister_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data);

/**
 * @brief Get the information of notification event
 * @details
 * @remarks
 * @param[in] list notification list handle
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure.
 * @retval NOTIFICATION_ERROR_NONE - success
 * @retval NOTIFICATION_ERROR_INVALID_DATA - invalid parameter
 * @pre notification_get_grouping_list() or notification_get_detail_list ()
 * @post
 * @see #notification_op
 */
notification_error_e notification_op_get_data(notification_op *noti_op,
						       notification_op_data_type_e type,
						       void *data);

/**
 * @}
 */

void notification_call_changed_cb(notification_op *op_list, int op_num);

int notification_is_service_ready(void);

notification_error_e notification_add_deffered_task(
		void (*deffered_task_cb)(void *data), void *user_data);

notification_error_e notification_del_deffered_task(
		void (*deffered_task_cb)(void *data));

/**
 * @addtogroup NOTIFICATION_DEPRECATED
 * @{
 */

/**
 * @brief This function will be deprecated.
 * @see notification_set_image()
 *
 */
notification_error_e notification_set_icon(notification_h noti,
					   const char *icon_path);

/**
 * @brief This function will be deprecated.
 * @see notification_get_image()
 *
 */
notification_error_e notification_get_icon(notification_h noti,
					   char **icon_path);

/**
 * @brief This function will be deprecated.
 * @see notification_set_text()
 *
 */
notification_error_e notification_set_title(notification_h noti,
					    const char *title,
					    const char *loc_title);

/**
 * @brief This function will be deprecated.
 * @see notification_get_text()
 *
 */
notification_error_e notification_get_title(notification_h noti,
					    char **title,
					    char **loc_title);

/**
 * @brief This function will be deprecated.
 * @see notification_set_text()
 *
 */
notification_error_e notification_set_content(notification_h noti,
					      const char *content,
					      const char *loc_content);

/**
 * @brief This function will be deprecated.
 * @see notification_get_text()
 *
 */
notification_error_e notification_get_content(notification_h noti,
					      char **content,
					      char **loc_content);

/**
 * @brief This function will be deprecated.
 * @see notification_set_execute_option()
 *
 */
notification_error_e notification_set_application(notification_h noti, const char *pkgname);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_get_execute_option()
 *
 */
notification_error_e notification_get_application(notification_h noti, char **pkgname);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_set_execute_option()
 *
 */
notification_error_e notification_set_args(notification_h noti, bundle * args, bundle * group_args);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_get_execute_option()
 *
 */
notification_error_e notification_get_args(notification_h noti, bundle ** args, bundle ** group_args);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_delete_by_priv_id()
 *
 */
notification_error_e notification_delete_group_by_group_id(const char *pkgname,
							   notification_type_e type,
							   int group_id);

/**
 * @brief This function will be deprecated.
 * @see notification_delete_by_priv_id()
 *
 */
notification_error_e notification_delete_group_by_priv_id(const char *pkgname,
							  notification_type_e type,
							  int priv_id);
/**
 * @brief This function will be deprecated.
 *
 */
notification_error_e notification_get_count(notification_type_e type,
					    const char *pkgname,
					    int group_id, int priv_id,
					    int *count);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_H__ */
