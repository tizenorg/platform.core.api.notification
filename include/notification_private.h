/*
 * Copyright (c) 2000 - 2016 Samsung Electronics Co., Ltd. All rights reserved.
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
 */

#ifndef __NOTIFICATION_PRIVATE_H__
#define __NOTIFICATION_PRIVATE_H__
#include <sys/types.h>

#ifndef EXPORT_API
#define EXPORT_API __attribute__ ((visibility("default")))
#endif

#define SAFE_STRDUP(s) ((s) ? strdup(s) : NULL)
#define SAFE_FREE(s)			\
	do {				\
		if (s) {		\
			free(s);	\
			s = NULL;	\
		}			\
	} while (0)

#define NOTIFICATION_SETTING_DB_TABLE "notification_setting"
#define NOTIFICATION_SYSTEM_SETTING_DB_TABLE "notification_system_setting"

struct _notification {
	notification_type_e type;
	notification_ly_type_e layout;

	int group_id;		/* Group ID */
	int internal_group_id;	/* Internal Group ID */
	int priv_id;		/* Private ID */

	char *caller_pkgname;	/* Caller App package name */
	char *launch_pkgname;	/* Launch App package name. It will be from appsvc_set_pkgname */
	bundle *args;		/* Will be removed. */
	bundle *group_args;	/* Will be removed. */

	bundle *b_execute_option;
	bundle *b_service_responding;
	bundle *b_service_single_launch;
	bundle *b_service_multi_launch;

	bundle *b_event_handler[NOTIFICATION_EVENT_TYPE_MAX];

	char *domain;		/* Text domain for localization */
	char *dir;		/* Text dir for localization */

	bundle *b_text;		/* basic text */
	bundle *b_key;		/* key for localized text */
	bundle *b_format_args;	/* args type and value for format string */
	int num_format_args;	/* number of format string args */

	bundle *b_image_path;	/* image path */

	notification_sound_type_e sound_type;
	char *sound_path;
	notification_vibration_type_e vibration_type;
	char *vibration_path;
	notification_led_op_e led_operation;
	int led_argb;
	int led_on_ms;
	int led_off_ms;

	time_t time;		/* time set by application */
	time_t insert_time;	/* insert time */

	int flags_for_property;	/* property NOTIFICATION_PROP_XXX */
	int display_applist;	/* display app list  NOTIFICATION_DISPLAY_APP_XXX */

	double progress_size;	/* size of progress */
	double progress_percentage;	/* percentage of progress */

	char *app_icon_path;	/* Temporary stored app icon path from AIL */
	char *app_name;		/* Temporary stored app name from AIL */
	char *temp_title;
	char *temp_content;
	char *tag;
	bool ongoing_flag;
	bool auto_remove;
	uid_t uid;
};

struct notification_system_setting {
	bool do_not_disturb;
	int  visibility_class;
	bool dnd_schedule_enabled;
	int dnd_schedule_day;
	int dnd_start_hour;
	int dnd_start_min;
	int dnd_end_hour;
	int dnd_end_min;
};

typedef enum notification_data_type {
	NOTIFICATION_DATA_TYPE_NOTI_TYPE = 1,
	NOTIFICATION_DATA_TYPE_LAYOUT,
	NOTIFICATION_DATA_TYPE_GROUP_ID,
	NOTIFICATION_DATA_TYPE_INTERNAL_GROUP_ID,
	NOTIFICATION_DATA_TYPE_PRIV_ID,
	NOTIFICATION_DATA_TYPE_CALLER_PKGNAME,
	NOTIFICATION_DATA_TYPE_LAUNCH_PKGNAME,
	NOTIFICATION_DATA_TYPE_ARGS,
	NOTIFICATION_DATA_TYPE_GROUP_ARGS,
	NOTIFICATION_DATA_TYPE_EXECUTE_OPTION,
	NOTIFICATION_DATA_TYPE_SERVICE_RESPONDING,
	NOTIFICATION_DATA_TYPE_SERVICE_SINGLE_LAUNCH,
	NOTIFICATION_DATA_TYPE_SERVICE_MULTI_LAUNCH,
	NOTIFICATION_DATA_TYPE_BUTTON1_EVENT,
	NOTIFICATION_DATA_TYPE_BUTTON2_EVENT,
	NOTIFICATION_DATA_TYPE_BUTTON3_EVENT,
	NOTIFICATION_DATA_TYPE_BUTTON4_EVENT,
	NOTIFICATION_DATA_TYPE_BUTTON5_EVENT,
	NOTIFICATION_DATA_TYPE_BUTTON6_EVENT,
	NOTIFICATION_DATA_TYPE_ICON_EVENT,
	NOTIFICATION_DATA_TYPE_THUMBNAIL_EVENT,
	NOTIFICATION_DATA_TYPE_DOMAIN,
	NOTIFICATION_DATA_TYPE_DIR,
	NOTIFICATION_DATA_TYPE_TEXT,
	NOTIFICATION_DATA_TYPE_KEY,
	NOTIFICATION_DATA_TYPE_FORMAT_ARGS,
	NOTIFICATION_DATA_TYPE_NUM_FORMAT_ARGS,
	NOTIFICATION_DATA_TYPE_IMAGE_PATH,
	NOTIFICATION_DATA_TYPE_SOUND_TYPE,
	NOTIFICATION_DATA_TYPE_SOUND_PATH,
	NOTIFICATION_DATA_TYPE_VIBRATION_TYPE,
	NOTIFICATION_DATA_TYPE_VIBRATION_PATH,
	NOTIFICATION_DATA_TYPE_LED_OPERATION,
	NOTIFICATION_DATA_TYPE_LED_ARGB,
	NOTIFICATION_DATA_TYPE_LED_ON_MS,
	NOTIFICATION_DATA_TYPE_LED_OFF_MS,
	NOTIFICATION_DATA_TYPE_TIME,
	NOTIFICATION_DATA_TYPE_INSERT_TIME,
	NOTIFICATION_DATA_TYPE_FLAGS_FOR_PROPERTY,
	NOTIFICATION_DATA_TYPE_DISPLAY_APPLIST,
	NOTIFICATION_DATA_TYPE_PROGRESS_SIZE,
	NOTIFICATION_DATA_TYPE_PROGRESS_PERCENTAGE,
	NOTIFICATION_DATA_TYPE_APP_ICON_PATH,
	NOTIFICATION_DATA_TYPE_APP_NAME,
	NOTIFICATION_DATA_TYPE_TEMP_TITLE,
	NOTIFICATION_DATA_TYPE_TEMP_CONTENT,
	NOTIFICATION_DATA_TYPE_TAG,
	NOTIFICATION_DATA_TYPE_ONGOING_FLAG,
	NOTIFICATION_DATA_TYPE_AUTO_REMOVE,
	NOTIFICATION_DATA_TYPE_UID,
} notification_data_type_e;

void notification_call_changed_cb_for_uid(notification_op *op_list, int op_num, uid_t uid);

char *notification_get_pkgname_by_pid(void);

#endif /* __NOTIFICATION_PRIVATE_H__ */

