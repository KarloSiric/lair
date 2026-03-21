# LAIR

Lightweight, self-hosted voice and text chat application.

LAIR is a cross-platform communication system inspired by the networking architecture of id Software's Quake III Arena. It provides real-time text chat with planned support for voice communication over UDP.

## Features

### Current
- TCP-based text chat with multiple clients
- Terminal User Interface (TUI) with ncurses
- Tab-based navigation (Chat, Private Messages, Friends, Settings)
- Command system with extensible architecture
- Non-blocking network connections with timeout handling
- Message types (Chat, System, Error) with color coding
- Scrollable chat history
- Terminal resize support

### Planned
- UDP voice chat
- Qt-based graphical user interface
- End-to-end encryption
- Private messaging
- Friends list and presence system
- Group/channel support
- User authentication and persistence

## Building

### Requirements
- C++11 compatible compiler
- CMake 3.10 or higher
- ncurses library (for TUI client)
- POSIX-compliant operating system (Linux, macOS, BSD)

### Compilation

```sh
mkdir build
cd build
cmake ..
make
```

This produces two executables:
- `lair_server` - The chat server
- `lair_client` - The TUI chat client

## Usage

### Starting the Server

```sh
./lair_server
```

The server listens on port 7331 by default.

### Starting the Client

```sh
./lair_client
```

On startup, enter the server address and port, then choose a username.

### Client Commands

| Command | Description |
|---------|-------------|
| `/connect <host> [port]` | Connect to a server |
| `/disconnect` | Disconnect from server |
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

## Architecture

LAIR follows a client-server model using TCP for reliable text communication:

```
┌─────────────────┐     TCP      ┌─────────────────┐
│   lair_client   │◄────────────►│   lair_server   │
└─────────────────┘              └─────────────────┘
        │                                │
        │                                │
   ┌────┴────┐                    ┌──────┴──────┐
   │   TUI   │                    │   Clients   │
   │ ncurses │                    │   Array     │
   └─────────┘                    └─────────────┘
```

### Code Structure

```
lair/
├── code/
│   ├── client/          # Client networking logic
│   ├── client-tui/      # ncurses TUI implementation
│   ├── server/          # Server implementation
│   ├── lcommon/         # Shared utilities (msg, cmd, etc.)
│   └── posix/           # Platform-specific networking
├── deps/                # Third-party dependencies
└── build/               # Build output
```

### Message Protocol

Communication uses a simple binary protocol:
- First byte: message type
- Remaining bytes: type-specific payload

Message types include: `MSG_CONNECT`, `MSG_DISCONNECT`, `MSG_CHAT`, `MSG_USERJOIN`, `MSG_USERLEAVE`, etc.

## Acknowledgments

This project's architecture and coding conventions are heavily influenced by id Software's Quake III Arena source code release. The networking model, command system, and general code structure follow patterns established in that codebase.

## License

This project is licensed under the GNU General Public License v2.0 - see the [LICENSE](LICENSE) file for details.

This license was chosen for compatibility with code patterns derived from Quake III Arena, which was released under GPL-2.0.

## Contributing

Contributions are welcome. Please ensure your code follows the existing style:
- Tabs for indentation
- Spaces around operators and after keywords
- Opening braces on same line
- Descriptive function and variable names

## Author

Karlo Siric
