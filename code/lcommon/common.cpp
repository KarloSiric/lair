/*======================================================================
   File: common.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 12:27:04
   Last Modified by: ksiric
   Last Modified: 2026-02-16 00:19:15
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 0.1.0
 ======================================================================
                                                                       */



#include "lcommon.h"
#include "platform.h"

#include <stdio.h>
#include <stdarg.h>


void Com_Printf( const char *fmt, ... )
{
    
    char buf[4096];
    
    va_list args;
    
    va_start( args, fmt );
    
    vsnprintf( buf, sizeof( buf ), fmt, args );
    
    va_end( args );
    
    Sys_Print( buf );
    
}


void Com_DPrintf( const char *fmt, ... )
{
    
    char buf[4096];
    
    va_list args; 
    
    va_start( args, fmt );
    vsnprintf( buf, sizeof( buf ), fmt, args );
    va_end( args ); 
    
    Sys_Print( buf );
    
}


void Com_Error( lerror_t level, const char *fmt, ... )
{
    
    char buf[4096];
    
    va_list args;
    
    va_start( args, fmt );
    
    vsnprintf( buf, sizeof( buf ), fmt, args );
    
    va_end( args );
    
    if ( level == ERR_FATAL ) 
    {
        Sys_Error( buf );
    } 
    else 
    {
        Com_Printf( "ERROR: %s\n", buf );
    }
     
     
}


void Com_Init( void )
{
    
    Sys_Init( );
    Net_Init( );
    
    Com_Printf( "%s %S %S\n", LAIR_VERSION, CPUSTRING, __DATE__ );
    Com_Printf( " --------------------------\n" );
    
}


void Com_Shutdown( void ) 
{
    
    Net_Shutdown( );
    Sys_Shutdown( );
    
}



u64 Com_Milliseconds( void )
{
    return Sys_GetMilliseconds( );
}
