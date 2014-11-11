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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file notification.h
 * @brief This file contains the notification API.
 */

/**
 * @addtogroup NOTIFICATION_MODULE
 * @{
 */

/**
 * @brief Sets an absolute path for an image file to display on the notification view.
 * @since_tizen 2.3
 * @param[in] noti       The notification handle
 * @param[in] type       The notification image type
 * @param[in] image_path The image file full path
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 * @see #notification_image_type_e
 * @see notification_create()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_image(notification_h noti,
					    notification_image_type_e type,
					    const char *image_path);

/**
 * @brief Gets the absolute path of an image file.
 * @since_tizen 2.3
 * @remarks Do not free @a image_path. It will be freed when notification_free() is called.
 * @param[in] noti Notification handle
 * @param[in] type Notification image type
 * @param[out] image_path image file full path
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 * @see #notification_image_type_e
 * @see notification_create()
 * @par Sample code:
 * @code
 #include <notification.h>
 ...
 {
 	char *image_path = NULL;
 	int noti_err = NOTIFICATION_ERROR_NONE;
 	
 	noti_err  = notification_get_image(noti, NOTIFICATION_IMAGE_TYPE_ICON, &image_path);
 	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
 	}
}
 * @endcode
 */
int notification_get_image(notification_h noti,
					    notification_image_type_e type,
					    char **image_path);

/**
 * @brief Sets a timestamp.
 * @details If input_time is @c 0, time information is taken from the current time.
 * @since_tizen 2.3
 * @param[in] noti       The notification handle
 * @param[in] input_time The input time
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 * @see notification_create()
 * @par Sample code:
 * @code
#include <notification.h>
 ...
  {
	 notification_h noti = NULL;
	 int noti_err = NOTIFICATION_ERROR_NONE;
	 
	 noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_time(notification_h noti,
					   time_t input_time);

/**
 * @brief Gets a timestamp.
 * @details If ret_time is @c 0, time information is not set before.
 * @since_tizen 2.3
 * @param[in]  noti     The notification handle
 * @param[out] ret_time The return time value
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 * @see notification_create()
 * @par Sample code:
 * @code
 #include <notification.h>
 ...
 {
	time_t ret_time;
 	int noti_err = NOTIFICATION_ERROR_NONE;
 	
 	noti_err  = notification_get_time(noti, &ret_time);
 	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
 	}
}
 * @endcode
 */
int notification_get_time(notification_h noti,
					   time_t * ret_time);

/**
 * @brief Gets an insertion timestamp of the notification.
 * @details If ret_time is @c 0, this notification data is not inserted before.
 * @since_tizen 2.3
 * @param[in]  noti      The notification handle
 * @param[out] ret_time  The return time value
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
 #include <notification.h>
  ...
  {
	 time_t ret_time;
	 int noti_err = NOTIFICATION_ERROR_NONE;
	 
	 noti_err  = notification_get_insert_time(noti, &ret_time);
	 if(noti_err != NOTIFICATION_ERROR_NONE) {
		 return;
	 }
 }
 * @endcode
 */
int notification_get_insert_time(notification_h noti,
						  time_t * ret_time);

/**
 * @brief Sets the text to display on the notification view.
 * @details Sets title, content string. If the text is formatted data (only %d, %f, %s are supported), type - value pair should be set.
 * If %d, the type #NOTIFICATION_VARIABLE_TYPE_INT and the value is an integer.
 * If %f, the type #NOTIFICATION_VARIABLE_TYPE_DOUBLE and the value is a double.
 * If %s, the type #NOTIFICATION_VARIABLE_TYPE_STRING and the value is a string.
 * If the type is #NOTIFICATION_VARIABLE_TYPE_COUNT, notification count is displaying with text.
 * If the value is #NOTIFICATION_COUNT_POS_LEFT, count is displayed at the left of the text.
 * If the value is #NOTIFICATION_COUNT_POS_IN, count is displayed in the text when text has %d format.
 * If the value is #NOTIFICATION_COUNT_POS_RIGHT, count is displayed at the right of the text.
 * Variable parameters should be terminated #NOTIFICATION_VARIABLE_TYPE_NONE.
 * @since_tizen 2.3
 * @param[in] noti      The notification handle
 * @param[in] type      The notification text type
 * @param[in] text      The basic text
 * @param[in] key       The text key for localization
 * @param[in] args_type The variable parameter that type - value pair
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre notification handle should be created by notification_create().
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_text(notification_h noti,
					   notification_text_type_e type,
					   const char *text,
					   const char *key,
					   int args_type, ...);

/**
 * @brief Gets the text from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti The notification handle
 * @param[in]  type The notification text type
 * @param[out] text The notification text
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *text = NULL;

	noti_err  = notification_get_text(noti, NOTIFICATION_TEXT_TYPE_TITLE, &text);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_text(notification_h noti,
					   notification_text_type_e type,
					   char **text);

/**
 * @brief Sets the timestamp to display on the notification view.
 * @details The timestamp will be converted to a formatted string and it will be displayed on the set text area.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] type The notification text type
 * @param[in] time The timestamp
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 */
int notification_set_time_to_text(notification_h noti, notification_text_type_e type,
								time_t time);

/**
 * @brief Gets the timestamp from the notification handle.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] type The notification text type
 * @param[in] time The pointer of time stamp
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 */
int notification_get_time_from_text(notification_h noti, notification_text_type_e type,
								time_t *time);

/**
 * @internal
 * @brief Sets the text domain to localize the notification.
 * @since_tizen 2.3
 * @param[in] noti   The notification handle
 * @param[in] domain The text domain
 * @param[in] dir    The text dir
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_text_domain(notification_h noti,
						  const char *domain,
						  const char *dir);

/**
 * @internal
 * @brief Gets the text domain from the notification handle.
 * @since_tizen 2.3
 * @remarks Do not free returned domain and dir. They are freed when notification_free() or notification_free_list() is called.
 * @param[in]  noti   The notification handle
 * @param[out] domain The domain
 * @param[out] dir    The locale dir
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *domain = NULL;
	char *dir = NULL;

	noti_err  = notification_get_text_domain(noti, &domain, &dir);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_text_domain(notification_h noti,
						  char **domain,
						  char **dir);

/**
 * @brief Sets the sound type for the notification.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] type The notification sound type
 * @param[in] path The user sound file path
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_sound(noti, NOTIFICATION_SOUND_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_set_sound(notification_h noti,
					    notification_sound_type_e type,
					    const char *path);

/**
 * @brief Gets the sound type from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti  The notification handle
 * @param[out] type  The notification sound type
 * @param[out] path  The user sound file path
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	notification_sound_type_e type = NOTIFICATION_SOUND_TYPE_NONE;

	noti_err  = notification_get_sound(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_sound(notification_h noti,
					    notification_sound_type_e *type,
					    const char **path);

/**
 * @brief Sets the vibration type for the notification.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] type The notification vibration type
 * @param[in] path The user vibration file path
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_vibration(noti, NOTIFICATION_VIBRATION_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_set_vibration(notification_h noti,
						notification_vibration_type_e type,
						const char *path);

/**
 * @brief Gets the vibrate type from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti The notification handle
 * @param[out] type The notification sound type
 * @param[out] path The user vibration file path
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	notification_vibration_type_e type = NOTIFICATION_VIBRATION_TYPE_NONE;

	noti_err  = notification_get_vibration(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
int notification_get_vibration(notification_h noti,
						notification_vibration_type_e *type,
						const char **path);

/**
 * @brief Sets the LED displaying option.
 * @since_tizen 2.3
 * @param[in] noti      The notification handle
 * @param[in] operation The LED notification operation
 * @param[in] led_argb  The notification led color
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_led(noti, NOTIFICATION_LED_TYPE_DEFAULT, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_set_led(notification_h noti,
						notification_led_op_e operation,
						int led_argb);

/**
 * @brief Gets the LED displaying option from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti      The notification handle
 * @param[out] operation The LED notification operation
 * @param[out] led_argb  The notification LED color
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	notification_led_type_e type = NOTIFICATION_LED_TYPE_NONE;

	noti_err  = notification_get_led(noti, &type, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
int notification_get_led(notification_h noti,
						notification_led_op_e *operation,
						int *led_argb);

/**
 * @brief Sets the time period of flashing the LED.
 * @since_tizen 2.3
 * @param[in] noti   The notification handle
 * @param[in] on_ms  The time for turning on the LED
 * @param[in] off_ms The time for turning off the LED
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_set_led_time_period(noti, 100, 100);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_set_led_time_period(notification_h noti,
						int on_ms, int off_ms);

/**
 * @brief Gets the time period of flashing the LED from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti   The notification handle
 * @param[out] on_ms  The time for turning on the LED
 * @param[out] off_ms The time for turning on the LED
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	int led_on_ms = 0;
	int led_off_ms = 0;

	noti_err  = notification_get_led_time_period(noti, &led_on_ms, &led_off_ms);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
  * @endcode
  */
int notification_get_led_time_period(notification_h noti,
						int *on_ms, int *off_ms);

/**
 * @brief Sets the launch option for a notification.
 * @details When notification data selected in display application, application launched by app_control_send_launch_request with app_control handle.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] type Launching option type
 * @param[in] option App Control handler
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	app_control_h app_control = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	...

	app_control_create(&app_control);
	app_control_set_app_id(app_control, "org.tizen.app");

	...

	noti_err  = notification_set_launch_option(noti, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, (void *)app_control);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}

	app_control_destroy(app_control);
}
 * @endcode
 */
int notification_set_launch_option(notification_h noti,
								notification_launch_option_type type, void *option);

/**
 * @brief Gets the launch option from the notification handle.
 * @since_tizen 2.3
 * @remarks You must release @a app_control using app_control_destroy().
 * @param[in]  noti        The notification handle
 * @param[in] type Launching option type
 * @param[out] option The pointer of App Control handler
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	app_control_h app_control = NULL;
	app_control_create(&app_control);

	...

	noti_err = notification_get_launch_option(noti, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, (void *)&app_control);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
int notification_get_launch_option(notification_h noti,
								notification_launch_option_type type, void *option);

/**
 * @brief Sets the property of the notification.
 * @since_tizen 2.3
 * @param[in] noti  The notification handle
 * @param[in] flags The property with | operation
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_property(notification_h noti,
					       int flags);

/**
 * @brief Gets the property of the notification from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti  The notification handle
 * @param[out] flags The notification property
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	int flags = 0;

	noti_err  = notification_get_property(noti, &flags);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_property(notification_h noti,
					       int *flags);

/**
 * @brief Sets applications to display the notification.
 * @details All display application is enable(NOTIFICATION_DISPLAY_APP_ALL) if you are not call this API.
 * @since_tizen 2.3
 * @param[in] noti    The notification handle
 * @param[in] applist The with | operation
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_display_applist(notification_h noti,
						      int applist);

/**
 * @brief Gets the application list displaying the notification from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti    The notification handle
 * @param[out] applist The display application list
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	int applist = 0;

	noti_err  = notification_get_display_applist(noti, &applist);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_display_applist(notification_h noti,
						      int *applist);

/**
 * @brief Sets the initial size for the ongoing type.
 * @details After notification_post() call, the size is not updated.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] size The double type size
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_size(notification_h noti,
					   double size);

/**
 * @brief Gets the progress size.
 * @since_tizen 2.3
 * @param[in]  noti The notification handle
 * @param[out] size The progress size
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	double size = 0.0;

	noti_err  = notification_get_size(noti, &size);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_size(notification_h noti,
					   double *size);

/**
 * @brief Sets the initial progress for the ongoing type.
 * @details After the notification_post() call, the progress is not updated.
 * @since_tizen 2.3
 * @param[in] noti       The notification handle
 * @param[in] percentage The progress percentage
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_set_progress(notification_h noti,
					       double percentage);

/**
 * @brief Gets the progress from the notification handle.
 * @since_tizen 2.3
 * @remarks At the end of the operation, the progress should be @c 1.0.
 * @param[in]  noti       The notification handle
 * @param[out] percentage The progress percentage
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	double percentage = 0.0;

	noti_err  = notification_get_progress(noti, &percentage);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_progress(notification_h noti,
					       double *percentage);

/**
 * @brief Sets the layout of the notification view.
 * @details Caller can set displaying layout of notification.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @param[in] layout The type of layout
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_ly_type_e
 */
int notification_set_layout(notification_h noti,
		notification_ly_type_e layout);

/**
 * @brief Gets the layout of the notification view from the notification handle.
 * @since_tizen 2.3
 * @param[in]  noti The notification handle
 * @param[out] layout The type of layout
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_ly_type_e
 */
int notification_get_layout(notification_h noti,
		notification_ly_type_e *layout);

/**
 * @brief Gets the type of a notification.
 * @since_tizen 2.3
 * @param[in]  noti The notification handle
 * @param[out] type The notification type
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;
	notification_type_e type;

	noti_err  = notification_get_type(noti, &type);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_type(notification_h noti,
					   notification_type_e * type);

/**
 * @internal
 * @brief Inserts a notification.
 * @details The notification will be inserted to the database and then it will appear in the notification area.
 *          When notification_create() is called, if priv_id is #NOTIFICATION_PRIV_ID_NONE, priv_id returns the internally set priv_id.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in]  noti    The notification handle
 * @param[out] priv_id The private ID
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @pre Notification handle should be created by notification_create().
 * @post notification_free().
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_insert(noti, NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_insert(notification_h noti,
					 int *priv_id);

/**
 * @brief Updates notification data.
 * @details The updated notification will appear in the notification area.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] noti The notification handle that is created by notification_create()
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @retval #NOTIFICATION_ERROR_NOT_EXIST_ID Priv ID does not exist
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update(NULL);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_update(notification_h noti);

/**
 * @internal
 * @brief Updates a notification, asynchronously.
 * @details The updated notification will appear in the notification area.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @remarks This function updates the notification asynchronously.
 * @param[in] noti      The notification handle that is created by notification_create()
 * @param[in] result_cb The callback called when an update completed
 * @param[in] user_data The user data which you want to use in callback
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @retval #NOTIFICATION_ERROR_NOT_EXIST_ID Priv ID does not exist
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_async(NULL, result_cb, data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_update_async(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data);

/**
 * @brief Deletes a notification with the given handle.
 * @details notification_delete() removes notification data from database and notification_free() releases memory of notification data.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] noti The notification handle
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @retval NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
 	notificaton_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	...

	noti_err  = notification_delete(noti);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}

}
 * @endcode
 */
int notification_delete(notification_h noti);

/**
 * @brief Creates internal structure data and returns a notification handle.
 * @details Available type is #NOTIFICATION_TYPE_NOTI and #NOTIFICATION_TYPE_ONGOING.
 * #NOTIFICATION_TYPE_NOTI is remaining notification data even if device is restarted.
 * #NOTIFICATION_TYPE_ONGOING can display progressive feather, but notification data is removed after device is restarted.
 * @since_tizen 2.3
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] type The notification type
 * @return Notification handle(notification_h) on success,
 *         otherwise @c NULL on failure
 * @retval notification_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @exception #NOTIFICATION_ERROR_OUT_OF_MEMORY Out of memory
 * @see #notification_type_e
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
 * @brief Creates a notification clone.
 * @details Newly created notification handle is returned.
 * @since_tizen 2.3
 * @remarks This cloned notification handle should be freed using notification_free().
 * @param[in]  noti  The notification handle
 * @param[out] clone The newly created notification handle that has same with input @a noti
 * @return #NOTIFICATION_ERROR_NONE if success,
 *         otherwise any other value if failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @see #notification_type_e
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_h clone = NULL;

	notification_clone(noti, &clone);
	...
}
 * @endcode
 */
int notification_clone(notification_h noti, notification_h *clone);

/**
 * @brief Frees the internal structure data of a notification handle.
 * @details Internal data of a notification handle is released. Data of the inserted notification is not deleted.
 * @since_tizen 2.3
 * @param[in] noti The notification handle
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre Notification handle should be created by notification_create().
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
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
int notification_free(notification_h noti);

/**
 * @}
 */

/**
 * @internal
 * @addtogroup NOTIFICATION_LIST
 * @{
 */

/**
 * @internal
 * @brief Returns the notification list handle.
 * @details If count is equal to @c -1, all notifications are returned.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in]  type  The notification type
 * @param[in]  count The returned notification data number
 * @param[out] #NOTIFICATION_ERROR_NONE on success, other value on failure
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_get_list(NOTIFICATION_TYPE_NONE, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_list(notification_type_e type,
					   int count,
					   notification_list_h * list);

/**
 * @internal
 * @brief Returns the notification detail list handle of grouping data.
 * @details If count is equal to c -1, all notifications are returned.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in]  pkgname  The caller application package name
 * @param[in]  group_id The group ID
 * @param[in]  priv_id  The private ID
 * @param[in]  count    The returned notification data number
 * @param[out] list     The notification list handle
 * @return #NOTIFICATION_ERROR_NONE if success,
 *         other value if failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_get_detail_list(pkgname, group_id, priv_id, -1, &noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_detail_list(const char *pkgname,
						  int group_id,
						  int priv_id,
						  int count,
						  notification_list_h *list);

/**
 * @internal
 * @brief Frees a notification list.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] list The notification list handle
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @pre notification_get_grouping_list() or notification_get_detail_list().
 * @see #notification_list_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_list_h noti_list = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	...

	noti_err = notification_free_list(noti_list);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_free_list(notification_list_h list);

/**
 * @}
 */

/**
 * @addtogroup NOTIFICATION_MODULE
 * @{
 */

/**
 * @internal
 * @brief Registers a callback for all notification events.
 * @details The registered callback could be called for all notification events.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] changed_cb The callback function
 * @param[in] user_data  The user data
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see notification_unregister_detailed_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_register_detailed_changed_cb(app_changed_cb, user_data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_register_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data);

/**
 * @internal
 * @brief Unregisters a callback for all notification events.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] changed_cb The callback function
 * @return #NOTIFICATION_ERROR_NONE on success,
 *         otherwise any other value on failure
 * @retval #NOTIFICATION_ERROR_NONE         Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see notification_register_detailed_changed_cb()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	noti_err = notification_register_detailed_changed_cb(app_changed_cb, user_data);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_unregister_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data);

/**
 * @brief Sets the tag of the notification handle.
 * @since_tizen 2.3
 * @param[in] noti Notification handle
 * @param[in] tag tag for loading notification handle
 * @return #NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see notification_get_tag()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_create(NOTIFICATION_TYPE_NOTI);
	if(noti == NULL) {
		return;
	}
	...

	noti_err = notification_set_tag(noti, tag);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_set_tag(notification_h noti, const char *tag);

/**
 * @brief Gets the tag of the notification handle.
 * @since_tizen 2.3
 * @param[in] noti Notification handle
 * @param[out] tag tag for loading notification handle
 * @return #NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see notification_set_tag()
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	const char *tag = NULL;

	...

	noti_err = notification_get_tag(noti, &tag);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_get_tag(notification_h noti, const char **tag);

/**
 * @brief Loads a notification from the notification's database with the tag.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @remarks The specific error code can be obtained using the get_last_result() method. Error codes are described in Exception section.
 * @param[in] tag tag for loading notification handle
 * @return Notification handle(notification_h) on success, NULL on failure
 * @retval notification_h Success
 * @retval NULL Failure
 * @exception #NOTIFICATION_ERROR_NONE Success
 * @exception #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @exception #NOTIFICATION_ERROR_OUT_OF_MEMORY Out of memory
 * @exception #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @see #notification_type_e
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_load_by_tag(tag);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
notification_h notification_load_by_tag(const char *tag);

/**
 * @brief Deletes all notifications of the given type.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] type Notification type
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_delete_all(NOTIFICATION_TYPE_NOTI);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_delete_all(notification_type_e type);

/**
 * @brief Posts a notification.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/notification
 * @param[in] noti Notification handle
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval #NOTIFICATION_ERROR_NONE Success
 * @retval #NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #NOTIFICATION_ERROR_PERMISSION_DENIED The application does not have the privilege to call this method
 * @pre Notification handle should be created by notification_create().
 * @post notification_free().
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_post(noti);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_post(notification_h noti);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_H__ */
