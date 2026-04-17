# LAIR Project Layout

This is the target structure we will grow into while preserving the current working code.

## Current state

The active implementation still lives under `code/`:
- `code/server/` server loop and TCP chat handling
- `code/client/` network client and slash commands
- `code/client-tui/` ncurses user interface
- `code/lcommon/` shared protocol, command, and common utilities
- `code/posix/` platform networking/system layer

## Target structure

### `src/app/`
- `client_qt/` Qt desktop entry point and application bootstrap
- `client_tui/` future home for the maintained TUI app shell
- `server/` future home for the production server bootstrap and runtime wiring

### `src/core/`
- app services, domain models, room/session managers, shared business logic

### `src/protocol/`
- TCP framing, packet encoding/decoding, versioning, transport contracts

### `src/storage/`
- SQLite/SQLCipher integration
- repositories for users, sessions, rooms, messages, preferences cache

### `src/security/`
- libsodium wrappers
- password hashing, session tokens, key management, encryption helpers

### `src/voice/`
- Opus codec integration
- UDP packetization, jitter buffering, voice session state
- WebRTC audio processing hooks later

### `src/ui/qt/`
- Qt windows, widgets, models, dialogs, view models, themes

### `src/platform/`
- future platform abstractions that are not tied to the legacy `code/posix` layout

### `include/lair/`
- stable public headers as modules mature

### `tests/`
- `unit/` fast tests for protocol, auth, storage, helpers
- `integration/` client/server/database interaction tests

### `cmake/`
- build options
- dependency discovery and future packaging helpers

## Build order we will follow

1. Keep the current TCP chat path working.
2. Add Qt client shell and begin migrating UI work there.
3. Replace stream-unsafe TCP reads with proper framed messages.
4. Add SQLite persistence and initial sync for users/history.
5. Add authentication and sessions with libsodium.
6. Add settings, preferences, and OS keychain integration.
7. Add UDP voice with Opus.
8. Add stronger transport security and local encryption layers.
