/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-25 11:29:14
   Last Modified by: ksiric
   Last Modified: 2026-03-13 11:21:12
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */

#include "cmd.h"
#include "tui.h"
#include "client.h"

int main( int argc, char *argv[] ) {
    ( void )argc;
    ( void )argv;
    
    Com_Init();
    CL_Init();
    CL_InitCommands();
    TUI_Init();
    
    // @NOTE(Karlo): Registering the function pointer calls to each and every single function that we will need 
    CL_ChatCallback   = TUI_AddChatMessage;
    CL_ErrorCallback  = TUI_AddErrorMessage;
    Cmd_ErrorCallback = TUI_AddErrorMessage;
    
    while( TUI_Frame() ) {
        //  everything happens in this
    }
    
    TUI_Shutdown();
    CL_Shutdown();
    
    return ( 0 );
}
