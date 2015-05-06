/*
 *  Test application for notification API
 *
 * Copyright (c) 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyuho Jo <kyuho.jo@samsung.com>
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



/* common header */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

/* open header */
#include <glib.h>

/* notification header */
#include <notification.h>
#include <notification_status.h>
#include <notification_setting.h>
#include <notification_setting_internal.h>

/*-----------------------------------------------------------------------------------------*/
/* types */
typedef enum testapp_menu_type {
	TESTAPP_MENU_TYPE_MAIN_MENU = 1,
	TESTAPP_MENU_TYPE_BASIC_TEST_MENU = 2,
	TESTAPP_MENU_TYPE_SETTING_TEST_MENU = 3
} testapp_menu_type_e;

/*-----------------------------------------------------------------------------------------*/
/* function prototypes */
static void testapp_system_signal_handler (int signal_number);
void testapp_show_prompt (testapp_menu_type_e menu);

/*-----------------------------------------------------------------------------------------*/
/* implementation */
void testapp_print (char *fmt, ...)
{
	va_list args = {0};
	va_start(args, fmt);
	vfprintf (stdout, fmt, args);
	va_end (args);
	fflush (stdout);
}

static gboolean testapp_initialize_testing ()
{
	struct timeval tv_1, tv_2;
	int interval;

	/* register signal handler */
	if ( signal (SIGINT, testapp_system_signal_handler) == SIG_ERR ) {
		testapp_print ("register signal handler fail\n");
		return FALSE;
	}

	if ( signal (SIGQUIT, testapp_system_signal_handler) == SIG_ERR ) {
		testapp_print ("register signal handler fail\n");
		return FALSE;
	}

	if ( signal (SIGTSTP, testapp_system_signal_handler) == SIG_ERR ) {
		testapp_print ("register signal handler fail\n");
		return FALSE;
	}

	if ( signal (SIGTERM, testapp_system_signal_handler) == SIG_ERR ) {
		testapp_print ("register signal handler fail\n");
		return FALSE;
	}


	gettimeofday(&tv_1, NULL);

	/* TODO : initializing notification */

	gettimeofday(&tv_2, NULL);
	interval = tv_2.tv_usec - tv_1.tv_usec;
	testapp_print("\t Initializing Proceed time %d us\n",interval);


	return TRUE;
}

static gboolean testapp_finalize_testing ()
{
	/* TODO : finalizing notification */

	return TRUE;
}

static void testapp_system_signal_handler (int signal_number)
{
	testapp_print ("signal:%d\n", signal_number);
	switch (signal_number) {
	case SIGQUIT:
	case SIGINT:
	case SIGTSTP:
	case SIGTERM:
		testapp_finalize_testing();
		break;

	default:
		testapp_print ("unhandled signal:%d\n", signal_number);
		break;
	}
	exit(0);
}




void testapp_show_menu (testapp_menu_type_e menu)
{
	switch (menu) {
	case TESTAPP_MENU_TYPE_MAIN_MENU:
		testapp_print ("==========================================\n");
		testapp_print ("    Notification test application \n");
		testapp_print ("==========================================\n");
		testapp_print ("1. Basic Test\n");
		testapp_print ("2. Setting Test\n");
		testapp_print ("0. Exit \n");
		testapp_print ("------------------------------------------\n");
		break;
	case TESTAPP_MENU_TYPE_BASIC_TEST_MENU:
		testapp_print ("==========================================\n");
		testapp_print ("    Basic test menu \n");
		testapp_print ("==========================================\n");
		testapp_print (" 1.  Post a simple notification\n");
		testapp_print (" 2.  Post simple notifications repeatedly\n");
		testapp_print (" 3.  Post a notification on indicator\n");
		testapp_print (" 4.  Post status status message\n");
		testapp_print (" 5.  Delete all notification\n");
		testapp_print (" 6.  Post a heads notification with a button\n");
		testapp_print ("------------------------------------------\n");
		break;
	case TESTAPP_MENU_TYPE_SETTING_TEST_MENU:
		testapp_print ("==========================================\n");
		testapp_print ("    Setting test menu \n");
		testapp_print ("==========================================\n");
		testapp_print (" 1.  Get setting list\n");
		testapp_print (" 2.  Update setting\n");
		testapp_print (" 3.  Update system setting\n");
		testapp_print ("------------------------------------------\n");
		break;
	default:
		break;
	}
}
/* Common { ------------------------------------------------------------------*/

static int testapp_add_a_notification()
{
	notification_h noti_handle = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

	if (noti_handle == NULL) {
		testapp_print("notification_create failed");
		goto FINISH_OFF;
	}

	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, "I'm Title", "TITLE", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, "I'm Content", "CONTENT", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_EVENT_COUNT, "3", "3", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_1, "I'm Info 1", "INFO_1", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_SUB_1, "I'm Info Sub 1", "INFO_SUB_1", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_2, "I'm Info 2", "INFO_2", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_SUB_2, "I'm Info Sub 2", "INFO_SUB_2", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_3, "I'm Info 3", "INFO_3", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_INFO_SUB_3, "I'm Info Sub 3", "INFO_SUB_3", NOTIFICATION_VARIABLE_TYPE_NONE);

	noti_err  = notification_post(noti_handle);

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_post failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	FINISH_OFF:
	if (noti_handle)
		notification_free(noti_handle);

	return noti_err;
}

/* Common } ------------------------------------------------------------------*/

/* Basic Test { --------------------------------------------------------------*/
static int testapp_test_post_notification()
{
	int err = NOTIFICATION_ERROR_NONE;

	if ((err = testapp_add_a_notification()) != NOTIFICATION_ERROR_NONE) {
		testapp_print("testapp_add_a_notification failed[%d]", err);
		goto FINISH_OFF;
	}


	FINISH_OFF:

	return err;
}

static int testapp_test_post_notifications()
{
	int err = NOTIFICATION_ERROR_NONE;
	int repeat_count = 0;
	int i = 0;

	testapp_print("Input count : ");

	if (0 >= scanf("%d", &repeat_count))
		testapp_print("Invalid input");

	for (i = 0; i < repeat_count; i++) {
		if ((err = testapp_add_a_notification()) != NOTIFICATION_ERROR_NONE) {
			testapp_print("testapp_add_a_notification failed[%d]", err);
			goto FINISH_OFF;
		}
	}

	FINISH_OFF:
	return err;
}

static int testapp_test_post_notification_on_indicator()
{
	notification_h noti_handle = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

	if (noti_handle == NULL) {
		testapp_print("notification_create failed");
		goto FINISH_OFF;
	}

	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, "I'm Title", "TITLE", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err  = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, "I'm Content", "CONTENT", NOTIFICATION_VARIABLE_TYPE_NONE);

	noti_err  = notification_set_display_applist(noti_handle, NOTIFICATION_DISPLAY_APP_INDICATOR);

	if(noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_set_display_applist failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	noti_err  = notification_post(noti_handle);

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_post failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	FINISH_OFF:
	if (noti_handle)
		notification_free(noti_handle);

	return noti_err;
}

static int testapp_test_post_status_message()
{
	int noti_err = NOTIFICATION_ERROR_NONE;


	noti_err = notification_status_message_post("This is only a test");

	if(noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_status_message_post failed[%d]", noti_err);
	}

	return noti_err;
}

static int testapp_test_delete_all_notifications()
{
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_delete_all(NOTIFICATION_TYPE_NOTI);

	testapp_print("notification_delete_all returns[%d]", noti_err);

	return noti_err;
}

static int testapp_test_post_heads_up_notification_with_button()
{
	notification_h noti_handle = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	int app_control_err = APP_CONTROL_ERROR_NONE;
	int priv_id = 0;
	int group_id = 0;
	char *app_id = NULL;
	app_control_h app_control = NULL;
	time_t result = time(NULL);
	char tag[100] = { 0, };

	noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

	if (noti_handle == NULL) {
		testapp_print("notification_create failed");
		goto FINISH_OFF;
	}

	noti_err = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, "I'm Title", "TITLE", NOTIFICATION_VARIABLE_TYPE_NONE);
	noti_err = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, "I'm Content", "CONTENT", NOTIFICATION_VARIABLE_TYPE_NONE);

	noti_err = notification_set_display_applist(noti_handle, NOTIFICATION_DISPLAY_APP_HEADS_UP | NOTIFICATION_DISPLAY_APP_NOTIFICATION_TRAY);

	snprintf(tag, 100, "%d", result);

	noti_err = notification_set_tag(noti_handle, tag);

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_set_display_applist failed[%d]\n", noti_err);
		goto FINISH_OFF;
	}

	app_control_err = app_control_create(&app_control);
	if (app_control_err != APP_CONTROL_ERROR_NONE ) {
		testapp_print("app_control_create failed[%d]\n", app_control_err);
		goto FINISH_OFF;
	}

	app_control_err = app_control_set_app_id(app_control, "org.tizen.quickpanel");
	if (app_control_err != APP_CONTROL_ERROR_NONE ) {
		testapp_print("app_control_set_app_id failed[%d]\n", app_control_err);
		goto FINISH_OFF;
	}

	noti_err = notification_set_event_handler(noti_handle, NOTIFICATION_EVENT_TYPE_CLICK_ON_BUTTON_1, app_control);

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_set_event_handler failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	noti_err = notification_post(noti_handle);

	app_control_destroy(app_control);
	app_control = NULL;

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_post failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	noti_err = notification_get_id(noti_handle, &group_id, &priv_id);

	if (noti_err != NOTIFICATION_ERROR_NONE) {
		testapp_print("notification_get_id failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	/*
	if (noti_handle)
		notification_free(noti_handle);
	noti_handle = NULL;
	*/

	noti_handle = notification_load(NULL, priv_id);

	if (noti_handle == NULL) {
		testapp_print("notification_load failed");
		goto FINISH_OFF;
	}

	noti_err = notification_get_event_handler(noti_handle, NOTIFICATION_EVENT_TYPE_CLICK_ON_BUTTON_1, &app_control);

	if (noti_err != NOTIFICATION_ERROR_NONE ||app_control == NULL) {
		testapp_print("notification_get_event_handler failed[%d]", noti_err);
		goto FINISH_OFF;
	}

	app_control_get_app_id(app_control, &app_id);

	if (app_id) {
		testapp_print("result app_id [%s]", app_id);
		free(app_id);
	}

FINISH_OFF:

	if (app_control)
		app_control_destroy(app_control);

	if (noti_handle)
		notification_free(noti_handle);

	return noti_err;
}

static gboolean testapp_interpret_command_basic_test (int selected_number)
{
	gboolean go_to_loop = TRUE;

	switch (selected_number) {
	case 1:
		testapp_test_post_notification();
		break;

	case 2:
		testapp_test_post_notifications();
		break;

	case 3:
		testapp_test_post_notification_on_indicator();
		break;

	case 4:
		testapp_test_post_status_message();
		break;

	case 5:
		testapp_test_delete_all_notifications();
		break;

	case 6:
		testapp_test_post_heads_up_notification_with_button();
		break;

	case 0:
		go_to_loop = FALSE;
		break;

	default:
		break;
	}

	return go_to_loop;

}

void testapp_notification_main ()
{
	gboolean go_to_loop = TRUE;
	int menu_number = 0;

	while (go_to_loop) {
		testapp_show_menu (TESTAPP_MENU_TYPE_BASIC_TEST_MENU);
		testapp_show_prompt (TESTAPP_MENU_TYPE_BASIC_TEST_MENU);

		if (0 >= scanf("%d", &menu_number))
			testapp_print("Invalid input");

		go_to_loop = testapp_interpret_command_basic_test(menu_number);
	}
}
/* Basic Test } ---------------------------------------------------------------*/

/* Setting Test } ---------------------------------------------------------------*/

static int testapp_test_get_setting_list()
{
	int err = NOTIFICATION_ERROR_NONE;
	int i = 0;
	int count = 0;
	char *package_name = NULL;
	bool  allow_to_notify = false;
	bool  do_not_disturb_except = false;
	bool  visibility_class = false;
	notification_setting_h setting_array = NULL;

	notification_setting_get_setting_array(&setting_array, &count);

	testapp_print("count [%d]\n", count);

	for (i = 0; i < count; i++) {
		notification_setting_get_package_name(setting_array + i, &package_name);
		notification_setting_get_allow_to_notify(setting_array + i, &allow_to_notify);
		notification_setting_get_do_not_disturb_except(setting_array + i, &do_not_disturb_except);
		notification_setting_get_visibility_class(setting_array + i, &visibility_class);

		testapp_print("[%d] : package_name[%s], allow_to_notify[%d], do_not_disturb_except[%d], visibility_class[%d]\n"
				,i, package_name, allow_to_notify, do_not_disturb_except, visibility_class);
		free(package_name);
		notification_setting_free_notification(setting_array );
	}

	if (setting_array)
		free(setting_array);

	return err;
}

static int testapp_test_update_setting()
{
	int err = NOTIFICATION_ERROR_NONE;
	notification_setting_h setting = NULL;

	err = notification_setting_get_setting_by_package_name("org.tizen.internet", &setting);

	if (err != NOTIFICATION_ERROR_NONE || setting == NULL) {
		testapp_print("notification_setting_get_setting_by_package_name failed [%d]", err);
	}
	else {
		notification_setting_set_allow_to_notify(setting, 0);
		notification_setting_update_setting(setting);
	}

	return err;
}

static int testapp_test_update_system_setting()
{
	int err = NOTIFICATION_ERROR_NONE;
	bool do_not_disturb;
	int visibility_class;
	notification_system_setting_h system_setting = NULL;


	err = notification_system_setting_load_system_setting(&system_setting);

	if (err != NOTIFICATION_ERROR_NONE || system_setting == NULL) {
		testapp_print("notification_system_setting_load_system_setting failed [%d]\n", err);
		goto out;
	}

	notification_system_setting_get_do_not_disturb(system_setting, &do_not_disturb);
	testapp_print("do_not_disturb [%d]\n", do_not_disturb);
	do_not_disturb = !do_not_disturb;
	notification_system_setting_set_do_not_disturb(system_setting, do_not_disturb);

	notification_system_setting_get_visibility_class(system_setting, &visibility_class);
	testapp_print("visibility_class [%d]\n", visibility_class);
	visibility_class = !visibility_class;
	notification_system_setting_set_visibility_class(system_setting, visibility_class);

	err = notification_system_setting_update_system_setting(system_setting);

	if (err != NOTIFICATION_ERROR_NONE || system_setting == NULL) {
		testapp_print("notification_system_setting_update_system_setting failed [%d]\n", err);
		goto out;
	}

out:
	if (system_setting)
		notification_system_setting_free_system_setting(system_setting);

	return err;
}

static gboolean testapp_interpret_command_setting_test (int selected_number)
{
	gboolean go_to_loop = TRUE;

	switch (selected_number) {
	case 1:
		testapp_test_get_setting_list();
		break;

	case 2:
		testapp_test_update_setting();
		break;

	case 3:
		testapp_test_update_system_setting();
		break;

	case 0:
		go_to_loop = FALSE;
		break;

	default:
		break;
	}

	return go_to_loop;

}

void testapp_setting_main()
{
	gboolean go_to_loop = TRUE;
	int menu_number = 0;

	while (go_to_loop) {
		testapp_show_menu (TESTAPP_MENU_TYPE_SETTING_TEST_MENU);
		testapp_show_prompt (TESTAPP_MENU_TYPE_SETTING_TEST_MENU);

		if (0 >= scanf("%d", &menu_number))
			testapp_print("Invalid input");

		go_to_loop = testapp_interpret_command_setting_test(menu_number);
	}
}
/* Setting Test } ---------------------------------------------------------------*/

/* Main { ---------------------------------------------------------------------*/
static gboolean testapp_interpret_command (int menu_number)
{
	gboolean go_to_loop = TRUE;

	switch (menu_number) {
	case 1:
		testapp_notification_main();
		break;

	case 2:
		testapp_setting_main();
		break;

	case 0:
		go_to_loop = FALSE;
		break;

	default:
		break;
	}

	return go_to_loop;
}

void testapp_show_prompt (testapp_menu_type_e menu)
{
	switch (menu) {
	case TESTAPP_MENU_TYPE_MAIN_MENU:
		testapp_print ("[MAIN]# ");
		break;

	case TESTAPP_MENU_TYPE_BASIC_TEST_MENU:
		testapp_print ("[BASIC_TEST]# ");
		break;

	case TESTAPP_MENU_TYPE_SETTING_TEST_MENU:
		testapp_print ("[SETTING_TEST]# ");
		break;
	}
}

int main (int argc, char *argv[])
{
	gboolean go_to_loop = TRUE;
	int menu_number = 0;

	if ( testapp_initialize_testing() == FALSE ) {
		testapp_print ("Initializing failed.\n");
		exit(0);
	}

	while (go_to_loop) {
		testapp_show_menu (TESTAPP_MENU_TYPE_MAIN_MENU);
		testapp_show_prompt (TESTAPP_MENU_TYPE_MAIN_MENU);
		if (0 >= scanf ("%d", &menu_number))
			testapp_print("Invalid input");
		go_to_loop = testapp_interpret_command (menu_number);
	}

	testapp_finalize_testing();

	exit(0);
}
/* Main } ---------------------------------------------------------------------*/

