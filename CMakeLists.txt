###############
# (C) 2012 by Matteo Pasotti <matteo.pasotti@gmail.com>
# License: GPLv3
###############
SET( VERSION_MAJOR "1")
SET( VERSION_MINOR "0" )
SET( VERSION_PATCH "0" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${GIT_SHA1_VERSION}" )

cmake_minimum_required(VERSION 2.8.11)

project(yaflight)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5WebKitWidgets_INCLUDE_DIRS})

# setup some const used by the application at runtime
set( DATA_DIR "/usr/share/yaflight" )
set( TRANSLATIONS_DIR "${DATA_DIR}/languages" )

add_definitions(-DMAX_VERSION=${VERSION_MAJOR})
add_definitions(-DMIN_VERSION=${VERSION_MINOR})
add_definitions(-DVERSION=${VERSION})
add_definitions(-DTRANSDIR="${TRANSLATIONS_DIR}")

set(CMAKE_CXX_FLAGS "-fPIC -std=c++0x")

file(GLOB YF_SOURCES *.cpp)
file(GLOB YF_HEADERS *.h)
file(GLOB YF_RESOURCES *.qrc)
file(GLOB YF_FORMS *.ui)

QT5_WRAP_UI(UIS_HDRS ${YF_FORMS})


add_executable(yaflight ${YF_SOURCES} ${UIS_HDRS})

target_link_libraries(yaflight yalib Qt5::Widgets Qt5::Network Qt5::WebKitWidgets z)
