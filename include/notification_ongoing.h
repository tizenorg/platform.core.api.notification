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

#ifndef __NOTIFICATION_ONGOING_H__
#define __NOTIFICATION_ONGOING_H__

#include <notification.h>
notification_error_e notification_ongoing_update_progress(const char *caller_pkgname,
							  int priv_id,
							  double progress);

notification_error_e notification_ongoing_update_size(const char *caller_pkgname,
						      int priv_id, double size);

notification_error_e notification_ongoing_update_content(const char *caller_pkgname,
						      int priv_id, const char *content);

#endif				/* __NOTIFICATION_ONGOING_H__ */
