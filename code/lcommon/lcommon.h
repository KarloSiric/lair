                            /* LAIR COMMON HEADER - lcommon.h */
                            

#ifndef __LAIR_COMMON_H
#define __LAIR_COMMON_H 


#include "l_shared.h"



/*
==============================================================
COMMON FUNCTIONS (lcommon.h)
==============================================================
*/


/* Printing */
void Com_Printf( const char *fmt, ... );


void Com_DPrintf( const char *fmt, ... );


/* Errors */
void Com_Error( lerror_t level, const char *fmt, ... );


/* Initialization */
void Com_Init( void );
void Com_Shutdown( void );


/* Time */
u64 Com_Milliseconds( void );



#endif        /* __LAIR_COMMON_H END */
