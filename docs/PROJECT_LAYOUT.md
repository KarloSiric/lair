# LAIR Project Layout

This is the target structure we will grow into while preserving the current working code.

## Current structure in use

The active implementation currently lives under `code/`:
- `code/server/` server loop and TCP chat handling
- `code/client/` network client and command system
- `code/client-tui/` ncurses user interface
- `code/lcommon/` shared protocol, command, and common utilities
- `code/posix/` platform networking/system layer

## Target structure while staying in `code/`

### `code/client-qt/`
- Qt desktop client entry point and bootstrap files

### `code/client-tui/`
- Existing ncurses shell while we keep it as legacy or maintenance mode

### `code/core/`
- app services, domain models, room/session managers, shared business logic

### `code/protocol/`
- TCP framing, packet encoding/decoding, versioning, transport contracts

### `code/storage/`
- SQLite/SQLCipher integration
- repositories for users, sessions, rooms, messages, preferences cache

### `code/security/`
- libsodium wrappers
- password hashing, session tokens, key management, encryption helpers

### `code/voice/`
- Opus codec integration
- UDP packetization, jitter buffering, voice session state
- WebRTC audio processing hooks later

### `code/ui/qt/`
- Qt windows, widgets, models, dialogs, view models

### `code/platform/`
- platform abstractions not tied to `code/posix`

### `tests/`
- `unit/` fast tests for protocol, auth, storage, helpers
- `integration/` client/server/database interaction tests

### `cmake/`
- build options
- dependency discovery and future packaging helpers

## Build order we will follow

1. keep the current TCP chat path working.
2. keep a minimal Qt shell and start moving UI work there.
3. fix stream-safe TCP message parsing in protocol.
4. add SQLite persistence and initial sync for users/history.
5. add authentication and sessions with libsodium.
6. add settings/preferences and OS keychain integration.
7. add UDP voice with Opus.
8. add transport and local security hardening.
