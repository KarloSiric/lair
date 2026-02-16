                            /* LAIR SERVER HEADER - server.h */


#ifndef __SERVER_H__
#define __SERVER_H__ 


#include "l_shared.h"
#include "lcommon.h"
#include "platform.h"
#include "msg.h"



typedef struct {
    
    lboolean        active;                  // Slot in use or not ?
    lsocket         socket;                  // Client's socket
    char            name[MAX_USERNAME];      // Username
    userstatus_t    status;                  // Client status: online, away, muted etc...
    u64             lastmsg;                 // Last message for timeout checking
    
} client_t;



typedef struct {
    
    lsocket         socket;                  // server socket
    lboolean        initialized;             // is the server initialized or not ?
    
    client_t        clients[MAX_CLIENTS];    // Connected clients array
    int             numclients;              // Current count
    
} server_t;

extern server_t     sv;

void         SV_Init( void );
void         SV_Shutdown( void );
void         SV_Frame( void ); 

int          SV_FindFreeClientSlot( void );
void         SV_DropClient( int clientnum, const char *reason );
void         SV_BroadcastMessage( msg_t *msg );
void         SV_ReadClientMessage( int clientnum );
client_t    *SV_GetClient( int clientnum );



#endif     /* __SERVER_H__ - END */
