/*======================================================================
   File: cl_commands.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-03-11 18:17:09
   Last Modified by: ksiric
   Last Modified: 2026-03-11 18:34:52
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
    const char *host;
    u16 port;
    
    if ( Cmd_Argc() < 2 ) {
        Com_Printf( "Usage: /connect <host> [port]\n" );
        return ;
    }
    
    host = Cmd_Argv( 1 );
    port = Cmd_Argv( Cmd_Argc() > 2 ) ? atoi( Cmd_Argv( 2 ) ) : DEFAULT_PORT; 
    
    CL_Connect( host, port );
    
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

