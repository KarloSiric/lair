/*======================================================================
   File: cl_main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-17 01:17:55
   Last Modified by: ksiric
   Last Modified: 2026-02-17 01:23:45
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */


#include "client.h"
#include "platform.h"

#include <string.h>
#include <sys/select.h>



cl_client_t cl;



void CL_Init( void ) {
    
    memset( &cl, 0, sizeof( cl ) );
    cl.socket = INVALID_SOCKET_HANDLE;
    cl.connected = lfalse;
    
}


void CL_Connect( const char *host, u16 port ) {
    
    cl.socket = Net_Socket();
    int c;
    
    if ( cl.socket == INVALID_SOCKET_HANDLE )
    {
        Com_Printf( "CL_Connect: Failed to create socket\n" );
        return ;
    }
    
    c = Net_Connect( cl.socket, host, port );
    
    if ( c == -1 )
    {
        Com_Printf( "CL_Connect: Failed to connect to %s:%d\n", host, port );
        Net_Close( cl.socket );
        cl.socket = INVALID_SOCKET_HANDLE;
        return ;
    }
    
    cl.connected = ltrue;
    Com_Printf( "Connected to %s:%d\n", host, port );
    
}

void CL_Disconnect( void ) {
    
    if ( !cl.connected )
    {
        return ;
    }
    
    CL_SendDisconnect();
    
    Net_Close( cl.socket );
    cl.socket = INVALID_SOCKET_HANDLE;
    cl.connected = lfalse;
    
    Com_Printf( "Disconnected from server\n" );
    
}
