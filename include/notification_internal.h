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

#ifndef __NOTIFICATION_INTERNAL_H__
#define __NOTIFICATION_INTERNAL_H__

#include <notification_error.h>
#include <notification_type.h>
#include <notification_list.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup NOTIFICATION_INTERNAL
 * @{
 */

/**
 * @brief This function add deferred task. the registered task will be executed when notification service become ready
 * @param[in] deferred_task_cb The callback function
 * @param[in] user_data The user data to be passed to the callback function
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification_error_e
 * @see notification_is_service_ready()
 */
int notification_add_deferred_task(
		void (*deferred_task_cb)(void *data), void *user_data);

/**
 * @brief This function remove deferred task.
 * @param[in] deferred_task_cb The callback function
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification_error_e
 * @see notification_is_service_ready()
 */
int notification_del_deferred_task(
		void (*deferred_task_cb)(void *data));

/**
 * @brief This function will be removed.
 * @see notification_unresister_changed_cb()
 */
int
notification_resister_changed_cb(
	void (*changed_cb)(void *data, notification_type_e type),
	void *user_data);

/**
 * @brief This function will be removed.
 * @see notification_resister_changed_cb()
 */
int
notification_unresister_changed_cb(
	void (*changed_cb)(void *data, notification_type_e type));

/**
 * @brief Updates the progress of the inserted notification. This only works for the ongoing notification (NOTIFICATION_TYPE_ONGOING).
 * @details The Notification view on the notification area could be updated.
 * @param[in] noti Notification handle or NULL if priv_id is valid
 * @param[in] priv_id Private ID
 * @param[in] progress Percentage value of progressive data
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_progress(NULL, APP_NOTI_PRIV_ID, 0.6);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_update_progress(notification_h noti,
						  int priv_id,
						  double progress);

/**
 * @brief Updates the size of inserted notification data. This only works for the ongoing notification (NOTIFICATION_TYPE_ONGOING).
 * @details Notification view on notification area could be updated.
 * @param[in] noti Notification handle or NULL if priv_id is valid
 * @param[in] priv_id Private ID
 * @param[in] size Bytes of progressive data
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_size(NULL, APP_NOTI_PRIV_ID, 3000000);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_update_size(notification_h noti,
					      int priv_id, double size);

/**
 * @brief Updates the content of the inserted notification data. This is only for the ongoing notification (NOTIFICATION_TYPE_ONGOING).
 * @details Notification view on notification area could be updated.
 * @param[in] noti Notification handle or NULL if priv_id is valid
 * @param[in] priv_id Private ID
 * @param[in] content Text to update
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_update_content(NULL, APP_NOTI_PRIV_ID, "updated string");
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
int notification_update_content(notification_h noti,
							 int priv_id,
							 const char *content);

/**
 * @brief This function will be deprecated.
 * @see notification_set_image()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_set_icon(notification_h noti,
					   const char *icon_path);

/**
 * @brief This function will be deprecated.
 * @see notification_get_image()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_icon(notification_h noti,
					   char **icon_path);

/**
 * @brief This function will be deprecated.
 * @see notification_set_text()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_set_title(notification_h noti,
					    const char *title,
					    const char *loc_title);

/**
 * @brief This function will be deprecated.
 * @see notification_get_text()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_title(notification_h noti,
					    char **title,
					    char **loc_title);

/**
 * @brief This function will be deprecated.
 * @see notification_set_text()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_set_content(notification_h noti,
					      const char *content,
					      const char *loc_content);

/**
 * @brief This function will be deprecated.
 * @see notification_get_text()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_content(notification_h noti,
					      char **content,
					      char **loc_content);

/**
 * @brief This function will be removed.
 * @see notification_set_execute_option()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_set_application(notification_h noti, const char *pkgname);	/* Do not use this */

/**
 * @brief This function will be removed.
 * @see notification_get_execute_option()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_application(notification_h noti, char **pkgname);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_set_execute_option()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_set_args(notification_h noti, bundle * args, bundle * group_args);	/* Do not use this */

/**
 * @brief This function will be deprecated.
 * @see notification_get_execute_option()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_args(notification_h noti, bundle ** args, bundle ** group_args);	/* Do not use this */

/**
 * @brief This function is deprecated.
 * @see notification_get_grouping_list()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_grouping_list(notification_type_e type,
						    int count,
						    notification_list_h *list);

/**
 * @brief This function will be deprecated.
 * @see notification_delete_by_priv_id()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_delete_group_by_group_id(const char *pkgname,
							   notification_type_e type,
							   int group_id);

/**
 * @brief This function will be deprecated.
 * @see notification_delete_by_priv_id()
 *
 */
NOTIFICATION_DEPRECATED_API int notification_delete_group_by_priv_id(const char *pkgname,
							  notification_type_e type,
							  int priv_id);
/**
 * @brief This function will be deprecated.
 *
 */
NOTIFICATION_DEPRECATED_API int notification_get_count(notification_type_e type,
					    const char *pkgname,
					    int group_id, int priv_id,
					    int *count);

/**
 * @brief This function will be deprecated.
 *
 */
NOTIFICATION_DEPRECATED_API int notifiation_clear(notification_type_e type);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details Use only for the notification tray's clear button operation.
 * @param[in] type Notification type
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @see #notification_type_e
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_clear(NOTIFICATION_TYPE_NOTI);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_clear(notification_type_e type);

/**
 * @internal
 * @brief This function will be deprecated.
 * @return 1 on ready, other value on not ready
 */
NOTIFICATION_DEPRECATED_API int notification_is_service_ready(void);

/**
 * @internal
 * @brief This function will be deprecated.
 * @param[in] list Notification list handle
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre notification_get_grouping_list() or notification_get_detail_list().
 * @see #notification_op
 */
NOTIFICATION_DEPRECATED_API int notification_op_get_data(notification_op *noti_op,
						       notification_op_data_type_e type,
						       void *data);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details Caller_pkgname is set automatically when notification_new() is called. We do not recommend to use this API.
 * @param[in] noti Notification handle
 * @param[in] pkgname Caller package name
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}

	noti_err  = notification_set_pkgname(noti, "org.tizen.phone");
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_set_pkgname(notification_h noti,
					      const char *pkgname);

/**
 * @internal
 * @brief This function will be deprecated.
 * @param[in] noti Notification handle
 * @param[out] pkgname Caller package name
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *pkgname = NULL;

	noti_err  = notification_get_pkgname(noti, &pkgname);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_get_pkgname(notification_h noti,
					      char **pkgname);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details If @a pkgname is NULL, caller_pkgname is set internally.
 * @param[in] pkgname Caller application package name or NULL
 * @param[in] type Notification type
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_delete_all_by_type(NULL, NOTIFICATION_TYPE_NOTI);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_delete_all_by_type(const char *pkgname,
						     notification_type_e type);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details If @a pkgname is NULL, caller_pkgname is set internally.
 * @param[in] pkgname Caller application package name or NULL
 * @param[in] type Notification type
 * @param[in] priv_id Priv ID
 * @return NOTIFICATION_ERROR_NONE if success, other value if failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid input value
 * @par Sample code:
 * @code
#include <notification.h>
...
 {
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err  = notification_delete_by_priv_id(NULL, NOTIFICATION_TYPE_NOTI, APP_PRIV_ID);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_delete_by_priv_id(const char *pkgname,
						    notification_type_e type,
						    int priv_id);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details When notification data selected in display application, application launched by appsvc_run_service with service_handle.
 * @param[in] noti Notification handle
 * @param[in] type Notification execute type
 * @param[in] text Basic text for button
 * @param[in] key Value for localized text
 * @param[in] service_handle Appsvc bundle data
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	...

	b = bundle_create();
	appsvc_set_operation(b, APPSVC_OPERATION_VIEW);
	appsvc_set_uri(b,"http://www.samsung.com");

	noti_err  = notification_set_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, b);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}

	bundle_free(b);
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_set_execute_option(notification_h noti,
						     notification_execute_type_e type,
						     const char *text,
						     const char *key,
						     bundle *service_handle);

/**
 * @internal
 * @brief This function will be deprecated.
 * @remarks ID is valid only after inserting the notification.
 * @param[in] noti Notification handle
 * @param[out] group_id Group ID
 * @param[out] priv_id Private ID
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
 ...
  {
	 int noti_err = NOTIFICATION_ERROR_NONE;
	 int group_id, priv_id;
 
	 noti_err  = notification_get_id(noti, &group_id, &priv_id);
	 if(noti_err != NOTIFICATION_ERROR_NONE) {
		 return;
	 }
 }
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_get_id(notification_h noti,
					 int *group_id, int *priv_id);

/**
 * @internal
 * @brief This function will be deprecated.
 * @param[in] type Notification type
 * @param[in] group_id Group ID
 * @param[in] priv_id Priv ID
 * @return Notification handle(#notification_h) on success, NULL on failure
 * @retval #notification_h Success
 * @retval NULL Failure
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_load("org.tizen.app", priv_id);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API notification_h notification_load(char *pkgname,
				int priv_id);

/**
 * @internal
 * @brief This function will be deprecated.
 * @details Available types are #NOTIFICATION_TYPE_NOTI and #NOTIFICATION_TYPE_ONGOING.
 * #NOTIFICATION_TYPE_NOTI is remaining notification data even if device is restarted.
 * #NOTIFICATION_TYPE_ONGOING can display progressive feather, but notification data is removed after device is restarted.
 * If group_id is #NOTIFICATION_GROUP_ID_NONE, notification data is not grouped. #NOTIFICATION_GROUP_ID_DEFAULT,
 * notification data is grouped with same title. Positive number ( > 0 ) is grouped with same number.
 * If priv_id is #NOTIFICATION_PRIV_ID_NONE, priv_id is set internally and return it when notification_insert() call.
 * Positive number and zero ( >= 0 ) is application set private ID. These ID should have be unique each application package.
 * @param[in] type Notification type
 * @param[in] group_id Group ID
 * @param[in] priv_id Priv ID
 * @return Notification handle(#notification_h) on success, NULL on failure
 * @retval #notification_h Success
 * @retval NULL Failure
 * @see #notification_type_e
 * @see #notification_h
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;

	noti = notification_new(NOTIFICATION_TYPE_NOTI, APP_GROUP_ID, NOTIFICATION_PRIV_ID_NONE);
	if(noti == NULL) {
		return;
	}
	...
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API notification_h notification_new(notification_type_e type, int group_id,
				int priv_id);

/**
 * @internal
 * @brief This function will be deprecated.
 * @param[in] noti Notification handle
 * @param[in] type Notification execute type
 * @param[out] text Text for button
 * @param[out] service_handle Appsvc bundle data
 * @return NOTIFICATION_ERROR_NONE on success, other value on failure
 * @retval NOTIFICATION_ERROR_NONE Success
 * @retval NOTIFICATION_ERROR_INVALID_PARAMETER Invalid parameter
 * @par Sample code:
 * @code
#include <notification.h>
...
{
	notification_h noti = NULL;
	int noti_err = NOTIFICATION_ERROR_NONE;
	bundle *b = NULL;

	...

	noti_err  = notification_get_execute_option(noti, NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, &b);
	if(noti_err != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return;
	}
}
 * @endcode
 */
NOTIFICATION_DEPRECATED_API int notification_get_execute_option(notification_h noti,
						     notification_execute_type_e type,
						     const char **text,
						     bundle **service_handle);

/**
 * @internal
 * @brief This function sets notification property for designated package.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @privlevel public
 * @privilege http://tizen.org/privilege/core/notification
 * @param[in] pkgname The name of designated package
 * @param[in] property name
 * @param[in] property value
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification_error_e
 */
int notification_setting_property_set(const char *pkgname, const char *property, const char *value);

/**
 * @internal
 * @brief This function gets notification property for designated package.
 * @since_tizen @if WEARABLE 2.3.1 @elseif MOBILE 2.3 @endif
 * @privlevel public
 * @privilege http://tizen.org/privilege/core/notification
 * @param[in] pkgname The name of designated package
 * @param[in] property name
 * @param[in] pointer which can save the getting value
 * @return #NOTIFICATION_ERROR_NONE if success, other value if failure
 * @see #notification
 */
int notification_setting_property_get(const char *pkgname, const char *property, char **value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif
#endif
