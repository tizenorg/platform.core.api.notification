/*
 *  libnotification
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seungtaek Chung <seungtaek.chung@samsung.com>, Mi-Ju Lee <miju52.lee@samsung.com>, Xi Zhichan <zhichan.xi@samsung.com>, Youngsub Ko <ys4610.ko@samsung.com>
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

#ifndef __NOTIFICATION_TYPE_H__
#define __NOTIFICATION_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup NOTIFICATION_LIBRARY
 * @defgroup NOTIFICATION_TYPE notification type
 * @brief Notification type
 */

/**
 * @addtogroup NOTIFICATION_TYPE
 * @{
 */

/**
 * @breief Enumeration for notification sound type.
 */
typedef enum _notification_sound_type {
	NOTIFICATION_SOUND_TYPE_NONE = -1,
					/**< Default value. Disable sound */
	NOTIFICATION_SOUND_TYPE_DEFAULT = 0,
					/**< New chat sound */
	NOTIFICATION_SOUND_TYPE_USER_DATA,
					/**< User sound data */
	NOTIFICATION_SOUND_TYPE_MAX,
				/**< Max flag */
} notification_sound_type_e;

/**
 * @breief Enumeration for notification vibration type.
 */
typedef enum _notification_vibration_type {
	NOTIFICATION_VIBRATION_TYPE_NONE = -1,
					/**< Default value. Disable vibration */
	NOTIFICATION_VIBRATION_TYPE_DEFAULT = 0,/**< New chat vibration */
	NOTIFICATION_VIBRATION_TYPE_USER_DATA,
					/**< User vibration data */
	NOTIFICATION_VIBRATION_TYPE_MAX,/**< Max flag */
} notification_vibration_type_e;

/**
 * @breief Will be deprecated.
 */
typedef enum _notification_count_display_type {
	NOTIFICATION_COUNT_DISPLAY_TYPE_NONE = -1,
	NOTIFICATION_COUNT_DISPLAY_TYPE_LEFT = 0,
	NOTIFICATION_COUNT_DISPLAY_TYPE_IN,
	NOTIFICATION_COUNT_DISPLAY_TYPE_RIGHT,
	NOTIFICATION_COUNT_DISPLAY_TYPE_MAX,
} notification_count_display_type_e;

/**
 * @breief Enumeration for notification count position in the text.
 */
typedef enum _notifcation_count_pos_type {
	NOTIFICATION_COUNT_POS_NONE = -1,
					/**< Count data is not displaying in the text */
	NOTIFICATION_COUNT_POS_LEFT = 0,/**< Count data is displaying at the left of the text */
	NOTIFICATION_COUNT_POS_IN,
				/**< Count data is displaying in the text */
	NOTIFICATION_COUNT_POS_RIGHT,
				/**< Count data is displaying at the right of the text */
	NOTIFICATION_COUNT_POS_MAX,
				/**< Max flag */
} notification_count_pos_type_e;

/**
 * @breief Enumeration for notification variable parameter type
 */
typedef enum _notification_variable_type {
	NOTIFICATION_VARIABLE_TYPE_NONE = -1,
					/**< Variable parameter type is NONE */
	NOTIFICATION_VARIABLE_TYPE_INT = 0,
					/**< Variable parameter type is int */
	NOTIFICATION_VARIABLE_TYPE_DOUBLE,
					/**< Variable parameter type is double */
	NOTIFICATION_VARIABLE_TYPE_STRING,
					/**< Variable parameter type is string */
	NOTIFICATION_VARIABLE_TYPE_COUNT,
					/**< Variable parameter type is count */
	NOTIFICATION_VARIABLE_TYPE_MAX,
				/**< Max flag */
} notification_variable_type_e;

/**
 * @breief Enumeration for notification text type.
 */
typedef enum _notification_text_type {
	NOTIFICATION_TEXT_TYPE_NONE = -1,
					/**< NONE */
	NOTIFICATION_TEXT_TYPE_TITLE = 0,
					/**< Title */
	NOTIFICATION_TEXT_TYPE_CONTENT,
				/**< Content */
	NOTIFICATION_TEXT_TYPE_CONTENT_FOR_DISPLAY_OPTION_IS_OFF,
								/**< Content for content display option is off of the Settings */
	NOTIFICATION_TEXT_TYPE_GROUP_TITLE,
					/**< Group title */
	NOTIFICATION_TEXT_TYPE_GROUP_CONTENT,
					/**< Group content */
	NOTIFICATION_TEXT_TYPE_GROUP_CONTENT_FOR_DISPLAY_OPTION_IS_OFF,
								/**< Group content for content display option is off of the Settings */
	NOTIFICATION_TEXT_TYPE_MAX,
				/**< Max flag */
} notification_text_type_e;

/**
 * @breief Enumeration for image text type.
 */
typedef enum _notification_image_type {
	NOTIFICATION_IMAGE_TYPE_NONE = -1,
					/**< NONE */
	NOTIFICATION_IMAGE_TYPE_ICON = 0,
					/**< Icon */
	NOTIFICATION_IMAGE_TYPE_ICON_FOR_INDICATOR,
						/**< Indicator icon */
	NOTIFICATION_IMAGE_TYPE_ICON_FOR_LOCK,
					/**< Lock screen icon */
	NOTIFICATION_IMAGE_TYPE_THUMBNAIL,
					/**< Thumbnail */
	NOTIFICATION_IMAGE_TYPE_THUMBNAIL_FOR_LOCK,
						/**< Lock screen thumbnail */
	NOTIFICATION_IMAGE_TYPE_MAX,
				/**< Max flag */
} notification_image_type_e;

/*typedef enum _notification_button_type {
	NOTIFICATION_BUTTON_TYPE_NONE = -1,
	NOTIFICATION_BUTTON_TYPE_RUN = 0,
	NOTIFICATION_BUTTON_TYPE_VIEW,
	NOTIFICATION_BUTTON_TYPE_DISMISS,
	NOTIFICATION_BUTTON_TYPE_MAX,
}notification_button_type_e;*/

/**
 * @breief Enumeration for application execution type.
 */
typedef enum _notification_execute_type {
	NOTIFICATION_EXECUTE_TYPE_NONE = -1,
					/**< No operation */
	NOTIFICATION_EXECUTE_TYPE_RESPONDING = 0,
						/**< Responding */
	NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH,/**< Launching when notification data is single */
	NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH,
					/**< Launching when notification data is grouping(multi) */
	NOTIFICATION_EXECUTE_TYPE_MAX,
				/**< Max flag */
} notification_execute_type_e;

/**
 * @breief Enumeration for notification type.
 */
typedef enum _notification_type {
	NOTIFICATION_TYPE_NONE = -1,
				/**< None */
	NOTIFICATION_TYPE_NOTI = 0,
				/**< Notification type */
	NOTIFICATION_TYPE_ONGOING,
				/**< Ongoing type */
	NOTIFICATION_TYPE_MAX,
			/**< Max flag */
} notification_type_e;

/**
 * @breief Enumeration for Group ID.
 */
enum _notification_group_id {
	NOTIFICATION_GROUP_ID_NONE = -1,/**< Not Grouping */
	NOTIFICATION_GROUP_ID_DEFAULT = 0,
					/**< Notification that has same title is grouping */
};

/**
 * @breief Enumeration for Private ID.
 */
enum _notification_priv_id {
	NOTIFICATION_PRIV_ID_NONE = -1,
				/**< Internally set priv_id */
};

/**
 * @breief Enumeration for notification property
 */
enum _notification_property {
	NOTIFICATION_PROP_DISPLAY_ONLY_SIMMODE = 0x00000001,
							/**< Display only SIM card inserted */
	NOTIFICATION_PROP_DISABLE_APP_LAUNCH = 0x00000002,
							/**< Disable application launch when it selected */
	NOTIFICATION_PROP_DISABLE_AUTO_DELETE = 0x00000004,
							/**< Disable auto delete when it selected */
	NOTIFICATION_PROP_LAUNCH_UG = 0x00000008,
						/**< Will be deprecated. Notification Tray should launch application using appsvc API */
	NOTIFICATION_PROP_DISABLE_TICKERNOTI = 0x00000010,
							/**< Will be deprecated. Use notification_set_display_applist API */
	NOTIFICATION_PROP_PERMANENT_DISPLAY = 0x00000020,
							/** < Will be deprecated. */
	NOTIFICATION_PROP_DISABLE_UPDATE_ON_INSERT = 0x00000040,/**< Disable update when it inserted. */
	NOTIFICATION_PROP_DISABLE_UPDATE_ON_DELETE = 0x00000080,/**< Disable update when it deleted. */
	NOTIFICATION_PROP_VOLATILE_DISPLAY = 0x00000100,/**< Deleted when device is rebooted eventhough NOTIFICATION_TYPE_NOTI type */
};

/**
 * @breief Enumeration for display application list
 */
enum _notificaton_display_applist {
	NOTIFICATION_DISPLAY_APP_NOTIFICATION_TRAY = 0x00000001,/**< Notification Tray(Quickpanel) */
	NOTIFICATION_DISPLAY_APP_TICKER = 0x00000002,
						/**< Ticker notification */
	NOTIFICATION_DISPLAY_APP_LOCK = 0x00000004,
						/**< Lock screen */
	NOTIFICATION_DISPLAY_APP_INDICATOR = 0x00000008,/**< Indicator */
	NOTIFICATION_DISPLAY_APP_ALL = 0xffffffff,
						/**< All display application */
};

/**
 * @brief Notification handle
 */
typedef struct _notification *notification_h;

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_TYPE_H__ */
