cmake_minimum_required(VERSION 3.21)
project(lair VERSION 0.2.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/LairOptions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/Dependencies.cmake)

add_subdirectory(deps/cjson)

add_library(lair_platform STATIC
    code/posix/sys_posix.cpp
    code/posix/net_posix.cpp
)
target_include_directories(lair_platform PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code/lcommon
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(lair_platform PROPERTIES FOLDER "legacy")

add_library(lair_common STATIC
    code/lcommon/common.cpp
    code/lcommon/msg.cpp
    code/lcommon/cmd.cpp
)
target_include_directories(lair_common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/code/lcommon
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(lair_common PUBLIC lair_platform cjson)
set_target_properties(lair_common PROPERTIES FOLDER "legacy")

add_library(lair_project_options INTERFACE)
target_compile_features(lair_project_options INTERFACE cxx_std_17)
target_include_directories(lair_project_options INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(LAIR_ENABLE_LOGGING AND TARGET spdlog::spdlog)
    target_link_libraries(lair_project_options INTERFACE spdlog::spdlog)
    target_compile_definitions(lair_project_options INTERFACE LAIR_HAS_SPDLOG=1)
endif()

if(LAIR_ENABLE_SQLITE AND TARGET SQLite::SQLite3)
    target_link_libraries(lair_project_options INTERFACE SQLite::SQLite3)
    target_compile_definitions(lair_project_options INTERFACE LAIR_HAS_SQLITE=1)
endif()

if(LAIR_ENABLE_SODIUM AND TARGET unofficial-sodium::sodium)
    target_link_libraries(lair_project_options INTERFACE unofficial-sodium::sodium)
    target_compile_definitions(lair_project_options INTERFACE LAIR_HAS_LIBSODIUM=1)
endif()

if(LAIR_ENABLE_OPUS AND TARGET Opus::opus)
    target_link_libraries(lair_project_options INTERFACE Opus::opus)
    target_compile_definitions(lair_project_options INTERFACE LAIR_HAS_OPUS=1)
endif()

if(LAIR_ENABLE_SQLCIPHER AND TARGET SQLCipher::SQLCipher)
    target_link_libraries(lair_project_options INTERFACE SQLCipher::SQLCipher)
    target_compile_definitions(lair_project_options INTERFACE LAIR_HAS_SQLCIPHER=1)
endif()

if(LAIR_BUILD_SERVER)
    add_executable(lair_server
        code/server/main.cpp
        code/server/sv_main.cpp
    )
    target_include_directories(lair_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/server
    )
    target_link_libraries(lair_server PRIVATE
        lair_common
        lair_platform
        lair_project_options
    )
    set_target_properties(lair_server PROPERTIES FOLDER "apps")
endif()

if(LAIR_BUILD_CLIENT_TUI)
    find_package(Curses REQUIRED)

    add_executable(lair_client
        code/client-tui/main.cpp
        code/client-tui/tui.cpp
        code/client/cl_main.cpp
        code/client/cl_commands.cpp
    )
    target_include_directories(lair_client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/client-tui
        ${CMAKE_CURRENT_SOURCE_DIR}/code/client
        ${CURSES_INCLUDE_DIR}
    )
    target_link_libraries(lair_client PRIVATE
        lair_common
        lair_platform
        lair_project_options
        ${CURSES_LIBRARIES}
    )
    set_target_properties(lair_client PROPERTIES FOLDER "apps")
endif()

if(LAIR_BUILD_CLIENT_QT)
    if(NOT TARGET Qt6::Core)
        message(FATAL_ERROR "LAIR_BUILD_CLIENT_QT=ON requires Qt 6. Configure with CMAKE_PREFIX_PATH or install Qt 6.")
    endif()

    qt_standard_project_setup()

    qt_add_executable(lair_qt_client
        src/app/client_qt/main.cpp
        src/ui/qt/main_window.cpp
    )
    target_include_directories(lair_qt_client PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(lair_qt_client PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        lair_common
        lair_platform
        lair_project_options
    )
    if(TARGET Qt6::Sql)
        target_link_libraries(lair_qt_client PRIVATE Qt6::Sql)
    endif()
    if(TARGET Qt6::Multimedia)
        target_link_libraries(lair_qt_client PRIVATE Qt6::Multimedia)
    endif()
    set_target_properties(lair_qt_client PROPERTIES FOLDER "apps")
endif()

if(LAIR_ENABLE_TESTS)
    enable_testing()
endif()

message(STATUS "================ LAIR Build Summary ================")
message(STATUS "LAIR_BUILD_SERVER      : ${LAIR_BUILD_SERVER}")
message(STATUS "LAIR_BUILD_CLIENT_TUI  : ${LAIR_BUILD_CLIENT_TUI}")
message(STATUS "LAIR_BUILD_CLIENT_QT   : ${LAIR_BUILD_CLIENT_QT}")
message(STATUS "LAIR_ENABLE_SQLITE     : ${LAIR_ENABLE_SQLITE}")
message(STATUS "LAIR_ENABLE_SODIUM     : ${LAIR_ENABLE_SODIUM}")
message(STATUS "LAIR_ENABLE_OPUS       : ${LAIR_ENABLE_OPUS}")
message(STATUS "LAIR_ENABLE_LOGGING    : ${LAIR_ENABLE_LOGGING}")
message(STATUS "LAIR_ENABLE_TESTS      : ${LAIR_ENABLE_TESTS}")
message(STATUS "====================================================")
