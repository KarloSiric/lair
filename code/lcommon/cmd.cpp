/*======================================================================
   File: cmd.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-17 17:02:56
   Last Modified by: ksiric
   Last Modified: 2026-02-17 17:33:14
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */


#include "cmd.h"



static int cmd_argc;
static char *cmd_argv[MAX_CMD_TOKENS];
static char cmd_tokenized[MAX_CMD_LENGTH];
static char cmd_args[MAX_CMD_LENGTH];

static cmd_t *cmd_list = NULL;

static char cmd_buffer[MAX_CMD_BUFFER];
static int cmd_buffer_len = 0;


void Cmd_TokenizeString( const char *text ) {
    
    char *out;
    
    cmd_argc = 0;
    cmd_args[0] = '\0';
    
    if ( !text || text[0] == '\0' ) 
    {
        return ;   
    }
    
    if ( text[0] == '/' ) 
    {
        text++;
    }
    
    out = cmd_tokenized;
    
    while ( 1 )
    {
            
        while ( *text && *text <= ' ' )             // loop while essentially we are skipping all whitespace and control characters, clean
        {
            text++;
        }
        
        if ( !*text )                               // end of string check ?
        {
            break;
        }
        
        if ( cmd_argc >= MAX_CMD_TOKENS )           // out of tokens check ?
        {
            break;
        }
         
        if ( *text == '"' )                         // checking if we are dealing with a quoted string, Quoted Tokenization
        {
            text++;
            while ( *text && *text != '"' )
            {
                *out++ = *text++;
            }
            
            if ( *text == '"' ) 
            {
                text++;
            }
            
            *out++ = '\0';                          // null terminate the token
        }
        else                                        // check regular characters, not quotes
        {
            while ( *text > ' ' )
            {
                *out++ = *text++;               
            }
            
            *out++ = '\0';                          // null terminate the token
        }
        
    }                                               // end while( 1 ) loop!
}

int Cmd_Argc( void ) {
    
    return cmd_argc;
    
}

char *Cmd_Argv( int n ) {
    if ( n < 0 || n > MAX_CMD_TOKENS ) {
        return ( char *)"";                         // DO not return NULL, instead just return empty string
    }
    
    return cmd_argv[n];
    
}








