#!/bin/sh

source /etc/tizen-platform.conf

if [ ! -d $TZ_SYS_DB ]
then
	mkdir $TZ_SYS_DB
fi

if [ ! -f $TZ_SYS_DB/.notification.db ]
then
	sqlite3 $TZ_SYS_DB/.notification.db 'PRAGMA journal_mode = PERSIST;
		create 	table if not exists noti_list (
			type INTEGER NOT NULL,
			layout INTEGER NOT NULL default 0,
			caller_pkgname TEXT NOT NULL,
			launch_pkgname TEXT,
			image_path TEXT,
			group_id INTEGER default 0,
			internal_group_id INTEGER default 0,
			priv_id INTERGER NOT NULL,
			title_key TEXT,
			b_text TEXT,
			b_key TEXT,
			b_format_args TEXT,
			num_format_args INTEGER default 0,
			text_domain TEXT,
			text_dir TEXT,
			time INTEGER default 0,  
			insert_time INTEGER default 0,
			args TEXT,
			group_args TEXT,
			b_execute_option TEXT,
			b_service_responding TEXT,
			b_service_single_launch TEXT,
			b_service_multi_launch TEXT,
			sound_type INTEGER default 0,
			sound_path TEXT,
			vibration_type INTEGER default 0,
			vibration_path TEXT,
			led_operation INTEGER default 0,
			led_argb INTEGER default 0,
			led_on_ms INTEGER default -1,
			led_off_ms INTEGER default -1,
			flags_for_property INTEGER default 0,
			flag_simmode INTEGER default 0,
			display_applist INTEGER,
			progress_size DOUBLE default 0,
			progress_percentage DOUBLE default 0,
			rowid INTEGER PRIMARY KEY AUTOINCREMENT,
			UNIQUE (caller_pkgname, priv_id)  
		);
		create table if not exists noti_group_data (
			caller_pkgname TEXT NOT NULL,
			group_id INTEGER default 0,
			badge INTEGER default 0,
			title TEXT,
			content TEXT,
			loc_title TEXT,
			loc_content TEXT,
			count_display_title INTEGER,
			count_display_content INTEGER,
			rowid INTEGER PRIMARY KEY AUTOINCREMENT,
			UNIQUE (caller_pkgname, group_id)
		);
		create 	table if not exists ongoing_list (
			caller_pkgname TEXT NOT NULL,
			launch_pkgname TEXT,
			icon_path TEXT,
			group_id INTEGER default 0,
			internal_group_id INTEGER default 0,
			priv_id INTERGER NOT NULL,
			title TEXT,
			content TEXT,
			default_content TEXT,
			loc_title TEXT,
			loc_content TEXT,
			loc_default_content TEXT,
			text_domain TEXT,
			text_dir TEXT,
			args TEXT,
			group_args TEXT,
			flag INTEGER default 0,
			progress_size DOUBLE default 0,
			progress_percentage DOUBLE default 0,
			rowid INTEGER PRIMARY KEY AUTOINCREMENT,
			UNIQUE (caller_pkgname, priv_id)
		);
	'
fi

users_gid=$(getent group $TZ_SYS_USER_GROUP | cut -f3 -d':')

chown :$TZ_SYS_USER_GROUP $TZ_SYS_DB/.notification.db
chown :$TZ_SYS_USER_GROUP $TZ_SYS_DB/.notification.db-journal
chmod 640 $TZ_SYS_DB/.notification.db
chmod 640 $TZ_SYS_DB/.notification.db-journal
chsmack -a 'notification::db' $TZ_SYS_DB/.notification.db*
SMACK_OPTION="-s system::vconf_misc"
vconftool set -t string memory/private/libstatus/message "" -i -g $users_gid -f  $SMACK_OPTION
