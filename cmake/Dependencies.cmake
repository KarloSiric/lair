include(FetchContent)

if(LAIR_BUILD_CLIENT_QT)
    # Some macOS/Homebrew environments require an explicit prefix hint for CMake
    # to discover the config files when Qt is installed outside default paths.
    set(_lair_qt_search_prefixes
        /opt/homebrew/opt/qt/lib/cmake
        /opt/homebrew/opt/qt@6/lib/cmake
        /opt/homebrew/lib/cmake
        /usr/local/opt/qt@6/lib/cmake
        /usr/local/opt/qt/lib/cmake
        /usr/local/lib/cmake
        /opt/local/lib/cmake
    )

    set(_lair_qt_prefixes)
    set(_lair_qt_dir_candidates)
    foreach(_lair_qt_prefix IN LISTS _lair_qt_search_prefixes)
        if(EXISTS "${_lair_qt_prefix}/Qt6")
            list(APPEND _lair_qt_prefixes "${_lair_qt_prefix}")
        endif()
        if(EXISTS "${_lair_qt_prefix}/Qt6/Qt6Config.cmake")
            list(APPEND _lair_qt_dir_candidates "${_lair_qt_prefix}/Qt6/Qt6Config.cmake")
        endif()
    endforeach()
    list(REMOVE_DUPLICATES _lair_qt_prefixes)
    list(REMOVE_DUPLICATES _lair_qt_dir_candidates)

    if(_lair_qt_dir_candidates)
        list(GET _lair_qt_dir_candidates 0 _lair_qt_config_file)
        set(Qt6_DIR "${_lair_qt_config_file}" CACHE PATH "Path to Qt6Config.cmake" FORCE)
    endif()

    if(_lair_qt_prefixes)
        list(PREPEND CMAKE_PREFIX_PATH ${_lair_qt_prefixes})
    endif()

    # Keep multimedia optional for now; GUI can build as long as core widgets stack exists.
    find_package(
        Qt6
        6.5
        COMPONENTS Core Gui Widgets Network Sql
        QUIET
        HINTS ${_lair_qt_prefixes}
        PATHS ${_lair_qt_prefixes}
    )

    if(Qt6_FOUND)
        message(STATUS "Found Qt6 for GUI client")
    else()
        message(STATUS "Qt6 not found. Tried:"
            " ${_lair_qt_prefixes}"
            "Set CMAKE_PREFIX_PATH explicitly when needed, e.g. -DCMAKE_PREFIX_PATH=/opt/homebrew/lib/cmake."
        )
    endif()

    find_package(
        Qt6Multimedia
        QUIET
        HINTS ${_lair_qt_prefixes}
        PATHS ${_lair_qt_prefixes}
    )
    if(Qt6Multimedia_FOUND)
        message(STATUS "Found optional Qt6Multimedia for voice features")
    else()
        message(STATUS "Qt6Multimedia not found; voice-related Qt features will be disabled.")
    endif()
endif()

if(LAIR_ENABLE_SQLCIPHER)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SQLCIPHER QUIET sqlcipher)
        if(SQLCIPHER_FOUND)
            add_library(SQLCipher::SQLCipher INTERFACE IMPORTED)
            target_include_directories(SQLCipher::SQLCipher INTERFACE ${SQLCIPHER_INCLUDE_DIRS})
            target_link_directories(SQLCipher::SQLCipher INTERFACE ${SQLCIPHER_LIBRARY_DIRS})
            target_link_libraries(SQLCipher::SQLCipher INTERFACE ${SQLCIPHER_LIBRARIES})
        endif()
    endif()
endif()

if(LAIR_ENABLE_SQLITE AND NOT TARGET SQLCipher::SQLCipher)
    find_package(SQLite3 QUIET)
    if(SQLite3_FOUND)
        message(STATUS "Found SQLite3")
    else()
        message(STATUS "SQLite3 not found. Persistence modules will stay scaffolded until installed.")
    endif()
endif()

if(LAIR_ENABLE_SODIUM)
    find_package(unofficial-sodium QUIET)
    if(unofficial-sodium_FOUND)
        message(STATUS "Found libsodium")
    else()
        message(STATUS "libsodium not found. Security modules will stay scaffolded until installed.")
    endif()
endif()

if(LAIR_ENABLE_OPUS)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(OPUS QUIET opus)
        if(OPUS_FOUND)
            add_library(Opus::opus INTERFACE IMPORTED)
            target_include_directories(Opus::opus INTERFACE ${OPUS_INCLUDE_DIRS})
            target_link_directories(Opus::opus INTERFACE ${OPUS_LIBRARY_DIRS})
            target_link_libraries(Opus::opus INTERFACE ${OPUS_LIBRARIES})
            message(STATUS "Found Opus")
        else()
            message(STATUS "Opus not found. Voice modules will stay scaffolded until installed.")
        endif()
    endif()
endif()

if(LAIR_ENABLE_LOGGING)
    find_package(spdlog CONFIG QUIET)
    if(spdlog_FOUND)
        message(STATUS "Found spdlog")
    else()
        message(STATUS "spdlog not found. Logging will use Com_Printf until installed.")
    endif()
endif()
