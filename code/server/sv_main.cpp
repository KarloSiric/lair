/*======================================================================
   File: sv_main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 01:36:42
   Last Modified by: ksiric
   Last Modified: 2026-02-16 23:21:57
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
#include <string.h>
#include <sys/select.h>


server_t sv;

void SV_Init( void ) {
	Com_Printf( "------ Server Initialiation ------\n" );

	// @NOTE(KARLO): Clearing the client array when first initializing
	memset( sv.clients, 0, sizeof( sv.clients ) );
	sv.numclients = 0;

	sv.socket = Net_Socket();

	if ( sv.socket == INVALID_SOCKET_HANDLE ) {
		Com_Error( ERR_FATAL, "SV_Init: couldn't create socket" );
	}

	if ( Net_Bind( sv.socket, DEFAULT_PORT ) == -1 ) {
		Com_Error( ERR_FATAL, "SV_Init: couldn't bind to port %d", DEFAULT_PORT );
	}

	if ( Net_Listen( sv.socket, 10 ) == -1 ) {
		Com_Error( ERR_FATAL, "SV_Init: couldn't listen" );
	}

	sv.initialized = ltrue;

	Com_Printf( "Server listening on port %d\n", DEFAULT_PORT );
}

void SV_Shutdown( void ) {
	if ( !sv.initialized ) {
		return;
	}

	Net_Close( sv.socket );
	sv.initialized = lfalse;
}

void SV_Frame( void ) {
     
    // @NOTE(KARLO): 
    
    fd_set readfds;                // bit array usually 1024 bits
    struct timeval timeout;
    int maxfd;
    int clientnum;
    lsocket clientsocket;
    
    FD_ZERO( &readfds );
    FD_SET( sv.socket, &readfds );
    maxfd = sv.socket;
     
    // Adding all connected sockets
    for ( int i = 0; i < MAX_CLIENTS; i++ )
    {
        if ( sv.clients[i].active )
        {
            FD_SET( sv.clients[i].socket, &readfds );
            if ( sv.clients[i].socket > maxfd )
            {
                maxfd = sv.clients[i].socket;
            }
        }
    }
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;                // 10 ms
    
    if ( select( maxfd + 1, &readfds, NULL, NULL, &timeout ) <= 0 )
    {
        return ;
    }
    
    if ( FD_ISSET( sv.socket, &readfds ) )
    {
        
        clientsocket = Net_Accept( sv.socket );
        
        if ( clientsocket != INVALID_SOCKET_HANDLE )
        {
            
            clientnum = SV_FindFreeClientSlot();
            
            if ( clientnum == -1 ) 
            {
                Com_Printf( "Server full, rejecting client\n" );
                Net_Close( clientsocket );
                
            }
            else 
            {
                
                sv.clients[clientnum].active = ltrue;
                sv.clients[clientnum].socket = clientsocket;
                sv.clients[clientnum].status = STATUS_ONLINE;
                sv.numclients++;
                
                Com_Printf( "Client %d connected\n", clientnum );
                
            }    
        }
    }
    
    for ( int i = 0; i < MAX_CLIENTS; i++ )
    {
        
        if ( !sv.clients[i].active ) 
        {
            continue;
        }
        
        if ( FD_ISSET( sv.clients[i].socket, &readfds ) )
        {
            SV_ReadClientMessage( i );
        } 
    }
}

int SV_FindFreeClientSlot( void ) {
	int i;

	for ( i = 0; i < MAX_CLIENTS; i++ ) {
		if ( !sv.clients[i].active ) {
			return ( i );
		}
	}

	return ( -1 ); // server full
}

void SV_DropClient( int clientnum, const char *reason ) {
	client_t *client = SV_GetClient( clientnum );

	if ( !client || !client->active ) {
		return;
	}

	Com_Printf( "Client %s dropped: %s\n", client->name, reason );
	Net_Close( client->socket );
	client->active = lfalse;
	client->socket = INVALID_SOCKET_HANDLE;
	client->name[0] = '\0';

	sv.numclients--;
}

void SV_BroadcastMessage( msg_t *msg ) {
	int i;

	for ( i = 0; i < MAX_CLIENTS; i++ ) {
		if ( sv.clients[i].active ) {
			Net_Send( sv.clients[i].socket, msg->data, msg->cursize );
		}
	} 
}

void SV_ReadClientMessage( int clientnum )
{
    
    client_t *client = SV_GetClient( clientnum );
    byte buf[MAX_MSG_LEN];
    
    size_t length;
    
    length = Net_Recv( client->socket, buf, sizeof( buf ) );
    
    if ( length <= 0 )
    {
        SV_DropClient( clientnum, "connection closed" );
        return ;
    }
    
    // @TODO(Karlo): Parsing messages with MSG system
    Com_Printf( "Received %d bytes from client %d\n", length, clientnum ); 
    
}

client_t *SV_GetClient( int clientenum )
{
    
    if ( clientenum < 0 || clientenum >= MAX_CLIENTS )
    {
        return ( NULL );
    }
    
    return &sv.clients[clientenum]; 
    
}
