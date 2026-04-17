option(LAIR_BUILD_SERVER "Build the TCP chat server" ON)
option(LAIR_BUILD_CLIENT_TUI "Build the legacy ncurses client" ON)
option(LAIR_BUILD_CLIENT_QT "Build the Qt desktop client" OFF)

option(LAIR_ENABLE_SQLITE "Enable SQLite-backed persistence" ON)
option(LAIR_ENABLE_SQLCIPHER "Prefer SQLCipher instead of plain SQLite when available" OFF)
option(LAIR_ENABLE_SODIUM "Enable libsodium for authentication and crypto" OFF)
option(LAIR_ENABLE_OPUS "Enable Opus for voice chat" OFF)
option(LAIR_ENABLE_LOGGING "Enable spdlog integration when available" OFF)
option(LAIR_ENABLE_TESTS "Enable unit and integration tests" ON)

