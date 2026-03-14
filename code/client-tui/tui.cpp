/*======================================================================
   File: tui.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 09:59:38
   Last Modified by: ksiric
   Last Modified: 2026-03-13 11:44:54
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
#include "cmd.h"
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
	TUI_InitColors(); // initialize color pairs
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

void TUI_HandleResize( void ) {
	endwin();
	refresh();
	clear();

	getmaxyx( stdscr, rows, cols );

	// Recreate all windows with new dimensions
	delwin( status_win );
	delwin( chat_win );
	delwin( input_win );
	delwin( connect_win );
	delwin( quit_win );
	delwin( name_win );

	int con_y = ( rows - CONNECT_WIN_HEIGHT ) / 2;
	int con_x = ( cols - CONNECT_WIN_WIDTH ) / 2;
	int quit_y = ( rows - QUIT_WIN_HEIGHT ) / 2;
	int quit_x = ( cols - QUIT_WIN_WIDTH ) / 2;
	int name_y = ( rows - NAME_WIN_HEIGHT ) / 2;
	int name_x = ( cols - NAME_WIN_WIDTH ) / 2;

	status_win = newwin( 1, cols, 0, 0 );
	chat_win = newwin( rows - 2, cols, 1, 0 );
	input_win = newwin( 1, cols, rows - 1, 0 );
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

	scrollok( chat_win, TRUE );

	// Force redraw
	touchwin( stdscr );
	refresh();
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
	wbkgd( status_win, COLOR_PAIR( COL_STATUS_BAR ) );
	wattron( status_win, COLOR_PAIR( COL_STATUS_BAR ) | A_BOLD );

	// Fill entire line for full background color
	for ( int i = 0; i < cols; i++ ) {
		mvwaddch( status_win, 0, i, ' ' );
	}

	if ( cl.connected ) {
		mvwprintw( status_win, 0, 1, " LAIR - Connected to %s:%d ", connect_ip, atoi( connect_port ) );
	} else {
		mvwprintw( status_win, 0, 1, " LAIR - Disconnected " );
	}

	wattroff( status_win, COLOR_PAIR( COL_STATUS_BAR ) | A_BOLD );
	wrefresh( status_win );
	return;
}

void TUI_DrawInputLine( void ) {
	werase( input_win );

	// Yellow prompt
	wattron( input_win, COLOR_PAIR( COL_INPUT_LINE ) | A_BOLD );
	mvwprintw( input_win, 0, 0, "> " );
	wattroff( input_win, COLOR_PAIR( COL_INPUT_LINE ) | A_BOLD );

	// Normal text for input
	wprintw( input_win, "%s", chat_input );
	wrefresh( input_win );

	return;
}

void TUI_DrawChatWindow( void ) {
	int win_h, win_w;
	int line = 1;
	chatmsg_t *msg;
	getmaxyx( chat_win, win_h, win_w );
	werase( chat_win );

	// Draw border in cyan
	wattron( chat_win, COLOR_PAIR( COL_DIALOG_BORDER ) );
	TUI_DrawDoubleBox( chat_win, 0, 0, win_h, win_w );
	wattroff( chat_win, COLOR_PAIR( COL_DIALOG_BORDER ) );

	int max_lines = win_h - 2;

	// Calculate how many lines each message needs and find start point
	int total_lines_needed = 0;
	int prefix_len = 14; // "[HH:MM:SS] X: " minimum
	int text_area = win_w - 4; // borders + padding

	for ( int i = 0; i < chat_message_count; i++ ) {
		int sender_len = strlen( chat_messages[i].sender );
		int first_line_width = text_area - 11 - sender_len - 2; // timestamp + sender + ": "
		int text_len = strlen( chat_messages[i].text );

		if ( text_len <= first_line_width ) {
			total_lines_needed += 1;
		} else {
			int remaining = text_len - first_line_width;
			int cont_width = text_area - 11 - sender_len - 2; // continuation indent
			total_lines_needed += 1 + ( remaining + cont_width - 1 ) / cont_width;
		}
	}

	int lines_to_skip = ( total_lines_needed > max_lines ) ? total_lines_needed - max_lines : 0;

	// Draw messages with word wrapping
	for ( int i = 0; i < chat_message_count && line < win_h - 1; i++ ) {
		msg = &chat_messages[i];

		int sender_len = strlen( msg->sender );
		int indent = 11 + sender_len + 2; // "[HH:MM:SS] name: "
		int first_line_width = text_area - indent;
		int text_len = strlen( msg->text );

		// Calculate lines this message needs
		int msg_lines = 1;
		if ( text_len > first_line_width ) {
			int remaining = text_len - first_line_width;
			msg_lines += ( remaining + first_line_width - 1 ) / first_line_width;
		}

		// Skip if needed
		if ( lines_to_skip >= msg_lines ) {
			lines_to_skip -= msg_lines;
			continue;
		}
        
        wattron( chat_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) );
        mvwprintw( chat_win, line, 2, "%s ", msg->timestamp );
        wattroff( chat_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) ); 
        
        // @NOTE(Karlo): Adding type checking for the new way of working on these things
        switch( msg->type ) {
		  case DISPLAY_CHAT: {
            wattron( chat_win, COLOR_PAIR( COL_CHAT_SELF ) | A_BOLD );
            wprintw( chat_win, "%s ", msg->sender );
            wattroff( chat_win, COLOR_PAIR( COL_CHAT_SELF ) | A_BOLD );         
            break;
		  }
		  case DISPLAY_ERROR: {
            wattron( chat_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );
            wprintw( chat_win, "%s ", msg->sender );
            wattroff( chat_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );         
            break;
		  }
		  case DISPLAY_SYSTEM: {
            wattron( chat_win, COLOR_PAIR( COL_CHAT_SYSTEM ) | A_BOLD );
            wprintw( chat_win, "%s ", msg->sender );
            wattroff( chat_win, COLOR_PAIR( COL_CHAT_SYSTEM ) | A_BOLD );         
			break;
	      }
		  default: {
            wattron( chat_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );
            wprintw( chat_win, "%s ", msg->sender );
            wattroff( chat_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );         
			break;
          }
		}
        
		// Draw message with word wrapping
		const char *text = msg->text;
		int remaining = text_len;
		int line_width = first_line_width;
		int first = 1;
        
        switch ( msg->type ) {
            case DISPLAY_ERROR: {
                wattron( chat_win, COLOR_PAIR( COL_ERROR ) );
                break;
            }
            case DISPLAY_SYSTEM: {
                wattron( chat_win, COLOR_PAIR( COL_CHAT_SYSTEM ) );
                break;
            }
            case DISPLAY_CHAT: {
                if ( strcmp( msg->sender, cl.name ) == 0 ) {
                    wattron( chat_win, COLOR_PAIR( COL_CHAT_SELF ) );
                } else {
                    wattron( chat_win, COLOR_PAIR( COL_CHAT_OTHER ) );
                }
                break;
            }
        }

		while ( remaining > 0 && line < win_h - 1 ) {
			int chunk = ( remaining > line_width ) ? line_width : remaining;

			// Try word boundary
			if ( remaining > line_width ) {
				int brk = chunk;
				while ( brk > 0 && text[brk] != ' ' ) brk--;
				if ( brk > chunk / 2 ) chunk = brk + 1;
			}

			if ( first ) {
				wprintw( chat_win, "%.*s", chunk, text );
				first = 0;
			} else {
				mvwprintw( chat_win, line, 2 + indent, "%.*s", chunk, text );
			}

			text += chunk;
			remaining -= chunk;
			if ( remaining > 0 ) line++;
		}
        
        switch ( msg->type ) {
            case DISPLAY_ERROR: {
                wattroff( chat_win, COLOR_PAIR( COL_ERROR ) );
                break;
            }
            case DISPLAY_SYSTEM: {
                wattroff( chat_win, COLOR_PAIR( COL_CHAT_SYSTEM ) );
                break;
            }
            case DISPLAY_CHAT: {
                if ( strcmp( msg->sender, cl.name ) == 0 ) {
                    wattroff( chat_win, COLOR_PAIR( COL_CHAT_SELF ) );
                } else {
                    wattroff( chat_win, COLOR_PAIR( COL_CHAT_OTHER ) );
                }
                break;
            }
        }

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
            // @NOTE: Checking if its a command or a regular message
            if ( chat_input[0] == '/' ) {
                Cmd_ExecuteString( chat_input );
            } else {
    			CL_SendChat( chat_input );
            }
            
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

	// Draw border in cyan
	wattron( connect_win, COLOR_PAIR( COL_DIALOG_BORDER ) );
	TUI_DrawDoubleBox( connect_win, 0, 0, CONNECT_WIN_HEIGHT, CONNECT_WIN_WIDTH );
	wattroff( connect_win, COLOR_PAIR( COL_DIALOG_BORDER ) );

	// Title in bold white
	wattron( connect_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );
	mvwprintw( connect_win, 2, 8, "W E L C O M E   T O " );
	mvwprintw( connect_win, 3, 14, "L A I R " );
	wattroff( connect_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );

	// Server field
	wattron( connect_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( connect_win, 5, 4, "Server:" );
	wattroff( connect_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( connect_win, 5, 12, "[                        ]" );
	mvwprintw( connect_win, 5, 13, "%s", connect_ip );

	// Port field
	wattron( connect_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( connect_win, 7, 4, "Port:" );
	wattroff( connect_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( connect_win, 7, 12, "[        ]" );
	mvwprintw( connect_win, 7, 13, "%s", connect_port );

	// Highlight active field with color
	if ( connect_field == 0 ) {
		mvwchgat( connect_win, 5, 12, 26, A_REVERSE, COL_FIELD_ACTIVE, NULL );
	} else {
		mvwchgat( connect_win, 7, 12, 10, A_REVERSE, COL_FIELD_ACTIVE, NULL );
	}

	// Connect button
	wattron( connect_win, A_BOLD );
	mvwprintw( connect_win, 9, 15, "[ CONNECT ]" );
	wattroff( connect_win, A_BOLD );

	// Help text in dim
	wattron( connect_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) );
	mvwprintw( connect_win, 11, 4, "TAB: switch   ENTER: connect" );
	wattroff( connect_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) );

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

	// Draw border in cyan
	wattron( quit_win, COLOR_PAIR( COL_DIALOG_BORDER ) );
	TUI_DrawDoubleBox( quit_win, 0, 0, QUIT_WIN_HEIGHT, QUIT_WIN_WIDTH );
	wattroff( quit_win, COLOR_PAIR( COL_DIALOG_BORDER ) );

	// Title in bold
	wattron( quit_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );
	mvwprintw( quit_win, 2, 10, "Quit LAIR?" );
	wattroff( quit_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );

	// YES Button
	if ( quit_selection == 0 ) {
		wattron( quit_win, COLOR_PAIR( COL_BUTTON_SELECTED ) | A_BOLD );
	}
	mvwprintw( quit_win, 4, 5, "[ YES ]" );
	wattroff( quit_win, COLOR_PAIR( COL_BUTTON_SELECTED ) | A_BOLD );

	// NO Button
	if ( quit_selection == 1 ) {
		wattron( quit_win, COLOR_PAIR( COL_BUTTON_SELECTED ) | A_BOLD );
	}
	mvwprintw( quit_win, 4, 17, "[ NO ]" );
	wattroff( quit_win, COLOR_PAIR( COL_BUTTON_SELECTED ) | A_BOLD );

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

	// Draw border in cyan
	wattron( name_win, COLOR_PAIR( COL_DIALOG_BORDER ) );
	TUI_DrawDoubleBox( name_win, 0, 0, NAME_WIN_HEIGHT, NAME_WIN_WIDTH );
	wattroff( name_win, COLOR_PAIR( COL_DIALOG_BORDER ) );

	// Error message in red
	if ( show_name_error ) {
		wattron( name_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );
		mvwprintw( name_win, 8, 8, "Invalid name! Try again." );
		wattroff( name_win, COLOR_PAIR( COL_ERROR ) | A_BOLD );
	}

	// Title in bold
	wattron( name_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );
	mvwprintw( name_win, 2, 12, "Enter your name" );
	wattroff( name_win, COLOR_PAIR( COL_DIALOG_TITLE ) | A_BOLD );

	// Name Field label
	wattron( name_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( name_win, 4, 4, "Name:" );
	wattroff( name_win, COLOR_PAIR( COL_CHAT_SELF ) );
	mvwprintw( name_win, 4, 10, "[                  ]" );
	mvwprintw( name_win, 4, 11, "%s", username );

	// Highlight the field
	mvwchgat( name_win, 4, 10, 20, A_REVERSE, COL_FIELD_ACTIVE, NULL );

	// Help text dim
	wattron( name_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) );
	mvwprintw( name_win, 6, 6, "ENTER: Confirm    ESC: Back" );
	wattroff( name_win, COLOR_PAIR( COL_CHAT_TIMESTAMP ) );

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

				// Hide dialog windows
				werase( name_win );
				werase( connect_win );
				wrefresh( name_win );
				wrefresh( connect_win );

				// Clear screen and switch state
				clear();
				refresh();
				tui_state = STATE_CHAT;

				// Draw chat UI IMMEDIATELY (don't wait for next frame)
				TUI_DrawStatusBar();
				TUI_DrawChatWindow();
				TUI_DrawInputLine();
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

/**
 * @brief      { Frame function, function that is beeing looped in the main program execution }
 *
 * @return     { description_of_the_return_value }
 */
lboolean TUI_Frame( void ) {
	// Check for terminal resize
	int new_rows, new_cols;
	getmaxyx( stdscr, new_rows, new_cols );
	if ( new_rows != rows || new_cols != cols ) {
		TUI_HandleResize();
	}

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

/**
 * @brief      { Main Helper function for displaying messages to the screen }
 *
 * @param[in]  type    The type
 * @param[in]  sender  The sender
 * @param[in]  text    The text
 */

void TUI_AddMessage( displaytype_t type, const char *sender, const char *text ) {
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
    
    msg->type = type;
    
    switch ( type ) {
        case DISPLAY_CHAT: {
            strncpy( msg->sender, sender, MAX_USERNAME - 1 );
            break;
        }
        case DISPLAY_ERROR: {
            strncpy( msg->sender, "[ERROR]", MAX_USERNAME - 1 );
            break;
        }
        case DISPLAY_SYSTEM: {
            strncpy( msg->sender, "[SYSTEM]", MAX_USERNAME - 1 );
            break;
        }
        default: {
            strncpy( msg->sender, "", MAX_USERNAME - 1 );
            break;
        }
    }

	strncpy( msg->sender, sender, MAX_USERNAME - 1 );
	msg->sender[MAX_USERNAME - 1] = '\0';
    
	strncpy( msg->text, text, MAX_STRING_CHARS - 1 );
	msg->text[MAX_STRING_CHARS - 1] = '\0';

	now = time( NULL );
	t = localtime( &now );
	snprintf( msg->timestamp, sizeof( msg->timestamp ), "[%02d:%02d:%02d]", t->tm_hour, t->tm_min, t->tm_sec );

	chat_message_count++;
}

void TUI_AddErrorMessage(const char *text ) {
    TUI_AddMessage( DISPLAY_ERROR , "[ERROR]", text );
}

void TUI_AddSystemMessage( const char *text ) {
    TUI_AddMessage( DISPLAY_SYSTEM, "[SYSTEM]", text);
}

void TUI_AddChatMessage( const char *sender, const char *text ) {
    TUI_AddMessage( DISPLAY_CHAT, sender, text);
}

void TUI_InitColors( void ) {
	if ( !has_colors() ) {
		return;
	}
	start_color();
	use_default_colors();

	init_pair( COL_STATUS_BAR, COLOR_YELLOW, COLOR_BLUE );
	init_pair( COL_CHAT_SYSTEM, COLOR_GREEN, -1 );
	init_pair( COL_CHAT_SELF, COLOR_CYAN, -1 );
	init_pair( COL_CHAT_OTHER, COLOR_WHITE, -1 );
	init_pair( COL_CHAT_TIMESTAMP, COLOR_MAGENTA, -1 );                            
    init_pair( COL_INPUT_LINE, COLOR_YELLOW, -1 );                              
    init_pair( COL_DIALOG_BORDER, COLOR_CYAN, -1 );                             
    init_pair( COL_DIALOG_TITLE, COLOR_WHITE, -1 );                             
    init_pair( COL_BUTTON_NORMAL, COLOR_WHITE, -1 );                            
    init_pair( COL_BUTTON_SELECTED, COLOR_BLACK, COLOR_CYAN );                  
    init_pair( COL_FIELD_ACTIVE, COLOR_BLACK, COLOR_CYAN );                     
    init_pair( COL_ERROR, COLOR_RED, -1 );     
    
    colors_initialized = ltrue;   
}

void TUI_DrawDoubleBox( WINDOW *win, int starty, int startx, int height, int width ) {
	int endx = startx + width - 1;
	int endy = starty + height - 1;

	// Draw corners using ACS characters
	mvwaddch( win, starty, startx, ACS_ULCORNER );
	mvwaddch( win, starty, endx, ACS_URCORNER );
	mvwaddch( win, endy, startx, ACS_LLCORNER );
	mvwaddch( win, endy, endx, ACS_LRCORNER );

	// Draw horizontal lines
    // @NOTE( Karlo ): Adding horizontal lines
	for ( int x = startx + 1; x < endx; x++ ) {
		mvwaddch( win, starty, x, ACS_HLINE );
		mvwaddch( win, endy, x, ACS_HLINE );
	}
    
	// Draw vertical lines
    // @NOTE( Karlo ): Adding horizontal lines
	for ( int y = starty + 1; y < endy; y++ ) {
		mvwaddch( win, y, startx, ACS_VLINE );
		mvwaddch( win, y, endx, ACS_VLINE );
	}
}
