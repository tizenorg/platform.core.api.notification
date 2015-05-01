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

/*-----------------------------------------------------------------------------------------*/
/* types */
typedef enum testapp_menu_type {
	TESTAPP_MENU_TYPE_MAIN_MENU = 1,
	TESTAPP_MENU_TYPE_BASIC_TEST_MENU = 2,
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


/* Main { ---------------------------------------------------------------------*/
static gboolean testapp_interpret_command (int menu_number)
{
	gboolean go_to_loop = TRUE;

	switch (menu_number) {
	case 1:
		testapp_notification_main();
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

