                            /* LAIR MESSAGE HEADER - msg.h */


#ifndef __MSG_H__
#define __MSG_H__ 


#include "l_shared.h"



#define MAX_MSG_LEN       16384



typedef struct {
    
    byte *data;            /* message itself */
    int maxsize;           /* max message size */
    int cursize;           /* current message size so far */
    int readcount;         /* amount that was read so far */
    lboolean overflowed;   /* did it overflow or not */
    
} msg_t;



typedef enum {
    
    MSG_BAD = 0,                // Invalid message

    // -------------------------------------
    // Connection & Authentication
    // -------------------------------------
    MSG_CONNECT,                // Client requests connection
    MSG_CONNECT_ACCEPTED,       // Server accepts client
    MSG_CONNECT_DENIED,         // Server rejects client
    MSG_DISCONNECT,             // Clean disconnect
    MSG_PING,                   // Keepalive ping
    MSG_PONG,                   // Keepalive response

    // -------------------------------------
    // User Management
    // -------------------------------------
    MSG_USERINFO,               // Set/update user info (name, status)
    MSG_USERLIST,               // Full list of connected users
    MSG_USERJOIN,               // User joined server
    MSG_USERLEAVE,              // User left server
    MSG_USERKICK,               // User was kicked
    MSG_USERBAN,                // User was banned
    MSG_USERMUTE,               // User was muted
    MSG_USERUNMUTE,             // User was unmuted

    // -------------------------------------
    // Text Chat
    // -------------------------------------
    MSG_CHAT,                   // Public chat message
    MSG_CHAT_PRIVATE,           // Private message to user
    MSG_CHAT_CHANNEL,           // Message to specific channel
    MSG_CHAT_SERVER,            // Server announcement

    // -------------------------------------
    // Channels
    // -------------------------------------
    MSG_CHANNELLIST,            // List of all channels
    MSG_CHANNELJOIN,            // Join a channel
    MSG_CHANNELLEAVE,           // Leave a channel
    MSG_CHANNELCREATE,          // Create new channel
    MSG_CHANNELDELETE,          // Delete channel
    MSG_CHANNELUSERS,           // Users in a channel

    // -------------------------------------
    // Voice (UDP - later)
    // -------------------------------------
    MSG_VOICEDATA,              // Voice audio chunk
    MSG_VOICESTART,             // Started talking
    MSG_VOICESTOP,              // Stopped talking

    // -------------------------------------
    // Server Control
    // -------------------------------------
    MSG_SERVERINFO,             // Server name, version, etc.
    MSG_SERVERSHUTDOWN,         // Server shutting down
    MSG_SERVERCOMMAND,          // Admin command

    // -------------------------------------
    // Errors
    // -------------------------------------
    MSG_ERROR,                  // Generic error

    MSG_COUNT                   // Total message types
    
} msgtype_t;



typedef enum {
    
    DENY_NONE = 0,
    DENY_SERVERFULL,            // No slots available
    DENY_BANNED,                // User is banned
    DENY_BADPASSWORD,           // Wrong password
    DENY_BADVERSION,            // Client version mismatch
    DENY_BADNAME,               // Invalid username
    DENY_TIMEOUT,               // Connection timed out

    DENY_COUNT
    
} deny_reason_t;








void MSG_Init( msg_t *msg, byte *data, int maxsize );


void MSG_Clear( msg_t *msg );


void MSG_WriteByte( msg_t *msg, int c );


void MSG_WriteShort( msg_t *msg, int c );











#endif
