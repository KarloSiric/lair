/*======================================================================
   File: tui.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 09:59:38
   Last Modified by: ksiric
   Last Modified: 2026-02-28 22:47:42
   ---------------------------------------------------------------------
   Description:

   ---------------------------------------------------------------------
   License:
   Company:
   Version: 0.1.0
 ======================================================================
																	   */
#include "tui.h"
#include "client.h"
#include <string.h>
#include <time.h>

static int rows;
static int cols;

static tuistate_t tui_state = STATE_CONNECT;

static char connect_ip[64];
static char connect_port[8];
static int connect_field = 0;

static char username[64];

static int quit_selection = 1;
static lboolean tui_running = lfalse;
static lboolean show_quit_prompt = lfalse;

static lboolean show_name_error = lfalse;

static char chat_input[512];
static size_t chat_input_len;

static chatmsg_t chat_messages[MAX_CHAT_MESSAGES];
static int chat_message_count = 0;

static lboolean colors_initialized = lfalse;

static WINDOW *main_win;
static WINDOW *status_win;
static WINDOW *chat_win;
static WINDOW *input_win;
static WINDOW *connect_win;
static WINDOW *quit_win;
static WINDOW *name_win;

void TUI_Init( void ) {
	initscr(); // initializing the ncurses library
	main_win = stdscr;
	keypad( main_win, TRUE );
	cbreak(); // used for line breaking, to make the TUI responsive
	noecho(); // no echo used for controlling what gets printed
	wtimeout( main_win, 50 );

	getmaxyx( main_win, rows, cols ); // used to make the terminal ncurses look match the size of the terminal window

	int con_y = ( rows - CONNECT_WIN_HEIGHT ) / 2;
	int con_x = ( cols - CONNECT_WIN_WIDTH ) / 2;

	int quit_y = ( rows - QUIT_WIN_HEIGHT ) / 2;
	int quit_x = ( cols - QUIT_WIN_WIDTH ) / 2;

	int name_y = ( rows - NAME_WIN_HEIGHT ) / 2;
	int name_x = ( cols - NAME_WIN_WIDTH ) / 2;

	status_win = newwin( 1, cols, 0, 0 ); // status window creation
	chat_win = newwin( rows - 2, cols, 1, 0 ); // chat window creation
	input_win = newwin( 1, cols, rows - 1, 0 ); // input window creation
	keypad( input_win, TRUE );
	wtimeout( input_win, 50 );

	connect_win = newwin( CONNECT_WIN_HEIGHT, CONNECT_WIN_WIDTH, con_y, con_x );
	keypad( connect_win, TRUE );
	wtimeout( connect_win, 50 );

	quit_win = newwin( QUIT_WIN_HEIGHT, QUIT_WIN_WIDTH, quit_y, quit_x );
	keypad( quit_win, TRUE );
	wtimeout( quit_win, 50 );

	name_win = newwin( NAME_WIN_HEIGHT, NAME_WIN_WIDTH, name_y, name_x );
	keypad( name_win, TRUE );
	wtimeout( name_win, 50 );

	scrollok( chat_win, TRUE ); // enables scrolling in the chat window, to see history of messages and so forth

	// Initializing the chat input and all
	chat_input[0] = '\0';
	chat_input_len = 0;

	tui_running = ltrue;
	refresh();

	return;
}

void TUI_Shutdown( void ) {
	delwin( status_win );
	delwin( chat_win );
	delwin( input_win );
	endwin();

	show_quit_prompt = lfalse;
	tui_running = lfalse;

	return;
}

void TUI_DrawStatusBar( void ) {
	werase( status_win );
	if ( cl.connected ) {
		mvwprintw( status_win, 0, 0, "LAIR - Connected to %s:%d", connect_ip, atoi( connect_port ) );
	} else {
		mvwprintw( status_win, 0, 0, "LAIR - Disconnected" );
	}

	wrefresh( status_win );
	return;
}

void TUI_DrawInputLine( void ) {
	werase( input_win );
	mvwprintw( input_win, 0, 0, "> %s", chat_input );
	wrefresh( input_win );

	return;
}

void TUI_DrawChatWindow( void ) {
	int win_h, win_w;
	int visible_messages = 0;
	int message_start = 0;
	int line = 1;
	chatmsg_t *msg;
	getmaxyx( chat_win, win_h, win_w );
	werase( chat_win );
	box( chat_win, 0, 0 );

	visible_messages = win_h - 2;

	if ( chat_message_count > visible_messages ) {
		message_start = chat_message_count - visible_messages;
	}
	for ( int i = message_start; i < chat_message_count && line < win_h - 1; i++ ) {
		msg = &chat_messages[i];
		mvwprintw( chat_win, line, 2, "%s %s: %s",
				   msg->timestamp, msg->sender, msg->text );
		line++;
	}

	wrefresh( chat_win );
	return;
}

lboolean TUI_HandleInput( void ) {
	int ch = wgetch( input_win );
	if ( ch == ERR ) {
		return ltrue;
	}
	if ( ch == 27 ) {
		show_quit_prompt = ltrue;
		return ltrue;
	} else if ( ch == '\n' || ch == KEY_ENTER ) {
		if ( chat_input_len > 0 ) {
			CL_SendChat( chat_input );
			chat_input[0] = '\0';
			chat_input_len = 0;
		}
	} else if ( ch == KEY_BACKSPACE || ch == 127 ) {
		if ( chat_input_len > 0 ) {
			chat_input_len--;
			chat_input[chat_input_len] = '\0';
		}
	}

	else if ( ch >= 32 && ch <= 126 ) {
		if ( chat_input_len < 255 ) {
			chat_input[chat_input_len] = ch;
			chat_input_len++;
			chat_input[chat_input_len] = '\0';
		}
	}

	return ltrue;
}

void TUI_DrawConnectScreen( void ) {
	werase( connect_win );
	box( connect_win, 0, 0 );

	mvwprintw( connect_win, 2, 8, "W E L C O M E   T O " );
	mvwprintw( connect_win, 3, 14, "L A I R " );

	// Server field
	mvwprintw( connect_win, 5, 4, "Server:" );
	mvwprintw( connect_win, 5, 12, "[                        ]" );
	mvwprintw( connect_win, 5, 13, "%s", connect_ip );

	// Port field
	mvwprintw( connect_win, 7, 4, "Port:" );
	mvwprintw( connect_win, 7, 12, "[        ]" );
	mvwprintw( connect_win, 7, 13, "%s", connect_port );

	// Highlight active field (reverse video)
	if ( connect_field == 0 ) {
		mvwchgat( connect_win, 5, 12, 26, A_REVERSE, 0, NULL );
	} else {
		mvwchgat( connect_win, 7, 12, 10, A_REVERSE, 0, NULL );
	}

	// Connect button
	mvwprintw( connect_win, 9, 15, "[ CONNECT ]" );

	// Help text
	mvwprintw( connect_win, 11, 4, "TAB: switch   ENTER: connect" );

	wrefresh( connect_win );

	return;
}

void TUI_HandleConnectScreenInput( void ) {
	int ch;
	int len;

	ch = wgetch( connect_win );
	if ( ch == ERR ) {
		return;
	}

	// switched the fields itself right
	if ( ch == '\t' || ch == KEY_UP || ch == KEY_DOWN ) {
		connect_field = ( connect_field == 0 ) ? 1 : 0;
	}
	if ( ch == 27 ) {
		show_quit_prompt = ltrue;
		return;
	} else if ( ch == '\n' || ch == KEY_ENTER ) {
		CL_Connect( connect_ip, atoi( connect_port ) );
		if ( cl.connected ) {
			tui_state = STATE_NAME;
		}
	} else if ( ch == KEY_BACKSPACE || ch == 127 ) {
		if ( connect_field == 0 ) {
			len = strlen( connect_ip );
			if ( len > 0 ) {
				connect_ip[len - 1] = '\0';
			}
		} else {
			len = strlen( connect_port );
			if ( len > 0 ) {
				connect_port[len - 1] = '\0';
			}
		}
	} else if ( ch >= 32 && ch <= 126 ) {
		if ( connect_field == 0 ) {
			len = strlen( connect_ip );
			if ( len < 63 ) {
				connect_ip[len] = ch;
				connect_ip[len + 1] = '\0';
			}
		} else {
			len = strlen( connect_port );
			if ( len < 7 ) {
				connect_port[len] = ch;
				connect_port[len + 1] = '\0';
			}
		}
	}
}

void TUI_DrawPromptQuitScreen( void ) {
	werase( quit_win );
	box( quit_win, 0, 0 );
	mvwprintw( quit_win, 2, 7, "Quit LAIR" );

	// YES Button
	if ( quit_selection == 0 ) {
		wattron( quit_win, A_REVERSE );
	}

	mvwprintw( quit_win, 4, 4, "[ YES ]" );
	wattroff( quit_win, A_REVERSE );
	// NO Button
	if ( quit_selection == 1 ) {
		wattron( quit_win, A_REVERSE );
	}
	mvwprintw( quit_win, 4, 14, "[ NO ]" );
	wattroff( quit_win, A_REVERSE );
	wrefresh( quit_win );
}

void TUI_HandleQuitInput( void ) {
	int ch;
	ch = wgetch( quit_win );

	if ( ch == ERR ) {
		return;
	}

	if ( ch == '\t' || ch == KEY_RIGHT || ch == KEY_LEFT ) {
		quit_selection = ( quit_selection == 0 ) ? 1 : 0;
	} else if ( ch == '\n' || ch == KEY_ENTER ) {
		// if we have a yes selection selected
		if ( quit_selection == 0 ) {
			tui_running = lfalse;
		}
		show_quit_prompt = lfalse;
	} else if ( ch == 27 ) {
		show_quit_prompt = lfalse;
	}

	return;
}

void TUI_DrawNameScreen( void ) {
	werase( name_win );
	box( name_win, 0, 0 );

	if ( show_name_error ) {
		mvwprintw( name_win, 8, 8, "Invalid name! Try again." );
	}

	mvwprintw( name_win, 2, 12, "Enter your name" );

	// Name Field
	mvwprintw( name_win, 4, 4, "Name: [                  ]" );
	mvwprintw( name_win, 4, 11, "%s", username );

	// Highlight the field
	mvwchgat( name_win, 4, 10, 26, A_REVERSE, 0, NULL );

	// Help
	mvwprintw( name_win, 6, 6, "ENTER: Confirm    ESC: Back" );

	wrefresh( name_win );
}

void TUI_HandleNameInput( void ) {
	int len;
	int ch = wgetch( name_win );

	if ( ch == ERR ) {
		return;
	}
	if ( ch == 27 ) {
		CL_Disconnect();
		tui_state = STATE_CONNECT;
	} else if ( ch == '\n' || ch == KEY_ENTER ) {
		if ( strlen( username ) > 0 ) {
			// We will validate here with another function for checking the name itself
			if ( TUI_HandleNameValidation() ) {
				CL_SendConnect( username );
				clear();
				refresh();
				tui_state = STATE_CHAT;
			} else {
				show_name_error = ltrue;
				touchwin( name_win );
			}
		}
	} else if ( ch == 127 || ch == KEY_BACKSPACE ) {
		len = strlen( username );
		if ( len > 0 ) {
			username[len - 1] = '\0';
		}
	} else if ( ch >= 32 && ch <= 126 ) {
		len = strlen( username );
		if ( len < 63 ) {
			username[len] = ch;
			username[len + 1] = '\0';
		}
	}
}

lboolean TUI_HandleNameValidation( void ) {
	lboolean valid_name = lfalse;

	for ( int i = 0; username[i] != '\0'; ++i ) {
		if ( username[i] == ' ' ) {
			continue;
		}

		if ( ( username[i] >= 'a' && username[i] <= 'z' ) || ( username[i] >= 'A' && username[i] <= 'Z' ) || ( username[i] >= '0' && username[i] <= '9' ) || ( username[i] == '_' || username[i] == '-' ) ) {
			valid_name = ltrue;
		} else {
			return lfalse;
		}
	}

	return valid_name;
}

lboolean TUI_Frame( void ) {
	if ( show_quit_prompt ) {
		flushinp();
		TUI_DrawPromptQuitScreen();
		TUI_HandleQuitInput();
		return tui_running;
	}

	switch ( tui_state ) {
	case STATE_CONNECT: {
		TUI_DrawConnectScreen();
		TUI_HandleConnectScreenInput();
		break;
	}
	case STATE_NAME: {
		TUI_DrawNameScreen();
		TUI_HandleNameInput();
		break;
	}
	case STATE_CHAT: {
		TUI_DrawStatusBar();
		TUI_DrawChatWindow();
		TUI_DrawInputLine();
		TUI_HandleInput();
		CL_Frame();
		break;
	}
	}

	return tui_running;
}

void TUI_AddChatMessage( const char *sender, const char *text ) {
	time_t now;
	struct tm *t;
	chatmsg_t *msg;

	if ( chat_message_count >= MAX_CHAT_MESSAGES ) {
		for ( int i = 0; i < MAX_CHAT_MESSAGES - 1; i++ ) {
			chat_messages[i] = chat_messages[i + 1];
		}

		chat_message_count = MAX_CHAT_MESSAGES - 1;
	}
	msg = &chat_messages[chat_message_count];

	strncpy( msg->sender, sender, MAX_USERNAME - 1 );
	msg->sender[MAX_USERNAME - 1] = '\0';

	strncpy( msg->text, text, MAX_STRING_CHARS - 1 );
	msg->text[MAX_STRING_CHARS - 1] = '\0';

	now = time( NULL );
	t = localtime( &now );
	snprintf( msg->timestamp, sizeof( msg->timestamp ), "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec );

	chat_message_count++;
}

void TUI_InitColors( void ) {
	if ( !has_colors() ) {
		return;
	}
	start_color();
	use_default_colors();

	init_pair( COL_STATUS_BAR, COLOR_WHITE, COLOR_BLUE );
    init_pair( COL_CHAT_SYSTEM, COLOR_GREEN, -1 );
    init_pair( COL_CHAT_SELF, COLOR_CYAN, -1 );                                 
    init_pair( COL_CHAT_OTHER, COLOR_WHITE, -1 );                               
    init_pair( COL_CHAT_TIMESTAMP, COLOR_BLUE, -1 );                            
    init_pair( COL_INPUT_LINE, COLOR_YELLOW, -1 );                              
    init_pair( COL_DIALOG_BORDER, COLOR_CYAN, -1 );                             
    init_pair( COL_DIALOG_TITLE, COLOR_WHITE, -1 );                             
    init_pair( COL_BUTTON_NORMAL, COLOR_WHITE, -1 );                            
    init_pair( COL_BUTTON_SELECTED, COLOR_BLACK, COLOR_CYAN );                  
    init_pair( COL_FIELD_ACTIVE, COLOR_BLACK, COLOR_CYAN );                     
    init_pair( COL_ERROR, COLOR_RED, -1 );     
    
    
    
    
    
    
    
    
}
