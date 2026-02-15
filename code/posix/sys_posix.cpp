/*======================================================================
   File: sys_posix.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-15 17:27:05
   Last Modified by: ksiric
   Last Modified: 2026-02-15 17:38:28
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */




#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


static u64 sys_timeBase = 0;



int Sys_Init( void ) 
{
    
    struct timeval tp;
    gettimeofday( &tp, NULL );
    sys_timeBase = tp.tv_sec;
    
    return ( 0 );
     
}



void Sys_Shutdown( void )
{
    
    
    
    return ;
    
}



void Sys_Print( const char *msg )
{
    
    fputs( msg, stdout );
    
}


void Sys_Error( const char *msg )
{
    
    fprintf( stderr, "Sys_Error: %s\n", msg );
    exit( 1 );
    
}



u64 Sys_GetMilliseconds( void )
{
    
       
    
    
    
    
    
}







