# LAIR

Lightweight, self-hosted voice and text chat application.

---

## Overview

LAIR is a cross-platform communication system built from the ground up in C++. The networking architecture and code conventions draw heavily from id Software's Quake III Arena — the same battle-tested patterns that powered online gaming for millions of players.

This project implements real-time text chat over TCP with a custom binary protocol, extensible command system, and terminal-based interface. Voice communication over UDP and a Qt-based GUI are planned.

---

## Screenshots

<!-- Add screenshots here -->

---

## Features

### Current Implementation
- Real-time TCP text chat supporting multiple concurrent clients
- Custom binary message protocol with type-safe serialization
- Terminal User Interface built on ncurses with Unicode box drawing
- Tab-based navigation: Chat, Private Messages, Friends, Settings
- Extensible command system with hash table lookups
- Non-blocking network I/O with configurable timeouts
- Color-coded message types: chat, system, error
- Scrollable chat history with timestamp display
- Dynamic terminal resize handling
- Cross-platform socket abstraction (POSIX/Win32)

### Planned
- UDP voice communication with Opus codec
- Qt-based graphical interface (ImGui integration prepared)
- End-to-end encryption
- Private messaging system
- Friends list with presence tracking
- Channel/room support
- User authentication and persistence
- Server-side moderation tools

---

## Architecture

LAIR follows a client-server model using TCP for reliable text delivery:

```
                    ┌─────────────────────────────────────────┐
                    │             lair_server                 │
                    │  ┌───────────────────────────────────┐  │
                    │  │         Client Manager            │  │
                    │  │  ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐  │  │
                    │  │  │ C0  │ │ C1  │ │ C2  │ │ ... │  │  │
                    │  │  └──┬──┘ └──┬──┘ └──┬──┘ └──┬──┘  │  │
                    │  └─────┼──────┼──────┼──────┼───────┘  │
                    └────────┼──────┼──────┼──────┼──────────┘
                             │      │      │      │
                         TCP │      │      │      │
                             │      │      │      │
              ┌──────────────┴──────┴──────┴──────┴──────────────┐
              │                                                  │
     ┌────────┴────────┐                              ┌──────────┴────────┐
     │   lair_client   │                              │    lair_client    │
     │  ┌───────────┐  │                              │  ┌─────────────┐  │
     │  │    TUI    │  │                              │  │   Qt GUI    │  │
     │  │  ncurses  │  │                              │  │  (planned)  │  │
     │  └───────────┘  │                              │  └─────────────┘  │
     └─────────────────┘                              └───────────────────┘
```

### Message Protocol

Communication uses a compact binary protocol optimized for low overhead:

```
┌──────────┬────────────────────────────────┐
│  Byte 0  │  Bytes 1..N                    │
├──────────┼────────────────────────────────┤
│  Type    │  Type-specific payload         │
└──────────┴────────────────────────────────┘
```

Message types include connection handshake, user management, text chat, channel operations, and voice data frames.

### Command System

The client implements an id Tech-style command system with:
- Tokenizer for parsing command strings
- Hash table for O(1) command lookups
- Command buffer for queued execution
- Extensible registration API

---

## Controls

### Client Commands

| Command | Description |
|---------|-------------|
| `/connect <host> [port]` | Connect to a server |
| `/disconnect` | Disconnect from current server |
| `/name <username>` | Set or change username |
| `/users` | List connected users |
| `/help` | Show available commands |
| `/quit` | Exit the application |

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `Tab` | Next tab |
| `Shift+Tab` | Previous tab |
| `Ctrl+D` | Scroll up (older messages) |
| `Ctrl+U` | Scroll down (newer messages) |
| `Escape` | Quit prompt |

---

## System Requirements

- **OS**: macOS, Linux, Windows
- **Compiler**: C++17 compatible (GCC, Clang, MSVC)
- **Build**: CMake 3.16+
- **Dependencies**: ncurses (TUI client)

---

## Build Instructions

### macOS

```bash
# Install dependencies
brew install cmake ncurses

# Build
mkdir build && cd build
cmake ..
make

# Run server
./lair_server

# Run client (separate terminal)
./lair_client
```

### Linux (Debian/Ubuntu)

```bash
# Install dependencies
sudo apt install build-essential cmake libncurses-dev

# Build
mkdir build && cd build
cmake ..
make
```

### Linux (Arch)

```bash
# Install dependencies
sudo pacman -S cmake ncurses

# Build
mkdir build && cd build
cmake ..
make
```

### Build Options

```bash
cmake -DLAIR_BUILD_SERVER=ON ..      # Build server (default: ON)
cmake -DLAIR_BUILD_CLIENT_TUI=ON ..  # Build TUI client (default: ON)
cmake -DLAIR_BUILD_CLIENT=OFF ..     # Build GUI client (default: OFF)
```

---

## Project Structure

```
lair/
├── code/
│   ├── lcommon/        # Shared library
│   │   ├── l_shared.h  # Type definitions and constants
│   │   ├── msg.cpp/h   # Message serialization
│   │   ├── cmd.cpp/h   # Command system
│   │   └── common.cpp  # Utility functions
│   ├── server/         # Server implementation
│   │   ├── sv_main.cpp # Server main loop
│   │   └── server.h    # Server structures
│   ├── client/         # Client networking
│   │   ├── cl_main.cpp # Client connection handling
│   │   └── cl_commands.cpp
│   ├── client-tui/     # ncurses interface
│   │   ├── tui.cpp     # Terminal UI implementation
│   │   └── tui.h       # UI state and drawing
│   ├── posix/          # POSIX network/system layer
│   └── win32/          # Windows network/system layer
├── deps/
│   ├── cjson/          # JSON parsing
│   └── imgui/          # GUI toolkit (future)
├── CMakeLists.txt
└── LICENSE
```

---

## Technical Details

### Network Layer
- Non-blocking sockets with select-based multiplexing
- Configurable connection timeouts
- Automatic reconnection handling
- Platform abstraction for POSIX and Win32

### Message Types
- Connection: `CONNECT`, `CONNECT_ACCEPTED`, `CONNECT_DENIED`, `DISCONNECT`
- Users: `USERJOIN`, `USERLEAVE`, `USERINFO`, `USERLIST`, `USERKICK`, `USERBAN`
- Chat: `CHAT`, `CHAT_PRIVATE`, `CHAT_CHANNEL`, `CHAT_SERVER`
- Channels: `CHANNELLIST`, `CHANNELJOIN`, `CHANNELLEAVE`, `CHANNELCREATE`
- Voice: `VOICEDATA`, `VOICESTART`, `VOICESTOP` (planned)

### Limits
- Max clients: 256
- Max message length: 4096 bytes
- Max username: 32 characters
- Default port: 7331

---

## Status

Active development. Core text chat functionality complete. Voice and GUI in progress.

---

## License

GNU General Public License v2.0. See [LICENSE](LICENSE) for details.

This license was chosen for compatibility with code patterns derived from Quake III Arena.

---

## Contributing

Contributions welcome. Follow the existing code style:
- Tabs for indentation
- Spaces around operators and after keywords
- Opening braces on same line
- Descriptive function and variable names

---

## Acknowledgments

Architecture and conventions influenced by id Software's Quake III Arena source code release.

---

## Author

Karlo Siric
