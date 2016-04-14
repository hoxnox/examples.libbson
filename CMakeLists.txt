# @author hoxnox <hoxnox@gmail.com>
# @date 20160414 22:34:41
# examples.libbson cmake build script

cmake_minimum_required(VERSION 2.6.4)

########################################################################
# options

option(WITH_TESTS          "Build tests."  OFF)
option(WITH_DOCS           "Generate docs" OFF)
option(WITH_SYSTEM_LIBBSON "Use system libbson, don't build it" OFF)

########################################################################
# general

project(examples.libbson)

set(examples.libbson_VERSION_MAJOR 0)
set(examples.libbson_VERSION_MINOR 0)
set(examples.libbson_VERSION_PATCH 0)
# configuration header
configure_file(
	"${PROJECT_SOURCE_DIR}/src/examples.libbson_config.cfg"
	"${PROJECT_SOURCE_DIR}/src/examples.libbson_config.h"
	ESCAPE_QUOTES
)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}"
                      "${PROJECT_SOURCE_DIR}/cmake/Modules")

include_directories("${PROJECT_SOURCE_DIR}/src")
add_definitions("-std=c++11")

########################################################################
# libbson

find_package(Threads)
list(APPEND LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

if (NOT WITH_SYSTEM_LIBBSON)
	include(ExternalProject)
	if(NOT LIBBSON_SRC)
		set(LIBBSON_SRC $ENV{LIBBSON})
	endif()
	if(NOT LIBBSON_SRC)
		set(LIBBSON_SRC https://github.com/mongodb/libbson/releases/download/1.3.5/libbson-1.3.5.tar.gz)
	endif()
	ExternalProject_Add(
		libbson
		URL ${LIBBSON_SRC}
		PREFIX "${CMAKE_CURRENT_BINARY_DIR}/libbson"
		CONFIGURE_COMMAND ./configure --enable-static --disable-shared --prefix <INSTALL_DIR>
		                  --enable-html-docs=no --enable-man-pages=no --enable-yelp=no
		BUILD_IN_SOURCE 1
		LOG_DOWNLOAD 1
		LOG_UPDATE 1
		LOG_CONFIGURE 1
		LOG_BUILD 1
		LOG_TEST 1
		LOG_INSTALL 1
	)
	set(LIBBSON_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/libbson/include/libbson-1.0")
	set(LIBBSON_LIBRARIES "${CMAKE_CURRENT_BINARY_DIR}/libbson/lib/libbson-1.0.a")
else ()
    find_package(LIBBSON REQUIRED) # TODO: FindLIBBSON.cmake
endif()
include_directories(${LIBBSON_INCLUDE_DIRS})
list(APPEND LIBRARIES ${LIBBSON_LIBRARIES})

########################################################################
# examples.libbson

add_executable(examples.libbson src/examples.libbson.cpp)
add_dependencies(examples.libbson libbson)
target_link_libraries(examples.libbson ${LIBRARIES})

########################################################################
# tests and docs

if(WITH_DOCS)
	add_subdirectory(doc)
endif()

if(WITH_TESTS)
	enable_testing()
	add_subdirectory(test)
endif()

########################################################################
# installation

set(CPACK_SET_DESTDIR ON)

#INSTALL(TARGETS
#	targetname
#	DESTINATION bin)
#INSTALL(FILES filepath DESTINATION bin
#	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
#	            GROUP_EXECUTE GROUP_READ
#	            WORLD_READ WORLD_EXECUTE)
#INSTALL(DIRECTORY domedir DESTINATION share/${PROJECT_NAME})
SET(CPACK_PACKAGE_NAME examples.libbson)
#if (WITH_SYSTEM_SOMELIB)
#	SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsomelib (>=1.1.1)")
#	SET(CPACK_RPM_PACKAGE_REQUIRES "libsomelib >= 1.1.1")
#endif()
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "")
SET(CPACK_PACKAGE_VENDOR "$uername$ <hoxnox@gmail.com>")
SET(CPACK_PACKAGE_CONTACT ${CPACK_PACKAGE_VENDOR})
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.markdown")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
SET(CPACK_PACKAGE_VERSION_MAJOR ${examples.libbson_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${examples.libbson_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${examples.libbson_VERSION_PATCH})
SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
SET(CPACK_RPM_PACKAGE_GROUP "Applications/System")
SET(CPACK_RPM_PACKAGE_LICENSE "MIT")
SET(CPACK_STRIP_FILES TRUE)
INCLUDE(CPack)
