                            /* LAIR SERVER HEADER - server.h */


#ifndef __SERVER_H__
#define __SERVER_H__ 


#include "l_shared.h"
#include "lcommon.h"
#include "platform.h"


typedef struct {
    
    lsocket socket;
    lboolean initialized;
    
} server_t;

extern server_t sv;

void SV_Init( void );
void SV_Shutdown( void );
void SV_Frame( void ); 


#endif     /* __SERVER_H__ - END */
