/* LAIR TYPES HEADER */



#ifndef __LAIR_TYPES_H__
#define __LAIR_TYPES_H__



#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>




/* boolean */

typedef enum {
    
    lfalse,
    ltrue
    
} lboolean;


typedef unsigned char byte;


/* Unsigned integers */

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;



/* Signed integers */

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;



/* Floating point values */

typedef float    f32;         // 32 bits pretty much everywhere ( 4 bytes )
typedef double   f64;         // 64 bits 



/* Defining math values */

typedef f32      vec_t;
typedef vec_t    vec2_t[2];
typedef vec_t    vec3_t[3];
typedef vec_t    vec4_t[4];


/* Size values */


typedef size_t    usize;
typedef ptrdiff_t ssize;



#endif        /* __LAIR_TYPES_H END */
