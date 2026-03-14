/*======================================================================
   File: net_posix.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 17:29:12
   Last Modified by: ksiric
   Last Modified: 2026-03-14 12:20:59
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
#include <sys/select.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>

#define CONNECT_TIMEOUT_SEC 3

const char *Net_ErrorString( void ) {
	return strerror( errno );
}

void Net_Init( void ) {
	// @NOTE( Karlo ): On POSIX we dont need initialization so we leave it as it is
}

void Net_Shutdown( void ) {
	// @NOTE( Karlo ): Nothing to cleanup so we just leave it
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
	int flags;
	int result;
	fd_set writefds;
	struct timeval timeout;
	int so_error;
	socklen_t len;

	memset( &addr, 0, sizeof( addr ) );

	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );

	// @NOTE(Karlo): First we try to parse the IP directly, if it fails we fallback to DNS
	addr.sin_addr.s_addr = inet_addr( host );

	if ( addr.sin_addr.s_addr == INADDR_NONE ) {
		h = gethostbyname( host );
		if ( !h ) {
			return ( -1 );
		}

		addr.sin_addr = *(struct in_addr *)h->h_addr_list[0];
	}

	// Set socket to non-blocking mode
	flags = fcntl( sock, F_GETFL, 0 );
	if ( flags == -1 ) {
		return ( -1 );
	}
	if ( fcntl( sock, F_SETFL, flags | O_NONBLOCK ) == -1 ) {
		return ( -1 );
	}

	// Attempt to connect (will return immediately)
	result = connect( sock, (struct sockaddr *)&addr, sizeof( addr ) );

	if ( result == -1 ) {
		if ( errno != EINPROGRESS ) {
			// Real error, not just "in progress"
			fcntl( sock, F_SETFL, flags ); // Restore blocking mode
			return ( -1 );
		}

		// Connection in progress, wait with timeout
		FD_ZERO( &writefds );
		FD_SET( sock, &writefds );

		timeout.tv_sec = CONNECT_TIMEOUT_SEC;
		timeout.tv_usec = 0;

		result = select( sock + 1, NULL, &writefds, NULL, &timeout );

		if ( result <= 0 ) {
			// Timeout or error
			fcntl( sock, F_SETFL, flags ); // Restore blocking mode
			errno = ETIMEDOUT;
			return ( -1 );
		}

		// Check if connection actually succeeded
		len = sizeof( so_error );
		if ( getsockopt( sock, SOL_SOCKET, SO_ERROR, &so_error, &len ) == -1 ) {
			fcntl( sock, F_SETFL, flags );
			return ( -1 );
		}

		if ( so_error != 0 ) {
			// Connection failed
			fcntl( sock, F_SETFL, flags );
			errno = so_error;
			return ( -1 );
		}
	}

	// Restore socket to blocking mode
	fcntl( sock, F_SETFL, flags );

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
	// @NOTE(Karlo): Similar to send, we dont know how many are coming in, if 0 ( con closed )
	ssize_t received;

	received = recv( sock, buf, buflen, 0 );

	if ( received == -1 ) {
		return ( -1 );
	}

	return ( (int)received );
}

void Net_Close( lsocket sock ) {
	close( sock );
}
