                            /* LAIR SERVER HEADER - server.h */


#ifndef __SERVER_H__
#define __SERVER_H__ 


#include "l_shared.h"
#include "lcommon.h"
#include "platform.h"
#include "msg.h"


typedef struct {
    
    lsocket         socket;
    lboolean        active;
    char            name[MAX_USERNAME];
    userstatus_t    status;
    u64             lastmsg;
    
    
} sv_client_t;



typedef struct {
    
    lsocket         socket;                  // server socket
    lboolean        initialized;             // is the server initialized or not ?
    
    sv_client_t

     clients[MAX_CLIENTS];    // Connected clients array
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
sv_client_t

  *SV_GetClient( int clientnum );
void         SV_HandleConnect( int clientnum, msg_t *msg );
void         SV_HandleChat( int clientnum, msg_t *msg );
void         SV_SendWelcome( int clientnum );
void         SV_BroadcastUserJoin( int clientnum );
void         SV_BroadcastUserLeave( int clientnum );
void         SV_BroadcastChat( int clientnum, const char *text );




#endif     /* __SERVER_H__ - END */
