/*======================================================================
   File: cl_commands.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-03-11 18:17:09
   Last Modified by: ksiric
   Last Modified: 2026-03-11 18:26:51
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */


#include "client.h"
#include "cmd.h"

internal void Cmd_Connect_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    
    
    
    
    return ;
}

internal void Cmd_Disconnect_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    
    return ;
}

internal void Cmd_Quit_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    
    return ;
}

internal void Cmd_Help_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    
    return ;
}


void CL_InitCommands( void ) {
    Cmd_AddCommand( "connect", Cmd_Connect_f );
    Cmd_AddCommand( "disconnect", Cmd_Disconnect_f );
    Cmd_AddCommand( "quit", Cmd_Quit_f );
    Cmd_AddCommand( "help", Cmd_Help_f );
    
}

