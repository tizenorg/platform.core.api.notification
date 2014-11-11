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

static void utc_notification_list_get_head_n(void);
static void utc_notification_list_get_head_p(void);
static void utc_notification_list_get_tail_n(void);
static void utc_notification_list_get_tail_p(void);
static void utc_notification_list_get_prev_n(void);
static void utc_notification_list_get_prev_p(void);
static void utc_notification_list_get_next_n(void);
static void utc_notification_list_get_next_p(void);
static void utc_notification_list_get_data_n(void);
static void utc_notification_list_get_data_p(void);
static void utc_notification_list_append_n(void);
static void utc_notification_list_append_p(void);
static void utc_notification_list_remove_n(void);
static void utc_notification_list_remove_p(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_notification_list_get_head_n, NEGATIVE_TC_IDX},
	{utc_notification_list_get_head_p, POSITIVE_TC_IDX},
	{utc_notification_list_get_tail_n, NEGATIVE_TC_IDX},
	{utc_notification_list_get_tail_p, POSITIVE_TC_IDX},
	{utc_notification_list_get_prev_n, NEGATIVE_TC_IDX},
	{utc_notification_list_get_prev_p, POSITIVE_TC_IDX},
	{utc_notification_list_get_next_n, NEGATIVE_TC_IDX},
	{utc_notification_list_get_next_p, POSITIVE_TC_IDX},
	{utc_notification_list_get_data_n, NEGATIVE_TC_IDX},
	{utc_notification_list_get_data_p, POSITIVE_TC_IDX},
	{utc_notification_list_append_n, NEGATIVE_TC_IDX},
	{utc_notification_list_append_p, POSITIVE_TC_IDX},
	{utc_notification_list_remove_n, NEGATIVE_TC_IDX},
	{utc_notification_list_remove_p, POSITIVE_TC_IDX},
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

/**
 * @brief Negative test case of notification_list_get_head()
 */
static void utc_notification_list_get_head_n(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list_value = notification_list_get_head(NULL);
	dts_check_eq("notification_list_get_head", list_value, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_get_head()
 */
static void utc_notification_list_get_head_p(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	/*Invalid parameter test*/
	list_value = notification_list_get_head(list);
	dts_check_ne("notification_list_get_head", list_value, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_get_tail()
 */
static void utc_notification_list_get_tail_n(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list_value = notification_list_get_tail(NULL);
	dts_check_eq("notification_list_get_tail", list_value, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_get_tail()
 */
static void utc_notification_list_get_tail_p(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	/*Invalid parameter test*/
	list_value = notification_list_get_tail(list);
	dts_check_ne("notification_list_get_tail", list_value, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_get_prev()
 */
static void utc_notification_list_get_prev_n(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list_value = notification_list_get_prev(NULL);
	dts_check_eq("notification_list_get_prev", list_value, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_get_prev()
 */
static void utc_notification_list_get_prev_p(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	/*Invalid parameter test*/
	list =  notification_list_get_tail(list);
	list_value = notification_list_get_prev(list);
	dts_check_ne("notification_list_get_prev", list_value, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_get_next()
 */
static void utc_notification_list_get_next_n(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list_value = notification_list_get_next(NULL);
	dts_check_eq("notification_list_get_next", list_value, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_get_next()
 */
static void utc_notification_list_get_next_p(void)
{
	notification_list_h list = NULL;
	notification_list_h list_value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	/*Invalid parameter test*/
	list =  notification_list_get_head(list);
	list_value = notification_list_get_next(list);
	dts_check_ne("notification_list_get_next", list_value, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_get_data()
 */
static void utc_notification_list_get_data_n(void)
{
	notification_list_h list = NULL;
	notification_h value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	value = notification_list_get_data(NULL);
	dts_check_eq("notification_list_get_data", value, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_get_data()
 */
static void utc_notification_list_get_data_p(void)
{
	notification_list_h list = NULL;
	notification_h value = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	/*Invalid parameter test*/
	value = notification_list_get_data(list);
	dts_check_ne("notification_list_get_data", value, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_append()
 */
static void utc_notification_list_append_n(void)
{
	notification_list_h list = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, NULL);
	dts_check_eq("notification_list_append", list, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_append()
 */
static void utc_notification_list_append_p(void)
{
	notification_list_h list = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	dts_check_ne("notification_list_append", list, NULL,
		"Must return valid pointer in case of invalid parameter");
}

/**
 * @brief Negative test case of notification_list_remove()
 */
static void utc_notification_list_remove_n(void)
{
	notification_list_h list = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list = notification_list_remove(NULL, NULL);
	dts_check_eq("notification_list_remove", list, NULL,
		"Must return NULL in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_list_remove()
 */
static void utc_notification_list_remove_p(void)
{
	notification_list_h list = NULL;
	notification_h notification_1 = NULL;
	notification_h notification_2 = NULL;
	notification_h notification_3 = NULL;
	notification_h notification_4 = NULL;

	notification_1 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_2 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_3 = notification_create(NOTIFICATION_TYPE_NOTI);
	notification_4 = notification_create(NOTIFICATION_TYPE_NOTI);
	list = notification_list_append(list, notification_1);
	list = notification_list_append(list, notification_2);
	list = notification_list_append(list, notification_3);
	list = notification_list_append(list, notification_4);

	list = notification_list_remove(list, notification_1);
	dts_check_ne("notification_list_remove", list, NULL,
		"Must return valid pointer in case of invalid parameter");
}
