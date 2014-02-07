Name:       notification
Summary:    Notification Library
Version:    0.2.3
Release:    1
Group:      System/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz
Source1001: 	notification.manifest
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(heynoti)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(dbus-1)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(ail)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(dbus-glib-1)
BuildRequires: pkgconfig(com-core)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(libtzplatform-config)
Requires:      libtzplatform-config

BuildRequires: cmake
Requires(post): /sbin/ldconfig
Requires(post): /usr/bin/sqlite3
Requires(postun): /sbin/ldconfig

%description
Client/Server library for sending notifications.

%prep
%setup -q
cp %{SOURCE1001} .

%package devel
Summary:    Notification library (devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description devel
Development files needed to build software that needs to system a system notification.

%package service-devel
Summary:    Notification library (service-devel)
Group:      Development/Libraries
Requires:   %{name} = %{version}-%{release}

%description service-devel
Development files needed to build notification service

%build
%cmake .
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp -f LICENSE.APLv2.0 %{buildroot}/usr/share/license/%{name}

%clean
rm -rf %{buildroot}

%post
/sbin/ldconfig
%{_datadir}/%{name}/notification_DB_init.sh

%postun -p /sbin/ldconfig

%files
%manifest notification.manifest
%defattr(-,root,root,-)
%{_libdir}/libnotification.so*
%attr(0755,root,root) %{_datadir}/%{name}/notification_DB_init.sh
/usr/share/license/%{name}

%files devel
%defattr(-,root,root,-)
%{_includedir}/notification/notification.h
%{_includedir}/notification/notification_error.h
%{_includedir}/notification/notification_type.h
%{_includedir}/notification/notification_list.h
%{_includedir}/notification/notification_status.h
%{_includedir}/notification/notification_setting.h
%{_libdir}/pkgconfig/notification.pc

%files service-devel
%defattr(-,root,root,-)
%{_includedir}/notification/service/notification_ipc.h
%{_includedir}/notification/service/notification_noti.h
%{_includedir}/notification/service/notification_setting_service.h
%{_libdir}/pkgconfig/notification-service.pc
