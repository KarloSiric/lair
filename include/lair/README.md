# LAIR Include Layout

This directory is reserved for the long-term public headers of the project.

Planned header groups:
- `lair/core/` for shared app services and models
- `lair/protocol/` for message framing and transport contracts
- `lair/storage/` for database interfaces
- `lair/security/` for authentication and cryptography helpers
- `lair/voice/` for voice transport and codec interfaces
- `lair/ui/` for GUI-facing interfaces and adapters

The current codebase still uses `code/lcommon` and legacy includes while the migration is in progress.

