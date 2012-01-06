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

#ifndef __NOTIFICATION_DEBUG_H__
#define __NOTIFICATION_DEBUG_H__

#define LOG_TAG "notification"
#include <dlog.h>

#ifndef EXPORT_API
#define EXPORT_API __attribute__ ((visibility("default")))
#endif

#ifndef _DLOG_H_
#include <stdio.h>
#define NOTIFICATION_ERR(fmt, arg...)\
	do { fprintf(stderr, "["LOG_TAG"] %s(%d):"fmt"\n", __FUNCTION__, __LINE__, ##arg); } while (0)
#define NOTIFICATION_INFO(fmt, arg...)\
	do { fprintf(stdout, "["LOG_TAG"] %s(%d):"fmt"\n", __FUNCTION__, __LINE__, ##arg); } while (0)
#define NOTIFICATION_DBG(fmt, arg...)\
	do { fprintf(stdout, "["LOG_TAG"] %s(%d):"fmt"\n", __FUNCTION__, __LINE__, ##arg); } while (0)
#else
#define NOTIFICATION_ERR(...) LOGE(__VA_ARGS__)
#define NOTIFICATION_INFO(...) LOGI(__VA_ARGS__)
#define NOTIFICATION_DBG(...) LOGD(__VA_ARGS__)
#endif

#endif				/* __NOTIFICATION_DEBUG_H__ */
