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

static void utc_notification_status_message_post_n(void);
static void utc_notification_status_message_post_p(void);
static void utc_notification_status_monitor_message_cb_set_n(void);
static void utc_notification_status_monitor_message_cb_set_p(void);
static void utc_notification_status_monitor_message_cb_unset_p(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_notification_status_message_post_n, NEGATIVE_TC_IDX},
	{utc_notification_status_message_post_p, POSITIVE_TC_IDX},
	{utc_notification_status_monitor_message_cb_set_n, NEGATIVE_TC_IDX},
	{utc_notification_status_monitor_message_cb_set_p, POSITIVE_TC_IDX},
	{utc_notification_status_monitor_message_cb_unset_p, POSITIVE_TC_IDX},
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

static void _message_callback(const char *message, void *data)
{
}

/**
 * @brief Negative test case of notification_status_message_post()
 */
static void utc_notification_status_message_post_n(void)
{
	int ret;

	ret = notification_status_message_post(NULL);
	dts_check_eq("notification_status_message_post", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_status_message_post()
 */
static void utc_notification_status_message_post_p(void)
{
	int ret;

	ret = notification_status_message_post("TETWARE-P");
	/*Invalid parameter test*/
	dts_check_eq("notification_status_message_post", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Negative test case of notification_status_monitor_message_cb_set()
 */
static void utc_notification_status_monitor_message_cb_set_n(void)
{
	int ret;

	ret = notification_status_monitor_message_cb_set(NULL, NULL);
	dts_check_eq("notification_status_message_post", ret, NOTIFICATION_ERROR_INVALID_DATA,
		"Must return NOTIFICATION_ERROR_INVALID_DATA in case of invalid parameter");
}

/**
 * @brief Positive test case of notification_status_monitor_message_cb_set()
 */
static void utc_notification_status_monitor_message_cb_set_p(void)
{
	int ret;

	ret = notification_status_monitor_message_cb_set(_message_callback, NULL);
	dts_check_eq("notification_status_monitor_message_cb_set", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}

/**
 * @brief Positive test case of notification_status_monitor_message_cb_unset()
 */
static void utc_notification_status_monitor_message_cb_unset_p(void)
{
	int ret;

	ret = notification_status_monitor_message_cb_unset();
	dts_check_eq("notification_status_monitor_message_cb_unset", ret, NOTIFICATION_ERROR_NONE,
		"Must return NOTIFICATION_ERROR_NONE in case of valid parameter");
}
