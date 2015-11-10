%bcond_with wayland
Name:       notification
Summary:    Notification library
Version:    0.2.41
Release:    1
Group:      TBD
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(ail)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(dbus-glib-1)
BuildRequires: pkgconfig(com-core)
BuildRequires: pkgconfig(appcore-common)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(edbus)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(libtzplatform-config)

BuildRequires: cmake
Requires(post): /sbin/ldconfig
Requires(post): %{TZ_SYS_BIN}/sqlite3
Requires(postun): /sbin/ldconfig

%description
Client/Server library for sending notifications.

%prep
%setup -q

%package devel
Summary:    Notification library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Development files needed to build software that needs to system a system notification.

%build
%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"
%endif
%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif
export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"
LDFLAGS="$LDFLAGS"
%if %{with wayland}
%cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DHAVE_WAYLAND=On
%else
%cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DHAVE_X11=On
%endif
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}%{TZ_SYS_SHARE}/license
cp -f LICENSE %{buildroot}%{TZ_SYS_SHARE}/license/%{name}

%clean
rm -rf %{buildroot}

%post
/sbin/ldconfig

if [ ! -d %{TZ_SYS_DB} ]
then
	mkdir %{TZ_SYS_DB}
fi

if [ ! -f %{TZ_SYS_DB}/.notification.db ]
then
	sqlite3 %{TZ_SYS_DB}/.notification.db 'PRAGMA journal_mode = PERSIST;
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
			tag TEXT,
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
			b_event_handler_click_on_button_1 TEXT,
			b_event_handler_click_on_button_2 TEXT,
			b_event_handler_click_on_button_3 TEXT,
			b_event_handler_click_on_button_4 TEXT,
			b_event_handler_click_on_button_5 TEXT,
			b_event_handler_click_on_button_6 TEXT,
			b_event_handler_click_on_icon TEXT,
			b_event_handler_click_on_thumbnail TEXT,
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
			ongoing_flag INTEGER default 0,
			auto_remove INTEGER default 1,
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
		create table if not exists ongoing_list ( 
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
		CREATE TABLE IF NOT EXISTS notification_setting ( 
			priv_id INTEGER PRIMARY KEY AUTOINCREMENT,
			package_name TEXT NOT NULL,
			allow_to_notify INTEGER DEFAULT 1,
			do_not_disturb_except INTEGER DEFAULT 0,
			visibility_class INTEGER DEFAULT 0,
			UNIQUE (priv_id, package_name)
		); 
		CREATE TABLE IF NOT EXISTS notification_system_setting ( 
			priv_id INTERGER PRIMARY KEY,
			do_not_disturb INTEGER DEFAULT 0,
			visibility_class INTEGER DEFAULT 0,
			UNIQUE (priv_id)
		); 

		INSERT INTO notification_system_setting (priv_id, do_not_disturb, visibility_class) VALUES (0, 0, 0);

		CREATE UNIQUE INDEX package_name_idx1 ON notification_setting (package_name);
	'
else
	echo %{TZ_SYS_DB}/.notification.db ": DB file is already exists"
fi

chown :5000 %{TZ_SYS_DB}/.notification.db
chown :5000 %{TZ_SYS_DB}/.notification.db-journal
chmod 644 %{TZ_SYS_DB}/.notification.db
chmod 644 %{TZ_SYS_DB}/.notification.db-journal

%postun -p /sbin/ldconfig

%files
%manifest notification.manifest
%defattr(-,root,root,-)
%{_libdir}/libnotification.so*
%{TZ_SYS_SHARE}/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_includedir}/notification/notification.h
%{_includedir}/notification/notification_internal.h
%{_includedir}/notification/notification_error.h
%{_includedir}/notification/notification_type.h
%{_includedir}/notification/notification_list.h
%{_includedir}/notification/notification_ongoing_flag.h
%{_includedir}/notification/notification_text_domain.h
%{_includedir}/notification/notification_status.h
%{_includedir}/notification/notification_status_internal.h
%{_includedir}/notification/notification_setting.h
%{_includedir}/notification/notification_setting_internal.h
%{_includedir}/notification/notification_ipc.h
%{_includedir}/notification/notification_noti.h
%{_includedir}/notification/notification_setting_service.h
%{_libdir}/pkgconfig/notification.pc
