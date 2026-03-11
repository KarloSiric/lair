/*======================================================================
   File: cl_commands.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-03-11 18:17:09
   Last Modified by: ksiric
   Last Modified: 2026-03-11 19:38:51
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
    if ( !cl.connected ) {
        Com_Printf( "User not connected!\n" );
        return ;
    }
    
    CL_Disconnect();
    
    return ;
}

internal void Cmd_Quit_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work  
    Com_Printf( "Goodbye!\n" );
    CL_Shutdown();
    
    return ;
}

internal void Cmd_Help_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    Com_Printf( "\n--- Available Commands ---\n" );
    Com_Printf( "  /connect <host> [port] - Connect to server\n" );
    Com_Printf( "  /disconnect            - Disconnect\n" );
    Com_Printf( "  /quit                  - Exit application\n" );
    Com_Printf( "  /name <username>       - Change name\n" );
    Com_Printf( "  /help                  - This message\n" );
    Com_Printf( "--------------------------\n\n" );
    
    return ;
}

internal void Cmd_Users_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    Com_Printf( "--------- Online Users [%d] ---------\n", cl.numUsers );
    for ( int i = 0; i < MAX_CLIENTS; ++i ) {
        if ( cl.users[i].active ) {
            Com_Printf( "   [%d] %s\n", cl.users[i].id, cl.users[i].name );
        }
    } 
    Com_Printf( "-------------------------------------\n", cl.numUsers );
    
    return ;
}


void CL_InitCommands( void ) {
    Cmd_AddCommand( "connect", Cmd_Connect_f );
    Cmd_AddCommand( "disconnect", Cmd_Disconnect_f );
    Cmd_AddCommand( "quit", Cmd_Quit_f );
    Cmd_AddCommand( "help", Cmd_Help_f );
    Cmd_AddCommand( "users", Cmd_Users_f );
    
}

