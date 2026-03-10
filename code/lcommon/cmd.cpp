/*======================================================================
   File: cmd.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-17 17:02:56
   Last Modified by: ksiric
   Last Modified: 2026-03-02 11:19:55
   ---------------------------------------------------------------------
   Description:

   ---------------------------------------------------------------------
   License:
   Company:
   Version: 0.1.0
 ======================================================================
																	   */

#include "cmd.h"
#include <string.h>

static int cmd_argc;
static char *cmd_argv[MAX_CMD_TOKENS];
static char cmd_tokenized[MAX_CMD_LENGTH];
static char cmd_args[MAX_CMD_LENGTH];
static cmd_t *cmd_list = NULL;
static char cmd_buffer[MAX_CMD_BUFFER];
static int cmd_buffer_len = 0;

void Cmd_TokenizeString( const char *text ) {
	char *out;

	cmd_argc = 0;
	cmd_args[0] = '\0';

	if ( !text || text[0] == '\0' ) {
		return;
	}

	// Skip leading slash (for console commands like "/connect")
	if ( text[0] == '/' ) {
		text++;
	}

	out = cmd_tokenized;

	while ( 1 ) {
		// Skip whitespace and control characters
		while ( *text && *text <= ' ' ) {
			text++;
		}

		if ( !*text ) {
			break;
		}

		if ( cmd_argc >= MAX_CMD_TOKENS ) {
			break;
		}

		// Store pointer to this token
		cmd_argv[cmd_argc] = out;

		if ( *text == '"' ) {
			// Quoted string tokenization
			text++;
			while ( *text && *text != '"' ) {
				*out++ = *text++;
			}

			if ( *text == '"' ) {
				text++;
			}
		} else {
			// Regular token (non-whitespace characters)
			while ( *text > ' ' ) {
				*out++ = *text++;
			}
		}

		*out++ = '\0';
		cmd_argc++;
	}
}

int Cmd_Argc( void ) {
	return cmd_argc;
}

char *Cmd_Argv( int n ) {
	if ( n < 0 || n > MAX_CMD_TOKENS ) {
		return (char *)""; // DO not return NULL, instead just return empty string
	}

	return cmd_argv[n];
}

char *Cmd_Args( void ) {
	int i;
	int len;

	cmd_args[0] = '\0';
	len = 0;

	// Start from 1 to skip the command name itself
	for ( i = 1; i < cmd_argc; i++ ) {
		// Add space between arguments
		if ( i > 1 ) {
			if ( len < MAX_CMD_LENGTH - 1 ) {
				cmd_args[len++] = ' ';
			}
		}

		// Copy the argument
		const char *arg = cmd_argv[i];
		while ( *arg && len < MAX_CMD_LENGTH - 1 ) {
			cmd_args[len++] = *arg++;
		}
	}

	cmd_args[len] = '\0';
	return cmd_args;
}

void Cmd_Init( void ) {
	cmd_list = NULL;
	cmd_buffer_len = 0;
	cmd_buffer[0] = '\0';

	Com_Printf( "Command system initialized\n" );
}

void Cmd_Shutdown( void ) {
	cmd_t *cmd, *next;

	cmd = cmd_list;

	while ( cmd ) {
		next = cmd->next;
		free( cmd->name );
		free( cmd ); // points to garbage once we free it
		cmd = next; // we make it point to the next
	}
}

void Cmd_AddCommand( const char *cmdname, cmdfunc_t func ) {
	cmd_t *cmd;

	if ( Cmd_FindCommand( cmdname ) ) {
		Com_Printf( "Cmd_AddCommand: %s already registered!\n", cmdname );
		return;
	}

	// Allocate new node for the command
	cmd = (cmd_t *)malloc( sizeof( cmd_t ) );
	cmd->name = strdup( cmdname );
	cmd->func = func;

	cmd->next = cmd_list;
	cmd_list = cmd;
}

void Cmd_RemoveCommand( const char *cmdname ) {
	cmd_t *cmd, *prev;

	prev = NULL;
	cmd = cmd_list;

	while ( cmd ) {
		if ( strcmp( cmd->name, cmdname ) == 0 ) {
			// Found the command, remove it from the list
			if ( prev ) {
				prev->next = cmd->next;
			} else {
				// It's the head of the list
				cmd_list = cmd->next;
			}

			free( cmd->name );
			free( cmd );
			return;
		}

		prev = cmd;
		cmd = cmd->next;
	}
}

cmdfunc_t Cmd_FindCommand( const char *name ) {
	cmd_t *cmd;

	for ( cmd = cmd_list; cmd; cmd = cmd->next ) {
		if ( strcmp( cmd->name, name ) == 0 ) {
			return cmd->func;
		}
	}
	return NULL;
}

void Cmd_ExecuteString( const char *text ) {
	cmdfunc_t func;

	Cmd_TokenizeString( text );

	// No tokens means nothing to execute
	if ( cmd_argc == 0 ) {
		return;
	}

	// Find and execute the command
	func = Cmd_FindCommand( cmd_argv[0] );
	if ( func ) {
		func();
		return;
	}

	// Command not found
	Com_Printf( "Unknown command: %s\n", cmd_argv[0] );
}

void Cbuf_Init( void ) {
	cmd_buffer[0] = '\0';
	cmd_buffer_len = 0;

	return;
}

void Cbuf_AddText( const char *text ) {
	size_t textLen = strlen( text );

	if ( cmd_buffer_len + textLen >= MAX_CMD_BUFFER ) {
		Com_Printf( "Cbuf_AddText: buffer overflow\n" );
		return;
	}

	memcpy( cmd_buffer + cmd_buffer_len, text, textLen + 1 );
	cmd_buffer_len += textLen;

	return;
}

void Cbuf_InsertText( const char *text ) {
	size_t textLen = strlen( text );

	char *cmdptr = cmd_buffer;

	if ( cmd_buffer_len + textLen >= MAX_CMD_BUFFER ) {
		Com_Printf( "Cbuf_InsertText: buffer overflow\n" );
		return;
	}

	memmove( cmd_buffer + textLen, cmd_buffer, cmd_buffer_len + 1 );

	memcpy( cmdptr, text, textLen );

	cmd_buffer_len += textLen;

	return;
}

void Cbuf_Execute( void ) {
	char *trackPos = cmd_buffer;
	char *trackWord = cmd_buffer;

	while ( *trackPos != '\0' ) {
		// @TODO(KARLO): Need to find
		if ( *trackPos == '\n' || *trackPos == ';' ) {
			*trackPos = '\0';
			Cmd_ExecuteString( trackWord );
			trackWord = trackPos + 1;
		}
		trackPos++;
	}
	if ( trackWord != trackPos ) {
		Cmd_ExecuteString( trackWord );
	}

	cmd_buffer_len = 0;
	cmd_buffer[0] = '\0';

	return;
}
