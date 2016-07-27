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
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(dbus-glib-1)
BuildRequires: pkgconfig(appcore-common)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(glib-2.0)

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

%postun -p /sbin/ldconfig

%files
%manifest notification.manifest
%defattr(-,root,root,-)
%{_libdir}/libnotification.so*
%{TZ_SYS_SHARE}/license/%{name}
%attr(755,root,root) %{_sysconfdir}/gumd/useradd.d/11_notification-add.post
%{_bindir}/notification_init
#%{_bindir}/notification-test-app

%files devel
%defattr(-,root,root,-)
%{_includedir}/notification/notification.h
%{_includedir}/notification/notification_db.h
%{_includedir}/notification/notification_internal.h
%{_includedir}/notification/notification_error.h
%{_includedir}/notification/notification_type.h
%{_includedir}/notification/notification_list.h
%{_includedir}/notification/notification_ongoing.h
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
