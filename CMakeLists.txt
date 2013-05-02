CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(notification C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR ${LIB_INSTALL_DIR})
SET(INCLUDEDIR "\${prefix}/include/${PROJECT_NAME}")
SET(ICONDIR "${PREFIX}/share/${PROJECT_NAME}")
SET(DBDIR "/opt/dbspace")
SET(DBFILE ".notification.db")
SET(MAJOR_VER 0)
SET(VERSION ${MAJOR_VER}.1.0)

SET(SRCS ./src/notification.c
	./src/notification_noti.c
	./src/notification_ongoing.c
	./src/notification_group.c
	./src/notification_db.c
	./src/notification_list.c
	./src/notification_status.c
	./src/notification_ipc.c)
SET(HEADERS ./include/notification.h 
	./include/notification_error.h 
	./include/notification_type.h 
	./include/notification_list.h
	./include/notification_status.h
	./include/notification_ipc.h
	./include/notification_noti.h)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED 
	sqlite3 
	db-util 
	vconf 
	bundle 
	dlog 
	ail 
	aul 
	appsvc
	dbus-1
	dbus-glib-1
	com-core
)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fvisibility=hidden -g -Wall")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

SET(CMAKE_SKIP_BUILD_RPATH TRUE)

ADD_DEFINITIONS("-DPREFIX=\"${PREFIX}\"")
ADD_DEFINITIONS("-DICONDIR=\"${ICONDIR}\"")
ADD_DEFINITIONS("-DDBDIR=\"${DBDIR}\"")
ADD_DEFINITIONS("-DDBFILE=\"${DBFILE}\"")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION ${MAJOR_VER})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${VERSION})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})

CONFIGURE_FILE(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR} COMPONENT RuntimeLibraries)
INSTALL(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
FOREACH(hfile ${HEADERS})
	INSTALL(FILES ${CMAKE_SOURCE_DIR}/${hfile} DESTINATION include/${PROJECT_NAME})
ENDFOREACH(hfile)

