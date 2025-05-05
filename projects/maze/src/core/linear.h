#ifndef LINEAR_H
#define LINEAR_H

#include <stdint.h>
#include <math.h>
#include <macros.h>

/* This is a header library.
 * The programmer can optionally 
 * create a c counterpart file
 */

#define FORCE_INLINE __attribute__((always_inline))




#define WECT2_TYPEGEN(__type) \
typedef union {
    struct {
        __type x;
        __type y;
    };
    struct {
        __type i;
        __type j;
    };
    __type raw[2];
} vect2_16;


// single word vector3 type
#define VECT3_WORD_TYPEGEN(__type, __bits, __pad, __pad0) \
typedef union {                                 \
    struct {                                    \
        IFNZERO(__pad0, __type _: (__pad0))     \
        __type x: (__bits);                     \
        IFNZERO(__pad, __type _: (__pad))       \
        __type y: (__bits);                     \
        IFNZERO(__pad, __type _: (__pad))       \
        __type z: (__bits);                     \
    };                                          \
    struct {                                    \
        IFNZERO(__pad0, __type _: (__pad0))     \
        __type i: (__bits);                     \
        IFNZERO(__pad, __type _: (__pad))       \
        __type j: (__bits);                     \
        IFNZERO(__pad, __type _: (__pad))       \
        __type k: (__bits);                     \
    };                                          \
    __type raw;                                 \
}


// multi word vector3 type
#define VECT3_TYPEGEN(__type) \
typedef union {     \
    struct {        \
        __type x;   \
        __type y;   \
        __type z;   \
    };              \
    struct {        \
        __type i;   \
        __type j;   \
        __type k;   \
    };              \
    __type raw[3];  \
}




typedef union {
    struct {
        uint8_t x: 4;
        uint8_t y: 4;
    };
    struct {
        uint8_t i: 4;
        uint8_t j: 4;
    };
    uint8_t raw;
} uvect2_8


typedef union {
    struct {
        int8_t x: 4;
        int8_t y: 4;
    };
    struct {
        int8_t i: 4;
        int8_t j: 4;
    };
    int8_t raw;
} vect2_8;


// multi word vector2 types
VECT2_TYPEGEN(uint8_t)  uvect2_16;
VECT2_TYPEGEN(uint16_t) uvect2_32;
VECT2_TYPEGEN(uint32_t) uvect2_64;
VECT2_TYPEGEN(uint64_t) uvect2_128;
VECT2_TYPEGEN(int8_t)   vect2_16;
VECT2_TYPEGEN(int16_t)  vect2_32;
VECT2_TYPEGEN(int32_t)  vect2_64;
VECT2_TYPEGEN(int64_t)  vect2_128;

// single word vector3 types
VECT3_WORD_TYPEGEN(uint16_t, 4,  2, 0) uvect3_16;
VECT3_WORD_TYPEGEN(uint32_t, 10, 1, 0) uvect3_32;
VECT3_WORD_TYPEGEN(uint64_t, 21, 0, 1) uvect3_64;
VECT3_WORD_TYPEGEN(int16_t,  4,  2, 0) vect3_16;
VECT3_WORD_TYPEGEN(int32_t,  10, 1, 0) vect3_32;
VECT3_WORD_TYPEGEN(int64_t,  21, 0, 1) vect3_64;

// multi word vector3 types
VECT3_TYPEGEN(uint8_t)  uvect3_24;
VECT3_TYPEGEN(uint16_t) uvect3_48;
VECT3_TYPEGEN(uint32_t) uvect3_96;
VECT3_TYPEGEN(uint64_t) uvect3_192;
VECT3_TYPEGEN(int8_t)   vect3_24;
VECT3_TYPEGEN(int16_t)  vect3_48;
VECT3_TYPEGEN(int32_t)  vect3_96;
VECT3_TYPEGEN(int64_t)  vect3_192;




typedef union {
    uvect2_16;
    uvect2_32;
    uvect2_64;
    uvect2_128;
    vect2_16;
    vect2_32;
    vect2_64;
    vect2_128;

    uvect3_16;
    uvect3_32;
    uvect3_64;
    vect3_16;
    vect3_32;
    vect3_64;

    uvect3_24;
    uvect3_48;
    uvect3_96;
    uvect3_192;
    vect3_24;
    vect3_48;
    vect3_96;
    vect3_192;
} vect;




#define TOVECT(__data, __type)              \
    (__type){                               \
        .i = typeof(__type.i)(__data).i,    \
        .j = typeof(__type.j)(__data).j,    \
        .k = typeof(__type.k)(__data).k     \
    }


/*
Lets look at what operators we need for vectors

> element-by-element
add
sub
mul
div
shr
shl
mod
cap
unit

equ

abs
dot
cross

to-matrix

*/

/*
And some common matrix operators

> element-by-element
add
sub
mul
div
shr
shl
mod

row to vector
column to vector

> matrix based
mmul/prod
inv
rref
trans


*/


#define VECT3_16_BITMASK 0b1111001111001111


FORCE_INLINE vect3_16 vect3_16_add(restrict const vect3_16* v1, restrict const vect3_16* v2) {
    register int16_t val = (v1.raw + v2.raw) & VECT3_16_BITMASK;
    return *(vect3_16*)&val;
}


// I wish C would fully evaluating the result of a typecast before erroring
FORCE_INLINE vect3_16 vect3_16_sub(restrict const vect3_16* v1, restrict const vect3_16* v2) {
    register int16_t val = ((v1.raw | ~VECT3_16_BITMASK) - v2.raw)) & VECT3_16_BITMASK
    return *(vect3_16*)&val;
}


FORCE_INLINE vect3_16 vect3_16_sub(restrict const vect3_16* v1, restrict const vect3_16* v2) {
    
}





FORCE_INLINE vect3_add(void* )








FORCE_INLINE void vect_test() {
    
}


#endif
