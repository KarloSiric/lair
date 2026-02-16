/*======================================================================
   File: sv_main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 01:36:42
   Last Modified by: ksiric
   Last Modified: 2026-02-16 01:47:29
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */


/* sv_main.cpp - Main entry point for the server initialization and setup */


#include "server.h"


server_t sv;


void SV_Init( void )
{
    
    Com_Printf( "------ Server Initialiation ------\n");
    
    sv.socket = Net_Socket();
    
    if ( sv.socket == INVALID_SOCKET_HANDLE )
    {
        
        Com_Error( ERR_FATAL, "SV_Init: couldn't create socket" );
    }
    
    if ( Net_Bind( sv.socket, DEFAULT_PORT ) == -1 )
    {
        Com_Error( ERR_FATAL, "SV_Init: couldn't bind to port %d", DEFAULT_PORT ); 
    }
    
    if ( Net_Listen( sv.socket, 10 ) == -1 )
    {
        Com_Error( ERR_FATAL, "SV_Init: couldn't listen" );
    }
    
    
    sv.initialized = ltrue;
    
    Com_Printf( "Server listening on port %d\n", DEFAULT_PORT );
    
}

void SV_Shutdown( void )
{
    
    if ( !sv.initialized )
    {
        return ;
    }
    
    Net_Close( sv.socket );
    sv.initialized = lfalse;
    
}


void SV_Frame( void )
{
    
    // TODO: Needs to be updated right 
    
}




