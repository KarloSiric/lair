                            /* LAIR CLIENT HEADER - client.h */



#ifndef __LAIR_CLIENT_H__
#define __LAIR_CLIENT_H__ 

#include "l_shared.h"
#include "msg.h"
#include "lcommon.h"


typedef struct {
    
    lboolean        connected;               // Slot in use or not ?
    lsocket         socket;                  // Client's socket
    int             clientnum;               // Client's id number used for FD_SET to setting each bit array index to slot being used
    char            name[MAX_USERNAME];      // Username
    
} cl_client_t;


extern cl_client_t cl;








#endif
