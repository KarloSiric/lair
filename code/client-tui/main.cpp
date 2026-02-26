/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 11:29:14
   Last Modified by: ksiric
   Last Modified: 2026-02-26 19:01:23
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
    
    
    // @TODO(KARLO): main program loop where everything will be running
    
    while( TUI_Frame() ) {
        //  everything happens in this
    }
    
    TUI_Shutdown();
    CL_Shutdown();
    
    return ( 0 );
}
