                                                        /* LAIR COMMAND SYSTEM HEADER - cmd.h */

#ifndef __LAIR_CMD_H__
#define __LAIR_CMD_H__ 

#include "l_shared.h"
#include "lcommon.h"

#define MAX_CMD_TOKENS              64                   // Max command arguments( tokens ) in a single line
#define MAX_CMD_LENGTH              1024                 // Max single command line length in bytes
#define MAX_CMD_BUFFER              16384                // Max command buffer length

#define CMD_HASH_SIZE               64                   // number of buckets for the hash table
#define CMD_HASH_MASK               (CMD_HASH_SIZE - 1)  // for fast modulo operations so & 63 which is faster and more efficient and produces the same answer

typedef void ( *cmd_func_t )( void );                    // Adding function pointer so taht we can easily call appropriate function that we need
typedef void ( *cmd_errorcallback_t )( const char *, const char * );   // Adding a function pointer so that we can call errors appropriately for all the errors we will need

extern cmd_errorcallback_t Cmd_ErrorCallback;

typedef struct cmd_s {
    char                  *name;                         // name so the command that we will execute
    cmd_func_t             func;                         // function pointer that we will use for functions
    struct cmd_s          *next;                         // pointer to the next command structure, essentially a linked list ( no realloc )
} cmd_t;

// Tokenizer
void        Cmd_TokenizeString( const char *text );      // Function used for taking a input as a string buffer and tokenizing it
int         Cmd_Argc( void );
char        *Cmd_Argv( int n );                          // Func for getting individual tokens from a string
char        *Cmd_Args( void );                           // Func for assembling the entire command out of all tokens in the line extracted

// Registration
void         Cmd_Init( void );
void         Cmd_Shutdown( void );
void         Cmd_AddCommand( const char *cmdname, cmd_func_t func );
void         Cmd_RemoveCommand( const char *cmdname );
cmd_func_t   Cmd_FindCommand( const char *name );

// Command Execution
void         Cmd_ExecuteString( const char *text );

// hashing function
internal u32 Cmd_HashString( const char *str ); 

// Command Buffer
void         Cbuf_Init( void );
void         Cbuf_AddText( const char *text );
void         Cbuf_InsertText( const char *text );
void         Cbuf_Execute( void );

#endif                                                  /* __LAIR_CMD_H__ - END */
