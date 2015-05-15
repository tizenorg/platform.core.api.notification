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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <libintl.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>

#include <app.h>
#include <app_control_internal.h>
#include <aul.h>
#include <ail.h>
#include <appsvc.h>
#include <tizen.h>
#include <vconf-keys.h>
#include <vconf.h>

#include <notification.h>
#include <notification_list.h>
#include <notification_debug.h>
#include <notification_private.h>
#include <notification_noti.h>
#include <notification_ongoing.h>
#include <notification_group.h>
#include <notification_ipc.h>
#include <notification_internal.h>

typedef struct _notification_cb_list notification_cb_list_s;

typedef enum __notification_cb_type {
	NOTIFICATION_CB_NORMAL = 1,
	NOTIFICATION_CB_DETAILED,
} _notification_cb_type_e;

struct _notification_cb_list {
	notification_cb_list_s *prev;
	notification_cb_list_s *next;

	_notification_cb_type_e cb_type;
	void (*changed_cb) (void *data, notification_type_e type);
	void (*detailed_changed_cb) (void *data, notification_type_e type, notification_op *op_list, int num_op);
	void *data;
};

static notification_cb_list_s *g_notification_cb_list = NULL;

static void (*posted_toast_message_cb) (void *data);

#define NOTI_TEXT_RESULT_LEN 2048
#define NOTI_PKGNAME_LEN	512

static char *_notification_get_pkgname_by_pid(void)
{
	char pkgname[NOTI_PKGNAME_LEN + 1] = { 0, };
	int pid = 0, ret = AUL_R_OK;
	int fd;
	char  *dup_pkgname;

	pid = getpid();

	ret = aul_app_get_pkgname_bypid(pid, pkgname, sizeof(pkgname));
	if (ret != AUL_R_OK) {
		char buf[NOTI_PKGNAME_LEN + 1] = { 0, };

		snprintf(buf, sizeof(buf), "/proc/%d/cmdline", pid);

		fd = open(buf, O_RDONLY);
		if (fd < 0) {
			return NULL;
		}

		ret = read(fd, pkgname, sizeof(pkgname) - 1);
		close(fd);

		if (ret <= 0) {
			return NULL;
		}

		pkgname[ret] = '\0';
		/*!
		 * \NOTE
		 * "ret" is not able to be larger than "sizeof(pkgname) - 1",
		 * if the system is not going wrong.
		 */
	} else {
		if (strlen(pkgname) <= 0) {
			return NULL;
		}
	}

	dup_pkgname = strdup(pkgname);
	if (!dup_pkgname)
		NOTIFICATION_ERR("Heap: %d\n", errno);

	return dup_pkgname;
}

static void _notification_get_text_domain(notification_h noti)
{
	if (noti->domain != NULL) {

	}

	if (noti->dir != NULL) {

	}
}

EXPORT_API int notification_set_image(notification_h noti,
						       notification_image_type_e type,
						       const char *image_path)
{
	bundle *b = NULL;
	char buf_key[32] = { 0, };
	const char *ret_val = NULL;

	/* Check noti and image_path are valid data */
	if (noti == NULL || image_path == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check image type is valid type */
	if (type <= NOTIFICATION_IMAGE_TYPE_NONE
	    || type >= NOTIFICATION_IMAGE_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check image path bundle is exist */
	if (noti->b_image_path) {
		/* If image path bundle is exist, store local bundle value */
		b = noti->b_image_path;

		/* Set image type to key as char string type */
		snprintf(buf_key, sizeof(buf_key), "%d", type);

		/* Get value using key */
		ret_val = bundle_get_val(b, buf_key);
		if (ret_val != NULL) {
			/* If key is exist, remove this value to store new image path */
			bundle_del(b, buf_key);
		}

		/* Add new image path with type key */
		bundle_add(b, buf_key, image_path);
	} else {
		/* If image path bundle is not exist, create new one */
		b = bundle_create();

		/* Set image type to key as char string type */
		snprintf(buf_key, sizeof(buf_key), "%d", type);

		/* Add new image path with type key */
		bundle_add(b, buf_key, image_path);

		/* Save to image path bundle */
		noti->b_image_path = b;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_image(notification_h noti,
						       notification_image_type_e type,
						       char **image_path)
{
	bundle *b = NULL;
	char buf_key[32] = { 0, };
	const char *ret_val = NULL;

	/* Check noti and image_path is valid data */
	if (noti == NULL || image_path == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check image type is valid data */
	if (type <= NOTIFICATION_IMAGE_TYPE_NONE
	    || type >= NOTIFICATION_IMAGE_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check image path bundle exist */
	if (noti->b_image_path) {
		/* If image path bundle exist, store local bundle data */
		b = noti->b_image_path;

		/* Set image type to key as char string type */
		snprintf(buf_key, sizeof(buf_key), "%d", type);

		/* Get value of key */
		ret_val = bundle_get_val(b, buf_key);

		*image_path = (char *)ret_val;
	} else {
		/* If image path bundle does not exist, image path is NULL */
		*image_path = NULL;
	}

	/* If image path is NULL and type is ICON, icon path set from AIL */
	/* order : user icon -> launch_pkgname icon -> caller_pkgname icon -> service app icon */
	if (*image_path == NULL && type == NOTIFICATION_IMAGE_TYPE_ICON) {
		/* Check App icon path is already set */
		if (noti->app_icon_path != NULL) {
			/* image path will be app icon path */
			*image_path = noti->app_icon_path;
		} else {
			*image_path = NULL;
		}
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_time(notification_h noti,
						      time_t input_time)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (input_time == 0) {
		/* If input time is 0, set current time */
		noti->time = time(NULL);
	} else {
		/* save input time */
		noti->time = input_time;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_time(notification_h noti,
						      time_t * ret_time)
{
	/* Check noti and time is valid data */
	if (noti == NULL || ret_time == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set time infomation */
	*ret_time = noti->time;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_insert_time(notification_h noti,
							     time_t * ret_time)
{
	/* Check noti and ret_time is valid data */
	if (noti == NULL || ret_time == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set insert time information */
	*ret_time = noti->insert_time;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_text(notification_h noti,
						      notification_text_type_e type,
						      const char *text,
						      const char *key,
						      int args_type, ...)
{
	bundle *b = NULL;
	char buf_key[32] = { 0, };
	char buf_val[1024] = { 0, };
	const char *ret_val = NULL;
	va_list var_args;
	notification_variable_type_e var_type;
	int num_args = 0;
	int noti_err = NOTIFICATION_ERROR_NONE;
	int var_value_int = 0;
	double var_value_double = 0.0;
	char *var_value_string = NULL;
	notification_count_pos_type_e var_value_count =
	    NOTIFICATION_COUNT_POS_NONE;

	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check text type is valid type */
	if (type <= NOTIFICATION_TEXT_TYPE_NONE
	    || type >= NOTIFICATION_TEXT_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check text bundle exist */
	if (text != NULL) {
		if (noti->b_text != NULL) {
			/* If text bundle exist, store local bundle data */
			b = noti->b_text;

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			/* Get value using type key */
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				/* If value exist, remove this to add new value */
				bundle_del(b, buf_key);
			}

			snprintf(buf_val, sizeof(buf_val), "%s", text);

			/* Add new text value */
			bundle_add(b, buf_key, buf_val);
		} else {
			/* If text bundle does not exist, create new one */
			b = bundle_create();

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			snprintf(buf_val, sizeof(buf_val), "%s", text);

			/* Add new text value */
			bundle_add(b, buf_key, buf_val);

			/* Save text bundle */
			noti->b_text = b;
		}
	} else {
		/* Reset if text is NULL */
		if (noti->b_text != NULL) {
			/* If text bundle exist, store local bundle data */
			b = noti->b_text;

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			/* Get value using type key */
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				/* If value exist, remove this */
				bundle_del(b, buf_key);
			}
		}
	}

	/* Save key if key is valid data */
	if (key != NULL) {
		/* Check key bundle exist */
		if (noti->b_key != NULL) {
			/* If key bundle exist,  store local bundle data */
			b = noti->b_key;

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			/* Get value using type key */
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				/* If value exist, remove this to add new value */
				bundle_del(b, buf_key);
			}

			snprintf(buf_val, sizeof(buf_val), "%s", key);

			/* Add new key value */
			bundle_add(b, buf_key, buf_val);
		} else {
			/* If key bundle does not exist, create new one */
			b = bundle_create();

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			snprintf(buf_val, sizeof(buf_val), "%s", key);

			/* Add new key value */
			bundle_add(b, buf_key, buf_val);

			/* Save key bundle */
			noti->b_key = b;
		}
	} else {
		/* Reset if key is NULL */
		if (noti->b_key != NULL) {
			/* If key bundle exist,  store local bundle data */
			b = noti->b_key;

			/* Make type to key as char string */
			snprintf(buf_key, sizeof(buf_key), "%d", type);

			/* Get value using type key */
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				/* If value exist, remove this */
				bundle_del(b, buf_key);
			}
		}
	}

	if (noti->b_format_args != NULL) {
		b = noti->b_format_args;
	} else {
		b = bundle_create();
	}

	va_start(var_args, args_type);

	var_type = args_type;
	num_args = 0;

	while (var_type != NOTIFICATION_VARIABLE_TYPE_NONE) {
		/* Type */
		snprintf(buf_key, sizeof(buf_key), "%dtype%d", type, num_args);
		snprintf(buf_val, sizeof(buf_val), "%d", var_type);

		ret_val = bundle_get_val(b, buf_key);
		if (ret_val != NULL) {
			bundle_del(b, buf_key);
		}

		bundle_add(b, buf_key, buf_val);

		switch (var_type) {
		case NOTIFICATION_VARIABLE_TYPE_INT:
			var_value_int = va_arg(var_args, int);

			/* Value */
			snprintf(buf_key, sizeof(buf_key), "%dvalue%d", type,
				 num_args);
			snprintf(buf_val, sizeof(buf_val), "%d", var_value_int);

			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				bundle_del(b, buf_key);
			}

			bundle_add(b, buf_key, buf_val);
			break;
		case NOTIFICATION_VARIABLE_TYPE_DOUBLE:
			var_value_double = va_arg(var_args, double);

			/* Value */
			snprintf(buf_key, sizeof(buf_key), "%dvalue%d", type,
				 num_args);
			snprintf(buf_val, sizeof(buf_val), "%.2f",
				 var_value_double);

			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				bundle_del(b, buf_key);
			}

			bundle_add(b, buf_key, buf_val);
			break;
		case NOTIFICATION_VARIABLE_TYPE_STRING:
			var_value_string = va_arg(var_args, char *);

			/* Value */
			snprintf(buf_key, sizeof(buf_key), "%dvalue%d", type,
				 num_args);
			snprintf(buf_val, sizeof(buf_val), "%s",
				 var_value_string);

			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				bundle_del(b, buf_key);
			}

			bundle_add(b, buf_key, buf_val);
			break;
		case NOTIFICATION_VARIABLE_TYPE_COUNT:
			var_value_count =
			    va_arg(var_args, notification_count_pos_type_e);

			/* Value */
			snprintf(buf_key, sizeof(buf_key), "%dvalue%d", type,
				 num_args);
			snprintf(buf_val, sizeof(buf_val), "%d",
				 var_value_count);

			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				bundle_del(b, buf_key);
			}

			bundle_add(b, buf_key, buf_val);
			break;
		default:
			NOTIFICATION_ERR("Error. invalid variable type. : %d",
					 var_type);
			noti_err = NOTIFICATION_ERROR_INVALID_PARAMETER;
			break;
		}

		num_args++;
		var_type = va_arg(var_args, notification_variable_type_e);
	}
	va_end(var_args);

	if (noti_err == NOTIFICATION_ERROR_NONE) {
		noti->num_format_args = num_args;
	} else {
		noti->num_format_args = 0;
	}

	snprintf(buf_key, sizeof(buf_key), "num%d", type);
	snprintf(buf_val, sizeof(buf_val), "%d", noti->num_format_args);

	ret_val = bundle_get_val(b, buf_key);
	if (ret_val != NULL) {
		bundle_del(b, buf_key);
	}

	bundle_add(b, buf_key, buf_val);

	noti->b_format_args = b;

	return noti_err;
}

EXPORT_API int notification_get_text(notification_h noti,
						      notification_text_type_e type,
						      char **text)
{
	bundle *b = NULL;
	char buf_key[32] = { 0, };
	const char *ret_val = NULL;
	const char *get_str = NULL;
	const char *get_check_type_str = NULL;
	notification_text_type_e check_type = NOTIFICATION_TEXT_TYPE_NONE;
	//int display_option_flag = 0;

	char *temp_str = NULL;
	char result_str[NOTI_TEXT_RESULT_LEN] = { 0, };
	char buf_str[1024] = { 0, };
	int num_args = 0;
	notification_variable_type_e ret_var_type = 0;
	int ret_variable_int = 0;
	double ret_variable_double = 0.0;

	/* Check noti is valid data */
	if (noti == NULL || text == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check text type is valid type */
	if (type <= NOTIFICATION_TEXT_TYPE_NONE
	    || type >= NOTIFICATION_TEXT_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check key */
	if (noti->b_key != NULL) {
		b = noti->b_key;

		/* Get text domain and dir */
		//_notification_get_text_domain(noti);

		snprintf(buf_key, sizeof(buf_key), "%d", type);

		ret_val = bundle_get_val(b, buf_key);
		if (ret_val != NULL && noti->domain != NULL
		    && noti->dir != NULL) {
			/* Get application string */
			bindtextdomain(noti->domain, noti->dir);

			get_str = dgettext(noti->domain, ret_val);
		} else if (ret_val != NULL) {
			/* Get system string */
			get_str = dgettext("sys_string", ret_val);
		} else {
			get_str = NULL;
		}
	}

	if (get_str == NULL && noti->b_text != NULL) {
		b = noti->b_text;
		/* Get basic text */
		snprintf(buf_key, sizeof(buf_key), "%d", type);

		get_str = bundle_get_val(b, buf_key);
	}

	check_type = type;

	/* Set display option is off type when option is off, type is noti */
	/*if (get_str != NULL && display_option_flag == 1
	    && noti->type == NOTIFICATION_TYPE_NOTI) {
		if (type == NOTIFICATION_TEXT_TYPE_CONTENT
		    || type == NOTIFICATION_TEXT_TYPE_GROUP_CONTENT) {
			// Set check_type to option content string 
			if (type == NOTIFICATION_TEXT_TYPE_CONTENT) {
				check_type =
				    NOTIFICATION_TEXT_TYPE_CONTENT_FOR_DISPLAY_OPTION_IS_OFF;
			} else if (type == NOTIFICATION_TEXT_TYPE_GROUP_CONTENT) {
				check_type =
				    NOTIFICATION_TEXT_TYPE_GROUP_CONTENT_FOR_DISPLAY_OPTION_IS_OFF;
			}

			// Check key 
			if (noti->b_key != NULL) {
				b = noti->b_key;

				// Get text domain and dir 
				_notification_get_text_domain(noti);

				snprintf(buf_key, sizeof(buf_key), "%d",
					 check_type);

				ret_val = bundle_get_val(b, buf_key);
				if (ret_val != NULL && noti->domain != NULL
				    && noti->dir != NULL) {
					// Get application string 
					bindtextdomain(noti->domain, noti->dir);

					get_check_type_str =
					    dgettext(noti->domain, ret_val);
				} else if (ret_val != NULL) {
					// Get system string 
					get_check_type_str =
					    dgettext("sys_string", ret_val);
				} else {
					get_check_type_str = NULL;
				}
			}

			if (get_check_type_str == NULL && noti->b_text != NULL) {
				b = noti->b_text;
				// Get basic text 
				snprintf(buf_key, sizeof(buf_key), "%d",
					 check_type);

				get_check_type_str = bundle_get_val(b, buf_key);
			}
		}

		if (get_check_type_str != NULL) {
			// Replace option off type string 
			get_str = get_check_type_str;
		} else {
			// Set default string 
			get_str =
			    dgettext("sys_string", "IDS_COM_POP_MISSED_EVENT");
		}
	}*/

	if (get_str != NULL) {
		/* Get number format args */
		b = noti->b_format_args;
		noti->num_format_args = 0;

		if (b != NULL) {
			snprintf(buf_key, sizeof(buf_key), "num%d", check_type);
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL) {
				noti->num_format_args = atoi(ret_val);
			}
		}

		if (noti->num_format_args == 0) {
			*text = (char *)get_str;
		} else {
			/* Check first variable is count, LEFT pos */
			snprintf(buf_key, sizeof(buf_key), "%dtype%d",
				 check_type, num_args);
			ret_val = bundle_get_val(b, buf_key);
			ret_var_type = atoi(ret_val);

			if (ret_var_type == NOTIFICATION_VARIABLE_TYPE_COUNT) {
				/* Get var Value */
				snprintf(buf_key, sizeof(buf_key), "%dvalue%d",
					 check_type, num_args);
				ret_val = bundle_get_val(b, buf_key);
				ret_variable_int = atoi(ret_val);

				if (ret_variable_int ==
				    NOTIFICATION_COUNT_POS_LEFT) {
					notification_noti_get_count(noti->type,
								    noti->caller_pkgname,
								    noti->group_id,
								    noti->priv_id,
								    &ret_variable_int);
					snprintf(buf_str, sizeof(buf_str),
						 "%d ", ret_variable_int);

					int src_len = strlen(result_str);
					int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

					strncat(result_str, buf_str,
							max_len);

					num_args++;
				}

			}

			/* Check variable IN pos */
			for (temp_str = (char *)get_str; *temp_str != '\0';
			     temp_str++) {
				if (*temp_str != '%') {
					strncat(result_str, temp_str, 1);
				} else {
					if (*(temp_str + 1) == '%') {
						strncat(result_str, temp_str,
							1);
					} else if (*(temp_str + 1) == 'd') {
						/* Get var Type */
						ret_variable_int = 0;

						snprintf(buf_key,
							 sizeof(buf_key),
							 "%dtype%d", check_type,
							 num_args);
						ret_val =
						    bundle_get_val(b, buf_key);
						ret_var_type = atoi(ret_val);
						if (ret_var_type ==
						    NOTIFICATION_VARIABLE_TYPE_COUNT)
						{
							/* Get notification count */
							notification_noti_get_count
							    (noti->type,
							     noti->caller_pkgname,
							     noti->group_id,
							     noti->priv_id,
							     &ret_variable_int);
						} else {
							/* Get var Value */
							snprintf(buf_key,
								 sizeof
								 (buf_key),
								 "%dvalue%d",
								 check_type,
								 num_args);
							ret_val =
							    bundle_get_val(b,
									   buf_key);
							ret_variable_int =
							    atoi(ret_val);
						}

						snprintf(buf_str,
							 sizeof(buf_str), "%d",
							 ret_variable_int);

						int src_len = strlen(result_str);
						int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

						strncat(result_str, buf_str,
								max_len);

						temp_str++;

						num_args++;
					} else if (*(temp_str + 1) == 's') {
						/* Get var Value */
						snprintf(buf_key,
							 sizeof(buf_key),
							 "%dvalue%d",
							 check_type, num_args);
						ret_val =
						    bundle_get_val(b, buf_key);

						snprintf(buf_str,
							 sizeof(buf_str), "%s",
							 ret_val);

						int src_len = strlen(result_str);
						int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

						strncat(result_str, buf_str,
								max_len);

						temp_str++;

						num_args++;
					} else if (*(temp_str + 1) == 'f') {
						/* Get var Value */
						snprintf(buf_key,
							 sizeof(buf_key),
							 "%dvalue%d",
							 check_type, num_args);
						ret_val =
						    bundle_get_val(b, buf_key);
						ret_variable_double =
						    atof(ret_val);

						snprintf(buf_str,
							 sizeof(buf_str),
							 "%.2f",
							 ret_variable_double);

						int src_len = strlen(result_str);
						int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

						strncat(result_str, buf_str,
								max_len);

						temp_str++;

						num_args++;
					} else if (*(temp_str + 1) >= '1' && *(temp_str + 1) <= '9') {
						if (*(temp_str + 3) == 'd') {
							/* Get var Type */
							ret_variable_int = 0;

							snprintf(buf_key,
								 sizeof(buf_key),
								 "%dtype%d", check_type,
								 num_args + *(temp_str + 1) - 49);
							ret_val =
							    bundle_get_val(b, buf_key);
							ret_var_type = atoi(ret_val);
							if (ret_var_type ==
							    NOTIFICATION_VARIABLE_TYPE_COUNT)
							{
								/* Get notification count */
								notification_noti_get_count
								    (noti->type,
								     noti->caller_pkgname,
								     noti->group_id,
								     noti->priv_id,
								     &ret_variable_int);
							} else {
								/* Get var Value */
								snprintf(buf_key,
									 sizeof
									 (buf_key),
									 "%dvalue%d",
									 check_type,
									 num_args + *(temp_str + 1) - 49);
								ret_val =
								    bundle_get_val(b,
										   buf_key);
								ret_variable_int =
								    atoi(ret_val);
							}

							snprintf(buf_str,
								 sizeof(buf_str), "%d",
								 ret_variable_int);

							int src_len = strlen(result_str);
							int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

							strncat(result_str, buf_str,
									max_len);

							temp_str += 3;
						} else if (*(temp_str + 3) == 's') {
							/* Get var Value */
							snprintf(buf_key,
								 sizeof(buf_key),
								 "%dvalue%d",
								 check_type, num_args + *(temp_str + 1) - 49);
							ret_val =
							    bundle_get_val(b, buf_key);

							snprintf(buf_str,
								 sizeof(buf_str), "%s",
								 ret_val);

							int src_len = strlen(result_str);
							int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

							strncat(result_str, buf_str,
									max_len);

							temp_str += 3;
						} else if (*(temp_str + 3) == 'f') {
							/* Get var Value */
							snprintf(buf_key,
								 sizeof(buf_key),
								 "%dvalue%d",
								 check_type, num_args + *(temp_str + 1) - 49);
							ret_val =
							    bundle_get_val(b, buf_key);
							ret_variable_double =
							    atof(ret_val);

							snprintf(buf_str,
								 sizeof(buf_str),
								 "%.2f",
								 ret_variable_double);

							int src_len = strlen(result_str);
							int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

							strncat(result_str, buf_str,
									max_len);

							temp_str += 3;
						}
					}
				}

			}

			/* Check last variable is count, LEFT pos */
			if (num_args < noti->num_format_args) {
				snprintf(buf_key, sizeof(buf_key), "%dtype%d",
					 check_type, num_args);
				ret_val = bundle_get_val(b, buf_key);
				ret_var_type = atoi(ret_val);
				if (ret_var_type ==
				    NOTIFICATION_VARIABLE_TYPE_COUNT) {
					/* Get var Value */
					snprintf(buf_key, sizeof(buf_key),
						 "%dvalue%d", check_type,
						 num_args);
					ret_val = bundle_get_val(b, buf_key);
					ret_variable_int = atoi(ret_val);

					if (ret_variable_int ==
					    NOTIFICATION_COUNT_POS_RIGHT) {
						notification_noti_get_count
						    (noti->type,
						     noti->caller_pkgname,
						     noti->group_id,
						     noti->priv_id,
						     &ret_variable_int);
						snprintf(buf_str,
							 sizeof(buf_str), " %d",
							 ret_variable_int);

						int src_len = strlen(result_str);
						int max_len = NOTI_TEXT_RESULT_LEN - src_len - 1;

						strncat(result_str, buf_str,
								max_len);

						num_args++;
					}

				}
			}

			switch (check_type) {
			case NOTIFICATION_TEXT_TYPE_TITLE:
			case NOTIFICATION_TEXT_TYPE_GROUP_TITLE:
				if (noti->temp_title != NULL)
					free(noti->temp_title);

				noti->temp_title = strdup(result_str);

				*text = noti->temp_title;
				break;
			case NOTIFICATION_TEXT_TYPE_CONTENT:
			case NOTIFICATION_TEXT_TYPE_CONTENT_FOR_DISPLAY_OPTION_IS_OFF:
			case NOTIFICATION_TEXT_TYPE_GROUP_CONTENT:
			case NOTIFICATION_TEXT_TYPE_GROUP_CONTENT_FOR_DISPLAY_OPTION_IS_OFF:
				if (noti->temp_content !=
				    NULL)
					free(noti->temp_content);

				noti->temp_content = strdup(result_str);

				*text = noti->temp_content;
				break;
			default:
				break;
			}

		}

	} else {
		*text = NULL;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_text_domain(notification_h noti,
							     const char *domain,
							     const char *dir)
{
	/* check noti and domain is valid data */
	if (noti == NULL || domain == NULL || dir == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check domain */
	if (noti->domain) {
		/* Remove previous domain */
		free(noti->domain);
	}
	/* Copy domain */
	noti->domain = strdup(domain);

	/* Check locale dir */
	if (noti->dir) {
		/* Remove previous locale dir */
		free(noti->dir);
	}
	/* Copy locale dir */
	noti->dir = strdup(dir);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_text_domain(notification_h noti,
							     char **domain,
							     char **dir)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Get domain */
	if (domain != NULL && noti->domain != NULL) {
		*domain = noti->domain;
	}

	/* Get locale dir */
	if (dir != NULL && noti->dir != NULL) {
		*dir = noti->dir;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_time_to_text(notification_h noti, notification_text_type_e type,
								time_t time)
{
	int ret = NOTIFICATION_ERROR_NONE;
	char buf[256] = { 0, };
	char buf_tag[512] = { 0, };

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (time <= 0) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (type <= NOTIFICATION_TEXT_TYPE_NONE
	    || type >= NOTIFICATION_TEXT_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}


	snprintf(buf, sizeof(buf), "%lu", time);
	ret = notification_noti_set_tag(TAG_TIME, buf, buf_tag, sizeof(buf_tag));

	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	return notification_set_text(noti, type, buf_tag, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
}

EXPORT_API int notification_get_time_from_text(notification_h noti, notification_text_type_e type,
								time_t *time)
{
	int ret = NOTIFICATION_ERROR_NONE;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (time == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (type <= NOTIFICATION_TEXT_TYPE_NONE
	    || type >= NOTIFICATION_TEXT_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	char *ret_text = NULL;
	ret = notification_get_text(noti, type, &ret_text);

	if (ret != NOTIFICATION_ERROR_NONE || ret_text == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (notification_noti_get_tag_type(ret_text) == TAG_TYPE_INVALID) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	char *tag_value = NULL;
	tag_value = notification_noti_strip_tag(ret_text);
	if (tag_value == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	*time = atol(tag_value);
	free(tag_value);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_sound(notification_h noti,
						       notification_sound_type_e type,
						       const char *path)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check type is valid */
	if (type < NOTIFICATION_SOUND_TYPE_NONE
	    || type >= NOTIFICATION_SOUND_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save sound type */
	noti->sound_type = type;

	/* Save sound path if user data type */
	if (type == NOTIFICATION_SOUND_TYPE_USER_DATA && path != NULL) {
		if (noti->sound_path != NULL) {
			free(noti->sound_path);
		}

		noti->sound_path = strdup(path);
	} else {
		if (noti->sound_path != NULL) {
			free(noti->sound_path);
			noti->sound_path = NULL;
		}
		if (type == NOTIFICATION_SOUND_TYPE_USER_DATA) {
			noti->sound_type = NOTIFICATION_SOUND_TYPE_DEFAULT;
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		}
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_sound(notification_h noti,
						       notification_sound_type_e *type,
						       const char **path)
{
	/* check noti and type is valid data */
	if (noti == NULL || type == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set sound type */
	*type = noti->sound_type;

	/* Set sound path if user data type */
	if (noti->sound_type == NOTIFICATION_SOUND_TYPE_USER_DATA
	    && path != NULL) {
		*path = noti->sound_path;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_vibration(notification_h noti,
							   notification_vibration_type_e type,
							   const char *path)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check type is valid */
	if (type < NOTIFICATION_VIBRATION_TYPE_NONE
	    || type >= NOTIFICATION_VIBRATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save vibration type */
	noti->vibration_type = type;

	/* Save sound path if user data type */
	if (type == NOTIFICATION_VIBRATION_TYPE_USER_DATA && path != NULL) {
		if (noti->vibration_path != NULL) {
			free(noti->vibration_path);
		}

		noti->vibration_path = strdup(path);
	} else {
		if (noti->vibration_path != NULL) {
			free(noti->vibration_path);
			noti->vibration_path = NULL;
		}
		if (type == NOTIFICATION_VIBRATION_TYPE_USER_DATA) {
			noti->vibration_type = NOTIFICATION_VIBRATION_TYPE_DEFAULT;
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		}
	}

	return NOTIFICATION_ERROR_NONE;

}

EXPORT_API int notification_get_vibration(notification_h noti,
							   notification_vibration_type_e *type,
							   const char **path)
{
	/* check noti and type is valid data */
	if (noti == NULL || type == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set vibration type */
	*type = noti->vibration_type;

	/* Set sound path if user data type */
	if (noti->vibration_type == NOTIFICATION_VIBRATION_TYPE_USER_DATA
	    && path != NULL) {
		*path = noti->vibration_path;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_led(notification_h noti,
							   notification_led_op_e operation,
							   int led_argb)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check operation is valid */
	if (operation < NOTIFICATION_LED_OP_OFF
	    || operation >= NOTIFICATION_LED_OP_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save led operation */
	noti->led_operation = operation;

	/* Save led argb if operation is turning on LED with custom color */
	if (operation == NOTIFICATION_LED_OP_ON_CUSTOM_COLOR) {
		noti->led_argb = led_argb;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_led(notification_h noti,
							   notification_led_op_e *operation,
							   int *led_argb)
{
	/* check noti and operation is valid data */
	if (noti == NULL || operation == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set led operation */
	*operation = noti->led_operation;

	/* Save led argb if operation is turning on LED with custom color */
	if (noti->led_operation == NOTIFICATION_LED_OP_ON_CUSTOM_COLOR
	    && led_argb != NULL) {
		*led_argb = noti->led_argb;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_led_time_period(notification_h noti,
									int on_ms, int off_ms)
{
	/* Check noti is valid data */
	if (noti == NULL || on_ms < 0 || off_ms < 0) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save led operation */
	noti->led_on_ms = on_ms;
	noti->led_off_ms = off_ms;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_led_time_period(notification_h noti,
									int *on_ms, int *off_ms)
{
	/* check noti and operation is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (on_ms)
		*(on_ms) = noti->led_on_ms;
	if (off_ms)
		*(off_ms) = noti->led_off_ms;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_application(notification_h noti,
							     const char *pkgname)
{
	if (noti == NULL || pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (noti->launch_pkgname) {
		free(noti->launch_pkgname);
	}

	noti->launch_pkgname = strdup(pkgname);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_application(notification_h noti,
							     char **pkgname)
{
	if (noti == NULL || pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (noti->launch_pkgname) {
		*pkgname = noti->launch_pkgname;
	} else {
		*pkgname = noti->caller_pkgname;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_launch_option(notification_h noti,
								notification_launch_option_type type, void *option)
{
	int ret = 0;
	bundle *b = NULL;
	app_control_h app_control = option;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (app_control == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (type != NOTIFICATION_LAUNCH_OPTION_APP_CONTROL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if ((ret = app_control_to_bundle(app_control, &b)) == APP_CONTROL_ERROR_NONE) {
		return notification_set_execute_option(noti,
				NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH,
				NULL, NULL,
				b);
	} else {
		NOTIFICATION_ERR("Failed to convert appcontrol to bundle:%d", ret);
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
}

EXPORT_API int notification_get_launch_option(notification_h noti,
								notification_launch_option_type type, void *option)
{
	int ret = 0;
	bundle *b = NULL;
	app_control_h *app_control = (app_control_h *)option;
	app_control_h app_control_new = NULL;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (app_control == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (type != NOTIFICATION_LAUNCH_OPTION_APP_CONTROL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_get_execute_option(noti,
				NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH,
				NULL,
				&b);
	if (ret == NOTIFICATION_ERROR_NONE && b != NULL) {
		ret = app_control_create(&app_control_new);
		if (ret == APP_CONTROL_ERROR_NONE && app_control_new != NULL) {
			ret = app_control_import_from_bundle(app_control_new, b);
			if (ret == APP_CONTROL_ERROR_NONE) {
				*app_control = app_control_new;
			} else {
				app_control_destroy(app_control_new);
				NOTIFICATION_ERR("Failed to import app control from bundle:%d", ret);
				return NOTIFICATION_ERROR_IO_ERROR;
			}
		} else {
			NOTIFICATION_ERR("Failed to create app control:%d", ret);
			return NOTIFICATION_ERROR_IO_ERROR;
		}
	} else {
		NOTIFICATION_ERR("Failed to get execute option:%d", ret);
		return ret;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_execute_option(notification_h noti,
								notification_execute_type_e type,
								const char *text,
								const char *key,
								bundle *service_handle)
{
	char buf_key[32] = { 0, };
	const char *ret_val = NULL;
	bundle *b = NULL;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (type <= NOTIFICATION_EXECUTE_TYPE_NONE
	    || type >= NOTIFICATION_EXECUTE_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Create execute option bundle if does not exist */
	if (noti->b_execute_option == NULL) {
		noti->b_execute_option = bundle_create();
	}

	b = noti->b_execute_option;

	/* Save text */
	if (text != NULL) {
		/* Make text key */
		snprintf(buf_key, sizeof(buf_key), "text%d", type);

		/* Check text key exist */
		ret_val = bundle_get_val(b, buf_key);
		if (ret_val != NULL) {
			/* Remove previous data */
			bundle_del(b, buf_key);
		}

		/* Add text data */
		bundle_add(b, buf_key, text);
	}

	/* Save key */
	if (key != NULL) {
		/* Make key key */
		snprintf(buf_key, sizeof(buf_key), "key%d", type);

		/* Check key key exist */
		ret_val = bundle_get_val(b, buf_key);
		if (ret_val != NULL) {
			/* Remove previous data */
			bundle_del(b, buf_key);
		}

		/* Add text data */
		bundle_add(b, buf_key, key);
	}

	switch ((int)type) {
		case NOTIFICATION_EXECUTE_TYPE_RESPONDING:
			/* Remove previous data if exist */
			if (noti->b_service_responding != NULL) {
				bundle_free(noti->b_service_responding);
				noti->b_service_responding = NULL;
			}

			/* Save service handle */
			if (service_handle != NULL) {
				noti->b_service_responding = bundle_dup(service_handle);
			}
			break;
		case NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH:
			/* Remove previous data if exist */
			if (noti->b_service_single_launch != NULL) {
				bundle_free(noti->b_service_single_launch);
				noti->b_service_single_launch = NULL;
			}

			/* Save service handle */
			if (service_handle != NULL) {
				noti->b_service_single_launch =
					bundle_dup(service_handle);
			}
			break;
		case NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH:
			/* Remove previous data if exist */
			if (noti->b_service_multi_launch != NULL) {
				bundle_free(noti->b_service_multi_launch);
				noti->b_service_multi_launch = NULL;
			}

			/* Save service handle */
			if (service_handle != NULL) {
				noti->b_service_multi_launch =
					bundle_dup(service_handle);
			}
			break;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_execute_option(notification_h noti,
								notification_execute_type_e type,
								const char **text,
								bundle **service_handle)
{
	char buf_key[32] = { 0, };
	const char *ret_val = NULL;
	char *get_str = NULL;
	bundle *b = NULL;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (type <= NOTIFICATION_EXECUTE_TYPE_NONE
	    || type >= NOTIFICATION_EXECUTE_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	switch (type) {
	case NOTIFICATION_EXECUTE_TYPE_RESPONDING:
		b = noti->b_service_responding;
		break;
	case NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH:
		b = noti->b_service_single_launch;
		break;
	case NOTIFICATION_EXECUTE_TYPE_MULTI_LAUNCH:
		b = noti->b_service_multi_launch;
	default:
		break;
	}

	if (b != NULL) {
		// Return text
		if (text != NULL) {
			// Get text domain and dir
			if (noti->domain == NULL || noti->dir == NULL) {
				_notification_get_text_domain(noti);
			}

			/* Make key */
			snprintf(buf_key, sizeof(buf_key), "key%d", type);

			/* Check key key exist */
			ret_val = bundle_get_val(b, buf_key);
			if (ret_val != NULL && noti->domain != NULL
			    && noti->dir != NULL) {
				/* Get application string */
				bindtextdomain(noti->domain, noti->dir);

				get_str = dgettext(noti->domain, ret_val);

				*text = get_str;
			} else if (ret_val != NULL) {
				/* Get system string */
				get_str = dgettext("sys_string", ret_val);

				*text = get_str;
			} else {
				/* Get basic text */
				snprintf(buf_key, sizeof(buf_key), "text%d",
					 type);

				ret_val = bundle_get_val(b, buf_key);

				*text = ret_val;
			}
		}
	}

	if (service_handle != NULL) {
		*service_handle = b;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_property(notification_h noti,
							  int flags)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set flags */
	noti->flags_for_property = flags;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_property(notification_h noti,
							  int *flags)
{
	/* Check noti and flags are valid data */
	if (noti == NULL || flags == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set flags */
	*flags = noti->flags_for_property;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_display_applist(notification_h noti,
								 int applist)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set app list */
	noti->display_applist = applist;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_display_applist(notification_h noti,
								 int *applist)
{
	/* Check noti and applist are valid data */
	if (noti == NULL || applist == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set app list */
	*applist = noti->display_applist;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_size(notification_h noti,
						      double size)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save progress size */
	noti->progress_size = size;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_size(notification_h noti,
						      double *size)
{
	/* Check noti and size is valid data */
	if (noti == NULL || size == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set progress size */
	*size = noti->progress_size;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_progress(notification_h noti,
							  double percentage)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save progress percentage */
	noti->progress_percentage = percentage;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_progress(notification_h noti,
							  double *percentage)
{
	/* Check noti and percentage are valid data */
	if (noti == NULL || percentage == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set progress percentage */
	*percentage = noti->progress_percentage;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_pkgname(notification_h noti,
							 const char *pkgname)
{
	/* check noti and pkgname are valid data */
	if (noti == NULL || pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Remove previous caller pkgname */
	if (noti->caller_pkgname) {
		free(noti->caller_pkgname);
		noti->caller_pkgname = NULL;
	}

	noti->caller_pkgname = strdup(pkgname);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_pkgname(notification_h noti,
							 char **pkgname)
{
	/* Check noti and pkgname are valid data */
	if (noti == NULL || pkgname == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Get caller pkgname */
	if (noti->caller_pkgname) {
		*pkgname = noti->caller_pkgname;
	} else {
		*pkgname = NULL;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_set_layout(notification_h noti,
		notification_ly_type_e layout)
{
	/* check noti and pkgname are valid data */
	if (noti == NULL || (layout < NOTIFICATION_LY_NONE || layout >= NOTIFICATION_LY_MAX)) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	noti->layout = layout;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_layout(notification_h noti,
		notification_ly_type_e *layout)
{
	/* Check noti and pkgname are valid data */
	if (noti == NULL || layout == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	*layout = noti->layout;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_id(notification_h noti,
						    int *group_id, int *priv_id)
{
	/* check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check group_id is valid data */
	if (group_id) {
		/* Set group id */
		if (noti->group_id < NOTIFICATION_GROUP_ID_NONE) {
			*group_id = NOTIFICATION_GROUP_ID_NONE;
		} else {
			*group_id = noti->group_id;
		}
	}

	/* Check priv_id is valid data */
	if (priv_id) {
		/* Set priv_id */
		*priv_id = noti->priv_id;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_type(notification_h noti,
						      notification_type_e *type)
{
	/* Check noti and type is valid data */
	if (noti == NULL || type == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set noti type */
	*type = noti->type;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_post(notification_h noti)
{
	int ret = 0;
	int id = 0;

	/* Check noti is vaild data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check noti type is valid type */
	if (noti->type <= NOTIFICATION_TYPE_NONE
	    || noti->type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save insert time */
	noti->insert_time = time(NULL);

	ret = notification_ipc_request_insert(noti, &id);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}
	noti->priv_id = id;
	NOTIFICATION_DBG("from master:%d", id);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_insert(notification_h noti,
						    int *priv_id)
{
	int ret = 0;
	int id = 0;

	/* Check noti is vaild data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Check noti type is valid type */
	if (noti->type <= NOTIFICATION_TYPE_NONE
	    || noti->type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Save insert time */
	noti->insert_time = time(NULL);
	ret = notification_ipc_request_insert(noti, &id);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}
	noti->priv_id = id;
	NOTIFICATION_DBG("from master:%d", id);

	/* If priv_id is valid data, set priv_id */
	if (priv_id != NULL) {
		*priv_id = noti->priv_id;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_update(notification_h noti)
{
	int ret = 0;

	/* Check noti is valid data */
	if (noti != NULL) {
		/* Update insert time ? */
		noti->insert_time = time(NULL);
		ret = notification_ipc_request_update(noti);
	} else {
		notification_ipc_request_refresh();
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	return ret;
}

EXPORT_API int notification_update_async(notification_h noti,
		void (*result_cb)(int priv_id, int result, void *data), void *user_data)
{
	int ret = 0;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Update insert time ? */
	noti->insert_time = time(NULL);
	ret = notification_ipc_request_update_async(noti, result_cb, user_data);

	return ret;
}

EXPORT_API int notifiation_clear(notification_type_e type)
{
	int ret = 0;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_ipc_request_delete_multiple(type, NULL);

	return ret;
}

EXPORT_API int notification_clear(notification_type_e type)
{
	int ret = 0;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_ipc_request_delete_multiple(type, NULL);

	return ret;
}

EXPORT_API int notification_delete_all(notification_type_e type)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	caller_pkgname = _notification_get_pkgname_by_pid();

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

EXPORT_API int notification_delete_all_by_type(const char *pkgname,
								notification_type_e type)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (pkgname == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(pkgname);
	}

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

EXPORT_API int notification_delete_by_priv_id(const char *pkgname,
							       notification_type_e type,
							       int priv_id)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (pkgname == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(pkgname);
	}

	ret = notification_ipc_request_delete_single(type, caller_pkgname, priv_id);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

EXPORT_API int notification_delete(notification_h noti)
{
	int ret = 0;

	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_ipc_request_delete_single(NOTIFICATION_TYPE_NONE, noti->caller_pkgname, noti->priv_id);

	return ret;
}

EXPORT_API int notification_update_progress(notification_h noti,
							     int priv_id,
							     double progress)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;
	double input_progress = 0.0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL) {
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		} else {
			input_priv_id = noti->priv_id;
		}
	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(noti->caller_pkgname);
	}

	if (progress < 0.0) {
		input_progress = 0.0;
	} else if (progress > 1.0) {
		input_progress = 1.0;
	} else {
		input_progress = progress;
	}

	ret = notification_ongoing_update_progress(caller_pkgname, input_priv_id,
					     input_progress);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

EXPORT_API int notification_update_size(notification_h noti,
							 int priv_id,
							 double size)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;
	double input_size = 0.0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL) {
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		} else {
			input_priv_id = noti->priv_id;
		}
	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(noti->caller_pkgname);
	}

	if (size < 0.0) {
		input_size = 0.0;
	} else {
		input_size = size;
	}

	ret = notification_ongoing_update_size(caller_pkgname, input_priv_id,
					 input_size);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

EXPORT_API int notification_update_content(notification_h noti,
							 int priv_id,
							 const char *content)
{
	char *caller_pkgname = NULL;
	int input_priv_id = 0;
	int ret = 0;

	if (priv_id <= NOTIFICATION_PRIV_ID_NONE) {
		if (noti == NULL) {
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
		} else {
			input_priv_id = noti->priv_id;
		}
	} else {
		input_priv_id = priv_id;
	}

	if (noti == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(noti->caller_pkgname);
	}

	ret = notification_ongoing_update_content(caller_pkgname, input_priv_id,
					 content);

	if (caller_pkgname) {
		free(caller_pkgname);
	}

	return ret;
}

static notification_h _notification_create(notification_type_e type)
{
	notification_h noti = NULL;

	if (type <= NOTIFICATION_TYPE_NONE || type >= NOTIFICATION_TYPE_MAX) {
		NOTIFICATION_ERR("INVALID TYPE : %d", type);
		set_last_result(NOTIFICATION_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	noti = (notification_h) calloc(1, sizeof(struct _notification));
	if (noti == NULL) {
		NOTIFICATION_ERR("NO MEMORY : noti == NULL");
		set_last_result(NOTIFICATION_ERROR_OUT_OF_MEMORY);
		return NULL;
	}

	noti->type = type;

	if (type == NOTIFICATION_TYPE_NOTI)
		noti->layout = NOTIFICATION_LY_NOTI_EVENT_SINGLE;
	else if (type == NOTIFICATION_TYPE_ONGOING)
		noti->layout = NOTIFICATION_LY_ONGOING_PROGRESS;

	noti->caller_pkgname = _notification_get_pkgname_by_pid();
	noti->group_id = NOTIFICATION_GROUP_ID_NONE;
	noti->priv_id = NOTIFICATION_PRIV_ID_NONE;
	noti->sound_type = NOTIFICATION_SOUND_TYPE_NONE;
	noti->vibration_type = NOTIFICATION_VIBRATION_TYPE_NONE;
	noti->led_operation = NOTIFICATION_LED_OP_OFF;
	noti->display_applist = NOTIFICATION_DISPLAY_APP_ALL;
	/*!
	 * \NOTE
	 * Other fields are already initialized with ZERO.
	 */
	set_last_result(NOTIFICATION_ERROR_NONE);
	return noti;
}

EXPORT_API notification_h notification_new(notification_type_e type,
					   int group_id, int priv_id)
{
	return _notification_create(type);
}

EXPORT_API notification_h notification_create(notification_type_e type)
{
	return _notification_create(type);
}

EXPORT_API notification_h notification_load(char *pkgname,
						      int priv_id)
{
	int ret = 0;
	notification_h noti = NULL;

	noti = (notification_h) calloc(1, sizeof(struct _notification));
	if (noti == NULL) {
		NOTIFICATION_ERR("NO MEMORY : noti == NULL");
		return NULL;
	}

	ret = notification_noti_get_by_priv_id(noti, pkgname, priv_id);
	if (ret != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return NULL;
	}

	return noti;
}

EXPORT_API notification_h  notification_load_by_tag(const char *tag)
{
	int ret = 0;
	notification_h noti = NULL;
	char *caller_pkgname = NULL;

	if (tag == NULL) {
		NOTIFICATION_ERR("Invalid parameter");
		set_last_result(NOTIFICATION_ERROR_INVALID_PARAMETER);
		return NULL;
	}

	caller_pkgname = _notification_get_pkgname_by_pid();
	if (!caller_pkgname) {
		NOTIFICATION_ERR("Failed to get a package name");
		set_last_result(NOTIFICATION_ERROR_OUT_OF_MEMORY);

		return NULL;
	}

	noti = (notification_h) calloc(1, sizeof(struct _notification));
	if (noti == NULL) {
		NOTIFICATION_ERR("Failed to alloc a new notification");
		set_last_result(NOTIFICATION_ERROR_OUT_OF_MEMORY);
		free(caller_pkgname);

		return NULL;
	}

	ret = notification_ipc_request_load_noti_by_tag(noti, caller_pkgname, tag);

	free(caller_pkgname);

	set_last_result(ret);

	if (ret != NOTIFICATION_ERROR_NONE) {
		notification_free(noti);
		return NULL;
	}

	return noti;
}

EXPORT_API int notification_clone(notification_h noti, notification_h *clone)
{
	notification_h new_noti = NULL;

	if (noti == NULL || clone == NULL) {
		NOTIFICATION_ERR("INVALID PARAMETER.");
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	new_noti = (notification_h) calloc(1, sizeof(struct _notification));
	if (new_noti == NULL) {
		NOTIFICATION_ERR("NO MEMORY : noti == NULL");
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	new_noti->type = noti->type;
	new_noti->layout = noti->layout;

	new_noti->group_id = noti->group_id;
	new_noti->internal_group_id = noti->internal_group_id;
	new_noti->priv_id = noti->priv_id;

	if(noti->caller_pkgname != NULL) {
		new_noti->caller_pkgname = strdup(noti->caller_pkgname);
	} else {
		new_noti->caller_pkgname = _notification_get_pkgname_by_pid();
	}
	if(noti->launch_pkgname != NULL) {
		new_noti->launch_pkgname = strdup(noti->launch_pkgname);
	} else {
		new_noti->launch_pkgname = NULL;
	}

	if(noti->args != NULL) {
		new_noti->args = bundle_dup(noti->args);
	} else {
		new_noti->args = NULL;
	}
	if(noti->group_args != NULL) {
		new_noti->group_args = bundle_dup(noti->group_args);
	} else {
		new_noti->group_args = NULL;
	}

	if(noti->b_execute_option != NULL) {
		new_noti->b_execute_option = bundle_dup(noti->b_execute_option);
	} else {
		new_noti->b_execute_option = NULL;
	}
	if(noti->b_service_responding != NULL) {
		new_noti->b_service_responding = bundle_dup(noti->b_service_responding);
	} else {
		new_noti->b_service_responding = NULL;
	}
	if(noti->b_service_single_launch != NULL) {
		new_noti->b_service_single_launch = bundle_dup(noti->b_service_single_launch);
	} else {
		new_noti->b_service_single_launch = NULL;
	}
	if(noti->b_service_multi_launch != NULL) {
		new_noti->b_service_multi_launch = bundle_dup(noti->b_service_multi_launch);
	} else {
		new_noti->b_service_multi_launch = NULL;
	}

	new_noti->sound_type = noti->sound_type;
	if(noti->sound_path != NULL) {
		new_noti->sound_path = strdup(noti->sound_path);
	} else {
		new_noti->sound_path = NULL;
	}
	new_noti->vibration_type = noti->vibration_type;
	if(noti->vibration_path != NULL) {
		new_noti->vibration_path = strdup(noti->vibration_path);
	} else {
		new_noti->vibration_path = NULL;
	}
	new_noti->led_operation = noti->led_operation;
	new_noti->led_argb = noti->led_argb;
	new_noti->led_on_ms = noti->led_on_ms;
	new_noti->led_off_ms = noti->led_off_ms;

	if(noti->domain != NULL) {
		new_noti->domain = strdup(noti->domain);
	} else {
		new_noti->domain = NULL;
	}
	if(noti->dir != NULL) {
		new_noti->dir = strdup(noti->dir);
	} else {
		new_noti->dir = NULL;
	}

	if(noti->b_text != NULL) {
		new_noti->b_text = bundle_dup(noti->b_text);
	} else {
		new_noti->b_text = NULL;
	}
	if(noti->b_key != NULL) {
		new_noti->b_key = bundle_dup(noti->b_key);
	} else {
		new_noti->b_key = NULL;
	}
	if(noti->b_format_args != NULL) {
		new_noti->b_format_args = bundle_dup(noti->b_format_args);
	} else {
		new_noti->b_format_args = NULL;
	}
	new_noti->num_format_args = noti->num_format_args;

	if(noti->b_image_path != NULL) {
		new_noti->b_image_path = bundle_dup(noti->b_image_path);
	} else {
		new_noti->b_image_path = NULL;
	}

	new_noti->time = noti->time;
	new_noti->insert_time = noti->insert_time;

	new_noti->flags_for_property = noti->flags_for_property;
	new_noti->display_applist = noti->display_applist;

	new_noti->progress_size = noti->progress_size;
	new_noti->progress_percentage = noti->progress_percentage;

	new_noti->app_icon_path = NULL;
	new_noti->app_name = NULL;
	new_noti->temp_title = NULL;
	new_noti->temp_content = NULL;

	*clone = new_noti;

	return NOTIFICATION_ERROR_NONE;
}


EXPORT_API int notification_free(notification_h noti)
{
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (noti->caller_pkgname) {
		free(noti->caller_pkgname);
	}
	if (noti->launch_pkgname) {
		free(noti->launch_pkgname);
	}
	if (noti->args) {
		bundle_free(noti->args);
	}
	if (noti->group_args) {
		bundle_free(noti->group_args);
	}

	if (noti->b_execute_option) {
		bundle_free(noti->b_execute_option);
	}
	if (noti->b_service_responding) {
		bundle_free(noti->b_service_responding);
	}
	if (noti->b_service_single_launch) {
		bundle_free(noti->b_service_single_launch);
	}
	if (noti->b_service_multi_launch) {
		bundle_free(noti->b_service_multi_launch);
	}

	if (noti->sound_path) {
		free(noti->sound_path);
	}
	if (noti->vibration_path) {
		free(noti->vibration_path);
	}

	if (noti->domain) {
		free(noti->domain);
	}
	if (noti->dir) {
		free(noti->dir);
	}

	if (noti->b_text) {
		bundle_free(noti->b_text);
	}
	if (noti->b_key) {
		bundle_free(noti->b_key);
	}
	if (noti->b_format_args) {
		bundle_free(noti->b_format_args);
	}

	if (noti->b_image_path) {
		bundle_free(noti->b_image_path);
	}

	if (noti->app_icon_path) {
		free(noti->app_icon_path);
	}
	if (noti->app_name) {
		free(noti->app_name);
	}
	if (noti->temp_title) {
		free(noti->temp_title);
	}
	if (noti->temp_content) {
		free(noti->temp_content);
	}

	if (noti->tag) {
		free(noti->tag);
	}

	free(noti);

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int
notification_resister_changed_cb(void (*changed_cb)
				 (void *data, notification_type_e type),
				 void *user_data)
{
	notification_cb_list_s *noti_cb_list_new = NULL;
	notification_cb_list_s *noti_cb_list = NULL;

	if (changed_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (notification_ipc_monitor_init() != NOTIFICATION_ERROR_NONE) {
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	noti_cb_list_new =
	    (notification_cb_list_s *) malloc(sizeof(notification_cb_list_s));

	if (noti_cb_list_new == NULL) {
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	noti_cb_list_new->next = NULL;
	noti_cb_list_new->prev = NULL;

	noti_cb_list_new->cb_type = NOTIFICATION_CB_NORMAL;
	noti_cb_list_new->changed_cb = changed_cb;
	noti_cb_list_new->detailed_changed_cb = NULL;
	noti_cb_list_new->data = user_data;

	if (g_notification_cb_list == NULL) {
		g_notification_cb_list = noti_cb_list_new;
	} else {
		noti_cb_list = g_notification_cb_list;

		while (noti_cb_list->next != NULL) {
			noti_cb_list = noti_cb_list->next;
		}

		noti_cb_list->next = noti_cb_list_new;
		noti_cb_list_new->prev = noti_cb_list;
	}
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int
notification_unresister_changed_cb(void (*changed_cb)
				   (void *data, notification_type_e type))
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_cb_list_s *noti_cb_list_prev = NULL;
	notification_cb_list_s *noti_cb_list_next = NULL;

	noti_cb_list = g_notification_cb_list;

	if (changed_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (noti_cb_list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	while (noti_cb_list->prev != NULL) {
		noti_cb_list = noti_cb_list->prev;
	}

	do {
		if (noti_cb_list->changed_cb == changed_cb) {
			noti_cb_list_prev = noti_cb_list->prev;
			noti_cb_list_next = noti_cb_list->next;

			if (noti_cb_list_prev == NULL) {
				g_notification_cb_list = noti_cb_list_next;
			} else {
				noti_cb_list_prev->next = noti_cb_list_next;
			}

			if (noti_cb_list_next == NULL) {
				if (noti_cb_list_prev != NULL) {
					noti_cb_list_prev->next = NULL;
				}
			} else {
				noti_cb_list_next->prev = noti_cb_list_prev;
			}

			free(noti_cb_list);

			if (g_notification_cb_list == NULL)
				notification_ipc_monitor_fini();

			return NOTIFICATION_ERROR_NONE;
		}
		noti_cb_list = noti_cb_list->next;
	} while (noti_cb_list != NULL);

	return NOTIFICATION_ERROR_INVALID_PARAMETER;
}

EXPORT_API int
notification_register_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data)
{
	notification_cb_list_s *noti_cb_list_new = NULL;
	notification_cb_list_s *noti_cb_list = NULL;

	if (detailed_changed_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (notification_ipc_monitor_init() != NOTIFICATION_ERROR_NONE) {
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	noti_cb_list_new =
	    (notification_cb_list_s *) malloc(sizeof(notification_cb_list_s));

	if (noti_cb_list_new == NULL) {
		return NOTIFICATION_ERROR_OUT_OF_MEMORY;
	}

	noti_cb_list_new->next = NULL;
	noti_cb_list_new->prev = NULL;

	noti_cb_list_new->cb_type = NOTIFICATION_CB_DETAILED;
	noti_cb_list_new->changed_cb = NULL;
	noti_cb_list_new->detailed_changed_cb = detailed_changed_cb;
	noti_cb_list_new->data = user_data;

	if (g_notification_cb_list == NULL) {
		g_notification_cb_list = noti_cb_list_new;
	} else {
		noti_cb_list = g_notification_cb_list;

		while (noti_cb_list->next != NULL) {
			noti_cb_list = noti_cb_list->next;
		}

		noti_cb_list->next = noti_cb_list_new;
		noti_cb_list_new->prev = noti_cb_list;
	}
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int
notification_unregister_detailed_changed_cb(
		void (*detailed_changed_cb)(void *data, notification_type_e type, notification_op *op_list, int num_op),
		void *user_data)
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_cb_list_s *noti_cb_list_prev = NULL;
	notification_cb_list_s *noti_cb_list_next = NULL;

	noti_cb_list = g_notification_cb_list;

	if (detailed_changed_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}
	if (noti_cb_list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	while (noti_cb_list->prev != NULL) {
		noti_cb_list = noti_cb_list->prev;
	}

	do {
		if (noti_cb_list->detailed_changed_cb == detailed_changed_cb) {
			noti_cb_list_prev = noti_cb_list->prev;
			noti_cb_list_next = noti_cb_list->next;

			if (noti_cb_list_prev == NULL) {
				g_notification_cb_list = noti_cb_list_next;
			} else {
				noti_cb_list_prev->next = noti_cb_list_next;
			}

			if (noti_cb_list_next == NULL) {
				if (noti_cb_list_prev != NULL) {
					noti_cb_list_prev->next = NULL;
				}
			} else {
				noti_cb_list_next->prev = noti_cb_list_prev;
			}

			free(noti_cb_list);

			if (g_notification_cb_list == NULL)
				notification_ipc_monitor_fini();

			return NOTIFICATION_ERROR_NONE;
		}
		noti_cb_list = noti_cb_list->next;
	} while (noti_cb_list != NULL);

	return NOTIFICATION_ERROR_INVALID_PARAMETER;
}

EXPORT_API int notification_get_count(notification_type_e type,
						       const char *pkgname,
						       int group_id,
						       int priv_id, int *count)
{
	int ret = 0;
	int noti_count = 0;

	if (count == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret =
	    notification_noti_get_count(type, pkgname, group_id, priv_id,
					&noti_count);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	*count = noti_count;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_list(notification_type_e type,
						      int count,
						      notification_list_h *list)
{
	notification_list_h get_list = NULL;
	int ret = 0;

	if (list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_noti_get_grouping_list(type, count, &get_list);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	*list = get_list;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int
notification_get_grouping_list(notification_type_e type, int count,
			       notification_list_h * list)
{
	notification_list_h get_list = NULL;
	int ret = 0;

	if (list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret = notification_noti_get_grouping_list(type, count, &get_list);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	*list = get_list;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_detail_list(const char *pkgname,
							     int group_id,
							     int priv_id,
							     int count,
							     notification_list_h *list)
{
	notification_list_h get_list = NULL;
	int ret = 0;

	if (list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	ret =
	    notification_noti_get_detail_list(pkgname, group_id, priv_id, count,
					      &get_list);
	if (ret != NOTIFICATION_ERROR_NONE) {
		return ret;
	}

	*list = get_list;

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_free_list(notification_list_h list)
{
	notification_list_h cur_list = NULL;
	notification_h noti = NULL;

	if (list == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	cur_list = notification_list_get_head(list);

	while (cur_list != NULL) {
		noti = notification_list_get_data(cur_list);
		cur_list = notification_list_remove(cur_list, noti);

		notification_free(noti);
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_op_get_data(notification_op *noti_op, notification_op_data_type_e type,
						       void *data)
{
	if (noti_op == NULL || data == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	switch (type) {
		case NOTIFICATION_OP_DATA_TYPE:
			*((int*)data) = noti_op->type;
			break;
		case NOTIFICATION_OP_DATA_PRIV_ID:
			*((int*)data) = noti_op->priv_id;
			break;
		case NOTIFICATION_OP_DATA_NOTI:
			*((notification_h *)data) = noti_op->noti;
			break;
		case NOTIFICATION_OP_DATA_EXTRA_INFO_1:
			*((int*)data) = noti_op->extra_info_1;
			break;
		case NOTIFICATION_OP_DATA_EXTRA_INFO_2:
			*((int*)data) = noti_op->extra_info_2;
			break;
		default:
			return NOTIFICATION_ERROR_INVALID_PARAMETER;
			break;
	}

	return NOTIFICATION_ERROR_NONE;
}

void notification_call_changed_cb(notification_op *op_list, int op_num)
{
	notification_cb_list_s *noti_cb_list = NULL;
	notification_type_e type = 0;

	if (g_notification_cb_list == NULL) {
		return;
	}
	noti_cb_list = g_notification_cb_list;

	while (noti_cb_list->prev != NULL) {
		noti_cb_list = noti_cb_list->prev;
	}

	if (op_list == NULL) {
		NOTIFICATION_ERR("invalid data");
		return ;
	}

	notification_get_type(op_list->noti, &type);

	while (noti_cb_list != NULL) {
		if (noti_cb_list->cb_type == NOTIFICATION_CB_NORMAL && noti_cb_list->changed_cb) {
			noti_cb_list->changed_cb(noti_cb_list->data,
						 type);
		}
		if (noti_cb_list->cb_type == NOTIFICATION_CB_DETAILED && noti_cb_list->detailed_changed_cb) {
			noti_cb_list->detailed_changed_cb(noti_cb_list->data,
						type, op_list, op_num);
		}

		noti_cb_list = noti_cb_list->next;
	}
}

EXPORT_API int notification_is_service_ready(void)
{
	return notification_ipc_is_master_ready();
}

EXPORT_API int
notification_add_deferred_task(
		void (*deferred_task_cb)(void *data), void *user_data)
{
	if (deferred_task_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	return notification_ipc_add_deffered_task(deferred_task_cb, user_data);
}

EXPORT_API int
notification_del_deferred_task(
		void (*deferred_task_cb)(void *data))
{
	if (deferred_task_cb == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	return notification_ipc_del_deffered_task(deferred_task_cb);
}

/* notification_set_icon will be removed */
EXPORT_API int notification_set_icon(notification_h noti,
						      const char *icon_path)
{
	int ret_err = NOTIFICATION_ERROR_NONE;

	ret_err =
	    notification_set_image(noti, NOTIFICATION_IMAGE_TYPE_ICON,
				   icon_path);

	return ret_err;
}

/* notification_get_icon will be removed */
EXPORT_API int notification_get_icon(notification_h noti,
						      char **icon_path)
{
	int ret_err = NOTIFICATION_ERROR_NONE;
	char *ret_image_path = NULL;

	ret_err =
	    notification_get_image(noti, NOTIFICATION_IMAGE_TYPE_ICON,
				   &ret_image_path);

	if (ret_err == NOTIFICATION_ERROR_NONE && icon_path != NULL) {
		*icon_path = ret_image_path;

		//NOTIFICATION_DBG("Get icon : %s", *icon_path);
	}

	return ret_err;
}

EXPORT_API int notification_set_title(notification_h noti,
						       const char *title,
						       const char *loc_title)
{
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_set_text(noti, NOTIFICATION_TEXT_TYPE_TITLE,
					 title, loc_title,
					 NOTIFICATION_VARIABLE_TYPE_NONE);

	return noti_err;
}

EXPORT_API int notification_get_title(notification_h noti,
						       char **title,
						       char **loc_title)
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *ret_text = NULL;

	noti_err =
	    notification_get_text(noti, NOTIFICATION_TEXT_TYPE_TITLE,
				  &ret_text);

	if (title != NULL) {
		*title = ret_text;
	}

	if (loc_title != NULL) {
		*loc_title = NULL;
	}

	return noti_err;
}

EXPORT_API int notification_set_content(notification_h noti,
							 const char *content,
							 const char *loc_content)
{
	int noti_err = NOTIFICATION_ERROR_NONE;

	noti_err = notification_set_text(noti, NOTIFICATION_TEXT_TYPE_CONTENT,
					 content, loc_content,
					 NOTIFICATION_VARIABLE_TYPE_NONE);

	return noti_err;
}

EXPORT_API int notification_get_content(notification_h noti,
							 char **content,
							 char **loc_content)
{
	int noti_err = NOTIFICATION_ERROR_NONE;
	char *ret_text = NULL;

	noti_err =
	    notification_get_text(noti, NOTIFICATION_TEXT_TYPE_CONTENT,
				  &ret_text);

	if (content != NULL) {
		*content = ret_text;
	}

	if (loc_content != NULL) {
		*loc_content = NULL;
	}

	return noti_err;

#if 0
	ret =
	    vconf_get_bool
	    (VCONFKEY_SETAPPL_STATE_TICKER_NOTI_DISPLAY_CONTENT_BOOL, &boolval);

	if (ret == -1 || boolval == 0) {
		if (content != NULL && noti->default_content != NULL) {
			*content = noti->default_content;
		}

		if (loc_content != NULL && noti->loc_default_content != NULL) {
			*loc_content = noti->loc_default_content;
		}
	}
#endif
}

EXPORT_API int notification_set_args(notification_h noti,
						      bundle * args,
						      bundle * group_args)
{
	if (noti == NULL || args == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (noti->args) {
		bundle_free(noti->args);
	}

	noti->args = bundle_dup(args);

	if (noti->group_args) {
		bundle_free(noti->group_args);
		noti->group_args = NULL;
	}

	if (group_args != NULL) {
		noti->group_args = bundle_dup(group_args);
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_get_args(notification_h noti,
						      bundle ** args,
						      bundle ** group_args)
{
	if (noti == NULL || args == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (noti->args) {
		*args = noti->args;
	} else {
		*args = NULL;
	}

	if (group_args != NULL && noti->group_args) {
		*group_args = noti->group_args;
	}

	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_delete_group_by_group_id(const char *pkgname,
								      notification_type_e type,
								      int group_id)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (pkgname == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(pkgname);
	}

	ret = notification_ipc_request_delete_multiple(type, caller_pkgname);
	if (ret != NOTIFICATION_ERROR_NONE) {
		if (caller_pkgname) {
			free(caller_pkgname);
		}
		return ret;
	}

	if (caller_pkgname) {
		free(caller_pkgname);
	}
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_delete_group_by_priv_id(const char *pkgname,
								     notification_type_e type,
								     int priv_id)
{
	int ret = 0;
	char *caller_pkgname = NULL;

	if (pkgname == NULL) {
		caller_pkgname = _notification_get_pkgname_by_pid();
	} else {
		caller_pkgname = strdup(pkgname);
	}

	ret = notification_ipc_request_delete_single(type, caller_pkgname, priv_id);
	if (ret != NOTIFICATION_ERROR_NONE) {
		if (caller_pkgname) {
			free(caller_pkgname);
		}
		return ret;
	}

	if (caller_pkgname) {
		free(caller_pkgname);
	}
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int  notification_set_tag(notification_h noti, const char *tag)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	if (tag != NULL) {
		/* save input TAG */
		if (noti->tag != NULL) {
			free(noti->tag);
		}
		noti->tag = strdup(tag);
	}

	return NOTIFICATION_ERROR_NONE;

}

EXPORT_API int  notification_get_tag(notification_h noti, const char **tag)
{
	/* Check noti is valid data */
	if (noti == NULL) {
		return NOTIFICATION_ERROR_INVALID_PARAMETER;
	}

	/* Set sound type */
	*tag = noti->tag;
	return NOTIFICATION_ERROR_NONE;
}

EXPORT_API int notification_register_toast_message(void (*posted_toast_cb) (void *data))
{
	if (notification_ipc_monitor_init() != NOTIFICATION_ERROR_NONE) {
		return NOTIFICATION_ERROR_IO_ERROR;
	}

	posted_toast_message_cb = posted_toast_cb;

	return NOTIFICATION_ERROR_NONE;
}

void notification_call_posted_toast_cb(const char *message)
{
	if (posted_toast_message_cb != NULL) {
		posted_toast_message_cb(message);
	}
}
