/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 01:52:19
   Last Modified by: ksiric
   Last Modified: 2026-02-17 13:31:35
   ---------------------------------------------------------------------
   Description:

   ---------------------------------------------------------------------
   License:
   Company:
   Version: 1.00a
 ======================================================================
																	   */

/* main.cpp - Lair server entry point */

#include "l_shared.h"
#include "lcommon.h"
#include "platform.h"
#include "server.h"

volatile int running = 1;

void SigHandler( int sig ) {
    Com_Printf( "\nShutting server down...\n" );
    running = 0;    
}

int main( int argc, char **argv ) {
    signal( SIGINT, SigHandler );
    
	Com_Init();
	SV_Init();

	while ( running ) {
		SV_Frame();
	}
    
    SV_Shutdown();

	return ( 0 );
}
