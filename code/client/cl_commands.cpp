/*======================================================================
   File: cl_commands.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-03-11 18:17:09
   Last Modified by: ksiric
   Last Modified: 2026-03-12 20:57:24
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
#include <stdio.h>

internal void Cmd_Connect_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    const char *host;
    u16 port;
    
    if ( Cmd_Argc() < 2 ) {
        if ( CL_ChatCallback ) {
            CL_ChatCallback( "*", "Usage: /connect <host> [port]\n" );
        }
        return ;
    }
    
    host = Cmd_Argv( 1 );
    port = ( Cmd_Argc() > 2 ) ? atoi( Cmd_Argv( 2 ) ) : DEFAULT_PORT; 
    
    CL_Connect( host, port );
    
    return ;
}

internal void Cmd_Name_f( void ) {
    // @NOTE(Karlo): Adding name command so that users can connect
    if ( Cmd_Argc() < 2 ) {
        if ( CL_ChatCallback ) {
            CL_ChatCallback( "*", "Usage: /name <username>" );
        }
        return ;
    }
    
    if ( !cl.connected ) {
        if ( CL_ChatCallback ) {
            CL_ChatCallback( "*", "User not connected!" );
        }
        return ;
    }
    
    CL_SendConnect( Cmd_Argv(1) );
}

internal void Cmd_Disconnect_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    if ( !cl.connected ) {
        if ( CL_ChatCallback ) {
            CL_ChatCallback( "*", "User not connected!\n" );
        }
        return ;
    }
    
    CL_Disconnect();
    
    return ;
}

internal void Cmd_Quit_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work  
    if ( CL_ChatCallback ) {
        CL_ChatCallback( "*", "Goodbye!\n" );
    }
    
    CL_Shutdown();
    
    return ;
}

internal void Cmd_Help_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    
    
    if ( CL_ChatCallback ) {
        CL_ChatCallback( "*", "\n--- Available Commands ---\n" );
        CL_ChatCallback( "*", "  /connect <host> [port] - Connect to server\n" );
        CL_ChatCallback( "*", "  /disconnect            - Disconnect\n" );
        CL_ChatCallback( "*", "  /quit                  - Exit application\n" );
        CL_ChatCallback( "*", "  /name <username>       - Change name\n" );
        CL_ChatCallback( "*", "  /help                  - This message\n" );
        CL_ChatCallback( "*", "--------------------------\n\n" );
    } else {
        Com_Printf( "\n--- Available Commands ---\n" );
        Com_Printf( "  /connect <host> [port] - Connect to server\n" );
        Com_Printf( "  /disconnect            - Disconnect\n" );
        Com_Printf( "  /quit                  - Exit application\n" );
        Com_Printf( "  /name <username>       - Change name\n" );
        Com_Printf( "  /help                  - This message\n" );
        Com_Printf( "--------------------------\n\n" );
    }
    
    return ;
}

internal void Cmd_Users_f( void ) {
    // @TODO(Karlo): Implementation needed in order to make it work
    if ( CL_ChatCallback ) {
        char buf[512];
        snprintf( buf, sizeof( buf ), "--------- Online Users [%d] ---------\n", cl.numUsers );
        CL_ChatCallback( "*", buf );
        for ( int i = 0; i < MAX_CLIENTS; ++i ) {
            if ( cl.users[i].active ) {
                snprintf( buf, sizeof( buf ), "   [%d] %s\n", cl.users[i].id, cl.users[i].name );
                CL_ChatCallback( "*", buf );
            }
        } 
        CL_ChatCallback( "*", "-------------------------------------\n" );
    }
    
    return ;
}


void CL_InitCommands( void ) {
    Cmd_AddCommand( "connect", Cmd_Connect_f );
    Cmd_AddCommand( "disconnect", Cmd_Disconnect_f );
    Cmd_AddCommand( "quit", Cmd_Quit_f );
    Cmd_AddCommand( "help", Cmd_Help_f );
    Cmd_AddCommand( "users", Cmd_Users_f );
    Cmd_AddCommand( "name", Cmd_Name_f );
    
}

