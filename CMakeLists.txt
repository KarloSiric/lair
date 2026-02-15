cmake_minimum_required(VERSION 3.16)
project(lair VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform detection
if(APPLE)
    set(LAIR_PLATFORM_MACOS ON)
    add_compile_definitions(LAIR_PLATFORM_MACOS)
elseif(UNIX)
    set(LAIR_PLATFORM_LINUX ON)
    add_compile_definitions(LAIR_PLATFORM_LINUX)
elseif(WIN32)
    set(LAIR_PLATFORM_WINDOWS ON)
    add_compile_definitions(LAIR_PLATFORM_WINDOWS)
endif()

# Build options
option(LAIR_BUILD_SERVER "Build the server" ON)
option(LAIR_BUILD_CLIENT "Build the GUI client" OFF)
option(LAIR_BUILD_CLIENT_TUI "Build the TUI client" OFF)

# Dependencies
add_subdirectory(deps/cjson)

# Core library
add_subdirectory(core)

# Server
if(LAIR_BUILD_SERVER)
    add_subdirectory(server)
endif()

# Clients
if(LAIR_BUILD_CLIENT)
    add_subdirectory(client)
endif()

if(LAIR_BUILD_CLIENT_TUI)
    add_subdirectory(client-tui)
endif()
