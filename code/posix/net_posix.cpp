/*======================================================================
   File: net_posix.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 17:29:12
   Last Modified by: ksiric
   Last Modified: 2026-02-15 17:54:43
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
    
    int sock;
    sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock == -1 ) {
        return ( INVALID_SOCKET_HANDLE );
    }
    
    
    return ( sock );
    
}


int Net_Bind( lsocket sock, u16 port )
{
    
    
    
    
}










