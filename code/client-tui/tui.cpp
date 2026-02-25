/*======================================================================
   File: tui.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 09:59:38
   Last Modified by: ksiric
   Last Modified: 2026-02-25 11:36:43
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */

#include "tui.h"
#include <ncurses.h>

static int rows;
static int cols;

static WINDOW *main_win;
static WINDOW *status_win;
static WINDOW *chat_win;
static WINDOW *input_win;

void TUI_Init( void ) { 
    initscr();                                      // initializing the ncurses library 
    main_win = stdscr;
    cbreak();                                       // used for line breaking, to make the TUI responsive
    noecho();                                       // no echo used for controlling what gets printed
    keypad( main_win, TRUE );                       // allows us to have special characters from the keypad
    getmaxyx( main_win, rows, cols );               // used to make the terminal ncurses look match the size of the terminal window
    status_win = newwin( 1, cols, 0, 0 );           // status window creation
    chat_win = newwin( rows - 2, cols, 1, 0 );      // chat window creation
    input_win = newwin( 1, cols, rows - 1, 0 );     // input window creation
    scrollok( chat_win, TRUE );                     // enables scrolling in the chat window, to see history of messages and so forth
    
    return ;
}

void TUI_Shutdown( void ) {
    delwin( status_win );
    delwin( chat_win );
    delwin( input_win );
    endwin();
    
    return ;
}

void TUI_DrawStatusBar( void ) {
    werase( status_win );
    mvwprintw( status_win, 0, 0, "LAIR - Disconnected" );
    wrefresh( status_win );
          
    return ;
}

void TUI_DrawInputLine( void ) {
    werase( input_win );
    mvwprintw( input_win, 0, 0, "> " );
    wrefresh( input_win );
    
    return ;
}

void TUI_DrawChatWindow( void ) {
    werase( chat_win );
    wrefresh( chat_win );
    
    return ;
} 

lboolean TUI_HandleInput( void ) {
    int ch;
    
    ch = wgetch( input_win );
    
    if ( ch == ERR ) {
        // nothing being pressed at all
        return ltrue;
    }
    
    if ( ch == 'q' ) {
        // for quitting the application for now TEST
        return lfalse;
    }
    
    // we will handle other keys here as well, probably better to do it witha. switch statement but we will see
    
    return ltrue;
}

