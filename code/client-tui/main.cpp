/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 11:29:14
   Last Modified by: ksiric
   Last Modified: 2026-02-25 11:34:05
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


int main( int argc, char *argv[] ) {
    
    ( void )argc;
    ( void )argv;
    
    Com_Init();
    CL_Init();
    TUI_Init();
    
    lboolean running = ltrue;
    
    // @TODO(KARLO): main program loop where everything will be running
    
    while( running ) {
        TUI_DrawStatusBar();
        TUI_DrawChatWindow();
        TUI_DrawInputLine();
        
        running = TUI_HandleInput();
        
        CL_Frame();
    }
    
    TUI_Shutdown();
    CL_Shutdown();
    
    return ( 0 );
}
