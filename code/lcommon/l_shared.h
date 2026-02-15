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



typedef enum {
    
    lfalse,
    ltrue
    
} lboolean;



typedef enum {
     
    LERR_FATAL,
    LERR_DROP
    
} lerror_t;


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



#endif        /* __LAIR_TYPES_H END */
