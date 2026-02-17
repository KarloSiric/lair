                                    /* LAIR CLIENT HEADER - client.h */

#ifndef __LAIR_CLIENT_H__
#define __LAIR_CLIENT_H__

#include "l_shared.h"
#include "msg.h"
#include "lcommon.h"

typedef struct {
	lboolean   connected;                  // Slot in use or not ?
	lsocket    socket;                     // Client's socket
	int        clientnum;                  // Client's id number used for FD_SET to setting each bit array index to slot being used
	char       name[MAX_USERNAME];         // Username

} cl_client_t;

extern cl_client_t cl;

void CL_Init( void );

void CL_Shutdown( void );

void CL_Frame( void );

void CL_Connect( const char *host, u16 port );

void CL_Disconnect( void );

void CL_SendConnect( const char *name );

void CL_SendChat( const char *text );

void CL_SendDisconnect( void );

void CL_ReadServerMessages( void );

void CL_Shutdown( void );

#endif /* __LAIR_CLIENT_H__ - END */
