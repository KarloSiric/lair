/*======================================================================
   File: net_posix.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 17:29:12
   Last Modified by: ksiric
   Last Modified: 2026-02-15 19:16:30
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */



#include "platform.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>



void Net_Init( void )
{
    
    // @Note( Karlo ): On POSIX we dont need initialization so we leave it as it is
    
    
}


void Net_Shutdown( void )
{
    
    // @Note( Karlo ): Nothing to cleanup so we just leave it
    
    
}


lsocket Net_Socket( void )
{
    
    int newsocket;
    
    newsocket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( newsocket == -1 ) {
        return ( INVALID_SOCKET_HANDLE );
    }
     
    return ( newsocket );
    
}


int Net_Bind( lsocket newsocket, u16 port )
{    
    
    struct sockaddr_in addr;
    
    int opt = 1;
    
    setsockopt( newsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) );
    
    memset( &addr, 0, sizeof( addr ) );
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons( port );
    
    if ( bind( newsocket, ( struct sockaddr * )&addr, sizeof( addr ) ) == -1 )
    {
        return ( -1 );
    }
    
    
    return ( 0 );
         
}


int Net_Listen( lsocket newsocket, int backlog )
{
    
    if ( listen( newsocket, backlog ) == -1 )
    {
        return ( -1 );
    }
    
    return ( 0 );
}
















