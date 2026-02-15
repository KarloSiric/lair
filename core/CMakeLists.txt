add_library(lair_core STATIC
    src/log.cpp
    src/error.cpp
    src/socket.cpp
    src/buffer.cpp
    src/protocol.cpp
)

target_include_directories(lair_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lair_core PUBLIC cjson)

# Platform-specific socket libraries
if(WIN32)
    target_link_libraries(lair_core PUBLIC ws2_32)
endif()
