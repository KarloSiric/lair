/*======================================================================
   File: cl_main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-17 01:17:55
   Last Modified by: ksiric
   Last Modified: 2026-02-17 13:25:39
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

#include <cstdio>
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

	if ( cl.socket == INVALID_SOCKET_HANDLE ) {
		Com_Printf( "CL_Connect: Failed to create socket\n" );
		return;
	}

	c = Net_Connect( cl.socket, host, port );

	if ( c == -1 ) {
		Com_Printf( "CL_Connect: Failed to connect to %s:%d\n", host, port );
		Net_Close( cl.socket );
		cl.socket = INVALID_SOCKET_HANDLE;
		return;
	}

	cl.connected = ltrue;
	Com_Printf( "Connected to %s:%d\n", host, port );
}

void CL_Disconnect( void ) {
	if ( !cl.connected ) {
		return;
	}

	CL_SendDisconnect();

	Net_Close( cl.socket );
	cl.socket = INVALID_SOCKET_HANDLE;
	cl.connected = lfalse;

	Com_Printf( "Disconnected from server\n" );
}

void CL_SendConnect( const char *name ) {
	byte buf[MAX_MSG_LEN];
	msg_t msg;

	if ( !cl.connected ) {
		return;
	}

	strncpy( cl.name, name, MAX_USERNAME - 1 );
	cl.name[MAX_USERNAME - 1] = '\0';

	MSG_Init( &msg, buf, sizeof( buf ) );
	MSG_WriteByte( &msg, MSG_CONNECT );
	MSG_WriteString( &msg, name );

	Net_Send( cl.socket, msg.data, msg.cursize );

	Com_Printf( "Sent connect as '%s'\n", cl.name );
}

void CL_SendChat( const char *text ) {
	byte buf[MAX_MSG_LEN];
	msg_t msg;

	if ( !cl.connected ) {
		return;
	}

	MSG_Init( &msg, buf, sizeof( buf ) );
	MSG_WriteByte( &msg, MSG_CHAT );
	MSG_WriteString( &msg, text );

	Net_Send( cl.socket, msg.data, msg.cursize );
}

void CL_SendDisconnect( void ) {
	byte buf[MAX_MSG_LEN];
	msg_t msg;

	if ( !cl.connected ) {
		return;
	}

	MSG_Init( &msg, buf, sizeof( buf ) );
	MSG_WriteByte( &msg, MSG_DISCONNECT );

	Net_Send( cl.socket, msg.data, msg.cursize );
}

void CL_ReadServerMessages( void ) {
	byte buf[MAX_MSG_LEN];
	msg_t msg;
	int length;
	int type;

	length = Net_Recv( cl.socket, buf, sizeof( buf ) );

	if ( length <= 0 ) {
		Com_Printf( "Server closed connection\n" );
		cl.connected = lfalse;
		return;
	}

	MSG_Init( &msg, buf, length );

	msg.cursize = length;

	type = MSG_ReadByte( &msg );

	switch ( type ) {
        
        case MSG_CONNECT_ACCEPTED: 
        {
            cl.clientnum =  MSG_ReadByte( &msg );
            char *welcome = MSG_ReadString( &msg );
            Com_Printf( "%s (You are client %d)\n", welcome, cl.clientnum );
            break;
        }
        
        case MSG_CONNECT_DENIED: 
        {
            int reason = MSG_ReadByte( &msg );
            Com_Printf( "Connection denied (reason: %d)\n", reason );
            cl.connected = lfalse;
            break;  
        }
        
        case MSG_CHAT: 
        {
            int fromid = MSG_ReadByte( &msg );
            char name[MAX_USERNAME];
            char text[MAX_STRING_CHARS];
            
            strncpy( name, MSG_ReadString( &msg ), MAX_USERNAME - 1 );
            name[MAX_USERNAME - 1] = '\0';
            
            strncpy( text, MSG_ReadString( &msg ), MAX_STRING_CHARS - 1);
            text[MAX_STRING_CHARS - 1] = '\0';
            
            Com_Printf( "~%s: %s\n", name, text );
            break; 
        }
        
        case MSG_USERJOIN: 
        {
            int id = MSG_ReadByte( &msg );
            char *name = MSG_ReadString( &msg );
            int status = MSG_ReadByte( &msg );
            
            Com_Printf( "* %s joined\n", name );
            break;
        }
        
        case MSG_USERLEAVE: 
        {
            int id = MSG_ReadByte( &msg );
            char *name = MSG_ReadString( &msg );
            Com_Printf( "* %s left\n", name );
            break;
        }
        
        default: 
        {
            Com_Printf( "Unknown message: %d\n", type );
            break;    
        }
	}
}

void CL_Frame( void ) { 
    fd_set readfds;
    struct timeval timeout;
    
    if ( !cl.connected ) {
        return ;
    }
    
    FD_ZERO( &readfds );
    FD_SET( cl.socket, &readfds );
    
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000; // 10 ms
    
    if ( select( cl.socket + 1, &readfds, NULL, NULL, &timeout) > 0 ) 
    {
        if ( FD_ISSET( cl.socket, &readfds ) )
        {
            CL_ReadServerMessages();
        }    
    } 
    
}

void CL_Shutdown( void ) {
    
    if ( !cl.connected ) {
        return ;
    }
    
    CL_Disconnect();
    
    Com_Printf( "Client shutdown complete!\n" ); 
        
}
