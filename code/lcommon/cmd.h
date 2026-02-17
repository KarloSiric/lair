                            /* LAIR COMMAND SYSTEM HEADER - cmd.h */



#ifndef __LAIR_CMD_H__
#define __LAIR_CMD_H__ 

#include "l_shared.h"
#include "lcommon.h"

#define MAX_CMD_TOKENS    64                     // Max command arguments( tokens ) in a single line
#define MAX_CMD_LENGTH    1024                   // Max single command line length in bytes
#define MAX_CMD_BUFFER    16384                  // Max command buffer length

typedef void ( *cmdfunc_t )( void );             // Adding function pointer so taht we can easily call appropriate function that we need

typedef struct cmd_s {
    
    char                 *name;                  // name so the command that we will execute
    cmdfunc_t             func;                  // function pointer that we will use for functions
    struct cmd_s         *next;                  // pointer to the next command structure, essentially a linked list ( no realloc )
    
} cmd_t;


























#endif                      /* __LAIR_CMD_H__ - END */
