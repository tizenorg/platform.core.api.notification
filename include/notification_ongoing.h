/*
 * Copyright (c) 2000 - 2016 Samsung Electronics Co., Ltd. All rights reserved.
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
 */

#ifndef __NOTIFICATION_ONGOING_H__
#define __NOTIFICATION_ONGOING_H__

#include <notification.h>
int notification_ongoing_update_progress(const char *caller_pkgname,
							  int priv_id,
							  double progress);

int notification_ongoing_update_size(const char *caller_pkgname,
						      int priv_id, double size);

int notification_ongoing_update_content(const char *caller_pkgname,
						      int priv_id, const char *content);

#endif /* __NOTIFICATION_ONGOING_H__ */

