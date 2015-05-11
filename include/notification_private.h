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

#ifndef __NOTIFICATION_PRIVATE_H__
#define __NOTIFICATION_PRIVATE_H__

#ifndef EXPORT_API
#define EXPORT_API __attribute__ ((visibility("default")))
#endif

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
};

void notification_call_changed_cb(notification_op *op_list, int op_num);

#endif				/* __NOTIFICATION_PRIVATE_H__ */
