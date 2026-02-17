/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-17 13:01:51
   Last Modified by: ksiric
   Last Modified: 2026-02-17 13:02:29
   ---------------------------------------------------------------------
   Description:

   ---------------------------------------------------------------------
   License:
   Company:
   Version: 1.00a
 ======================================================================
																	   */

#include "client.h"
#include <sys/select.h>
#include <stdio.h>
#include <string.h>

int main( int argc, char const *argv[] ) {
	char input[256];
	fd_set readfds;

	struct timeval timeout;

	Com_Init();
	CL_Init();

	Com_Printf( "=== Lair Client ===\n" );
	CL_Connect( "127.0.0.1", DEFAULT_PORT );

	if ( !cl.connected ) {
		return ( 1 );
	}

	Com_Printf( "Enter your name: " );
	fflush( stdout );
	fgets( input, sizeof( input ), stdin );
	input[strcspn( input, "\n" )] = '\0';

	CL_SendConnect( input );

	Com_Printf( "Type to chat. CTRL + C to quit.\n" );

	while ( cl.connected ) {
		CL_Frame();

		FD_ZERO( &readfds );
		FD_SET( 0, &readfds );

		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;

		if ( select( 1, &readfds, NULL, NULL, &timeout ) > 0 ) {
			if ( FD_ISSET( 0, &readfds ) ) {
				if ( fgets( input, sizeof( input ), stdin ) ) {
					input[strcspn( input, "\n" )] = '\0';

					if ( strlen( input ) > 0 ) {
						CL_SendChat( input );
					}
				}
			}
		}
	}

	CL_Shutdown();

	return 0;
}
