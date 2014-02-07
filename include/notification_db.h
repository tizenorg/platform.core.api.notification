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

#ifndef __NOTIFICATION_DB_H__
#define __NOTIFICATION_DB_H__

#include <bundle.h>
#include <sqlite3.h>
#include <tzplatform_config.h>

#ifndef DBFILE
#error "DBFILE not defined"
#endif

#define DBPATH tzplatform_mkpath(TZ_SYS_DB,DBFILE)

#define NOTIFICATION_QUERY_MAX 4096

#define NOTIFICATION_EMPTY_STR ""
#define NOTIFICATION_CHECK_STR(p) ((p)?(p):NOTIFICATION_EMPTY_STR)

sqlite3 *notification_db_open(const char *dbfile);

int notification_db_close(sqlite3 ** db);

int notification_db_exec(sqlite3 * db, const char *query, int *num_changes);

char *notification_db_column_text(sqlite3_stmt * stmt, int col);

bundle *notification_db_column_bundle(sqlite3_stmt * stmt, int col);

#endif				/* __NOTIFICATION_DB_H__ */
