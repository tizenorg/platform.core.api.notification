Name:       notification
Summary:    notification library
Version:    0.1.1
Release:    4.11
Group:      System/Library
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(heynoti)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(ail)
BuildRequires: pkgconfig(aul)
BuildRequires: cmake
Requires(post): /sbin/ldconfig
Requires(post): /usr/bin/sqlite3
requires(postun): /sbin/ldconfig
%description
Notificaiton library.


%prep
%setup -q

%package devel
Summary:    Notification library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}
Requires: pkgconfig(sqlite3)
Requires: pkgconfig(db-util)
Requires: pkgconfig(heynoti)
Requires: pkgconfig(vconf)
Requires: pkgconfig(bundle)
Requires: pkgconfig(dlog)
Requires: pkgconfig(ail)
Requires: pkgconfig(aul)

%description devel
Notificaiton library (devel).

%build
export LDFLAGS+="-Wl,--rpath=%{_prefix}/lib -Wl,--as-needed"
LDFLAGS="$LDFLAGS" cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
%make_install


%post
/sbin/ldconfig

mkdir -p /opt/dbspace

if [ ! -f /opt/dbspace/.notification.db ]
then
	sqlite3 /opt/dbspace/.notification.db 'PRAGMA journal_mode = PERSIST;
		create 	table if not exists noti_list ( 
			type INTEGER NOT NULL,
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

chown root:5000 /opt/dbspace/.notification.db
chown root:5000 /opt/dbspace/.notification.db-journal
chmod 660 /opt/dbspace/.notification.db
chmod 660 /opt/dbspace/.notification.db-journal

%postun -p /sbin/ldconfig

%files
%{_libdir}/libnotification.so.0*

%files devel
%{_includedir}/notification/notification.h
%{_includedir}/notification/notification_error.h
%{_includedir}/notification/notification_list.h
%{_includedir}/notification/notification_type.h
%{_libdir}/libnotification.so
%{_libdir}/pkgconfig/notification.pc

