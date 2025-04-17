# 0 "macros_test.h"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "macros_test.h"
/* 
 *  NOTICE!
 *
 *  This is solely for testing the macros file. 
 *  Do not include this in your project
 */


# 1 "macros.h" 1





/*  NOTES ON ORDER OF SUBSTITUTION
 *  ------------------------------
 *
 *  - Variables names are passed as parameters, and only
 *    substituted in the macro
 *    (in other words functions are called before variables get substituted)
 *  
 *  - Order goes: Substiute var
 *
 *
 *
 *  WARNINGS
 *  --------
 *  Unpacked values will only register as a single param.
 *  But __VA_ARGS__ likely expands to fill params.
 *  So an extra layer is required for some of these macros.
 *
 *  You can't concat a keyword with a parenthesis.
 *  So in order to combine a keyword with parenthesis to create
 *  a function macro call, you must simply put them next to each-other.
 *  The whitespace is ignored and the preprocessor still sees it as a 
 *  function
 */


//  INSPIRATION FOR THIS LIBRARY:
/*
    Alright lets test an idea.
    To force an undefined value to return nothing
    by having all defined values be defined as multiple arguments, 
    and always only ever returning the second argument
*/

/*
    YES IT WORKS!
    I gave up on the possability months ago!
*/



/*
  --------------
    BRAINSTORM
  --------------

  So first I think the param function should only index arrays, not multiple
  params. This just heavily simplifies things, reduces neccissary checking,
  and improves consistancy with design, all by leaving that up to the programmer.
  It was initially a feature designed for ease. But in the long run it would make
  things harder, even for the user. So it is being abolished in favor of packed
  arrays.

  On that note, I should push for packed array values for most of my functions.
  And this is for so many good reasons. So just do it.


  I should probably enforce number sizes too so as to also greatly simplify
  number arithmatic.
  There isn't really any need to include different sizes anyhow

*/





    //////////////////////////////////////
   ////                              ////
  ////      ARRAY MACROS            ////
 ////                              ////
//////////////////////////////////////

    /* Place common C inliner macros here */
# 107 "macros.h"
// I wonder if these would be better off with a forced inline function instead
# 132 "macros.h"
    //////////////////////////////////////
   ////                              ////
  ////      FUNDAMENTAL MACROS      ////
 ////                              ////
//////////////////////////////////////

    /* Macros that have no dependancies go here
       As well as fundamental macros that most other
       Macros rely on 
    */


// NOTE:
// The concat operator ## will only expand function parameters once. 
// everything else will be prevented from expanding

// Will fully expand every argument



// Will only fully expand function arguments. 
// Will prevent expansion of anything else
// This level of concat cant be done with more than two at a time



// Disabled in favor of making "packed arrays" more explicit
// for the programmer, and making them more integral to this library
//#define PACK(...) (__VA_ARGS__)

/* !!! WARNING !!!
 * ###############
 *
 * Unpacked values will still get treated as a single argument when passed
 * to a function unless passed as VA_ARGS. Use FUNPACK instead.
 */



// The version of unpack to be used for unpacking a packed value
// to be passed to a macro function.

//#define FUNPACK(__macro, ...) _FUNPACK(__macro, UNPACK(MERGE(__VA_ARGS__)))
// it is still only ever passed as a single argument, but the __VA_ARGS__ is what is 
// important here, and how it basically fixes the whole unpack parameter issue


// merge up to 4 packed values
// TODO: test what happens to parameters when __VA_ARGS__ is empty
//#define MERGE(...) _MERGE(__VA_ARGS__, (), (), (), ())
//#define _MERGE(__0, __1, __2, __3) (UNPACK(__0),UNPACK(__1),UNPACK(__2),UNPACK(__3))

// merge two packed values


// for when I need a NOP macro to be called
// mostly for function conditionals


// for when I want blanks to be explicit



/*
    I should give users the option to use n and p with numbers
    n527
    p572
    so as to not have to use NEG(527)
*/

// runs the same 2-argument macro in a recursive tree call up to 8 arguments
// the __d parameter is default argument, as this function will pretend you always 
// pass in 8 arguments
// TODO: expand number of arguments
// TODO: possible bug if __pack is empty, as it would leave an empty parameter where
//       it gets unpacked, as the comma after stays

//#define __MACRO_TREE_CALL(__m,__0,__1,__2,__3,__4,__5,__6,__7,...) __m(__m(__m(__0,__1), __m(__2,__3)), __m(__m(__4,__5), __m(__6,__7)))
// I think this will always require the same number of m__m calls, so 
// I decided to go with this one as it is simpler to write, and easier 
// to add more. Albiet the behavior may differ slightly more depending 
// on the macro call.



//#define DECAST(__n) PARAM((_DECAST_ __n, __n), 1)
// note an old alternative that also works:
//#define DECAST(__n) PARAM((_DECAST_ __n, __n), 1)
//#define _DECAST_(__n) BLANK, 




// to prevent recursively expanding every call in a loop
// this will only call a macro if the condition is true
// the last value is the params
// TODO: consider making the extra params be a single packed array rather than
//       variable arguments







    //////////////////////////////////////
   ////                              ////
  ////      ARRAY MACROS            ////
 ////                              ////
//////////////////////////////////////


// select a value from a packed array up to 16 elements
// no longer accepts variable args
// TODO: Consider extending to 256 or 2^16
// TODO: explore (limited) binary indexing of array to reduce extensions
// TODO: remove BLANK parameters, as it turns out ... can be passed nothing
//#define PARAM(__a, __n) _CONCAT(_PARAM_, __n)(UNPACK(__a))
# 269 "macros.h"
// Counts up to 16 parameters
// TODO: This can be binary indexed, so look into implementing that later
// TODO: This can also probably be replaced with a loop macro
//       Although it would probably require a secondary loop macro definition
//       so as to avoid the recursion blocks
//#define PLEN(__n) _PLEN_0(UNPACK(__n))
// TODO: fix this
/*real definition*///#define PLEN(__n) FUNPACK(_PLEN_0, __n)

//#define _PLEN_0(__n) IFISBLANK(PARAM(0, __n), 0, _PLEN_0A(__n, 0))
# 297 "macros.h"
// sets element in packed array to value
// TODO: Finish this macro
//       there is a lot involved with this, included accounting for inserting
//       items that are farther than the macro is currently sized
//       either I need to automatically resize macros, or I need to make
//       it explicit and controlled by the user.


// aliases for LPUSH and LPOP



// push onto left end of packed array
// returns a single value of the modified array


// pop from left end of packed array
// returns a packed value of two elements
// the first is the value, the second is the modified packed array



// push onto right end of packed array
// returns the modified packed array


// pop from right end of packed array
// returns a packed value of two elements
// the first is the value, the second is the modified packed array
// TODO: finish this macro. Will probably require PSET()







    //////////////////////////////////////
   ////                              ////
  ////      BOOLEAN MACROS          ////
 ////                              ////
//////////////////////////////////////

/*
    I should consider replacing 'false' and 'true' with 'f' and 't'
*/





// this is a keystone macro to detecting defined behavior
# 359 "macros.h"
// this macro attempts to remove and ignore C casts



// works only with bytes for now
// TODO: get this to work with words
//#define ISZERO(__n) _ISZERO(UNPACK(BYTE(__n)))






// detect is value is blank
//#define ISBLANK(__n) _AND(PARAM((_CONCAT(_ISBLANK, DECAST(__n)), false), 1), ISNCAST(__n))




// rewrote without ISTRUE, as istrue ends up using this
//#define ISPACKED(__n) ISTRUE(_ISPACKED __n)
//#define _ISPACKED(...) true
//#define ISPACKED(__n) PARAM((_ISPACKED __n, false), 1)
//#define _ISPACKED(...) true
# 394 "macros.h"
// Not sure how to detect this yet
// TODO: Finish this macro
// if you call this, it will derail the preprocessor



// TODO: Add this 



// this is for cases (particularly casts) where substitution for truth
// returns two items delimited with a space.
// This is combatable so long as we know what the first items is suppose to be
// which in this case is a true or false
// basically this extracts a bool from a string with extra stuff added onto it
// after a space
// TODO: Remember this trick, it seems very useful for detecting space delimiters





// some truth tables
# 443 "macros.h"
// public variable argument version of truth tables
// handles up to 8 arguments. Will not accept packed values
// TODO: Expand number of usable arguments


// the XOR doesn't support multiple arguments yet
// TODO: add multiple argument support for XOR
//#define XOR(...) _MACRO_TREE_CALL()



// various macro aliases to keep boolean checks concise
# 471 "macros.h"
    //////////////////////////////////////
   ////                              ////
  ////      CONDITIONAL MACROS      ////
 ////                              ////
//////////////////////////////////////

    /* The philosphy of the conditionals is to not create aliases for the
       conditional macros, but to create aliases for the boolean macros.
       We never want to force more than one boolean macro in the conditional
       parameter of a conditional macro.
       There is no need to get rid of the conditional parameter of the IFTHEN(...)
       For instance, we don't need IFZERO(...)
       Actually, IFZERO sounds kind of nice, so I may consider some aliases
    */


// acts as an if/than with an optional third else block
# 498 "macros.h"
     //////////////////////////////////////
    ////                              ////
   ////     NUMBER AND               ////
  ////   ARITHMATIC MACROS          ////
 ////                              ////
//////////////////////////////////////

    /* I should have byte wraparound so as to not have to write
       A whole other massive table of 256 entries for subtraction
       And god help me with multiplication 

       Alright, new consensus,
       Signed numbers are now implicit.
       The number type would determine where it is signed
       Basically, I am going to have a signed multiply
       and an unsigned multiply, etc.
    */

// bytes are expected to be fixed digits for now


// TODO: figure out how to generate negative numbers
//       perhaps with a cast.
//       perhaps the user is expected to use a cast
//       perhaps I can make a macro that helps them cast





// will basically remove or add leading zeroes, like 0x00 to 0x0000
//#define BYTE_TO_WORD(__n) _UNWORD(_BYTE_TO_WORD(UNPACK(BYTE(__n))))



//#define WORD_TO_BYTE(__n) UNBYTE(_WORD_TO_BYTE(UNPACK(WORD(__n))))
# 556 "macros.h"
//#define INV(__n) UNBYTE(_CONCAT(_INV_, _NTYPE(__n))(UNPACK(__n)))
//#define _INV_b(__t,__0,__1) (__t,_INV(__0),_INV(__1))
//#define _INV_w(__t,__0,__1,__2,__3) (__t,_INV(__0),_INV(__1),_INV(__2),_INV(__3))
//#define INV_B(__n) UNBYTE(_INV_B(UNPACK(BYTE(__n))))



// TODO: Add extension conditional check
// TODO: Add word based versions
// #define INV_W(__n) UNBYTE(_INV_B(UNPACK(_WORD(__n))))

//#define _INV_W(__t,__0,__1) (__t,_INV(__0),_INV(__1))


//#define NEG(__n) __CONCAT(_NEG_, _NTYPE(__n))(__n)
//#define _NEG_b(__n) INC(INV(__n))



// add two hex digits together




// TODO: at this point, are the number types even neccissary?
//#define ADD_B(__0, __1) UNBYTE(_ADD_B(UNPACK(BYTE(__0)), UNPACK(BYTE(__1))))
# 593 "macros.h"
// some aliases for the default number size operators
# 608 "macros.h"
    //////////////////////////////////////
   ////                              ////
  ////      LOOP MACROS             ////
 ////                              ////
//////////////////////////////////////
# 623 "macros.h"
    //////////////////////////////////////
   ////                              ////
  ////      LARGE MACRO EXTENTIONS  ////
 ////                              ////
//////////////////////////////////////


// First some macro shorteners to reduce character count,
# 1183 "macros.h"
// TODO to save space, replace _B_456 with _456, and have no leading zeroes
# 10 "macros_test.h" 2
//#include "macros_ext.h"
# 21 "macros_test.h"
 /*#############################################
   ####                                     ####
   ####         TESTS BEGIN HERE            ####
   ####                                     ####
   ############################################*/



 /* Test setup */
////////////////
# 41 "macros_test.h"
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





// #define ALL_SAFE_TESTS B, C, D, F, G, H, I, T
// #define ALL_TESTS A, B, C, D, E, F, G, H, I, T
# 67 "macros_test.h"
 /* Order of operation tests */
//////////////////////////////

// B##C
gold##12

// #B###C (errors)
//#B###C


// #define TEST_1(__a, __b) B##C##__a##__b #__b
// TEST_1(A, B)
BCAB "B"


// B##C##__a##__b #__b #B
gold##12##__a##__b #__b #gold




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
gold12voidvoidMANFvoidMANFT

// _CONCAT(B, C)
BC
// _CONCAT(D, F)
DF
// _CONCAT(G, H)
GH
// _CONCAT(I, T)
IT
// _CONCAT(I, T(I))
IT(MANF)






// #define TEST_2A(__n) I##__n
// #define TEST_2B(__n) _CONCAT(I, __n)
// #define TEST_2C(__n) CONCAT(I, __n)

// TEST_2A(I)
II
// TEST_2B(I)
IMANF
// TEST_2C(I)
MANFMANF



// UNPACK((A, B, C, D))
"sand", gold, 12, void
// UNPACK((((A), B), C, D))
(("sand"), gold), 12, void
// UNPACK(((A, B, C, D)))
("sand", gold, 12, void)




// FUNPACK(TEST_FUNPACK, MERGE((0, 1), (2, 3)))
0 +1 +2 +3



// #define TEST_X(__a, __b, __c, __d) _TEST_X1((__a, __b, __c, __d))
// #define _TEST_X1(__n) _TEST_X2(UNPACK(__n), xx, xx, xx, xx)// _TEST_X3(UNPACK(__n), xx, xx, xx, xx) // _TEST_X5(UNPACK(__n))


// #define _TEST_X2(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
// #define _TEST_X3(...) _TEST_X4(__VA_ARGS__)
// #define _TEST_X4(__a, __b, __c, __d, ...) [__a][__b][__c][__d](__VA_ARGS__)
// #define _TEST_X5(...) __VA_ARGS__







//errors #define _TEST_X5(__a, __b, __c, __d)



// TEST_X(a, b, c, d)
[a, b, c, d][xx][xx][xx](xx) \n [a][b][c][d](xx, xx, xx, xx) \n a, b, c, d




// #define TEST_3A(...) UNPACK((__VA_ARGS__))
// #define TEST_3B(__n) UNPACK(__n)
// #define TEST_3C(__a, __b, __c, __d) _TEST_3C(UNPACK((__a, __b, __c, __d)))
// #define _TEST_3C(__a, __b, __c, __d) __a/__b/__c/__d
# 196 "macros_test.h"
// TEST_3A(A, B, C, D)
"sand", gold, 12, void
// TEST_3B((A, B, C, D))
"sand", gold, 12, void
/* errors
// TEST_3C(A, B, C, D)
TEST_3C(A, B, C, D)*/
// TEST_3D(A, B, C, D)
"sand"/gold/12/void



//DECAST((void)k)
k
//DECAST((yehee (yolo) holo)(beans)x)
(beans)x
//DECAST((boom))






    //////////////////////////////////////
   ////                              ////
  ////      ARRAY MACROS            ////
 ////                              ////
//////////////////////////////////////



// PARAM((A, B, C, D), 0)
"sand"
// PARAM((A, B, C, D), 2)
12
// PARAM((A, B, C, D), 7)

// PARAM((ALL_TESTS), 0)
"sand"
// PARAM((ALL_TESTS), 2)
12
// PARAM((ALL_TESTS), 7)
void

// #define PARAM_TEST_1(n, ...) PARAM((__VA_ARGS__), n)
// #define PARAM_TEST_2(...) PARAM(__VA_ARGS__)



// PARAM_TEST_1(2, A, B, C, D)
12
// PARAM_TEST_2((A, B, C, D), 2)
12



//#define PARAM_TEST(a, ...) a __VA_ARGS__


// trurns out variable args ... can accept zero parameters
//PARAM_TEST("x")
"x"



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
x a
// UNPACK(a)
_UNPACK a

// DECAST((x)a)
a
// DECAST(a)
a




// IFTHEN(ISVOID(0),      // return; ,          // return (0);    // )



// IFTHEN(ISVOID(0),      // return; ,          // return (0);    // )






// ISVOID(0)
//ISVOID(0)



    //////////////////////////////////////
   ////                              ////
  ////      BOOLEAN MACROS          ////
 ////                              ////
//////////////////////////////////////




// ISPACKED(x)
(false, _NOT___AND_trueISNCAST(x))
// ISPACKED((x))
(true, _NOT___AND_trueISNCAST())
// ISPACKED((x)a)
(true a, _NOT___AND_trueISNCAST(a))



// NOT(true)
false
// NOT(false)
true
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
"istrue"
// IFTHEN(false, "istrue", "isfalse")
"isfalse"






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
