                            /* LAIR TYPES HEADER - l_shared.h */



#ifndef __LAIR_SHARED_H__
#define __LAIR_SHARED_H__



#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>


/*
  ======================================
  VERSION
  ======================================
*/

#define LAIR_VERSION           "Lair 1.00a"


/*
  ======================================
  PLATFORM
  ======================================
*/

/* Platform OS detection */

#if defined( _WIN32 )

    #define LAIR_PLATFORM_WINDOWS

#elif defined( __APPLE__ )

    #define LAIR_PLATFORM_MACOS

#elif defined( __linux__ )

    #define LAIR_PLATFORM_LINUX

#else

    #error "Unsupported platform"

#endif


/* CPUSTRING - platform + CPU architecture */

#if defined(LAIR_PLATFORM_WINDOWS)
  #if defined(_M_ARM64)
      #define LAIR_ARCH_ARM64
      #if defined(_DEBUG) || defined(DEBUG)
          #define CPUSTRING "win-arm64-debug"
      #else
          #define CPUSTRING "win-arm64"
      #endif
  #elif defined(_M_X64) || defined(_WIN64)
      #define LAIR_ARCH_X64
      #if defined(_DEBUG) || defined(DEBUG)
          #define CPUSTRING "win-x86_64-debug"
      #else
          #define CPUSTRING "win-x86_64"
      #endif
  #elif defined(_M_IX86)
      #define LAIR_ARCH_X86
      #if defined(_DEBUG) || defined(DEBUG)
          #define CPUSTRING "win-x86-debug"
      #else
          #define CPUSTRING "win-x86"
      #endif
  #else
      #define CPUSTRING "win-unknown"
  #endif

#elif defined(LAIR_PLATFORM_MACOS)
  #if defined(__arm64__) || defined(__aarch64__)
      #define LAIR_ARCH_ARM64
      #if defined(DEBUG)
          #define CPUSTRING "macos-arm64-debug"
      #else
          #define CPUSTRING "macos-arm64"
      #endif
  #elif defined(__x86_64__)
      #define LAIR_ARCH_X64
      #if defined(DEBUG)
          #define CPUSTRING "macos-x86_64-debug"
      #else
          #define CPUSTRING "macos-x86_64"
      #endif
  #else
      #define CPUSTRING "macos-unknown"
  #endif

#elif defined(LAIR_PLATFORM_LINUX)
  #if defined(__aarch64__)
      #define LAIR_ARCH_ARM64
      #if defined(DEBUG)
          #define CPUSTRING "linux-arm64-debug"
      #else
          #define CPUSTRING "linux-arm64"
      #endif
  #elif defined(__x86_64__)
      #define LAIR_ARCH_X64
      #if defined(DEBUG)
          #define CPUSTRING "linux-x86_64-debug"
      #else
          #define CPUSTRING "linux-x86_64"
      #endif
  #elif defined(__i386__)
      #define LAIR_ARCH_X86
      #if defined(DEBUG)
          #define CPUSTRING "linux-x86-debug"
      #else
          #define CPUSTRING "linux-x86"
      #endif
  #else
      #define CPUSTRING "linux-unknown"
  #endif

#else
  #define CPUSTRING "unknown"
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
