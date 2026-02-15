add_executable(lair_server
    src/main.cpp
    src/server.cpp
    src/client_session.cpp
    src/room.cpp
)

target_include_directories(lair_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(lair_server PRIVATE lair_core)
