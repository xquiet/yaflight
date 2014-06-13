###############
# (C) 2014 by Matteo Pasotti <matteo.pasotti@gmail.com>
# License: GPLv3
###############
SET( VERSION_MAJOR "0")
SET( VERSION_MINOR "99" )
SET( VERSION_PATCH "28" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${GIT_SHA1_VERSION}" )

cmake_minimum_required(VERSION 2.8.11)

project(yaflight)

# Debug, RelWithDebInfo, Release
# set(CMAKE_BUILD_TYPE Debug)

# Find includes in corresponding build directories
# usefull for MOC
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtCore library - Not Really Needed as Qt5Widgets already requires it!
find_package(Qt5Core REQUIRED)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
# Find the Qt5Network library
find_package(Qt5Network REQUIRED)
# Find the Qt5WebKitWidgets library
find_package(Qt5WebKitWidgets REQUIRED)
# Find the Qt5LinguistTools library (libqt5help-devel)
find_package(Qt5LinguistTools REQUIRED)
# TODO: check if qttools5 is installed (lrelease)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
# NOTE: using qt5_use_modules makes include_directories droppable
# include_directories(${Qt5Widgets_INCLUDE_DIRS})
# include_directories(${Qt5Network_INCLUDE_DIRS})
# include_directories(${Qt5WebKitWidgets_INCLUDE_DIRS})

if(NOT DATA_DIR)
set( DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}" )
endif(NOT DATA_DIR)
if(NOT TRANSLATIONS_DIR)
set( TRANSLATIONS_DIR "${DATA_DIR}/languages" )
endif(NOT TRANSLATIONS_DIR)
if(NOT THEMES_DIR)
set( THEMES_DIR "${DATA_DIR}/themes" )
endif(NOT THEMES_DIR)

message( STATUS "DATA_DIR: ${DATA_DIR}" )
message( STATUS "TRANSLATIONS_DIR: ${TRANSLATIONS_DIR}" )
message( STATUS "THEMES_DIR: ${THEMES_DIR}" )

add_definitions(-DMAX_VERSION=${VERSION_MAJOR})
add_definitions(-DMIN_VERSION=${VERSION_MINOR})
add_definitions(-DPATCH_VERSION=${VERSION_PATCH})
add_definitions(-DVERSION="${VERSION}")
add_definitions(-DDATA_DIR="${DATA_DIR}")
add_definitions(-DTRANSDIR="${TRANSLATIONS_DIR}")
add_definitions(-DTHEMES_DIR="${THEMES_DIR}")

# if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++0x")
# endif()

file(GLOB YF_SOURCES *.cpp)
file(GLOB YF_HEADERS *.h)
file(GLOB YF_RESOURCES *.qrc)
file(GLOB YF_FORMS *.ui)
file(GLOB TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/languages/*.ts)
file(GLOB YF_THEMES ${CMAKE_CURRENT_SOURCE_DIR}/themes/*.qss)

QT5_WRAP_UI(UIS_HDRS ${YF_FORMS})

qt5_add_resources(RC_ADDED ${YF_RESOURCES})

qt5_add_translation( QM_FILES ${TS_FILES} )

add_executable(yaflight ${YF_SOURCES} ${UIS_HDRS} ${RC_ADDED} ${QM_FILES})

qt5_use_modules( yaflight Widgets LinguistTools Network WebKitWidgets)

if(WIN32)
    include_directories("../yalib/src")
endif(WIN32)

if(UNIX)
    # NOTE: using qt5_use_modules target_link_libraries can refer just to the other libraries
    # target_link_libraries(yaflight yalib Qt5::Widgets Qt5::Network Qt5::WebKitWidgets z)
    target_link_libraries(yaflight yalib z)
elseif(WIN32)
    #Determine Yalib library
    FIND_LIBRARY(YALIB_LIBRARY NAMES yalib
         PATH_SUFFIXES lib
         PATHS ../yalib-build/src                 # Extension to localize MinGW root
         DOC "The file that corresponds to the base il library."
    )
    MESSAGE("Found YaLib library at: ${YALIB_LIBRARY}")
    target_link_libraries(yaflight ${YALIB_LIBRARY} z)
endif()

install( TARGETS yaflight DESTINATION bin )
install( FILES ${QM_FILES} DESTINATION ${TRANSLATIONS_DIR} )
install( FILES ${YF_THEMES} DESTINATION ${THEMES_DIR} )
