/* 
 *  NOTICE!
 *
 *  This is solely for testing the macros file. 
 *  Do not include this in your project
 */


#include "macros.h"
//#include "macros_ext.h"










 /*#############################################
   ####                                     ####
   ####         TESTS BEGIN HERE            ####
   ####                                     ####
   ############################################*/



 /* Test setup */
////////////////

#define A "sand"
#define B gold
#define C 12
#define D void
#define E (int*)void
#define F D
#define G T(F)
#define H F
#define I G
// #define A "sand"
// #define B gold
// #define C 12
// #define D void
// #define E (int*)void
// #define F D
// #define G T(F)
// #define H F
// #define I G

// #define T(__n) MAN##__n
#define T(__n) MAN##__n


#define ALL_SAFE_TESTS B, C, D, F, G, H, I, T
#define ALL_TESTS A, B, C, D, E, F, G, H, I, T
// #define ALL_SAFE_TESTS B, C, D, F, G, H, I, T
// #define ALL_TESTS A, B, C, D, E, F, G, H, I, T








 /* Order of operation tests */
//////////////////////////////

// B##C
B##C

// #B###C (errors)
//#B###C

#define TEST_1(__a, __b) B##C##__a##__b #__b
// #define TEST_1(__a, __b) B##C##__a##__b #__b
// TEST_1(A, B)
TEST_1(A, B)


// B##C##__a##__b #__b #B
B##C##__a##__b #__b #B




// #B (errors)
//#B
// # B (errors)
//# B
// #define B (just redefines B)
//#define B






    //////////////////////////////////////
   ////                              ////
  ////      FUNDAMENTAL MACROS      ////
 ////                              ////
//////////////////////////////////////



 /* Macro tests */
/////////////////


// CONCAT(ALL_SAFE_TESTS)
CONCAT(ALL_SAFE_TESTS)

// _CONCAT(B, C)
_CONCAT(B, C)
// _CONCAT(D, F)
_CONCAT(D, F)
// _CONCAT(G, H)
_CONCAT(G, H)
// _CONCAT(I, T)
_CONCAT(I, T)
// _CONCAT(I, T(I))
_CONCAT(I, T(I))



#define TEST_2A(__n) I##__n
#define TEST_2B(__n) _CONCAT(I, __n)
#define TEST_2C(__n) CONCAT(I, __n)
// #define TEST_2A(__n) I##__n
// #define TEST_2B(__n) _CONCAT(I, __n)
// #define TEST_2C(__n) CONCAT(I, __n)

// TEST_2A(I)
TEST_2A(I)
// TEST_2B(I)
TEST_2B(I)
// TEST_2C(I)
TEST_2C(I)



// UNPACK((A, B, C, D))
UNPACK((A, B, C, D))
// UNPACK((((A), B), C, D))
UNPACK((((A), B), C, D))
// UNPACK(((A, B, C, D)))
UNPACK(((A, B, C, D)))


#define TEST_FUNPACK(__0, __1, __2, __3) __0+__1+__2+__3

// FUNPACK(TEST_FUNPACK, MERGE((0, 1), (2, 3)))
FUNPACK(TEST_FUNPACK, MERGE((0, 1), (2, 3)))



// #define TEST_X(__a, __b, __c, __d) _TEST_X1((__a, __b, __c, __d))
// #define _TEST_X1(__n) _TEST_X2(UNPACK(__n), xx, xx, xx, xx)\
// _TEST_X3(UNPACK(__n), xx, xx, xx, xx) \
// _TEST_X5(UNPACK(__n))
// #define _TEST_X2(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
// #define _TEST_X3(...) _TEST_X4(__VA_ARGS__)
// #define _TEST_X4(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
// #define _TEST_X5(...) __VA_ARGS__
#define TEST_X(__a, __b, __c, __d) _TEST_X1((__a, __b, __c, __d))
#define _TEST_X1(__n) _TEST_X2(UNPACK(__n), xx, xx, xx, xx) \n \
_TEST_X3(UNPACK(__n), xx, xx, xx, xx) \n \
_TEST_X5(UNPACK(__n))
#define _TEST_X2(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
#define _TEST_X3(...) _TEST_X4(__VA_ARGS__)
#define _TEST_X4(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
//errors #define _TEST_X5(__a, __b, __c, __d)
#define _TEST_X5(...) __VA_ARGS__


// TEST_X(a, b, c, d)
TEST_X(a, b, c, d)




// #define TEST_3A(...) UNPACK((__VA_ARGS__))
// #define TEST_3B(__n) UNPACK(__n)
// #define TEST_3C(__a, __b, __c, __d) _TEST_3C(UNPACK((__a, __b, __c, __d)))
// #define _TEST_3C(__a, __b, __c, __d) __a/__b/__c/__d
#define TEST_3A(...) UNPACK((__VA_ARGS__))
#define TEST_3B(__n) UNPACK(__n)
#define TEST_3C(__a, __b, __c, __d) _TEST_3C(UNPACK((__a, __b, __c, __d)))
#define _TEST_3C_error(__a, __b, __c, __d) __a/__b/__c/__d
#define TEST_3D(__a, __b, __c, __d) FUNPACK(_TEST_3D, (__a, __b, __c, __d))
#define _TEST_3D(__a, __b, __c, __d) __a/__b/__c/__d


// TEST_3A(A, B, C, D)
TEST_3A(A, B, C, D)
// TEST_3B((A, B, C, D))
TEST_3B((A, B, C, D))
/* errors
// TEST_3C(A, B, C, D)
TEST_3C(A, B, C, D)*/
// TEST_3D(A, B, C, D)
TEST_3D(A, B, C, D)



//DECAST((void)k)
DECAST((void)k)
//DECAST((yehee (yolo) holo)(beans)x)
DECAST((yehee (yolo) holo)(beans)x)
//DECAST((boom))
DECAST((boom))





    //////////////////////////////////////
   ////                              ////
  ////      ARRAY MACROS            ////
 ////                              ////
//////////////////////////////////////



// PARAM((A, B, C, D), 0)
PARAM((A, B, C, D), 0)
// PARAM((A, B, C, D), 2)
PARAM((A, B, C, D), 2)
// PARAM((A, B, C, D), 7)
PARAM((A, B, C, D), 7)
// PARAM((ALL_TESTS), 0)
PARAM((ALL_TESTS), 0)
// PARAM((ALL_TESTS), 2)
PARAM((ALL_TESTS), 2)
// PARAM((ALL_TESTS), 7)
PARAM((ALL_TESTS), 7)

// #define PARAM_TEST_1(n, ...) PARAM((__VA_ARGS__), n)
// #define PARAM_TEST_2(...) PARAM(__VA_ARGS__)
#define PARAM_TEST_1(n, ...) PARAM((__VA_ARGS__), n)
#define PARAM_TEST_2(...) PARAM(__VA_ARGS__)

// PARAM_TEST_1(2, A, B, C, D)
PARAM_TEST_1(2, A, B, C, D)
// PARAM_TEST_2((A, B, C, D), 2)
PARAM_TEST_2((A, B, C, D), 2)



//#define PARAM_TEST(a, ...) a __VA_ARGS__
#define PARAM_TEST(a, ...) a __VA_ARGS__

// trurns out variable args ... can accept zero parameters
//PARAM_TEST("x")
PARAM_TEST("x")



/*
// errors
// PLEN((A, B, C, D, E))
//PLEN((A, B, C, D, E))
// PLEN((B, C, D, E))
PLEN((B, C, D, E))
// PLEN((,,,,,,))
PLEN((,,,,,,))
// PLEN(())
PLEN(())
*/


//#define DECAST2(__n) PARAM((_DECAST2_ __n, __n), 1)
//#define _DECAST2_(...) BLANK, BLANK



// UNPACK((x)a)
UNPACK((x)a)
// UNPACK(a)
UNPACK(a)

// DECAST((x)a)
DECAST((x)a)
// DECAST(a)
DECAST(a)




// IFTHEN(ISVOID(0),  \
    // return; ,      \
    // return (0);    \
// )
// IFTHEN(ISVOID(0),  \
    // return; ,      \
    // return (0);    \
// )



// ISVOID(0)
//ISVOID(0)



    //////////////////////////////////////
   ////                              ////
  ////      BOOLEAN MACROS          ////
 ////                              ////
//////////////////////////////////////




// ISPACKED(x)
ISPACKED(x)
// ISPACKED((x))
ISPACKED((x))
// ISPACKED((x)a)
ISPACKED((x)a)



// NOT(true)
NOT(true)
// NOT(false)
NOT(false)
// ISTRUE(true)
/*ISTRUE(true)
// ISTRUE(yope)
ISTRUE(yope)
// ISTRUE(habla dabla)
ISTRUE(habla dabla)
// ISTRUE((bindy, mooby))
ISTRUE((bindy, mooby))
// ISBOOL(true)
ISBOOL(true)
// ISBOOL(bon)
ISBOOL(bon)
// ISVOID(void)
ISVOID(void)
// ISVOID((cast)void)
ISVOID((cast)void)
// ISZERO(0)
ISZERO(0)
// ISZERO(tongues)
ISZERO(tongues)
// ISPACKED(pack)
ISPACKED(pack)
// ISPACKED((A, B, C))
ISPACKED((A, B, C))*/
// ISBYTE(BYTE(5))
/* TODO: remember to re-enable
ISBYTE(BYTE(5))
// ISBYTE((w,0,1,2,3))
ISBYTE((w,0,1,2,3))
// AND(true, true, true)
AND(true, true, true)
// AND(false, true, true)
AND(false, true, true)
// OR(true, true, false, false)
OR(true, true, false, false)
// OR(false, false, false, false, false)
OR(false, false, false, false, false)
// XOR(true true)
XOR(true true)
// XOR(true, false)
XOR(true, false)
// IMPLY(true, false)
IMPLY(true, false)
// IMPLY(false, true)
IMPLY(false, true)
*/




    //////////////////////////////////////
   ////                              ////
  ////      CONDITIONAL MACROS      ////
 ////                              ////
//////////////////////////////////////



// IFTHEN(ISZERO(0), "is zero")
//TODO: remember to re-enable
//IFTHEN(ISZERO(0), "is zero")
// IFTHEN(true, "istrue", "isfalse")
IFTHEN(true, "istrue", "isfalse")
// IFTHEN(false, "istrue", "isfalse")
IFTHEN(false, "istrue", "isfalse")






     //////////////////////////////////////
    ////                              ////
   ////     NUMBER AND               ////
  ////   ARITHMATIC MACROS          ////
 ////                              ////
//////////////////////////////////////
/* TODO: remember to re-enable

// BYTE(2)
BYTE(2)
// BYTE(20)
BYTE(20)
// BYTE(200)
BYTE(200)
// BYTE(2000)
//BYTE(2000)
// BYTE(20000)
//BYTE(20000)


// UNBYTE(BYTE(2))
UNBYTE(BYTE(2))
// UNBYTE(BYTE(111))
UNBYTE(BYTE(111))

// INV(73)
INV(73)
// INV(255)
INV(255)
// INV(NEG(5))
INV(NEG(5))

// NEG(100)
NEG(100)
// NEG(NEG(100))
NEG(NEG(100))

// ADD(100,5)
ADD(100,5)
// ADD(210, 189)
ADD(210, 189)
// SUB(10, 5)
SUB(10, 5)
// SUB(5, 10)
SUB(5, 10)
// INC(10)
INC(10)
// DEC(3)
DEC(3)
*/
