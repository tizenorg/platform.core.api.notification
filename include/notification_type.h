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
 * @file notification_type.h
 * @brief This file contains type definitions and enumerations for Notification API.
 */

/**
 * @addtogroup NOTIFICATION_MODULE
 * @{
 */

/**
 * @brief Enumeration for notification layout type.
 * @since_tizen 2.3
 */
typedef enum _notification_ly_type {
	NOTIFICATION_LY_NONE = 0,            /**< Default */
	NOTIFICATION_LY_NOTI_EVENT_SINGLE,   /**< Layout for notification. Used to inform single event*/
	NOTIFICATION_LY_NOTI_EVENT_MULTIPLE, /**< Layout for notification. Used to inform multiple event*/
	NOTIFICATION_LY_NOTI_THUMBNAIL,      /**< Layout for notification. Used to display images*/
	NOTIFICATION_LY_ONGOING_EVENT,       /**< Layout for ongoing notification. Used to display text message*/
	NOTIFICATION_LY_ONGOING_PROGRESS,    /**< Layout for ongoing notification. Used to display progress*/
	NOTIFICATION_LY_MAX,                 /**< TBD */
} notification_ly_type_e;

/**
 * @brief Enumeration for notification launch option type.
 * @since_tizen 2.3
 */
typedef enum  _notification_launch_option_type {
	NOTIFICATION_LAUNCH_OPTION_APP_CONTROL = 1, /**< launching with app control */
} notification_launch_option_type;

/**
 * @brief Enumeration for notification sound type.
 * @since_tizen 2.3
 */
typedef enum _notification_sound_type {
	NOTIFICATION_SOUND_TYPE_NONE = -1,   /**< Default value. no sound */
	NOTIFICATION_SOUND_TYPE_DEFAULT = 0, /**< Default sound */
	NOTIFICATION_SOUND_TYPE_USER_DATA,   /**< User sound data */
	NOTIFICATION_SOUND_TYPE_MAX,         /**< Max flag */
} notification_sound_type_e;

/**
 * @brief Enumeration for notification vibration type.
 * @since_tizen 2.3
 */
typedef enum _notification_vibration_type {
	NOTIFICATION_VIBRATION_TYPE_NONE = -1,   /**< Default value. No vibration */
	NOTIFICATION_VIBRATION_TYPE_DEFAULT = 0, /**< Default vibrate pattern */
	NOTIFICATION_VIBRATION_TYPE_USER_DATA,   /**< User vibration data */
	NOTIFICATION_VIBRATION_TYPE_MAX,         /**< Max flag */
} notification_vibration_type_e;

/**
 * @brief Enumeration for notification LED operation.
 * @since_tizen 2.3
 */
typedef enum _notification_led_op {
	NOTIFICATION_LED_OP_OFF = -1,        /**< Default value. Disable the LED notification */
	NOTIFICATION_LED_OP_ON = 0,          /**< Turn on the LED with default color */
	NOTIFICATION_LED_OP_ON_CUSTOM_COLOR, /**< Turn on the LED with custom color */
	NOTIFICATION_LED_OP_MAX,             /**< Max flag */
} notification_led_op_e;

/**
 * @brief This will be deprecated.
 * @since_tizen 2.3
 */
typedef enum _notification_count_display_type {
	NOTIFICATION_COUNT_DISPLAY_TYPE_NONE = -1, /**< None */
	NOTIFICATION_COUNT_DISPLAY_TYPE_LEFT = 0,  /**< The number is placed to left */
	NOTIFICATION_COUNT_DISPLAY_TYPE_IN,        /**< The number is placed to center */
	NOTIFICATION_COUNT_DISPLAY_TYPE_RIGHT,     /**< The number is placed to right */
	NOTIFICATION_COUNT_DISPLAY_TYPE_MAX,       /**< Max flag */
} notification_count_display_type_e;

/**
 * @brief Enumeration for notification text type.
 * @since_tizen 2.3
 */
typedef enum _notification_text_type {
	NOTIFICATION_TEXT_TYPE_NONE = -1,                               /**< NONE */
	NOTIFICATION_TEXT_TYPE_TITLE = 0,                               /**< Title */
	NOTIFICATION_TEXT_TYPE_CONTENT,                                 /**< Content */
	NOTIFICATION_TEXT_TYPE_CONTENT_FOR_DISPLAY_OPTION_IS_OFF,       /**< Content for content display option is off of the Settings */
	NOTIFICATION_TEXT_TYPE_EVENT_COUNT,                             /**< Text to display event count */
	NOTIFICATION_TEXT_TYPE_INFO_1,                                  /**< Box contents 1 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_1,                              /**< Box contents 1-1 */
	NOTIFICATION_TEXT_TYPE_INFO_2,                                  /**< Box contents 2 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_2,                              /**< Box contents 2-1 */
	NOTIFICATION_TEXT_TYPE_INFO_3,                                  /**< Box contents 3 */
	NOTIFICATION_TEXT_TYPE_INFO_SUB_3,                              /**< Box contents 3-1 */
	NOTIFICATION_TEXT_TYPE_GROUP_TITLE,                             /**< Group title */
	NOTIFICATION_TEXT_TYPE_GROUP_CONTENT,                           /**< Group content */
	NOTIFICATION_TEXT_TYPE_GROUP_CONTENT_FOR_DISPLAY_OPTION_IS_OFF, /**< Group content for content display option is off of the Settings */
	NOTIFICATION_TEXT_TYPE_MAX,                                     /**< Max flag */
} notification_text_type_e;

/**
 * @brief Enumeration for image type.
 * @since_tizen 2.3
 */
typedef enum _notification_image_type {
	NOTIFICATION_IMAGE_TYPE_NONE = -1,          /**< NONE */
	NOTIFICATION_IMAGE_TYPE_ICON = 0,           /**< Icon */
	NOTIFICATION_IMAGE_TYPE_ICON_FOR_INDICATOR, /**< Indicator icon */
	NOTIFICATION_IMAGE_TYPE_ICON_FOR_LOCK,      /**< Lock screen icon */
	NOTIFICATION_IMAGE_TYPE_THUMBNAIL,          /**< Thumbnail */
	NOTIFICATION_IMAGE_TYPE_THUMBNAIL_FOR_LOCK, /**< Lock screen thumbnail */
	NOTIFICATION_IMAGE_TYPE_ICON_SUB,           /**< Icon */
	NOTIFICATION_IMAGE_TYPE_BACKGROUND,         /**< image displayed on background */
	NOTIFICATION_IMAGE_TYPE_LIST_1,             /**< Image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_2,             /**< Image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_3,             /**< Image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_4,             /**< Image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_LIST_5,             /**< Image for thumbnail list */
	NOTIFICATION_IMAGE_TYPE_MAX,                /**< Max flag */
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
 * @since_tizen 2.3
 */
typedef enum _notification_execute_type {
	NOTIFICATION_EXECUTE_TYPE_NONE = -1,      /**< No operation */
	NOTIFICATION_EXECUTE_TYPE_RESPONDING = 0, /**< Responding action*/
	NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH,  /**< Launching when notification data is single */
	NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH,   /**< Launching when notification data is grouping(multi) */
	NOTIFICATION_EXECUTE_TYPE_MAX,            /**< Max flag */
} notification_execute_type_e;

/**
 * @brief Enumeration for notification type.
 * @since_tizen 2.3
 */
typedef enum _notification_type {
	NOTIFICATION_TYPE_NONE = -1, /**< None */
	NOTIFICATION_TYPE_NOTI = 0,  /**< Notification type */
	NOTIFICATION_TYPE_ONGOING,   /**< Ongoing type */
	NOTIFICATION_TYPE_MAX,       /**< Max flag */
} notification_type_e;

/**
 * @brief Enumeration for Group ID.
 * @since_tizen 2.3
 */
enum _notification_group_id {
	NOTIFICATION_GROUP_ID_NONE = -1,   /**< Not Grouping */
	NOTIFICATION_GROUP_ID_DEFAULT = 0, /**< Notification that has same title is grouping */
};

/**
 * @brief Enumeration for Private ID.
 * @since_tizen 2.3
 */
enum _notification_priv_id {
	NOTIFICATION_PRIV_ID_NONE = -1, /**< Internally set priv_id */
};

/**
 * @brief Enumeration for notification property.
 * @since_tizen 2.3
 */
enum _notification_property {
	NOTIFICATION_PROP_DISPLAY_ONLY_SIMMODE = 0x00000001,     /**< Display only SIM card inserted */
	NOTIFICATION_PROP_DISABLE_APP_LAUNCH = 0x00000002,       /**< Disable application launch when it selected */
	NOTIFICATION_PROP_DISABLE_AUTO_DELETE = 0x00000004,      /**< Disable auto delete when it selected */
	NOTIFICATION_PROP_LAUNCH_UG = 0x00000008,                /**< Will be deprecated. Notification Tray should launch application using appsvc API */
	NOTIFICATION_PROP_DISABLE_TICKERNOTI = 0x00000010,       /**< Will be deprecated. Use notification_set_display_applist API */
	NOTIFICATION_PROP_PERMANENT_DISPLAY = 0x00000020,        /**< Will be deprecated. */
	NOTIFICATION_PROP_DISABLE_UPDATE_ON_INSERT = 0x00000040, /**< Disable update when it inserted. */
	NOTIFICATION_PROP_DISABLE_UPDATE_ON_DELETE = 0x00000080, /**< Disable update when it deleted. */
	NOTIFICATION_PROP_VOLATILE_DISPLAY = 0x00000100,         /**< Deleted when device is rebooted eventhough NOTIFICATION_TYPE_NOTI type */
};

/**
 * @brief Enumeration for display application list.
 * @since_tizen 2.3
 */
enum _notificaton_display_applist {
	NOTIFICATION_DISPLAY_APP_NOTIFICATION_TRAY = 0x00000001,/**< Notification Tray(Quickpanel) */
	NOTIFICATION_DISPLAY_APP_TICKER = 0x00000002,/**< Ticker notification */
	NOTIFICATION_DISPLAY_APP_LOCK = 0x00000004,/**< Lock screen */
	NOTIFICATION_DISPLAY_APP_INDICATOR = 0x00000008,/**< Indicator */
	NOTIFICATION_DISPLAY_APP_ALL = 0xffffffff,/**< All display application */
};

/**
 * @brief Enumeration for notification operation code.
 * @since_tizen 2.3
 */
typedef enum _notification_op_type {
	NOTIFICATION_OP_NONE = 0,      /**< Default */
	NOTIFICATION_OP_INSERT = 1,    /**< Notification inserted */
	NOTIFICATION_OP_UPDATE,        /**< Notification updated */
	NOTIFICATION_OP_DELETE,        /**< Notification deleted */
	NOTIFICATION_OP_DELETE_ALL,    /**< Notifications deleted */
	NOTIFICATION_OP_REFRESH,       /**< Deprecated */
	NOTIFICATION_OP_SERVICE_READY, /**< Notification service is ready  */
} notification_op_type_e;

/**
 * @brief Enumeration for notification operation data code
 * @since_tizen 2.3
 */
typedef enum _notification_op_data_type {
	NOTIFICATION_OP_DATA_MIN = 0,      /**< Default */
	NOTIFICATION_OP_DATA_TYPE,         /**< Operation type */
	NOTIFICATION_OP_DATA_PRIV_ID,      /**< Private ID */
	NOTIFICATION_OP_DATA_NOTI,         /**< Notification handler */
	NOTIFICATION_OP_DATA_EXTRA_INFO_1, /**< Reserved */
	NOTIFICATION_OP_DATA_EXTRA_INFO_2, /**< Reserved */
	NOTIFICATION_OP_DATA_MAX,          /**< Max flag */
} notification_op_data_type_e;

/**
 * @brief Enumeration for notification count position in the text.
 * @since_tizen 2.3
 */
typedef enum _notifcation_count_pos_type {
	NOTIFICATION_COUNT_POS_NONE = -1, /**< Count data is not displaying in the text */
	NOTIFICATION_COUNT_POS_LEFT = 0,  /**< Count data is displaying at the left of the text */
	NOTIFICATION_COUNT_POS_IN,        /**< Count data is displaying in the text */
	NOTIFICATION_COUNT_POS_RIGHT,     /**< Count data is displaying at the right of the text */
	NOTIFICATION_COUNT_POS_MAX,       /**< Max flag */
} notification_count_pos_type_e;

/**
 * @brief Enumeration for notification variable parameter type.
 * @since_tizen 2.3
 */
typedef enum _notification_variable_type {
	NOTIFICATION_VARIABLE_TYPE_NONE = -1, /**< Variable parameter type is NONE */
	NOTIFICATION_VARIABLE_TYPE_INT = 0,   /**< Variable parameter type is int */
	NOTIFICATION_VARIABLE_TYPE_DOUBLE,    /**< Variable parameter type is double */
	NOTIFICATION_VARIABLE_TYPE_STRING,    /**< Variable parameter type is string */
	NOTIFICATION_VARIABLE_TYPE_COUNT,     /**< Variable parameter type is count */
	NOTIFICATION_VARIABLE_TYPE_MAX,       /**< Max flag */
} notification_variable_type_e;

/**
 * @brief Notification handle.
 * @since_tizen 2.3
 */
typedef struct _notification *notification_h;

/**
 * @brief The structure for notification operation.
 * @since_tizen 2.3
 */
typedef struct _notification_op {
	notification_op_type_e type; /**< Notification operation type */
	int priv_id;                 /**< private ID */
	int extra_info_1;            /**< Reserved */
	int extra_info_2;            /**< Reserved */
	notification_h noti;         /**< Notification handler */
} notification_op;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __NOTIFICATION_TYPE_H__ */
