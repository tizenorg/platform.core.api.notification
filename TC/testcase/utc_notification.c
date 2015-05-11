/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libintl.h>
#include <tet_api.h>
#include <notification.h>

#define TEST_PKG "org.tizen.tetware"

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

static void startup(void);
static void cleanup(void);

static void utc_notification_create_1_n(void);
static void utc_notification_create_2_n(void);
static void utc_notification_create_1_p(void);
static void utc_notification_create_2_p(void);
static void utc_notification_set_image_n(void);
static void utc_notification_set_image_p(void);
static void utc_notification_get_image_n(void);
static void utc_notification_get_image_p(void);
static void utc_notification_set_time_n(void);
static void utc_notification_set_time_p(void);
static void utc_notification_get_time_n(void);
static void utc_notification_get_time_p(void);
static void utc_notification_get_insert_time_n(void);
static void utc_notification_get_insert_time_p(void);
static void utc_notification_set_text_n(void);
static void utc_notification_set_text_p(void);
static void utc_notification_get_text_n(void);
static void utc_notification_get_text_p(void);
static void utc_notification_set_text_domain_n(void);
static void utc_notification_set_text_domain_p(void);
static void utc_notification_get_text_domain_n(void);
static void utc_notification_get_text_domain_p(void);
static void utc_notification_set_time_to_text_n(void);
static void utc_notification_set_time_to_text_p(void);
static void utc_notification_get_time_from_text_n(void);
static void utc_notification_get_time_from_text_p(void);
static void utc_notification_set_sound_n(void);
static void utc_notification_set_sound_p(void);
static void utc_notification_get_sound_n(void);
static void utc_notification_get_sound_p(void);
static void utc_notification_set_vibration_n(void);
static void utc_notification_set_vibration_p(void);
static void utc_notification_get_vibration_n(void);
static void utc_notification_get_vibration_p(void);
static void utc_notification_set_led_n(void);
static void utc_notification_set_led_p(void);
static void utc_notification_get_led_n(void);
static void utc_notification_get_led_p(void);
static void utc_notification_set_led_time_period_n(void);
static void utc_notification_set_led_time_period_p(void);
static void utc_notification_get_led_time_period_n(void);
static void utc_notification_get_led_time_period_p(void);
static void utc_notification_set_application_n(void);
static void utc_notification_set_application_p(void);
static void utc_notification_get_application_n(void);
static void utc_notification_get_application_p(void);
static void utc_notification_set_launch_option_n(void);
static void utc_notification_set_launch_option_p(void);
static void utc_notification_get_launch_option_n(void);
static void utc_notification_get_launch_option_p(void);
static void utc_notification_set_execute_option_n(void);
static void utc_notification_set_execute_option_p(void);
static void utc_notification_get_execute_option_n(void);
static void utc_notification_get_execute_option_p(void);
static void utc_notification_set_property_n(void);
static void utc_notification_set_property_p(void);
static void utc_notification_get_property_n(void);
static void utc_notification_get_property_p(void);
static void utc_notification_set_display_applist_n(void);
static void utc_notification_set_display_applist_p(void);
static void utc_notification_get_display_applist_n(void);
static void utc_notification_get_display_applist_p(void);
static void utc_notification_set_size_n(void);
static void utc_notification_set_size_p(void);
static void utc_notification_get_size_n(void);
static void utc_notification_get_size_p(void);
static void utc_notification_set_progress_n(void);
static void utc_notification_set_progress_p(void);
static void utc_notification_get_progress_n(void);
static void utc_notification_get_progress_p(void);
static void utc_notification_set_pkgname_n(void);
static void utc_notification_set_pkgname_p(void);
static void utc_notification_get_pkgname_n(void);
static void utc_notification_get_pkgname_p(void);
static void utc_notification_set_layout_n(void);
static void utc_notification_set_layout_p(void);
static void utc_notification_get_layout_n(void);
static void utc_notification_get_layout_p(void);
static void utc_notification_get_id_n(void);
static void utc_notification_get_id_p(void);
static void utc_notification_get_type_n(void);
static void utc_notification_get_type_p(void);
static void utc_notification_insert_n(void);
static void utc_notification_insert_p(void);
static void utc_notification_clear_n(void);
static void utc_notification_clear_p(void);
static void utc_notification_update_n(void);
static void utc_notification_update_p(void);
static void utc_notification_delete_all_by_type_n(void);
static void utc_notification_delete_all_by_type_p(void);
static void utc_notification_delete_by_priv_id_n(void);
static void utc_notification_delete_by_priv_id_p(void);
static void utc_notification_delete_n(void);
static void utc_notification_delete_p(void);
static void utc_notification_update_progress_n(void);
static void utc_notification_update_progress_p(void);
static void utc_notification_update_size_n(void);
static void utc_notification_update_size_p(void);
static void utc_notification_update_content_n(void);
static void utc_notification_update_content_p(void);
static void utc_notification_new_1_n(void);
static void utc_notification_new_2_n(void);
static void utc_notification_new_1_p(void);
static void utc_notification_new_2_p(void);
static void utc_notification_load_n(void);
static void utc_notification_load_p(void);
static void utc_notification_clone_n(void);
static void utc_notification_clone_p(void);
static void utc_notification_free_n(void);
static void utc_notification_free_p(void);
static void utc_notification_resister_changed_cb_n(void);
static void utc_notification_resister_changed_cb_p(void);
static void utc_notification_unresister_changed_cb_n(void);
static void utc_notification_unresister_changed_cb_p(void);
static void utc_notification_register_detailed_changed_cb_n(void);
static void utc_notification_register_detailed_changed_cb_p(void);
static void utc_notification_unregister_detailed_changed_cb_n(void);
static void utc_notification_unregister_detailed_changed_cb_p(void);
static void utc_notification_get_count_n(void);
static void utc_notification_get_count_p(void);
static void utc_notification_get_list_n(void);
static void utc_notification_get_list_p(void);
static void utc_notification_get_grouping_list_n(void);
static void utc_notification_get_grouping_list_p(void);
static void utc_notification_get_detail_list_n(void);
static void utc_notification_get_detail_list_p(void);
static void utc_notification_free_list_n(void);
static void utc_notification_free_list_p(void);
static void utc_notification_op_get_data_n(void);
static void utc_notification_op_get_data_p(void);
static void utc_notification_is_service_ready_p(void);
static void utc_notification_add_deferred_task_n(void);
static void utc_notification_add_deferred_task_p(void);
static void utc_notification_del_deferred_task_n(void);
static void utc_notification_del_deferred_task_p(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_notification_create_1_n, NEGATIVE_TC_IDX},
	{utc_notification_create_2_n, NEGATIVE_TC_IDX},
	{utc_notification_create_1_p, POSITIVE_TC_IDX},
	{utc_notification_create_2_p, POSITIVE_TC_IDX},
	{utc_notification_set_image_n, NEGATIVE_TC_IDX},
	{utc_notification_set_image_p, POSITIVE_TC_IDX},
	{utc_notification_get_image_n, NEGATIVE_TC_IDX},
	{utc_notification_get_image_p, POSITIVE_TC_IDX},
	{utc_notification_set_time_n, NEGATIVE_TC_IDX},
	{utc_notification_set_time_p, POSITIVE_TC_IDX},
	{utc_notification_get_time_n, NEGATIVE_TC_IDX},
	{utc_notification_get_time_p, POSITIVE_TC_IDX},
	{utc_notification_get_insert_time_n, NEGATIVE_TC_IDX},
	{utc_notification_get_insert_time_p, POSITIVE_TC_IDX},
	{utc_notification_set_text_n, NEGATIVE_TC_IDX},
	{utc_notification_set_text_p, POSITIVE_TC_IDX},
	{utc_notification_get_text_n, NEGATIVE_TC_IDX},
	{utc_notification_get_text_p, POSITIVE_TC_IDX},
	{utc_notification_set_text_domain_n, NEGATIVE_TC_IDX},
	{utc_notification_set_text_domain_p, POSITIVE_TC_IDX},
	{utc_notification_get_text_domain_n, NEGATIVE_TC_IDX},
	{utc_notification_get_text_domain_p, POSITIVE_TC_IDX},
	{utc_notification_set_time_to_text_n, NEGATIVE_TC_IDX},
	{utc_notification_set_time_to_text_p, POSITIVE_TC_IDX},
	{utc_notification_get_time_from_text_n, NEGATIVE_TC_IDX},
	{utc_notification_get_time_from_text_p, POSITIVE_TC_IDX},
	{utc_notification_set_sound_n, NEGATIVE_TC_IDX},
	{utc_notification_set_sound_p, POSITIVE_TC_IDX},
	{utc_notification_get_sound_n, NEGATIVE_TC_IDX},
	{utc_notification_get_sound_p, POSITIVE_TC_IDX},
	{utc_notification_set_vibration_n, NEGATIVE_TC_IDX},
	{utc_notification_set_vibration_p, POSITIVE_TC_IDX},
	{utc_notification_get_vibration_n, NEGATIVE_TC_IDX},
	{utc_notification_get_vibration_p, POSITIVE_TC_IDX},
	{utc_notification_set_led_n, NEGATIVE_TC_IDX},
	{utc_notification_set_led_p, POSITIVE_TC_IDX},
	{utc_notification_get_led_n, NEGATIVE_TC_IDX},
	{utc_notification_get_led_p, POSITIVE_TC_IDX},
	{utc_notification_set_led_time_period_n, NEGATIVE_TC_IDX},
	{utc_notification_set_led_time_period_p, POSITIVE_TC_IDX},
	{utc_notification_get_led_time_period_n, NEGATIVE_TC_IDX},
	{utc_notification_get_led_time_period_p, POSITIVE_TC_IDX},
	{utc_notification_set_application_n, NEGATIVE_TC_IDX},
	{utc_notification_set_application_p, POSITIVE_TC_IDX},
	{utc_notification_get_application_n, NEGATIVE_TC_IDX},
	{utc_notification_get_application_p, POSITIVE_TC_IDX},
	{utc_notification_set_launch_option_n, NEGATIVE_TC_IDX},
	{utc_notification_set_launch_option_p, POSITIVE_TC_IDX},
	{utc_notification_get_launch_option_n, NEGATIVE_TC_IDX},
	{utc_notification_get_launch_option_p, POSITIVE_TC_IDX},
	{utc_notification_set_execute_option_n, NEGATIVE_TC_IDX},
	{utc_notification_set_execute_option_p, POSITIVE_TC_IDX},
	{utc_notification_get_execute_option_n, NEGATIVE_TC_IDX},
	{utc_notification_get_execute_option_p, POSITIVE_TC_IDX},
	{utc_notification_set_property_n, NEGATIVE_TC_IDX},
	{utc_notification_set_property_p, POSITIVE_TC_IDX},
	{utc_notification_get_property_n, NEGATIVE_TC_IDX},
	{utc_notification_get_property_p, POSITIVE_TC_IDX},
	{utc_notification_set_display_applist_n, NEGATIVE_TC_IDX},
	{utc_notification_set_display_applist_p, POSITIVE_TC_IDX},
	{utc_notification_get_display_applist_n, NEGATIVE_TC_IDX},
	{utc_notification_get_display_applist_p, POSITIVE_TC_IDX},
	{utc_notification_set_size_n, NEGATIVE_TC_IDX},
	{utc_notification_set_size_p, POSITIVE_TC_IDX},
	{utc_notification_get_size_n, NEGATIVE_TC_IDX},
	{utc_notification_get_size_p, POSITIVE_TC_IDX},
	{utc_notification_set_progress_n, NEGATIVE_TC_IDX},
	{utc_notification_set_progress_p, POSITIVE_TC_IDX},
	{utc_notification_get_progress_n, NEGATIVE_TC_IDX},
	{utc_notification_get_progress_p, POSITIVE_TC_IDX},
	{utc_notification_set_pkgname_n, NEGATIVE_TC_IDX},
	{utc_notification_set_pkgname_p, POSITIVE_TC_IDX},
	{utc_notification_get_pkgname_n, NEGATIVE_TC_IDX},
	{utc_notification_get_pkgname_p, POSITIVE_TC_IDX},
	{utc_notification_set_layout_n, NEGATIVE_TC_IDX},
	{utc_notification_set_layout_p, POSITIVE_TC_IDX},
	{utc_notification_get_layout_n, NEGATIVE_TC_IDX},
	{utc_notification_get_layout_p, POSITIVE_TC_IDX},
	{utc_notification_get_id_n, NEGATIVE_TC_IDX},
	{utc_notification_get_id_p, POSITIVE_TC_IDX},
	{utc_notification_get_type_n, NEGATIVE_TC_IDX},
	{utc_notification_get_type_p, POSITIVE_TC_IDX},
	{utc_notification_insert_n, NEGATIVE_TC_IDX},
	{utc_notification_insert_p, POSITIVE_TC_IDX},
	{utc_notification_clear_n, NEGATIVE_TC_IDX},
	{utc_notification_clear_p, POSITIVE_TC_IDX},
	{utc_notification_update_n, NEGATIVE_TC_IDX},
	{utc_notification_update_p, POSITIVE_TC_IDX},
	{utc_notification_delete_all_by_type_n, NEGATIVE_TC_IDX},
	{utc_notification_delete_all_by_type_p, POSITIVE_TC_IDX},
	{utc_notification_delete_by_priv_id_n, NEGATIVE_TC_IDX},
	{utc_notification_delete_by_priv_id_p, POSITIVE_TC_IDX},
	{utc_notification_delete_n, NEGATIVE_TC_IDX},
	{utc_notification_delete_p, POSITIVE_TC_IDX},
	{utc_notification_update_progress_n, NEGATIVE_TC_IDX},
	{utc_notification_update_progress_p, POSITIVE_TC_IDX},
	{utc_notification_update_size_n, NEGATIVE_TC_IDX},
	{utc_notification_update_size_p, POSITIVE_TC_IDX},
	{utc_notification_update_content_n, NEGATIVE_TC_IDX},
	{utc_notification_update_content_p, POSITIVE_TC_IDX},
	{utc_notification_new_1_n, NEGATIVE_TC_IDX},
	{utc_notification_new_2_n, NEGATIVE_TC_IDX},
	{utc_notification_new_1_p, POSITIVE_TC_IDX},
	{utc_notification_new_2_p, POSITIVE_TC_IDX},
	{utc_notification_load_n, NEGATIVE_TC_IDX},
	{utc_notification_load_p, POSITIVE_TC_IDX},
	{utc_notification_clone_n, NEGATIVE_TC_IDX},
	{utc_notification_clone_p, POSITIVE_TC_IDX},
	{utc_notification_free_n, NEGATIVE_TC_IDX},
	{utc_notification_free_p, POSITIVE_TC_IDX},
	{utc_notification_resister_changed_cb_n, NEGATIVE_TC_IDX},
	{utc_notification_resister_changed_cb_p, POSITIVE_TC_IDX},
	{utc_notification_unresister_changed_cb_n, NEGATIVE_TC_IDX},
	{utc_notification_unresister_changed_cb_p, POSITIVE_TC_IDX},
	{utc_notification_register_detailed_changed_cb_n, NEGATIVE_TC_IDX},
	{utc_notification_register_detailed_changed_cb_p, POSITIVE_TC_IDX},
	{utc_notification_unregister_detailed_changed_cb_n, NEGATIVE_TC_IDX},
	{utc_notification_unregister_detailed_changed_cb_p, POSITIVE_TC_IDX},
	{utc_notification_get_count_n, NEGATIVE_TC_IDX},
	{utc_notification_get_count_p, POSITIVE_TC_IDX},
	{utc_notification_get_list_n, NEGATIVE_TC_IDX},
	{utc_notification_get_list_p, POSITIVE_TC_IDX},
	{utc_notification_get_grouping_list_n, NEGATIVE_TC_IDX},
	{utc_notification_get_grouping_list_p, POSITIVE_TC_IDX},
	{utc_notification_get_detail_list_n, NEGATIVE_TC_IDX},
	{utc_notification_get_detail_list_p, POSITIVE_TC_IDX},
	{utc_notification_free_list_n, NEGATIVE_TC_IDX},
	{utc_notification_free_list_p, POSITIVE_TC_IDX},
	{utc_notification_op_get_data_n, NEGATIVE_TC_IDX},
	{utc_notification_op_get_data_p, POSITIVE_TC_IDX},
	{utc_notification_is_service_ready_p, POSITIVE_TC_IDX},
	{utc_notification_add_deferred_task_n, NEGATIVE_TC_IDX},
	{utc_notification_add_deferred_task_p, POSITIVE_TC_IDX},
	{utc_notification_del_deferred_task_n, NEGATIVE_TC_IDX},
	{utc_notification_del_deferred_task_p, POSITIVE_TC_IDX},
	{ NULL, 0 },
};

static void startup(void)
{
	/* start of TC */
	notification_clear(NOTIFICATION_TYPE_NONE);
	tet_printf("\n TC start");
}


static void cleanup(void)
{
	/* end of TC */
	tet_printf("\n TC end");
}

static void _changed_cb(void *data, notification_type_e type)
{
}

static void _detailed_changed_cb(void *data, notification_type_e type, notification_op *op_list, int num_op)
{
}

static void _deffered_job_cb(void *data)
{
}

/**
 * @brief Negative test case of notification_create()
 */
static void utc_notification_create_1_n(void)
{
	notification_h notification = NULL;
	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NONE);
	dts_check_eq("notification_create", notification, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_create()
 */
static void utc_notification_create_2_n(void)
{
	notification_h notification = NULL;
	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_MAX);
	dts_check_eq("notification_create", notification, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_create()
 */
static void utc_notification_create_1_p(void)
{
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	dts_check_ne("notification_create", notification, NULL,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_create()
 */
static void utc_notification_create_2_p(void)
{
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	dts_check_ne("notification_create", notification, NULL,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_set_image()
 */
static void utc_notification_set_image_n(void)
{
	int ret = 0;

	ret = notification_set_image(NULL, NOTIFICATION_IMAGE_TYPE_ICON, NULL);

	dts_check_eq("notification_set_image", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_image()
 */
static void utc_notification_set_image_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_image(notification, NOTIFICATION_IMAGE_TYPE_ICON, "test.png");

	dts_check_eq("notification_set_image", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_image()
 */
static void utc_notification_get_image_n(void)
{
	int ret = 0;

	ret = notification_get_image(NULL, NOTIFICATION_IMAGE_TYPE_ICON, NULL);

	dts_check_eq("notification_get_image", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_image()
 */
static void utc_notification_get_image_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	char *image = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_image(notification, NOTIFICATION_IMAGE_TYPE_ICON, &image);

	dts_check_eq("notification_get_image", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_time()
 */
static void utc_notification_set_time_n(void)
{
	int ret = 0;

	ret = notification_set_time(NULL, 0);

	dts_check_eq("notification_set_time", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_time()
 */
static void utc_notification_set_time_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_time(notification, 0xFFFFFFF);

	dts_check_eq("notification_set_time", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_time()
 */
static void utc_notification_get_time_n(void)
{
	int ret = 0;

	ret = notification_get_time(NULL, NULL);

	dts_check_eq("notification_get_time", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_time()
 */
static void utc_notification_get_time_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	time_t t = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_time(notification, &t);

	dts_check_eq("notification_get_time", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_insert_time()
 */
static void utc_notification_get_insert_time_n(void)
{
	int ret = 0;

	ret = notification_get_insert_time(NULL, NULL);

	dts_check_eq("notification_get_insert_time", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_insert_time()
 */
static void utc_notification_get_insert_time_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	time_t t = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_insert_time(notification, &t);

	dts_check_eq("notification_get_insert_time", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_text()
 */
static void utc_notification_set_text_n(void)
{
	int ret = 0;

	ret = notification_set_text(NULL, NOTIFICATION_TEXT_TYPE_NONE, NULL, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);

	dts_check_eq("notification_set_text", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_text()
 */
static void utc_notification_set_text_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "test", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);

	dts_check_eq("notification_set_text", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_text()
 */
static void utc_notification_get_text_n(void)
{
	int ret = 0;

	ret = notification_get_text(NULL, NOTIFICATION_TEXT_TYPE_TITLE, NULL);

	dts_check_eq("notification_get_text", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_text()
 */
static void utc_notification_get_text_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	char *value = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, &value);

	dts_check_eq("notification_get_text", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_text_domain()
 */
static void utc_notification_set_text_domain_n(void)
{
	int ret = 0;

	ret = notification_set_text_domain(NULL, NULL, NULL);

	dts_check_eq("notification_set_text_domain", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_text_domain()
 */
static void utc_notification_set_text_domain_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_text_domain(notification, "domain", "domain.po");

	dts_check_eq("notification_set_text_domain", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_text_domain()
 */
static void utc_notification_get_text_domain_n(void)
{
	int ret = 0;

	ret = notification_get_text_domain(NULL, NULL, NULL);

	dts_check_eq("notification_get_text_domain", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_text_domain()
 */
static void utc_notification_get_text_domain_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	char *value1 = NULL;
	char *value2 = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_text_domain(notification, &value1, &value2);

	dts_check_eq("notification_get_text_domain", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_time_to_text()
 */
static void utc_notification_set_time_to_text_n(void)
{
	int ret = 0;

	ret = notification_set_time_to_text(NULL, NOTIFICATION_TEXT_TYPE_NONE, 0);

	dts_check_eq("notification_set_time_to_text", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_time_to_text()
 */
static void utc_notification_set_time_to_text_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_time_to_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, time(NULL));

	dts_check_eq("notification_set_time_to_text", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_time_from_text()
 */
static void utc_notification_get_time_from_text_n(void)
{
	int ret = 0;

	ret = notification_get_time_from_text(NULL, NOTIFICATION_TEXT_TYPE_TITLE, NULL);

	dts_check_eq("notification_get_time_from_text", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_time_from_text()
 */
static void utc_notification_get_time_from_text_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	time_t t = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_time_to_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, time(NULL));
	ret = notification_get_time_from_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, &t);

	dts_check_eq("notification_get_time_from_text", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_sound()
 */
static void utc_notification_set_sound_n(void)
{
	int ret = 0;

	ret = notification_set_sound(NULL, NOTIFICATION_SOUND_TYPE_MAX, NULL);

	dts_check_eq("notification_set_sound", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_sound()
 */
static void utc_notification_set_sound_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_sound(notification, NOTIFICATION_SOUND_TYPE_DEFAULT, "path");

	dts_check_eq("notification_set_sound", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_sound()
 */
static void utc_notification_get_sound_n(void)
{
	int ret = 0;

	ret = notification_get_sound(NULL, NULL, NULL);

	dts_check_eq("notification_get_sound", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_sound()
 */
static void utc_notification_get_sound_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_sound_type_e type;
	const char *value = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_sound(notification, &type, &value);

	dts_check_eq("notification_get_sound", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_vibration()
 */
static void utc_notification_set_vibration_n(void)
{
	int ret = 0;

	ret = notification_set_vibration(NULL, NOTIFICATION_VIBRATION_TYPE_MAX, NULL);

	dts_check_eq("notification_set_vibration", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_vibration()
 */
static void utc_notification_set_vibration_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_vibration(notification, NOTIFICATION_VIBRATION_TYPE_DEFAULT, "path");

	dts_check_eq("notification_set_vibration", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_vibration()
 */
static void utc_notification_get_vibration_n(void)
{
	int ret = 0;

	ret = notification_get_vibration(NULL, NULL, NULL);

	dts_check_eq("notification_get_vibration", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_vibration()
 */
static void utc_notification_get_vibration_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_vibration_type_e type;
	const char *value = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_vibration(notification, &type, &value);

	dts_check_eq("notification_get_vibration", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_led()
 */
static void utc_notification_set_led_n(void)
{
	int ret = 0;

	ret = notification_set_led(NULL, NOTIFICATION_LED_OP_MAX, 0);

	dts_check_eq("notification_set_led", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_led()
 */
static void utc_notification_set_led_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_led(notification, NOTIFICATION_LED_OP_ON, 0);

	dts_check_eq("notification_set_led", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_led()
 */
static void utc_notification_get_led_n(void)
{
	int ret = 0;

	ret = notification_get_led(NULL, NULL, NULL);

	dts_check_eq("notification_get_led", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_led()
 */
static void utc_notification_get_led_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_led_op_e type;
	int value = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_led(notification, &type, &value);

	dts_check_eq("notification_get_led", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_led_time_period()
 */
static void utc_notification_set_led_time_period_n(void)
{
	int ret = 0;

	ret = notification_set_led_time_period(NULL, 0, 0);

	dts_check_eq("notification_set_led_time_period", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_led_time_period()
 */
static void utc_notification_set_led_time_period_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_led_time_period(notification, 10, 10);

	dts_check_eq("notification_set_led_time_period", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_led_time_period()
 */
static void utc_notification_get_led_time_period_n(void)
{
	int ret = 0;

	ret = notification_get_led_time_period(NULL, NULL, NULL);

	dts_check_eq("notification_get_led_time_period", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_led_time_period()
 */
static void utc_notification_get_led_time_period_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	int value1 = 0;
	int value2 = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_led_time_period(notification, &value1, &value2);

	dts_check_eq("notification_get_led_time_period", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_application()
 */
static void utc_notification_set_application_n(void)
{
	int ret = 0;

	ret = notification_set_application(NULL, NULL);

	dts_check_eq("notification_set_application", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_application()
 */
static void utc_notification_set_application_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_application(notification, "org.tizen.quickpanel");

	dts_check_eq("notification_set_application", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_application()
 */
static void utc_notification_get_application_n(void)
{
	int ret = 0;

	ret = notification_get_application(NULL, NULL);

	dts_check_eq("notification_get_application", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_application()
 */
static void utc_notification_get_application_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	char *value = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_application(notification, &value);

	dts_check_eq("notification_get_application", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_launch()
 */
static void utc_notification_set_launch_option_n(void)
{
	int ret = 0;

	ret = notification_set_launch_option(NULL, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, NULL);

	dts_check_eq("notification_set_launch_option", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_launch_option()
 */
static void utc_notification_set_launch_option_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	app_control_h app_control = NULL;
	bundle *b = NULL;

	app_control_create(&app_control);
	app_control_set_app_id(app_control, "org.tizen.app");
	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_launch_option(notification, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, app_control);
	app_control_destroy(app_control);

	dts_check_eq("notification_set_launch_option", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_launch_option()
 */
static void utc_notification_get_launch_option_n(void)
{
	int ret = 0;
	app_control_h app_control = NULL;

	ret = notification_get_launch_option(NULL, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, NULL);

	dts_check_eq("notification_get_launch_option", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_launch_option()
 */
static void utc_notification_get_launch_option_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	app_control_h app_control = NULL;

	app_control_create(&app_control);
	app_control_set_app_id(app_control, "org.tizen.app");
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_launch_option(notification, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, app_control);
	app_control_destroy(app_control);
	app_control = NULL;

	ret = notification_get_launch_option(notification, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, &app_control);
	notification_free(notification);

	dts_check_eq("notification_get_launch_option", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_execute_option()
 */
static void utc_notification_set_execute_option_n(void)
{
	int ret = 0;

	ret = notification_set_execute_option(NULL, NOTIFICATION_EXECUTE_TYPE_NONE, NULL, NULL, NULL);

	dts_check_eq("notification_set_execute_option", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_execute_option()
 */
static void utc_notification_set_execute_option_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	bundle *b = NULL;

	b = bundle_create();
	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_execute_option(notification, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, b);

	dts_check_eq("notification_set_execute_option", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_execute_option()
 */
static void utc_notification_get_execute_option_n(void)
{
	int ret = 0;

	ret = notification_get_execute_option(NULL, NOTIFICATION_EXECUTE_TYPE_NONE, NULL, NULL);

	dts_check_eq("notification_get_execute_option", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_execute_option()
 */
static void utc_notification_get_execute_option_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	bundle *b = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_execute_option(notification, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, &b);

	dts_check_eq("notification_get_execute_option", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_property()
 */
static void utc_notification_set_property_n(void)
{
	int ret = 0;

	ret = notification_set_property(NULL, 0);

	dts_check_eq("notification_set_property", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_property()
 */
static void utc_notification_set_property_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_property(notification, NOTIFICATION_PROP_DISABLE_AUTO_DELETE);

	dts_check_eq("notification_set_property", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_property()
 */
static void utc_notification_get_property_n(void)
{
	int ret = 0;

	ret = notification_get_property(NULL, NULL);

	dts_check_eq("notification_get_property", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_property()
 */
static void utc_notification_get_property_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	int value = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_property(notification, &value);

	dts_check_eq("notification_get_property", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_display_applist()
 */
static void utc_notification_set_display_applist_n(void)
{
	int ret = 0;

	ret = notification_set_display_applist(NULL, 0);

	dts_check_eq("notification_set_display_applist", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_display_applist()
 */
static void utc_notification_set_display_applist_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_display_applist(notification, NOTIFICATION_DISPLAY_APP_ALL);

	dts_check_eq("notification_set_display_applist", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_display_applist()
 */
static void utc_notification_get_display_applist_n(void)
{
	int ret = 0;

	ret = notification_get_display_applist(NULL, NULL);

	dts_check_eq("notification_get_display_applist", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_display_applist()
 */
static void utc_notification_get_display_applist_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	int value = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_display_applist(notification, &value);

	dts_check_eq("notification_get_display_applist", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_size()
 */
static void utc_notification_set_size_n(void)
{
	int ret = 0;

	ret = notification_set_size(NULL, 0.0);

	dts_check_eq("notification_set_size", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_size()
 */
static void utc_notification_set_size_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_size(notification, 0.5);

	dts_check_eq("notification_set_size", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_size()
 */
static void utc_notification_get_size_n(void)
{
	int ret = 0;

	ret = notification_get_size(NULL, NULL);

	dts_check_eq("notification_get_size", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_size()
 */
static void utc_notification_get_size_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	double value = 0.0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_size(notification, &value);

	dts_check_eq("notification_get_size", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_progress()
 */
static void utc_notification_set_progress_n(void)
{
	int ret = 0;

	ret = notification_set_progress(NULL, 0.0);

	dts_check_eq("notification_set_progress", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_progress()
 */
static void utc_notification_set_progress_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_progress(notification, 0.5);

	dts_check_eq("notification_set_progress", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_progress()
 */
static void utc_notification_get_progress_n(void)
{
	int ret = 0;

	ret = notification_get_progress(NULL, NULL);

	dts_check_eq("notification_get_progress", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_progress()
 */
static void utc_notification_get_progress_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	double value = 0.0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_progress(notification, &value);

	dts_check_eq("notification_get_progress", ret, NOTIFICATION_ERROR_NONE,
			"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_set_pkgname()
 */
static void utc_notification_set_pkgname_n(void)
{
	int ret = 0;

	ret = notification_set_pkgname(NULL, NULL);

	dts_check_eq("notification_set_pkgname", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_pkgname()
 */
static void utc_notification_set_pkgname_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_pkgname(notification, "org.tizen.quickpanel");

	dts_check_eq("notification_set_pkgname", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_pkgname()
 */
static void utc_notification_get_pkgname_n(void)
{
	int ret = 0;

	ret = notification_get_pkgname(NULL, NULL);

	dts_check_eq("notification_get_pkgname", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_pkgname()
 */
static void utc_notification_get_pkgname_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	char *value = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_pkgname(notification, &value);

	dts_check_eq("notification_get_pkgname", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_set_layout()
 */
static void utc_notification_set_layout_n(void)
{
	int ret = 0;

	ret = notification_set_layout(NULL, NOTIFICATION_LY_NONE);

	dts_check_eq("notification_set_layout", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_set_layout()
 */
static void utc_notification_set_layout_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_set_layout(notification, NOTIFICATION_LY_NOTI_EVENT_SINGLE);

	dts_check_eq("notification_set_layout", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_layout()
 */
static void utc_notification_get_layout_n(void)
{
	int ret = 0;

	ret = notification_get_layout(NULL, NULL);

	dts_check_eq("notification_get_layout", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_layout()
 */
static void utc_notification_get_layout_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_ly_type_e type;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_layout(notification, &type);

	dts_check_eq("notification_get_layout", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_id()
 */
static void utc_notification_get_id_n(void)
{
	int ret = 0;

	ret = notification_get_id(NULL, NULL, NULL);

	dts_check_eq("notification_get_id", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_id()
 */
static void utc_notification_get_id_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	int value = 0;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_id(notification, NULL, &value);

	dts_check_eq("notification_get_id", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_get_type()
 */
static void utc_notification_get_type_n(void)
{
	int ret = 0;

	ret = notification_get_type(NULL, NULL);

	dts_check_eq("notification_get_type", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_type()
 */
static void utc_notification_get_type_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_type_e type;

	/*Invalid parameter test*/
	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	ret = notification_get_type(notification, &type);

	dts_check_eq("notification_get_type", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_insert()
 */
static void utc_notification_insert_n(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "test", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	ret = notification_insert(NULL, NULL);

	dts_check_eq("notification_insert", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_insert()
 */
static void utc_notification_insert_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "test", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	ret = notification_insert(notification, NULL);

	dts_check_eq("notification_insert", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_clear()
 */
static void utc_notification_clear_n(void)
{
	int ret = 0;

	ret = notification_clear(NOTIFICATION_TYPE_MAX);
	dts_check_eq("notification_clear", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_clear()
 */
static void utc_notification_clear_p(void)
{
	int ret = 0;

	ret = notification_clear(NOTIFICATION_TYPE_NOTI);
	dts_check_eq("notification_clear", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_update()
 */
static void utc_notification_update_n(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "test", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, NULL);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "updated", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	ret = notification_update(NULL);

	dts_check_eq("notification_update", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_update()
 */
static void utc_notification_update_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "test", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, NULL);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "updated", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	ret = notification_update(notification);

	dts_check_eq("notification_update", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_delete_all_by_type()
 */
static void utc_notification_delete_all_by_type_n(void)
{
	int ret = 0;

	ret = notification_delete_all_by_type(NULL, NOTIFICATION_TYPE_MAX);
	dts_check_eq("notification_delete_all_by_type", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_delete_all_by_type()
 */
static void utc_notification_delete_all_by_type_p(void)
{
	int ret = 0;

	ret = notification_delete_all_by_type(NULL, NOTIFICATION_TYPE_NOTI);
	dts_check_eq("notification_delete_all_by_type", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_delete_by_priv_id()
 */
static void utc_notification_delete_by_priv_id_n(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "privid_delete", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	ret = notification_delete_by_priv_id(TEST_PKG, NOTIFICATION_TYPE_NOTI, NOTIFICATION_PRIV_ID_NONE);
	dts_check_eq("notification_delete_by_priv_id", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_delete_by_priv_id()
 */
static void utc_notification_delete_by_priv_id_p(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "privid_delete", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	ret = notification_delete_by_priv_id(TEST_PKG, NOTIFICATION_TYPE_NOTI, priv_id);
	dts_check_eq("notification_delete_by_priv_id", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_delete()
 */
static void utc_notification_delete_n(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "privid_delete", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	ret = notification_delete(NULL);
	dts_check_eq("notification_delete", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_delete()
 */
static void utc_notification_delete_p(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "privid_delete", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	ret = notification_delete(notification);
	dts_check_eq("notification_delete", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_update_progress()
 */
static void utc_notification_update_progress_n(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "progress_test-N", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_set_progress(notification, 0.1);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_progress(NULL, NOTIFICATION_PRIV_ID_NONE, 0.9);
	dts_check_eq("notification_update_progress", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_update_progress()
 */
static void utc_notification_update_progress_p(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "progress_test-P", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_set_progress(notification, 0.1);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_progress(notification, priv_id, 0.9);
	dts_check_eq("notification_update_progress", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_update_size()
 */
static void utc_notification_update_size_n(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "size_test-N", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_set_size(notification, 1000);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_size(NULL, NOTIFICATION_PRIV_ID_NONE, 10000000);
	dts_check_eq("notification_update_size", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_update_size()
 */
static void utc_notification_update_size_p(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_TITLE, "size_test-P", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_set_size(notification, 1000);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_size(notification, priv_id, 10000000);
	dts_check_eq("notification_update_size", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_update_content()
 */
static void utc_notification_update_content_n(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_CONTENT, "contents_test-N", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_content(NULL, NOTIFICATION_PRIV_ID_NONE, NULL);
	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_update_content()
 */
static void utc_notification_update_content_p(void)
{
	int ret = 0;
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_CONTENT, "contents_test-P", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	sleep(3);

	ret = notification_update_content(notification, priv_id, "contents_test-P-done");
	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_new()
 */
static void utc_notification_new_1_n(void)
{
	notification_h notification = NULL;
	/*Invalid parameter test*/
	notification = notification_new(NOTIFICATION_TYPE_NONE, NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE);
	dts_check_eq("notification_new", notification, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_new()
 */
static void utc_notification_new_2_n(void)
{
	notification_h notification = NULL;
	/*Invalid parameter test*/
	notification = notification_new(NOTIFICATION_TYPE_MAX, NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE);
	dts_check_eq("notification_new", notification, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_new()
 */
static void utc_notification_new_1_p(void)
{
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_new(NOTIFICATION_TYPE_NOTI, NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE);
	dts_check_ne("notification_new", notification, NULL,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_new()
 */
static void utc_notification_new_2_p(void)
{
	notification_h notification = NULL;

	/*Invalid parameter test*/
	notification = notification_new(NOTIFICATION_TYPE_ONGOING, NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE);
	dts_check_ne("notification_new", notification, NULL,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_load()
 */
static void utc_notification_load_n(void)
{
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_CONTENT, "contents_test-P", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	sleep(1);

	/*Invalid parameter test*/
	notification = notification_load(NULL, NOTIFICATION_PRIV_ID_NONE);
	dts_check_eq("notification_load", notification, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_load()
 */
static void utc_notification_load_p(void)
{
	int priv_id = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	notification_set_pkgname(notification, TEST_PKG);
	notification_set_text(notification, NOTIFICATION_TEXT_TYPE_CONTENT, "contents_test-P", NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_insert(notification, &priv_id);

	sleep(1);

	/*Invalid parameter test*/
	notification = notification_load(TEST_PKG, priv_id);
	dts_check_ne("notification_load", notification, NULL,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_clone()
 */
static void utc_notification_clone_n(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	ret = notification_clone(NULL, NULL);

	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_clone()
 */
static void utc_notification_clone_p(void)
{
	int ret = 0;
	notification_h notification = NULL;
	notification_h notification_cloned = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	ret = notification_clone(notification, &notification_cloned);

	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_free()
 */
static void utc_notification_free_n(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	ret = notification_free(NULL);

	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_free()
 */
static void utc_notification_free_p(void)
{
	int ret = 0;
	notification_h notification = NULL;

	notification = notification_create(NOTIFICATION_TYPE_ONGOING);
	ret = notification_free(notification);

	dts_check_eq("notification_update_content", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_resister_changed_cb()
 */
static void utc_notification_resister_changed_cb_n(void)
{
	int ret = 0;

	ret = notification_resister_changed_cb(NULL, NULL);
	dts_check_eq("notification_resister_changed_cb", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_resister_changed_cb()
 */
static void utc_notification_resister_changed_cb_p(void)
{
	int ret = 0;

	ret = notification_resister_changed_cb(_changed_cb, NULL);
	notification_unresister_changed_cb(_changed_cb);
	dts_check_eq("notification_resister_changed_cb", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_unresister_changed_cb()
 */
static void utc_notification_unresister_changed_cb_n(void)
{
	int ret = 0;

	ret = notification_unresister_changed_cb(NULL);
	dts_check_eq("notification_unresister_changed_cb", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_unresister_changed_cb()
 */
static void utc_notification_unresister_changed_cb_p(void)
{
	int ret = 0;

	notification_resister_changed_cb(_changed_cb, NULL);
	ret = notification_unresister_changed_cb(_changed_cb);
	dts_check_eq("notification_unresister_changed_cb", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_register_detailed_changed_cb()
 */
static void utc_notification_register_detailed_changed_cb_n(void)
{
	int ret = 0;

	ret = notification_register_detailed_changed_cb(NULL, NULL);
	dts_check_eq("notification_register_detailed_changed_cb", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_register_detailed_changed_cb()
 */
static void utc_notification_register_detailed_changed_cb_p(void)
{
	int ret = 0;

	ret = notification_register_detailed_changed_cb(_detailed_changed_cb, NULL);
	notification_unregister_detailed_changed_cb(_detailed_changed_cb, NULL);
	dts_check_eq("notification_register_detailed_changed_cb", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_unregister_detailed_changed_cb()
 */
static void utc_notification_unregister_detailed_changed_cb_n(void)
{
	int ret = 0;

	ret = notification_unregister_detailed_changed_cb(NULL, NULL);
	dts_check_eq("notification_unregister_detailed_changed_cb", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_unregister_detailed_changed_cb()
 */
static void utc_notification_unregister_detailed_changed_cb_p(void)
{
	int ret = 0;

	notification_register_detailed_changed_cb(_detailed_changed_cb, NULL);
	ret = notification_unregister_detailed_changed_cb(_detailed_changed_cb, NULL);
	dts_check_eq("notification_unregister_detailed_changed_cb", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_get_count()
 */
static void utc_notification_get_count_n(void)
{
	int ret = 0;

	ret = notification_get_count(NOTIFICATION_TYPE_NONE, NULL,
			NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE
			,NULL);
	dts_check_eq("notification_get_count", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_count()
 */
static void utc_notification_get_count_p(void)
{
	int ret = 0;
	int value = 0;

	ret = notification_get_count(NOTIFICATION_TYPE_NONE, TEST_PKG,
			NOTIFICATION_GROUP_ID_NONE, NOTIFICATION_PRIV_ID_NONE
			, &value);
	dts_check_eq("notification_get_count", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_get_list()
 */
static void utc_notification_get_list_n(void)
{
	int ret = 0;

	ret = notification_get_list(NOTIFICATION_TYPE_NONE, -1, NULL);
	dts_check_eq("notification_get_list", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_list()
 */
static void utc_notification_get_list_p(void)
{
	int ret = 0;
	notification_list_h list;

	ret = notification_get_list(NOTIFICATION_TYPE_NOTI, -1, &list);
	dts_check_eq("notification_get_list", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_get_grouping_list()
 */
static void utc_notification_get_grouping_list_n(void)
{
	int ret = 0;

	ret = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1, NULL);
	dts_check_eq("notification_get_grouping_list", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_grouping_list()
 */
static void utc_notification_get_grouping_list_p(void)
{
	int ret = 0;
	notification_list_h list;

	ret = notification_get_grouping_list(NOTIFICATION_TYPE_NOTI, -1, &list);
	dts_check_eq("notification_get_grouping_list", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_get_detail_list()
 */
static void utc_notification_get_detail_list_n(void)
{
	int ret = 0;

	ret = notification_get_detail_list(TEST_PKG,
			NOTIFICATION_GROUP_ID_NONE,
			NOTIFICATION_PRIV_ID_NONE,
			-1, NULL);
	dts_check_eq("notification_get_detail_list", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_get_detail_list()
 */
static void utc_notification_get_detail_list_p(void)
{
	int ret = 0;
	notification_list_h list;

	ret = notification_get_detail_list(TEST_PKG,
			NOTIFICATION_GROUP_ID_NONE,
			NOTIFICATION_PRIV_ID_NONE,
			-1, &list);
	dts_check_eq("notification_get_detail_list", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_free_list()
 */
static void utc_notification_free_list_n(void)
{
	int ret = 0;

	ret = notification_free_list(NULL);
	dts_check_eq("notification_free_list", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_free_list()
 */
static void utc_notification_free_list_p(void)
{
	int ret = 0;
	notification_list_h list;

	ret = notification_get_detail_list(TEST_PKG,
			NOTIFICATION_GROUP_ID_NONE,
			NOTIFICATION_PRIV_ID_NONE,
			-1, &list);

	ret = notification_free_list(list);
	dts_check_eq("notification_free_list", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_op_get_data()
 */
static void utc_notification_op_get_data_n(void)
{
	int ret = 0;

	ret = notification_op_get_data(NULL, NOTIFICATION_OP_DATA_MIN, NULL);
	dts_check_eq("notification_op_get_data", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_op_get_data()
 */
static void utc_notification_op_get_data_p(void)
{
	int ret = 0;
	int value = 0;
	notification_op noti_op;
	noti_op.type = NOTIFICATION_OP_INSERT;
	noti_op.priv_id = 1;

	ret = notification_op_get_data(&noti_op, NOTIFICATION_OP_DATA_PRIV_ID, &value);
	dts_check_eq("notification_op_get_data", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_is_service_ready()
 */
static void utc_notification_is_service_ready_p(void)
{
	int value = 0;

	value = notification_is_service_ready();
	dts_check_eq("notification_is_service_ready", value, 1,
		"Must return 1 in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_add_deferred_task()
 */
static void utc_notification_add_deferred_task_n(void)
{
	int ret = 0;

	ret = notification_add_deferred_task(NULL, NULL);
	dts_check_eq("notification_add_deferred_task", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_add_deferred_task()
 */
static void utc_notification_add_deferred_task_p(void)
{
	int ret = 0;

	ret = notification_add_deferred_task(_deffered_job_cb, NULL);
	notification_del_deferred_task(_deffered_job_cb);
	dts_check_eq("notification_add_deferred_task", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_del_deferred_task()
 */
static void utc_notification_del_deferred_task_n(void)
{
	int ret = 0;

	ret = notification_del_deferred_task(NULL);
	dts_check_eq("notification_del_deferred_task", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_del_deferred_task()
 */
static void utc_notification_del_deferred_task_p(void)
{
	int ret = 0;

	ret = notification_add_deferred_task(_deffered_job_cb, NULL);
	ret = notification_del_deferred_task(_deffered_job_cb);
	dts_check_eq("notification_del_deferred_task", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of invalid parameter");
}
