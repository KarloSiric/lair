                            /* LAIR PLATFORM HEADER - platform.h */


#ifndef __LAIR_PLATFORM_H
#define __LAIR_PLATFORM_H 



#include "l_shared.h"



/*
 ======================================
            SYSTEM ( Sys_ )
 ======================================
*/


int Sys_Init( void );

void Sys_Shutdown( void );

void Sys_Print( const char *msg );

void Sys_Error( const char *msg );

u64 Sys_GetMilliseconds( void );



/*
 ======================================
            NETWORK ( Net_ )
 ======================================
*/


const char *Net_ErrorString( void );

void Net_Init( void );

void Net_Shutdown( void );

lsocket Net_Socket( void );

int Net_Bind( lsocket sock, u16 port );

int Net_Listen( lsocket sock, int backlog );

lsocket Net_Accept( lsocket sock );

int Net_Connect( lsocket sock, const char *host, u16 port );

int Net_Send( lsocket sock, byte *buf, usize buflen );

int Net_Recv( lsocket sock, byte *buf, usize buflen );

void Net_Close( lsocket sock );



#endif             /* __LAIR_PLATFORM_H END */
