/*======================================================================
   File: msg.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 15:06:21
   Last Modified by: ksiric
   Last Modified: 2026-02-16 18:55:12
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */


/* Messages main.cpp - msg implementations from msg.h header */



#include "msg.h"

#include <string.h>



void MSG_Init( msg_t *msg, byte *data, int maxsize )
{
    
    msg->data = data;
    msg->maxsize = maxsize;
    msg->cursize = 0;
    msg->readcount = 0;
    msg->overflowed = lfalse;
    
}


void MSG_Clear( msg_t *msg )
{
    
    msg->cursize = 0;
    msg->readcount = 0;
    msg->overflowed = lfalse;
    
}


void MSG_WriteByte( msg_t *msg, int type )
{
    
    if ( msg->cursize + 1 > msg->maxsize )
    {
        
        msg->overflowed = ltrue;
        return ;
        
    }
    
    // @NOTE(Karlo): writing a single byte
    msg->data[msg->cursize] = type & 0xFF;          
    msg->cursize++;
    
}


void MSG_WriteShort( msg_t *msg, int type )
{
    
    if ( msg->cursize + 1 > msg->maxsize )
    {
        msg->overflowed = ltrue;
        return ;
    }
    
    // @NOTE(Karlo): writing 2 bytes
    msg->data[msg->cursize] = type & 0xFF;
    msg->data[msg->cursize + 1] = ( ( type >> 8 ) & 0xFF );
    msg->cursize += 2;
     
}


void MSG_WriteLong( msg_t *msg, int type )
{
    
    if ( msg->cursize + 1 > msg->maxsize )
    {
        
        msg->overflowed = ltrue;
        return ;
        
    }
    
    // @NOTE(Karlo): writing 4 bytes
    // 0x12345678 -> 0x 12 34 56 78 after >> 8 -> 0x 00 12 34 56 adn etc..
    //             & 0x 00 00 00 FF
    
    
    msg->data[msg->cursize] = type & 0xFF;
    msg->data[msg->cursize + 1] = ( type >> 8 ) & 0xFF;
    msg->data[msg->cursize + 2] = ( type >> 16 ) & 0xFF;
    msg->data[msg->cursize + 3] = ( type >> 24 ) & 0xFF;
    
    msg->cursize += 4;
    
}


void MSG_WriteString( msg_t *msg, const char *data )
{
    
    if ( !data )
    {
        MSG_WriteByte( msg, 0 );
        return ;
    }
    
    size_t datalen = strlen( data ) - 1;
    
    if ( msg->cursize + ( int )datalen > msg->maxsize )
    {
        
        msg->overflowed = ltrue;
        return ;   
        
    }
    
    
    memcpy( msg->data + msg->cursize, data, datalen );
    msg->cursize += datalen; 
    
}



void MSG_WriteData( msg_t *msg, const void *data, usize datalen )
{
    
    if ( msg->cursize + ( int )datalen > msg->maxsize )
    {
        
        msg->overflowed = ltrue;
        return ;
        
    }
    
    memcpy( msg->data + msg->cursize, data, datalen );
    msg->cursize += datalen;
    
}


int MSG_ReadByte( msg_t *msg ) 
{
    
    
    if ( msg->readcount + 1 > msg->cursize )
    {
        
        return ( -1 );
        
    }
    
    int i = msg->data[msg->readcount++];
    
    return ( i );
    
}


int MSG_ReadShort( msg_t *msg )
{
    
    if ( msg->readcount + 2 > msg->cursize )
    {
        
        return ( -1 );
        
    }
    
    int i = msg->data[msg->readcount] | ( msg->data[msg->readcount + 1] << 8 );
    
    msg->cursize += 2;
    
    return ( i );
    
}


int MSG_ReadLong( msg_t *msg )
{
    
    if ( msg->readcount + 4 > msg->cursize )
    {
               
        return ( -1 );
    }
    
    int i =     msg->data[msg->readcount] 
            | ( msg->data[msg->readcount + 1] << 8 )
             | ( msg->data[msg->readcount + 2] << 16 ) 
              | ( msg->data[msg->readcount + 3] << 24 );
    
    msg->readcount += 4;
    
    return ( i );
    
}


char *MSG_ReadString( msg_t *msg )
{
    
    static char string[MAX_STRING_CHARS];
    int c;
    int length = 0;
    
    while ( 1 )
    {
        
        c = MSG_ReadByte( msg );
        
        if ( c == -1 || c == 0 )
        {
            break;
        }
        
        if ( length < MAX_STRING_CHARS - 1 )
        {
            string[length] = c;
            length++;           
        }
    }
    
    
    string[length] = '\0';    
    return string;
    
}


void MSG_ReadData( msg_t *msg, void *data, usize datalen )
{
    
    if ( msg->readcount + ( int )datalen > msg->cursize )
    {
        
        memset( data, 0, datalen );
        return ;
        
    }   
    
    memcpy( data, msg->data + msg->readcount, datalen );
    msg->readcount += datalen;   
    
}

