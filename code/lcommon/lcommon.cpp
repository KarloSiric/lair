                        /* lcommon.cpp - Common functions implementation */



#include "lcommon.h"
#include "platform.h"

#include <stdio.h>
#include <stdarg.h>


void Com_Printf( const char *fmt, ... )
{
    
    va_list argptr;
    char msg[MAX_STRING_CHARS];
    
    va_start( argptr, fmt );
    vsnprintf( msg, sizeof( msg ), fmt, argptr );
    va_end( argptr );
    
    Sys_Print( msg );
    
}


void Com_DPrintf( const char *fmt, ... )
{
    
    va_list argptr;
    char msg[MAX_STRING_CHARS];
    
    // @Note( Karlo ): only print if developer mode is on
    // TODO: Check developer cvar here
    
    va_start( argptr, fmt );
    vsnprintf( msg, sizeof( msg ), fmt, argptr );
    va_end( argptr );
    
    Sys_Print( msg );
    
}


void Com_Error( lerror_t level, const char *fmt, ... )
{
    
    va_list argptr;
    char msg[MAX_STRING_CHARS];
    
    va_start( argptr, fmt );
    vsnprintf( msg, sizeof( msg ), fmt, argptr );
    va_end( argptr );
    
    if ( level == ERR_FATAL )
    {  
        Sys_Error( msg );
        // never returns
    }
    
    Com_Printf( "ERROR: %s\n", msg );
    
    // TODO (Karlo ): Need to handle the the other ERR enums once we start implementing them so
    
    /*
    
    
    
    
    
    
    
    
    */
    
    
    
}













