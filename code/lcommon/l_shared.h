                            /* LAIR TYPES HEADER - l_shared.h */


#ifndef __LAIR_SHARED_H__
#define __LAIR_SHARED_H__



#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>



/*
=============================================================
VERSION
==============================================================
*/

#define LAIR_VERSION           "Lair 1.00a"

/*
=============================================================
CPUSTRING
==============================================================
*/

#ifdef _WIN32

  #ifdef NDEBUG
      #ifdef _M_ARM64
          #define CPUSTRING   "win-arm64"
      #elif defined _M_X64
          #define CPUSTRING   "win-x86_64"
      #elif defined _M_IX86
          #define CPUSTRING   "win-x86"
      #else
          #define CPUSTRING   "win-unknown"
      #endif
  #else
      #ifdef _M_ARM64
          #define CPUSTRING   "win-arm64-debug"
      #elif defined _M_X64
          #define CPUSTRING   "win-x86_64-debug"
      #elif defined _M_IX86
          #define CPUSTRING   "win-x86-debug"
      #else
          #define CPUSTRING   "win-unknown-debug"
      #endif
  #endif

#endif


#ifdef __APPLE__

  #ifdef __arm64__
      #define CPUSTRING   "macos-arm64"
  #elif defined __x86_64__
      #define CPUSTRING   "macos-x86_64"
  #else
      #define CPUSTRING   "macos-unknown"
  #endif

#endif


#ifdef __linux__

  #ifdef __x86_64__
      #define CPUSTRING   "linux-x86_64"
  #elif defined __aarch64__
      #define CPUSTRING   "linux-arm64"
  #elif defined __i386__
      #define CPUSTRING   "linux-i386"
  #else
      #define CPUSTRING   "linux-unknown"
  #endif

#endif



/*
==============================================================
PRINT LEVELS
==============================================================
*/



typedef enum {
    
    PRINT_ALL,
    PRINT_DEVELOPER,
    PRINT_WARNING,
    PRINT_ERROR
    
} printlevel_t;



/*
=============================================================
ERROR LEVELS
=============================================================
*/


typedef enum {
    
    ERR_FATAL,
    ERR_DROP,
    ERR_SERVERDISCONNECT,
    ERR_DISCONNECT,
    ERR_TIMEOUT,
    ERR_AUTH
    
} lerror_t;



typedef enum {
    
    lfalse,
    ltrue
    
} lboolean;



/*
==============================================================
LIMITS
==============================================================
*/



#define MAX_STRING_CHARS    1024
#define MAX_USERNAME        32
#define MAX_MESSAGE_LEN     4096
#define MAX_ROOM_NAME       64
#define MAX_PATH_LEN        256

#define MAX_CLIENTS         256
#define MAX_ROOMS           64

#define DEFAULT_PORT        7331


/*
==============================================================
UTILITY MACROS
==============================================================
*/



#define MIN( a,b )            ( ( a ) < ( b ) ? ( a ) : ( b ))
#define MAX( a,b )            ( ( a ) > ( b ) ? ( a ) : ( b ))
#define CLAMP( x,lo,hi )      ( MIN( MAX( ( x ),( lo ) ),( hi ) ) )

#define ARRAY_COUNT( arr )    ( sizeof( arr ) / sizeof( ( arr )[0]) )



/*
==============================================================
SOCKET TYPE
==============================================================
*/

typedef int lsockett;

#define INVALID_SOCKET_HANDLE   (-1)



/*
==============================================================
CONNECTION STATE
==============================================================
*/

typedef enum {
    
  CONN_DISCONNECTED,
  CONN_CONNECTING,
  CONN_CONNECTED,
  CONN_AUTHENTICATED
  
} connstate_t;



/*
==============================================================
DATA TYPES
==============================================================
*/



typedef unsigned char byte;



typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;



typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;



typedef float    f32;         // 32 bits pretty much everywhere ( 4 bytes )
typedef double   f64;         // 64 bits 



typedef f32      vec_t;
typedef vec_t    vec2_t[2];
typedef vec_t    vec3_t[3];
typedef vec_t    vec4_t[4];



typedef size_t    usize;
typedef ptrdiff_t ssize;



#endif        /* __LAIR_SHARED_H__ END */
