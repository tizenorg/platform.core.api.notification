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

#ifndef __NOTIFICATION_TYPE_H__
#define __NOTIFICATION_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file notification_type.h
 * @brief This file contains defines and enumerations for Notification APIs
 */

/**
 * @addtogroup NOTIFICATION_TYPE
 * @{
 */

/**
 * @brief Enumeration for notification layout type
 */
typedef enum _notification_ly_type {
	NOTIFICATION_LY_NONE = 0,
	NOTIFICATION_LY_NOTI_EVENT_SINGLE,
		/**< layout for notification. used to inform single event*/
	NOTIFICATION_LY_NOTI_EVENT_MULTIPLE,
		/**< layout for notification. used to inform multiple event*/
	NOTIFICATION_LY_NOTI_THUMBNAIL,
		/**< layout for notification. used to display images*/
	NOTIFICATION_LY_ONGOING_EVENT,
		/**< layout for ongoing notification. used to display text message*/
	NOTIFICATION_LY_ONGOING_PROGRESS,
		/**< layout for ongoing notification. used to display progress*/
	NOTIFICATION_LY_MAX,
} notification_ly_type_e;

/**
 * @brief Enumeration for notification sound type.
 */
typedef enum _notification_sound_type {
	NOTIFICATION_SOUND_TYPE_NONE = -1,
					/**< Default value. no sound */
	NOTIFICATION_SOUND_TYPE_DEFAULT = 0,
					/**< default sound */
	NOTIFICATION_SOUND_TYPE_USER_DATA,
					/**< User sound data */
	NOTIFICATION_SOUND_TYPE_MAX,
				/**< Max flag */
} notification_sound_type_e;

/**
 * @brief Enumeration for notification vibration type.
 */
typedef enum _notification_vibration_type {
	NOTIFICATION_VIBRATION_TYPE_NONE = -1,
					/**< Default value. no vibration */
	NOTIFICATION_VIBRATION_TYPE_DEFAULT = 0,/**< default vibrate pattern */
	NOTIFICATION_VIBRATION_TYPE_USER_DATA,
					/**< User vibration data */
	NOTIFICATION_VIBRATION_TYPE_MAX,/**< Max flag */
} notification_vibration_type_e;

/**
 * @brief Enumeration for notification led operation.
 */
typedef enum _notification_led_op {
	NOTIFICATION_LED_OP_OFF = -1,
					/**< Default value. Disable the LED notification */
	NOTIFICATION_LED_OP_ON = 0,/**< turn on the LED with default color */
	NOTIFICATION_LED_OP_ON_CUSTOM_COLOR,
					/**< turn on the LED with custom color */
	NOTIFICATION_LED_OP_MAX,/**< Max flag */
} notification_led_op_e;

/**
 * @brief Will be deprecated.
 */
typedef enum _notification_count_display_type {
	NOTIFICATION_COUNT_DISPLAY_TYPE_NONE = -1,
	NOTIFICATION_COUNT_DISPLAY_TYPE_LEFT = 0,
	NOTIFICATION_COUNT_DISPLAY_TYPE_IN,
	NOTIFICATION_COUNT_DISPLAY_TYPE_RIGHT,
	NOTIFICATION_COUNT_DISPLAY_TYPE_MAX,
} notification_count_display_type_e;

/**
 * @brief Enumeration for notification text type.
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
	NOTIFICATION_TEXT_TYPE_EVENT_COUNT,
								/**< text to display event count */
	NOTIFICATION_TEXT_TYPE_INFO_1,
								/**< box contents 1 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_1,
								/**< box contents 1-1 */
	NOTIFICATION_TEXT_TYPE_INFO_2,
								/**< box contents 2 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_2,
								/**< box contents 2-1 */
	NOTIFICATION_TEXT_TYPE_INFO_3,
								/**< box contents 3 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_3,
								/**< box contents 3-1 */
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
 * @brief Enumeration for image text type.
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
	NOTIFICATION_IMAGE_TYPE_ICON_SUB,
					/**< Icon */
	NOTIFICATION_IMAGE_TYPE_BACKGROUND,
						/**< image displayed on background */
	NOTIFICATION_IMAGE_TYPE_LIST_1,
						/**< image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_2,
						/**< image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_3,
						/**< image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_4,
						/**< image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_5,
						/**< image for thumbnail list */
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
 * @brief Enumeration for application execution type.
 */
typedef enum _notification_execute_type {
	NOTIFICATION_EXECUTE_TYPE_NONE = -1,
					/**< No operation */
	NOTIFICATION_EXECUTE_TYPE_RESPONDING = 0,
						/**< Responding action*/
	NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH,/**< Launching when notification data is single */
	NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH,
					/**< Launching when notification data is grouping(multi) */
	NOTIFICATION_EXECUTE_TYPE_MAX,
				/**< Max flag */
} notification_execute_type_e;

/**
 * @brief Enumeration for notification type.
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
 * @brief Enumeration for Group ID.
 */
enum _notification_group_id {
	NOTIFICATION_GROUP_ID_NONE = -1,/**< Not Grouping */
	NOTIFICATION_GROUP_ID_DEFAULT = 0,
					/**< Notification that has same title is grouping */
};

/**
 * @brief Enumeration for Private ID.
 */
enum _notification_priv_id {
	NOTIFICATION_PRIV_ID_NONE = -1,
				/**< Internally set priv_id */
};

/**
 * @brief Enumeration for notification property
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
 * @brief Enumeration for display application list
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
 * @}
 */

/**
 * @brief Enumeration for notification operation code
 */
typedef enum _notification_op_type {
	NOTIFICATION_OP_NONE = 0,
	NOTIFICATION_OP_INSERT = 1,
	NOTIFICATION_OP_UPDATE,
	NOTIFICATION_OP_DELETE,
	NOTIFICATION_OP_DELETE_ALL,
	NOTIFICATION_OP_REFRESH,
	NOTIFICATION_OP_SERVICE_READY,
} notification_op_type_e;

/**
 * @brief Enumeration for notification operation data code
 */
typedef enum _notification_op_data_type {
	NOTIFICATION_OP_DATA_MIN = 0,
	NOTIFICATION_OP_DATA_TYPE,
	NOTIFICATION_OP_DATA_PRIV_ID,
	NOTIFICATION_OP_DATA_NOTI,
	NOTIFICATION_OP_DATA_EXTRA_INFO_1,
	NOTIFICATION_OP_DATA_EXTRA_INFO_2,
	NOTIFICATION_OP_DATA_MAX,
} notification_op_data_type_e;

/**
 * @brief Enumeration for notification count position in the text.
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
 * @brief Enumeration for notification variable parameter type
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
 * @brief Notification handle
 */
typedef struct _notification *notification_h;

/**
 * @brief Notification operation handle
 */
typedef struct _notification_op {
	notification_op_type_e type;
	int priv_id;
	int extra_info_1;
	int extra_info_2;
	notification_h noti;
} notification_op;

#ifdef __cplusplus
}
#endif
#endif				/* __NOTIFICATION_TYPE_H__ */
