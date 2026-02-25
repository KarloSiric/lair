/*======================================================================
   File: tui.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 09:59:38
   Last Modified by: ksiric
   Last Modified: 2026-02-25 10:09:03
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

void TUI_Init( void ) {
    
    initscr();                  // initializing the ncurses library 
    cbreak();                   // used for line breaking, to make the TUI responsive
    noecho();                   // no echo used for controlling what gets printed
    
    
    
    
    return ;
}
