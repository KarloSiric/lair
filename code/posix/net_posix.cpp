/*======================================================================
   File: net_posix.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 17:29:12
   Last Modified by: ksiric
   Last Modified: 2026-02-17 13:13:25
   ---------------------------------------------------------------------
   Description:

   ---------------------------------------------------------------------
   License:
   Company:
   Version: 1.00a
 ======================================================================
																	   */

#include "lcommon.h"
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

const char *Net_ErrorString( void ) {
	return strerror( errno );
}

void Net_Init( void ) {
	// @Note( Karlo ): On POSIX we dont need initialization so we leave it as it is
}

void Net_Shutdown( void ) {
	// @Note( Karlo ): Nothing to cleanup so we just leave it
}

lsocket Net_Socket( void ) {
	int newsocket;

	newsocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( newsocket == -1 ) {
		return ( INVALID_SOCKET_HANDLE );
	}

	return ( newsocket );
}

int Net_Bind( lsocket newsocket, u16 port ) {
	struct sockaddr_in addr;

	int opt = 1;

	setsockopt( newsocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof( opt ) );

	memset( &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( port );

	if ( bind( newsocket, (struct sockaddr *)&addr, sizeof( addr ) ) == -1 ) {
		Com_Printf( "ERROR: TCP_OpenSocket: bind: %s\n", Net_ErrorString() );
		close( newsocket );
		return ( -1 );
	}

	return ( 0 );
}

int Net_Listen( lsocket newsocket, int backlog ) {
	if ( listen( newsocket, backlog ) == -1 ) {
		return ( -1 );
	}

	return ( 0 );
}

lsocket Net_Accept( lsocket sock ) {
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof( addr );
	int newsocket;

	if ( ( newsocket = accept( sock, (struct sockaddr *)&addr, &addrlen ) ) == -1 ) {
		Com_Printf( "ERROR: Net_Accept: %s\n", Net_ErrorString() );
		return ( INVALID_SOCKET_HANDLE );
	}

	return ( newsocket );
}

int Net_Connect( lsocket sock, const char *host, u16 port ) {
	struct sockaddr_in addr;
	struct hostent *h;

	memset( &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );

	// @Note(Karlo): First we try to parse the IP directly, if it fails we fallback to DNS
	addr.sin_addr.s_addr = inet_addr( host );

	if ( addr.sin_addr.s_addr == INADDR_NONE ) {
		h = gethostbyname( host );
		if ( !h ) {
			Com_Printf( "ERROR: Net_Connect: couldn't resolve %s\n", host );
			return ( -1 );
		}

		addr.sin_addr = *(struct in_addr *)h->h_addr_list[0];
	}

	if ( connect( sock, (struct sockaddr *)&addr, sizeof( addr ) ) == -1 ) {
		Com_Printf( "ERROR: Net_Connect: %s\n", Net_ErrorString() );
		return ( -1 );
	}

	return ( 0 );
}

int Net_Send( lsocket sock, byte *buf, usize buflen ) {
	ssize_t sent;
	usize total = 0;

	while ( total < buflen ) {
		sent = send( sock, buf + total, buflen - total, 0 );
		if ( sent == -1 ) {
			Com_Printf( "ERROR: Net_Send: %s\n", Net_ErrorString() );
			return ( -1 );
		}

		total += sent;
	}

	return ( (int)total );
}

int Net_Recv( lsocket sock, byte *buf, usize buflen ) {
	// @Note(Karlo): Similar to send, we dont know how many are coming in, if 0 ( con closed )

	ssize_t received;
    
    received = recv( sock, buf, buflen, 0 );
    
    if ( received == -1 )
    {
        return ( -1 );
    }
    
    return ( ( int )received );

}

void Net_Close( lsocket sock ) {
	close( sock );
}
