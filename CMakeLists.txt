cmake_minimum_required(VERSION 3.16)
project(lair VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Build options
option(LAIR_BUILD_SERVER "Build the server" ON)
option(LAIR_BUILD_CLIENT "Build the GUI client" OFF)
option(LAIR_BUILD_CLIENT_TUI "Build the TUI client" OFF)


# Include paths - so we can just #include "l_shared.h"
include_directories(code/lcommon)


# Dependencies
add_subdirectory(deps/cjson)


# Platform library (posix or win32)
if(WIN32)
    add_library(lair_platform STATIC
        code/win32/sys_win32.cpp
        code/win32/net_win32.cpp
    )
    target_link_libraries(lair_platform ws2_32)
else()
    add_library(lair_platform STATIC
        code/posix/sys_posix.cpp
        code/posix/net_posix.cpp
    )
endif()

target_include_directories(lair_platform PUBLIC code/lcommon)


# Common library
add_library(lair_common STATIC
    code/lcommon/lcommon.cpp
)

target_include_directories(lair_common PUBLIC code/lcommon)
target_link_libraries(lair_common lair_platform cjson)


# Server
if(LAIR_BUILD_SERVER)
    add_executable(lair_server
        code/server/main.cpp
    )
    target_link_libraries(lair_server lair_common lair_platform)
endif()


# Clients (for later)
if(LAIR_BUILD_CLIENT)
    # add_subdirectory(code/client)
endif()

if(LAIR_BUILD_CLIENT_TUI)
    # add_subdirectory(code/client-tui)
endif()
