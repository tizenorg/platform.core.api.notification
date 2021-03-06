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

#include <stdlib.h>

#include <notification.h>
#include <notification_list.h>
#include <notification_debug.h>
#include <notification_internal.h>

struct _notification_list {
	notification_list_h prev;
	notification_list_h next;

	notification_h noti;
};

notification_list_h _notification_list_create(void)
{
	notification_list_h list = NULL;

	list = (notification_list_h) malloc(sizeof(struct _notification_list));
	if (list == NULL) {
		NOTIFICATION_ERR("NO MEMORY");
		return NULL;
	}

	list->prev = NULL;
	list->next = NULL;

	list->noti = NULL;

	return list;
}

EXPORT_API notification_list_h notification_list_get_head(notification_list_h list)
{
	notification_list_h cur_list = NULL;

	if (list == NULL) {
		NOTIFICATION_ERR("INVALID DATA : list == NULL");
		return NULL;
	}

	cur_list = list;

	while (cur_list->prev != NULL) {
		cur_list = cur_list->prev;
	}

	return cur_list;
}

EXPORT_API notification_list_h notification_list_get_tail(notification_list_h list)
{
	notification_list_h cur_list = NULL;

	if (list == NULL) {
		NOTIFICATION_ERR("INVALID DATA : list == NULL");
		return NULL;
	}

	cur_list = list;

	while (cur_list->next != NULL) {
		cur_list = cur_list->next;
	}

	return cur_list;
}

EXPORT_API notification_list_h notification_list_get_prev(notification_list_h list)
{
	notification_list_h cur_list = NULL;

	if (list == NULL) {
		NOTIFICATION_ERR("INVALID DATA : list == NULL");
		return NULL;
	}

	cur_list = list;

	return cur_list->prev;
}

EXPORT_API notification_list_h notification_list_get_next(notification_list_h list)
{
	notification_list_h cur_list = NULL;

	if (list == NULL) {
		NOTIFICATION_ERR("INVALID DATA : list == NULL");
		return NULL;
	}

	cur_list = list;

	return cur_list->next;
}

EXPORT_API notification_h notification_list_get_data(notification_list_h list)
{
	notification_list_h cur_list = NULL;

	if (list == NULL) {
		NOTIFICATION_ERR("INVALID DATA : list == NULL");
		return NULL;
	}

	cur_list = list;

	return cur_list->noti;
}

EXPORT_API notification_list_h notification_list_append(notification_list_h list,
							notification_h noti)
{
	notification_list_h new_list = NULL;
	notification_list_h cur_list = NULL;

	if (noti == NULL) {
		NOTIFICATION_ERR("INVALID DATA : data == NULL");
		return NULL;
	}

	if (list != NULL) {
		cur_list = notification_list_get_tail(list);

		new_list = _notification_list_create();
		if (new_list == NULL) {
			NOTIFICATION_ERR("NO MEMORY");
			return NULL;
		}

		cur_list->next = new_list;
		new_list->prev = cur_list;

		new_list->noti = noti;
	} else {
		cur_list = _notification_list_create();
		if (cur_list == NULL) {
			NOTIFICATION_ERR("NO MEMORY");
			return NULL;
		}

		new_list = cur_list;
		new_list->noti = noti;
	}

	return new_list;
}

EXPORT_API notification_list_h notification_list_remove(notification_list_h list,
							notification_h noti)
{
	notification_list_h cur_list = NULL;
	notification_list_h prev_list = NULL;
	notification_list_h next_list = NULL;

	cur_list = notification_list_get_head(list);
	while (cur_list != NULL) {
		if (cur_list->noti == noti) {
			//remove
			prev_list = cur_list->prev;
			next_list = cur_list->next;

			if (next_list != NULL) {
				if (prev_list != NULL) {
					prev_list->next = next_list;
					next_list->prev = prev_list;
				} else {
					next_list->prev = NULL;
				}
			} else {
				if (prev_list != NULL) {
					prev_list->next = NULL;
				}
			}

			free(cur_list);
			break;
		}

		cur_list = cur_list->next;
	}

	if (prev_list != NULL) {
		return notification_list_get_head(prev_list);
	} else if (next_list != NULL) {
		return next_list;
	}

	return NULL;
}
