/*======================================================================
   File: sv_main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 01:36:42
   Last Modified by: ksiric
   Last Modified: 2026-02-17 14:11:41
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

	fd_set readfds; // bit array usually 1024 bits
	struct timeval timeout;
	int maxfd;
	int clientnum;
	lsocket clientsocket;

	FD_ZERO( &readfds );
	FD_SET( sv.socket, &readfds );
	maxfd = sv.socket;

	// Adding all connected sockets
	for ( int i = 0; i < MAX_CLIENTS; i++ ) {
		if ( sv.clients[i].active ) {
			FD_SET( sv.clients[i].socket, &readfds );
			if ( sv.clients[i].socket > maxfd ) {
				maxfd = sv.clients[i].socket;
			}
		}
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 10000; // 10 ms

	if ( select( maxfd + 1, &readfds, NULL, NULL, &timeout ) <= 0 ) {
		return;
	}

	if ( FD_ISSET( sv.socket, &readfds ) ) {
		clientsocket = Net_Accept( sv.socket );

		if ( clientsocket != INVALID_SOCKET_HANDLE ) {
			clientnum = SV_FindFreeClientSlot();

			if ( clientnum == -1 ) {
				Com_Printf( "Server full, rejecting client\n" );
				Net_Close( clientsocket );

			} else {
				sv.clients[clientnum].active = ltrue;
				sv.clients[clientnum].socket = clientsocket;
				sv.clients[clientnum].status = STATUS_ONLINE;
				sv.numclients++;
                
                Com_Printf( "Client %d connected (awaiting identification)\n", clientnum );
                
			}
		}
	}

	for ( int i = 0; i < MAX_CLIENTS; i++ ) {
		if ( !sv.clients[i].active ) {
			continue;
		}

		if ( FD_ISSET( sv.clients[i].socket, &readfds ) ) {
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
	sv_client_t *client = SV_GetClient( clientnum );

	if ( !client || !client->active ) {
		return;
	}
    
    // @NOTE(KARLO): Adding notify for all users that the client has left
    SV_BroadcastUserLeave( clientnum );

	Com_Printf( "Client %d: %s dropped: %s\n", clientnum, client->name, reason );
    
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

void SV_ReadClientMessage( int clientnum ) { 
	sv_client_t *client = SV_GetClient( clientnum );
	byte buf[MAX_MSG_LEN];
    msg_t msg;
    int length;
    int type;

	length = Net_Recv( client->socket, buf, sizeof( buf ) );

	if ( length <= 0 ) {
		SV_DropClient( clientnum, length == 0 ? "disconnected" : "connection error" );
		return;
	}
    
    
    // @TODO(KARLO): Adding proper message initialization 
    
    MSG_Init( &msg, buf, length );
    msg.cursize = length;
    
    type = MSG_ReadByte( &msg );
    
    switch ( type ) 
    {
        
        case MSG_CONNECT:
            SV_HandleConnect( clientnum, &msg );
            break;
        case MSG_CHAT:
            SV_HandleChat( clientnum, &msg );
            break;
        case MSG_DISCONNECT:
            SV_DropClient( clientnum, "client quit" );
            break; 
        default:    
            Com_Printf( "Unknown message type %d from client %d: %s\n", type, clientnum, sv.clients[clientnum].name );
            break;
    }
    
}

void SV_HandleConnect( int clientnum, msg_t *msg ) {
	sv_client_t *client = SV_GetClient( clientnum );

	char *name;

	name = MSG_ReadString( msg );

	strncpy( client->name, name, MAX_USERNAME - 1 );

	client->name[MAX_USERNAME - 1] = '\0';

	Com_Printf( "Client %d: set name: %s\n", clientnum, client->name );

	// We send a welcome to the client from the server
	SV_SendWelcome( clientnum );

	SV_BroadcastUserJoin( clientnum );
}

void SV_HandleChat( int clientnum, msg_t *msg ) {
    
    sv_client_t *client = SV_GetClient( clientnum );
    char *text;
    
    text = MSG_ReadString( msg );
    
    Com_Printf( "~%s: %s\n", client->name, text );
    
    SV_BroadcastChat( clientnum, text ); 
    
}

void SV_SendWelcome( int clientnum ) {
    
    sv_client_t *client = SV_GetClient( clientnum );
    msg_t msg;
    byte buf[MAX_MSG_LEN];
    
    MSG_Init( &msg, buf, sizeof( buf ) );
    
    MSG_WriteByte( &msg, MSG_CONNECT_ACCEPTED );
    MSG_WriteByte( &msg, clientnum );
    MSG_WriteString( &msg, "Welcome to Lair!" );
    
    Net_Send( client->socket, msg.data, msg.cursize );
    
}

void SV_BroadcastUserJoin( int clientnum ) {
    
    sv_client_t *client = SV_GetClient( clientnum );
    byte buf[MAX_MSG_LEN];
    msg_t msg;
    
    MSG_Init( &msg, buf, sizeof( buf ) );
    
    MSG_WriteByte( &msg, MSG_USERJOIN );
    MSG_WriteByte( &msg, clientnum );
    MSG_WriteString( &msg, client->name );
    MSG_WriteByte( &msg, client->status );
    
    SV_BroadcastMessage( &msg ); 
     
}

void SV_BroadcastUserLeave( int clientnum ) {
    
    sv_client_t *client = SV_GetClient( clientnum );
    byte buf[MAX_MSG_LEN];
    msg_t msg;
    
    if ( !client || !client->active || client->name[0] == '\0' )
    {
        return ; // We do not broadcast if no name is set
    } 
    
    MSG_Init( &msg, buf, sizeof( buf ) );
    
    MSG_WriteByte( &msg, MSG_USERLEAVE );
    MSG_WriteByte( &msg, clientnum );
    MSG_WriteString( &msg, client->name );
    
    SV_BroadcastMessage( &msg ); 
    
}

void SV_BroadcastChat( int clientnum, const char *text ) {
    
    sv_client_t *client = SV_GetClient( clientnum );
    byte buf[MAX_MSG_LEN];
    msg_t msg;
    
    MSG_Init( &msg, buf, sizeof( buf ) );
    
    MSG_WriteByte( &msg, MSG_CHAT );
    MSG_WriteByte( &msg, clientnum );
    MSG_WriteString( &msg, client->name );
    MSG_WriteString( &msg, text );
    
    SV_BroadcastMessage( &msg );  
    
}

sv_client_t *SV_GetClient( int clientenum ) {
	if ( clientenum < 0 || clientenum >= MAX_CLIENTS ) {
		return ( NULL );
	}

	return &sv.clients[clientenum];
}
