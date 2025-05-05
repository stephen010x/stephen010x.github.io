#ifndef MACROS_H
#define MACROS_H



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

#if 0
#define LENOF(__n) (sizeof(__n)/sizeof(*(__n)))


#ifdef DEBUG_MODE
    #define ASSERT(__cond, __code, ...)     \
        do {                                \
            if (!(__cond)) {                \
                printf(__VA_ARGS__);        \
                IFTHEN(ISVOID(__code),      \
                    return; ,               \
                    return (__code);        \
                )                           \
            }                               \
        } while(0)
#else
    #define ASSERT(__cond, __code, ...)     \
        do {                                \
            if (!(__cond))                  \
                IFTHEN(ISVOID(__code),      \
                    return; ,               \
                    return (__code);        \
                )                           \
        } while(0)
#endif


// I wonder if these would be better off with a forced inline function instead
#define MIN(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a <= b ? a : b;         \
    })

#define MAX(__a, __b) ({        \
        typeof(__a) a = (__a);  \
        typeof(__b) b = (__b);  \
        a > b ? a : b;          \
    })

#define ABS(__n) ({typeof(__n) n = (__n); n >= 0 ? n : -n;})
#define SIGN(__n) ({typeof(__n) n = (__n); n == 0 ? 0 : (n > 0 ? 1 : -1);})

#define MATH_PI 3.141592653589793238462643383
#endif







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
#define CONCAT(...) __CONCAT(__VA_ARGS__,,,,,,,,,,)
#define __CONCAT(__0,__1,__2,__3,__4,__5,__6,__7,__8,__9,...) __0##__1##__2##__3##__4##__5##__6##__7##__8##__9

// Will only fully expand function arguments. 
// Will prevent expansion of anything else
// This level of concat cant be done with more than two at a time
#define _CONCAT(__0, __1) __0 ## __1


// Disabled in favor of making "packed arrays" more explicit
// for the programmer, and making them more integral to this library
//#define PACK(...) (__VA_ARGS__)

/* !!! WARNING !!!
 * ###############
 *
 * Unpacked values will still get treated as a single argument when passed
 * to a function unless passed as VA_ARGS. Use FUNPACK instead.
 */
#define UNPACK(__n) _UNPACK __n
#define _UNPACK(...) __VA_ARGS__

// The version of unpack to be used for unpacking a packed value
// to be passed to a macro function.
#define FUNPACK(__macro, __n) _FUNPACK(__macro, UNPACK(__n))
//#define FUNPACK(__macro, ...) _FUNPACK(__macro, UNPACK(MERGE(__VA_ARGS__)))
// it is still only ever passed as a single argument, but the __VA_ARGS__ is what is 
// important here, and how it basically fixes the whole unpack parameter issue
#define _FUNPACK(__macro, ...) __macro(__VA_ARGS__)

// merge up to 4 packed values
// TODO: test what happens to parameters when __VA_ARGS__ is empty
//#define MERGE(...) _MERGE(__VA_ARGS__, (), (), (), ())
//#define _MERGE(__0, __1, __2, __3) (UNPACK(__0),UNPACK(__1),UNPACK(__2),UNPACK(__3))

// merge two packed values
#define MERGE(__0, __1) (UNPACK(__0), UNPACK(__1))

// for when I need a NOP macro to be called
// mostly for function conditionals
#define NOP(...)

// for when I want blanks to be explicit
#define BLANK


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
#define _MACRO_TREE_CALL(__macro, __pack, __d) __MACRO_TREE_CALL(__macro, UNPACK(__pack), __d,__d,__d,__d,__d,__d,__d,__d)
//#define __MACRO_TREE_CALL(__m,__0,__1,__2,__3,__4,__5,__6,__7,...) __m(__m(__m(__0,__1), __m(__2,__3)), __m(__m(__4,__5), __m(__6,__7)))
// I think this will always require the same number of m__m calls, so 
// I decided to go with this one as it is simpler to write, and easier 
// to add more. Albiet the behavior may differ slightly more depending 
// on the macro call.
#define __MACRO_TREE_CALL(__m,__0,__1,__2,__3,__4,__5,__6,__7, ...) __m(__m(__m(__m(__m(__m(__m(__0,__1),__2),__3),__4),__5),__6),__7)


//#define DECAST(__n) PARAM((_DECAST_ __n, __n), 1)
// note an old alternative that also works:
//#define DECAST(__n) PARAM((_DECAST_ __n, __n), 1)
//#define _DECAST_(__n) BLANK, 
#define DECAST(__n) PARAM((_DECAST __n, __n), 1)
#define _DECAST(...) BLANK, BLANK


// to prevent recursively expanding every call in a loop
// this will only call a macro if the condition is true
// the last value is the params
// TODO: consider making the extra params be a single packed array rather than
//       variable arguments
#define CCALL(__cond, __macro, ...) IFTHEN(__cond, __macro, NOP)(__VA_ARGS__)






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
#define PARAM(__a, __n) FUNPACK(_CONCAT(_PARAM_, __n), __a)
#define _PARAM_0(__n, ...) __n
#define _PARAM_1(__n, ...) _PARAM_0(__VA_ARGS__, BLANK)
#define _PARAM_2(__n, ...) _PARAM_1(__VA_ARGS__, BLANK)
#define _PARAM_3(__n, ...) _PARAM_2(__VA_ARGS__, BLANK)
#define _PARAM_4(__n, ...) _PARAM_3(__VA_ARGS__, BLANK)
#define _PARAM_5(__n, ...) _PARAM_4(__VA_ARGS__, BLANK)
#define _PARAM_6(__n, ...) _PARAM_5(__VA_ARGS__, BLANK)
#define _PARAM_7(__n, ...) _PARAM_6(__VA_ARGS__, BLANK)
#define _PARAM_8(__n, ...) _PARAM_7(__VA_ARGS__, BLANK)
#define _PARAM_9(__n, ...) _PARAM_8(__VA_ARGS__, BLANK)
#define _PARAM_10(__n, ...) _PARAM_9(__VA_ARGS__, BLANK)
#define _PARAM_11(__n, ...) _PARAM_10(__VA_ARGS__, BLANK)
#define _PARAM_12(__n, ...) _PARAM_11(__VA_ARGS__, BLANK)
#define _PARAM_13(__n, ...) _PARAM_12(__VA_ARGS__, BLANK)
#define _PARAM_14(__n, ...) _PARAM_13(__VA_ARGS__, BLANK)
#define _PARAM_15(__n, ...) _PARAM_14(__VA_ARGS__, BLANK)


// Counts up to 16 parameters
// TODO: This can be binary indexed, so look into implementing that later
// TODO: This can also probably be replaced with a loop macro
//       Although it would probably require a secondary loop macro definition
//       so as to avoid the recursion blocks
//#define PLEN(__n) _PLEN_0(UNPACK(__n))
// TODO: fix this
/*real definition*///#define PLEN(__n) FUNPACK(_PLEN_0, __n)
#define PLEN(__n) 0
//#define _PLEN_0(__n) IFISBLANK(PARAM(0, __n), 0, _PLEN_0A(__n, 0))
#define _PLEN_0(__0, ...) IFTHEN(ISBLANK(__0), 0, _PLEN_1(__VA_ARGS__))
#define _PLEN_1(__0, ...) IFTHEN(ISBLANK(__0), 1, _PLEN_2(__VA_ARGS__))
#define _PLEN_2(__0, ...) IFTHEN(ISBLANK(__0), 2, _PLEN_3(__VA_ARGS__))
#define _PLEN_3(__0, ...) IFTHEN(ISBLANK(__0), 3, _PLEN_4(__VA_ARGS__))
#define _PLEN_4(__0, ...) IFTHEN(ISBLANK(__0), 4, _PLEN_5(__VA_ARGS__))
#define _PLEN_5(__0, ...) IFTHEN(ISBLANK(__0), 5, _PLEN_6(__VA_ARGS__))
#define _PLEN_6(__0, ...) IFTHEN(ISBLANK(__0), 6, _PLEN_7(__VA_ARGS__))
#define _PLEN_7(__0, ...) IFTHEN(ISBLANK(__0), 7, _PLEN_8(__VA_ARGS__))
#define _PLEN_8(__0, ...) IFTHEN(ISBLANK(__0), 8, _PLEN_9(__VA_ARGS__))
#define _PLEN_9(__0, ...) IFTHEN(ISBLANK(__0), 9, _PLEN_10(__VA_ARGS__))
#define _PLEN_10(__0, ...) IFTHEN(ISBLANK(__0), 10, _PLEN_11(__VA_ARGS__))
#define _PLEN_11(__0, ...) IFTHEN(ISBLANK(__0), 11, _PLEN_12(__VA_ARGS__))
#define _PLEN_12(__0, ...) IFTHEN(ISBLANK(__0), 12, _PLEN_13(__VA_ARGS__))
#define _PLEN_13(__0, ...) IFTHEN(ISBLANK(__0), 13, _PLEN_14(__VA_ARGS__))
#define _PLEN_14(__0, ...) IFTHEN(ISBLANK(__0), 14, _PLEN_15(__VA_ARGS__))
#define _PLEN_15(__0, ...) IFTHEN(ISBLANK(__0), 15, 16)


// sets element in packed array to value
// TODO: Finish this macro
//       there is a lot involved with this, included accounting for inserting
//       items that are farther than the macro is currently sized
//       either I need to automatically resize macros, or I need to make
//       it explicit and controlled by the user.
#define PSET(__a, __i, __n)

// aliases for LPUSH and LPOP
#define PUSH(...) LPUSH(...)
#define POP(...) LPOP(...)

// push onto left end of packed array
// returns a single value of the modified array
#define LPUSH(__a, __n) (__n, UNPACK(__a))

// pop from left end of packed array
// returns a packed value of two elements
// the first is the value, the second is the modified packed array
#define LPOP(__a) (_LPOP(UNPACK(__a)))
#define _LPOP(__0, ...) __0, (__VA_ARGS__)

// push onto right end of packed array
// returns the modified packed array
#define RPUSH(__a, __n) (UNPACK(__a), __n)

// pop from right end of packed array
// returns a packed value of two elements
// the first is the value, the second is the modified packed array
// TODO: finish this macro. Will probably require PSET()
#define RPOP(__a) _RPOP(__a, PLEN(__a))
#define _RPOP(__a, __len) 





    //////////////////////////////////////
   ////                              ////
  ////      BOOLEAN MACROS          ////
 ////                              ////
//////////////////////////////////////

/*
    I should consider replacing 'false' and 'true' with 'f' and 't'
*/

#define NOT(__b) _CONCAT(_NOT_, __b)
#define _NOT_true false
#define _NOT_false true

// this is a keystone macro to detecting defined behavior
#define ISTRUE(__b) IFTHEN(ISPACKED(__b), _ISTRUE_packed(__b), _ISTRUE_unpacked(__b))
#define _ISTRUE_packed(__b) false
#define _ISTRUE_unpacked(__b) PARAM((_CONCAT(_ISTRUE_, __b), false), 1)
#define _ISTRUE_true BLANK, true

#define ISFALSE(__b) PARAM((_CONCAT(_ISFALSE_, __b), false), 1)
#define _ISFALSE_false BLANK, true

#define ISBOOL(__b) OR(ISTRUE(__b), ISFALSE(__b))

// this macro attempts to remove and ignore C casts
#define ISVOID(__n) ISTRUE(_CONCAT(_ISVOID_, DECAST(__n)))
#define _ISVOID_void true

// works only with bytes for now
// TODO: get this to work with words
//#define ISZERO(__n) _ISZERO(UNPACK(BYTE(__n)))
#define ISZERO(__n) FUNPACK(_ISZERO, BYTE(__n))
#define _ISZERO(__t, __0, __1) AND(__ISZERO(__0), __ISZERO(__1))
#define __ISZERO(__d) ISTRUE(_CONCAT(__ISZERO_, __d))
#define __ISZERO_0 true


// detect is value is blank
//#define ISBLANK(__n) _AND(PARAM((_CONCAT(_ISBLANK, DECAST(__n)), false), 1), ISNCAST(__n))
#define ISBLANK(__n) _AND(PARAM((_ISBLANK, DECAST(__n), false), 1), ISNCAST(__n))
#define _ISBLANK BLANK, true


// rewrote without ISTRUE, as istrue ends up using this
//#define ISPACKED(__n) ISTRUE(_ISPACKED __n)
//#define _ISPACKED(...) true
//#define ISPACKED(__n) PARAM((_ISPACKED __n, false), 1)
//#define _ISPACKED(...) true
#define ISPACKED(__n) (PARAM((_ISPACKED __n, false), 1), ISNBLANK(DECAST(__n)))
#define _ISPACKED(...) BLANK, true

#define ISBYTE(__n) ISTRUE(_CONCAT(_ISBYTE_, PARAM(__n, 0)))
#define _ISBYTE_b true

#define ISWORD(__n) ISTRUE(_CONCAT(_ISWORD_, PARAM(__n, 0)))
#define _ISWORD_w true

#define ISNUM(__n) OR(ISBYTE(__n), ISWORD(__n))

// Not sure how to detect this yet
// TODO: Finish this macro
// if you call this, it will derail the preprocessor
#define ISRAWNUM(__n) ((((((


// TODO: Add this 
#define ISCAST(__n) 


// this is for cases (particularly casts) where substitution for truth
// returns two items delimited with a space.
// This is combatable so long as we know what the first items is suppose to be
// which in this case is a true or false
// basically this extracts a bool from a string with extra stuff added onto it
// after a space
// TODO: Remember this trick, it seems very useful for detecting space delimiters
#define _EXTRACT_BOOL(__n) PARAM((CONCAT(__EXTRACT_BOOL_, __n, test)), 1)
#define __EXTRACT_BOOL_truetest BLANK, true
#define __EXTRACT_BOOL_falsetest BLANK, true


// some truth tables

#define _AND(__0, __1) CONCAT(__AND_, __0, __1)
#define __AND_falsefalse false
#define __AND_truefalse  false
#define __AND_falsetrue  false
#define __AND_truetrue   true

#define _OR(__0, __1) CONCAT(__OR_, __0, __1)
#define __OR_falsefalse false
#define __OR_truefalse  true
#define __OR_falsetrue  true
#define __OR_truetrue   true

#define _XOR(__0, __1) CONCAT(__XOR_, __0, __1)
#define __XOR_falsefalse false
#define __XOR_truefalse  true
#define __XOR_falsetrue  true
#define __XOR_truetrue   false

#define IMPLY(__0, __1) CONCAT(_IMPLY_, __0, __1)
#define _IMPLY_falsefalse true
#define _IMPLY_truefalse  false
#define _IMPLY_falsetrue  true
#define _IMPLY_truetrue   true


// public variable argument version of truth tables
// handles up to 8 arguments. Will not accept packed values
// TODO: Expand number of usable arguments
#define AND(...) _MACRO_TREE_CALL(_AND, (__VA_ARGS__), true)
#define OR(...) _MACRO_TREE_CALL(_OR, (__VA_ARGS__), false)
// the XOR doesn't support multiple arguments yet
// TODO: add multiple argument support for XOR
//#define XOR(...) _MACRO_TREE_CALL()
#define XOR(...) _XOR(__VA_ARGS__)


// various macro aliases to keep boolean checks concise
#define ISNVOID(...) NOT(ISVOID(__VA_ARGS__))
#define ISNZERO(...) NOT(ISZERO(__VA_ARGS__))
#define ISNPACKED(...) NOT(ISPACKED(__VA_ARGS__))
#define ISNBLANK(...) NOT(ISBLANK(__VA_ARGS__))

#define NAND(...) NOT(AND(__VA_ARGS__))
#define NOR(...) NOT(OR(__VA_ARGS__))
#define XNOR(...) NOT(XOR(__VA_ARGS__))
#define NIMPLY(...) NOT(IMPLY(__VA_ARGS__))







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
#define IFTHEN(__cond, __block, ...) _CONCAT(_IFTHEN_, __cond)(__block, __VA_ARGS__)
#define _IFTHEN_true(__then, __else) __then
#define _IFTHEN_false(__then, __else) __else







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
#define BYTE(__n) (b,CONCAT(_B_, __n))

// TODO: figure out how to generate negative numbers
//       perhaps with a cast.
//       perhaps the user is expected to use a cast
//       perhaps I can make a macro that helps them cast
#define UNBYTE(__n) CONCAT(0x, UNPACK(__n))
#define UNWORD(__n) CONCAT(0x, UNPACK(__n))

#define _NTYPE(__n) PARAM(__n, 0)

// will basically remove or add leading zeroes, like 0x00 to 0x0000
//#define BYTE_TO_WORD(__n) _UNWORD(_BYTE_TO_WORD(UNPACK(BYTE(__n))))
#define BYTE_TO_WORD(__n) _UNWORD(FUNPACK(, _BYTE_TO_WORDBYTE(__n)))
#define _BYTE_TO_WORD(__t, __0,__1) (w,0,0,__0,__1)

//#define WORD_TO_BYTE(__n) UNBYTE(_WORD_TO_BYTE(UNPACK(WORD(__n))))
#define WORD_TO_BYTE(__n) UNBYTE(FUNPACK(_WORD_TO_BYTE, WORD(__n)))
#define _WORD_TO_BYTE(__t, __0,__1,__2,__3) (b,__1,__2)


#define _INV(__0) _CONCAT(__INV_, __0)
#define __INV_0 f
#define __INV_1 e
#define __INV_2 d
#define __INV_3 c
#define __INV_4 b
#define __INV_5 a
#define __INV_6 9
#define __INV_7 8
#define __INV_8 7
#define __INV_9 6
#define __INV_a 5
#define __INV_b 4
#define __INV_c 3
#define __INV_d 2
#define __INV_e 1
#define __INV_f 0

//#define INV(__n) UNBYTE(_CONCAT(_INV_, _NTYPE(__n))(UNPACK(__n)))
//#define _INV_b(__t,__0,__1) (__t,_INV(__0),_INV(__1))
//#define _INV_w(__t,__0,__1,__2,__3) (__t,_INV(__0),_INV(__1),_INV(__2),_INV(__3))
//#define INV_B(__n) UNBYTE(_INV_B(UNPACK(BYTE(__n))))
#define INV_B(__n) UNBYTE(FUNPACK(_INV_B, BYTE(__n)))
#define _INV_B(__t,__0,__1) (__t,_INV(__0),_INV(__1))

// TODO: Add extension conditional check
// TODO: Add word based versions
// #define INV_W(__n) UNBYTE(_INV_B(UNPACK(_WORD(__n))))
#define INV_W(__n) UNBYTE(FUNPACK(_INV_B, _WORD(__n)))
//#define _INV_W(__t,__0,__1) (__t,_INV(__0),_INV(__1))


//#define NEG(__n) __CONCAT(_NEG_, _NTYPE(__n))(__n)
//#define _NEG_b(__n) INC(INV(__n))
#define NEG_B(__n) INC_B(INV_B(__n))


// add two hex digits together
#define _ADD(__0, __1, __c) CONCAT(_A_, __0, __1, __c)

#define _DISCARD_CARRY(__c, ...) __VA_ARGS__

// TODO: at this point, are the number types even neccissary?
//#define ADD_B(__0, __1) UNBYTE(_ADD_B(UNPACK(BYTE(__0)), UNPACK(BYTE(__1))))
#define ADD_B(__0, __1) UNBYTE(FUNPACK(_ADD_B, MERGE(BYTE(__0), BYTE(__1))))
#define _ADD_B0(__t1,__0,__1,__t2,__2,__3) (b,_ADD_B1(__0,__2, _ADD(__1,__3,0)))
#define _ADD_B1(__0,__2,__c,__d) _DISCARD_CARRY(_ADD(__0, __2)),__d


#define SUB_B(__0, __1) ADD_B(__0, NEG_B(__1))

#define INC_B(__0) ADD_B(__0, 1)
#define DEC_B(__0) SUB_B(__0, 1)


// some aliases for the default number size operators
#define INV(...) INV_B(__VA_ARGS__)
#define NEG(...) NEG_B(__VA_ARGS__)
#define ADD(...) ADD_B(__VA_ARGS__)
#define SUB(...) SUB_B(__VA_ARGS__)
#define INC(...) INC_B(__VA_ARGS__)
#define DEC(...) DEC_B(__VA_ARGS__)
#define MUL(...) MUL_B(__VA_ARGS__)
#define DIV(...) DIV_B(__VA_ARGS__)
#define MOD(...) MOD_B(__VA_ARGS__)
#define SHR(...) SHR_B(__VA_ARGS__)
#define SHL(...) SHL_B(__VA_ARGS__)



    //////////////////////////////////////
   ////                              ////
  ////      LOOP MACROS             ////
 ////                              ////
//////////////////////////////////////










    //////////////////////////////////////
   ////                              ////
  ////      LARGE MACRO EXTENTIONS  ////
 ////                              ////
//////////////////////////////////////


// First some macro shorteners to reduce character count,
#define _IT(...) IFTHEN(__VA_ARGS__)




#define _A_000 0,0
#define _A_010 0,1
#define _A_020 0,2
#define _A_030 0,3
#define _A_040 0,4
#define _A_050 0,5
#define _A_060 0,6
#define _A_070 0,7
#define _A_080 0,8
#define _A_090 0,9
#define _A_0a0 0,a
#define _A_0b0 0,b
#define _A_0c0 0,c
#define _A_0d0 0,d
#define _A_0e0 0,e
#define _A_0f0 0,f

#define _A_100 0,1
#define _A_110 0,2
#define _A_120 0,3
#define _A_130 0,4
#define _A_140 0,5
#define _A_150 0,6
#define _A_160 0,7
#define _A_170 0,8
#define _A_180 0,9
#define _A_190 0,a
#define _A_1a0 0,b
#define _A_1b0 0,c
#define _A_1c0 0,d
#define _A_1d0 0,e
#define _A_1e0 0,f
#define _A_1f0 1,0

#define _A_200 0,2
#define _A_210 0,3
#define _A_220 0,4
#define _A_230 0,5
#define _A_240 0,6
#define _A_250 0,7
#define _A_260 0,8
#define _A_270 0,9
#define _A_280 0,a
#define _A_290 0,b
#define _A_2a0 0,c
#define _A_2b0 0,d
#define _A_2c0 0,e
#define _A_2d0 0,f
#define _A_2e0 1,0
#define _A_2f0 1,1

#define _A_300 0,3
#define _A_310 0,4
#define _A_320 0,5
#define _A_330 0,6
#define _A_340 0,7
#define _A_350 0,8
#define _A_360 0,9
#define _A_370 0,a
#define _A_380 0,b
#define _A_390 0,c
#define _A_3a0 0,d
#define _A_3b0 0,e
#define _A_3c0 0,f
#define _A_3d0 1,0
#define _A_3e0 1,1
#define _A_3f0 1,2

#define _A_400 0,4
#define _A_410 0,5
#define _A_420 0,6
#define _A_430 0,7
#define _A_440 0,8
#define _A_450 0,9
#define _A_460 0,a
#define _A_470 0,b
#define _A_480 0,c
#define _A_490 0,d
#define _A_4a0 0,e
#define _A_4b0 0,f
#define _A_4c0 1,0
#define _A_4d0 1,1
#define _A_4e0 1,2
#define _A_4f0 1,3

#define _A_500 0,5
#define _A_510 0,6
#define _A_520 0,7
#define _A_530 0,8
#define _A_540 0,9
#define _A_550 0,a
#define _A_560 0,b
#define _A_570 0,c
#define _A_580 0,d
#define _A_590 0,e
#define _A_5a0 0,f
#define _A_5b0 1,0
#define _A_5c0 1,1
#define _A_5d0 1,2
#define _A_5e0 1,3
#define _A_5f0 1,4

#define _A_600 0,6
#define _A_610 0,7
#define _A_620 0,8
#define _A_630 0,9
#define _A_640 0,a
#define _A_650 0,b
#define _A_660 0,c
#define _A_670 0,d
#define _A_680 0,e
#define _A_690 0,f
#define _A_6a0 1,0
#define _A_6b0 1,1
#define _A_6c0 1,2
#define _A_6d0 1,3
#define _A_6e0 1,4
#define _A_6f0 1,5

#define _A_700 0,7
#define _A_710 0,8
#define _A_720 0,9
#define _A_730 0,a
#define _A_740 0,b
#define _A_750 0,c
#define _A_760 0,d
#define _A_770 0,e
#define _A_780 0,f
#define _A_790 1,0
#define _A_7a0 1,1
#define _A_7b0 1,2
#define _A_7c0 1,3
#define _A_7d0 1,4
#define _A_7e0 1,5
#define _A_7f0 1,6

#define _A_800 0,8
#define _A_810 0,9
#define _A_820 0,a
#define _A_830 0,b
#define _A_840 0,c
#define _A_850 0,d
#define _A_860 0,e
#define _A_870 0,f
#define _A_880 1,0
#define _A_890 1,1
#define _A_8a0 1,2
#define _A_8b0 1,3
#define _A_8c0 1,4
#define _A_8d0 1,5
#define _A_8e0 1,6
#define _A_8f0 1,7

#define _A_900 0,9
#define _A_910 0,a
#define _A_920 0,b
#define _A_930 0,c
#define _A_940 0,d
#define _A_950 0,e
#define _A_960 0,f
#define _A_970 1,0
#define _A_980 1,1
#define _A_990 1,2
#define _A_9a0 1,3
#define _A_9b0 1,4
#define _A_9c0 1,5
#define _A_9d0 1,6
#define _A_9e0 1,7
#define _A_9f0 1,8

#define _A_a00 0,a
#define _A_a10 0,b
#define _A_a20 0,c
#define _A_a30 0,d
#define _A_a40 0,e
#define _A_a50 0,f
#define _A_a60 1,0
#define _A_a70 1,1
#define _A_a80 1,2
#define _A_a90 1,3
#define _A_aa0 1,4
#define _A_ab0 1,5
#define _A_ac0 1,6
#define _A_ad0 1,7
#define _A_ae0 1,8
#define _A_af0 1,9

#define _A_b00 0,b
#define _A_b10 0,c
#define _A_b20 0,d
#define _A_b30 0,e
#define _A_b40 0,f
#define _A_b50 1,0
#define _A_b60 1,1
#define _A_b70 1,2
#define _A_b80 1,3
#define _A_b90 1,4
#define _A_ba0 1,5
#define _A_bb0 1,6
#define _A_bc0 1,7
#define _A_bd0 1,8
#define _A_be0 1,9
#define _A_bf0 1,a

#define _A_c00 0,c
#define _A_c10 0,d
#define _A_c20 0,e
#define _A_c30 0,f
#define _A_c40 1,0
#define _A_c50 1,1
#define _A_c60 1,2
#define _A_c70 1,3
#define _A_c80 1,4
#define _A_c90 1,5
#define _A_ca0 1,6
#define _A_cb0 1,7
#define _A_cc0 1,8
#define _A_cd0 1,9
#define _A_ce0 1,a
#define _A_cf0 1,b

#define _A_d00 0,d
#define _A_d10 0,e
#define _A_d20 0,f
#define _A_d30 1,0
#define _A_d40 1,1
#define _A_d50 1,2
#define _A_d60 1,3
#define _A_d70 1,4
#define _A_d80 1,5
#define _A_d90 1,6
#define _A_da0 1,7
#define _A_db0 1,8
#define _A_dc0 1,9
#define _A_dd0 1,a
#define _A_de0 1,b
#define _A_df0 1,c

#define _A_e00 0,e
#define _A_e10 0,f
#define _A_e20 1,0
#define _A_e30 1,1
#define _A_e40 1,2
#define _A_e50 1,3
#define _A_e60 1,4
#define _A_e70 1,5
#define _A_e80 1,6
#define _A_e90 1,7
#define _A_ea0 1,8
#define _A_eb0 1,9
#define _A_ec0 1,a
#define _A_ed0 1,b
#define _A_ee0 1,c
#define _A_ef0 1,d

#define _A_f00 0,f
#define _A_f10 1,0
#define _A_f20 1,1
#define _A_f30 1,2
#define _A_f40 1,3
#define _A_f50 1,4
#define _A_f60 1,5
#define _A_f70 1,6
#define _A_f80 1,7
#define _A_f90 1,8
#define _A_fa0 1,9
#define _A_fb0 1,a
#define _A_fc0 1,b
#define _A_fd0 1,c
#define _A_fe0 1,d
#define _A_ff0 1,e

#define _A_001 0,1
#define _A_011 0,2
#define _A_021 0,3
#define _A_031 0,4
#define _A_041 0,5
#define _A_051 0,6
#define _A_061 0,7
#define _A_071 0,8
#define _A_081 0,9
#define _A_091 0,a
#define _A_0a1 0,b
#define _A_0b1 0,c
#define _A_0c1 0,d
#define _A_0d1 0,e
#define _A_0e1 0,f
#define _A_0f1 1,0

#define _A_101 0,2
#define _A_111 0,3
#define _A_121 0,4
#define _A_131 0,5
#define _A_141 0,6
#define _A_151 0,7
#define _A_161 0,8
#define _A_171 0,9
#define _A_181 0,a
#define _A_191 0,b
#define _A_1a1 0,c
#define _A_1b1 0,d
#define _A_1c1 0,e
#define _A_1d1 0,f
#define _A_1e1 1,0
#define _A_1f1 1,1

#define _A_201 0,3
#define _A_211 0,4
#define _A_221 0,5
#define _A_231 0,6
#define _A_241 0,7
#define _A_251 0,8
#define _A_261 0,9
#define _A_271 0,a
#define _A_281 0,b
#define _A_291 0,c
#define _A_2a1 0,d
#define _A_2b1 0,e
#define _A_2c1 0,f
#define _A_2d1 1,0
#define _A_2e1 1,1
#define _A_2f1 1,2

#define _A_301 0,4
#define _A_311 0,5
#define _A_321 0,6
#define _A_331 0,7
#define _A_341 0,8
#define _A_351 0,9
#define _A_361 0,a
#define _A_371 0,b
#define _A_381 0,c
#define _A_391 0,d
#define _A_3a1 0,e
#define _A_3b1 0,f
#define _A_3c1 1,0
#define _A_3d1 1,1
#define _A_3e1 1,2
#define _A_3f1 1,3

#define _A_401 0,5
#define _A_411 0,6
#define _A_421 0,7
#define _A_431 0,8
#define _A_441 0,9
#define _A_451 0,a
#define _A_461 0,b
#define _A_471 0,c
#define _A_481 0,d
#define _A_491 0,e
#define _A_4a1 0,f
#define _A_4b1 1,0
#define _A_4c1 1,1
#define _A_4d1 1,2
#define _A_4e1 1,3
#define _A_4f1 1,4

#define _A_501 0,6
#define _A_511 0,7
#define _A_521 0,8
#define _A_531 0,9
#define _A_541 0,a
#define _A_551 0,b
#define _A_561 0,c
#define _A_571 0,d
#define _A_581 0,e
#define _A_591 0,f
#define _A_5a1 1,0
#define _A_5b1 1,1
#define _A_5c1 1,2
#define _A_5d1 1,3
#define _A_5e1 1,4
#define _A_5f1 1,5

#define _A_601 0,7
#define _A_611 0,8
#define _A_621 0,9
#define _A_631 0,a
#define _A_641 0,b
#define _A_651 0,c
#define _A_661 0,d
#define _A_671 0,e
#define _A_681 0,f
#define _A_691 1,0
#define _A_6a1 1,1
#define _A_6b1 1,2
#define _A_6c1 1,3
#define _A_6d1 1,4
#define _A_6e1 1,5
#define _A_6f1 1,6

#define _A_701 0,8
#define _A_711 0,9
#define _A_721 0,a
#define _A_731 0,b
#define _A_741 0,c
#define _A_751 0,d
#define _A_761 0,e
#define _A_771 0,f
#define _A_781 1,0
#define _A_791 1,1
#define _A_7a1 1,2
#define _A_7b1 1,3
#define _A_7c1 1,4
#define _A_7d1 1,5
#define _A_7e1 1,6
#define _A_7f1 1,7

#define _A_801 0,9
#define _A_811 0,a
#define _A_821 0,b
#define _A_831 0,c
#define _A_841 0,d
#define _A_851 0,e
#define _A_861 0,f
#define _A_871 1,0
#define _A_881 1,1
#define _A_891 1,2
#define _A_8a1 1,3
#define _A_8b1 1,4
#define _A_8c1 1,5
#define _A_8d1 1,6
#define _A_8e1 1,7
#define _A_8f1 1,8

#define _A_901 0,a
#define _A_911 0,b
#define _A_921 0,c
#define _A_931 0,d
#define _A_941 0,e
#define _A_951 0,f
#define _A_961 1,0
#define _A_971 1,1
#define _A_981 1,2
#define _A_991 1,3
#define _A_9a1 1,4
#define _A_9b1 1,5
#define _A_9c1 1,6
#define _A_9d1 1,7
#define _A_9e1 1,8
#define _A_9f1 1,9

#define _A_a01 0,b
#define _A_a11 0,c
#define _A_a21 0,d
#define _A_a31 0,e
#define _A_a41 0,f
#define _A_a51 1,0
#define _A_a61 1,1
#define _A_a71 1,2
#define _A_a81 1,3
#define _A_a91 1,4
#define _A_aa1 1,5
#define _A_ab1 1,6
#define _A_ac1 1,7
#define _A_ad1 1,8
#define _A_ae1 1,9
#define _A_af1 1,a

#define _A_b01 0,c
#define _A_b11 0,d
#define _A_b21 0,e
#define _A_b31 0,f
#define _A_b41 1,0
#define _A_b51 1,1
#define _A_b61 1,2
#define _A_b71 1,3
#define _A_b81 1,4
#define _A_b91 1,5
#define _A_ba1 1,6
#define _A_bb1 1,7
#define _A_bc1 1,8
#define _A_bd1 1,a
#define _A_be1 1,b
#define _A_bf1 1,c

#define _A_c01 0,d
#define _A_c11 0,e
#define _A_c21 0,f
#define _A_c31 1,0
#define _A_c41 1,1
#define _A_c51 1,2
#define _A_c61 1,3
#define _A_c71 1,4
#define _A_c81 1,5
#define _A_c91 1,6
#define _A_ca1 1,7
#define _A_cb1 1,8
#define _A_cc1 1,9
#define _A_cd1 1,a
#define _A_ce1 1,b
#define _A_cf1 1,c

#define _A_d01 0,e
#define _A_d11 0,f
#define _A_d21 1,0
#define _A_d31 1,1
#define _A_d41 1,2
#define _A_d51 1,3
#define _A_d61 1,4
#define _A_d71 1,5
#define _A_d81 1,6
#define _A_d91 1,7
#define _A_da1 1,8
#define _A_db1 1,9
#define _A_dc1 1,0
#define _A_dd1 1,1
#define _A_de1 1,2
#define _A_df1 1,3

#define _A_e01 0,f
#define _A_e11 1,0
#define _A_e21 1,1
#define _A_e31 1,2
#define _A_e41 1,3
#define _A_e51 1,4
#define _A_e61 1,5
#define _A_e71 1,6
#define _A_e81 1,7
#define _A_e91 1,8
#define _A_ea1 1,9
#define _A_eb1 1,a
#define _A_ec1 1,b
#define _A_ed1 1,c
#define _A_ee1 1,d
#define _A_ef1 1,e

#define _A_f01 1,0
#define _A_f11 1,1
#define _A_f21 1,2
#define _A_f31 1,3
#define _A_f41 1,4
#define _A_f51 1,5
#define _A_f61 1,6
#define _A_f71 1,7
#define _A_f81 1,8
#define _A_f91 1,9
#define _A_fa1 1,a
#define _A_fb1 1,b
#define _A_fc1 1,c
#define _A_fd1 1,d
#define _A_fe1 1,e
#define _A_ff1 1,f




// TODO to save space, replace _B_456 with _456, and have no leading zeroes
#define _B_0 0,0
#define _B_1 0,1
#define _B_2 0,2
#define _B_3 0,3
#define _B_4 0,4
#define _B_5 0,5
#define _B_6 0,6
#define _B_7 0,7
#define _B_8 0,8
#define _B_9 0,9
#define _B_10 0,a
#define _B_11 0,b
#define _B_12 0,c
#define _B_13 0,d
#define _B_14 0,e
#define _B_15 0,f
#define _B_16 1,0
#define _B_17 1,1
#define _B_18 1,2
#define _B_19 1,3
#define _B_20 1,4
#define _B_21 1,5
#define _B_22 1,6
#define _B_23 1,7
#define _B_24 1,8
#define _B_25 1,9
#define _B_26 1,a
#define _B_27 1,b
#define _B_28 1,c
#define _B_29 1,d
#define _B_30 1,e
#define _B_31 1,f
#define _B_32 2,0
#define _B_33 2,1
#define _B_34 2,2
#define _B_35 2,3
#define _B_36 2,4
#define _B_37 2,5
#define _B_38 2,6
#define _B_39 2,7
#define _B_40 2,8
#define _B_41 2,9
#define _B_42 2,a
#define _B_43 2,b
#define _B_44 2,c
#define _B_45 2,d
#define _B_46 2,e
#define _B_47 2,f
#define _B_48 3,0
#define _B_49 3,1
#define _B_50 3,2
#define _B_51 3,3
#define _B_52 3,4
#define _B_53 3,5
#define _B_54 3,6
#define _B_55 3,7
#define _B_56 3,8
#define _B_57 3,9
#define _B_58 3,a
#define _B_59 3,b
#define _B_60 3,c
#define _B_61 3,d
#define _B_62 3,e
#define _B_63 3,f
#define _B_64 4,0
#define _B_65 4,1
#define _B_66 4,2
#define _B_67 4,3
#define _B_68 4,4
#define _B_69 4,5
#define _B_70 4,6
#define _B_71 4,7
#define _B_72 4,8
#define _B_73 4,9
#define _B_74 4,a
#define _B_75 4,b
#define _B_76 4,c
#define _B_77 4,d
#define _B_78 4,e
#define _B_79 4,f
#define _B_80 5,0
#define _B_81 5,1
#define _B_82 5,2
#define _B_83 5,3
#define _B_84 5,4
#define _B_85 5,5
#define _B_86 5,6
#define _B_87 5,7
#define _B_88 5,8
#define _B_89 5,9
#define _B_90 5,a
#define _B_91 5,b
#define _B_92 5,c
#define _B_93 5,d
#define _B_94 5,e
#define _B_95 5,f
#define _B_96 6,0
#define _B_97 6,1
#define _B_98 6,2
#define _B_99 6,3
#define _B_100 6,4
#define _B_101 6,5
#define _B_102 6,6
#define _B_103 6,7
#define _B_104 6,8
#define _B_105 6,9
#define _B_106 6,a
#define _B_107 6,b
#define _B_108 6,c
#define _B_109 6,d
#define _B_110 6,e
#define _B_111 6,f
#define _B_112 7,0
#define _B_113 7,1
#define _B_114 7,2
#define _B_115 7,3
#define _B_116 7,4
#define _B_117 7,5
#define _B_118 7,6
#define _B_119 7,7
#define _B_120 7,8
#define _B_121 7,9
#define _B_122 7,a
#define _B_123 7,b
#define _B_124 7,c
#define _B_125 7,d
#define _B_126 7,e
#define _B_127 7,f
#define _B_128 8,0
#define _B_129 8,1
#define _B_130 8,2
#define _B_131 8,3
#define _B_132 8,4
#define _B_133 8,5
#define _B_134 8,6
#define _B_135 8,7
#define _B_136 8,8
#define _B_137 8,9
#define _B_138 8,a
#define _B_139 8,b
#define _B_140 8,c
#define _B_141 8,d
#define _B_142 8,e
#define _B_143 8,f
#define _B_144 9,0
#define _B_145 9,1
#define _B_146 9,2
#define _B_147 9,3
#define _B_148 9,4
#define _B_149 9,5
#define _B_150 9,6
#define _B_151 9,7
#define _B_152 9,8
#define _B_153 9,9
#define _B_154 9,a
#define _B_155 9,b
#define _B_156 9,c
#define _B_157 9,d
#define _B_158 9,e
#define _B_159 9,f
#define _B_160 a,0
#define _B_161 a,1
#define _B_162 a,2
#define _B_163 a,3
#define _B_164 a,4
#define _B_165 a,5
#define _B_166 a,6
#define _B_167 a,7
#define _B_168 a,8
#define _B_169 a,9
#define _B_170 a,a
#define _B_171 a,b
#define _B_172 a,c
#define _B_173 a,d
#define _B_174 a,e
#define _B_175 a,f
#define _B_176 b,0
#define _B_177 b,1
#define _B_178 b,2
#define _B_179 b,3
#define _B_180 b,4
#define _B_181 b,5
#define _B_182 b,6
#define _B_183 b,7
#define _B_184 b,8
#define _B_185 b,9
#define _B_186 b,a
#define _B_187 b,b
#define _B_188 b,c
#define _B_189 b,d
#define _B_190 b,e
#define _B_191 b,f
#define _B_192 c,0
#define _B_193 c,1
#define _B_194 c,2
#define _B_195 c,3
#define _B_196 c,4
#define _B_197 c,5
#define _B_198 c,6
#define _B_199 c,7
#define _B_200 c,8
#define _B_201 c,9
#define _B_202 c,a
#define _B_203 c,b
#define _B_204 c,c
#define _B_205 c,d
#define _B_206 c,e
#define _B_207 c,f
#define _B_208 d,0
#define _B_209 d,1
#define _B_210 d,2
#define _B_211 d,3
#define _B_212 d,4
#define _B_213 d,5
#define _B_214 d,6
#define _B_215 d,7
#define _B_216 d,8
#define _B_217 d,9
#define _B_218 d,a
#define _B_219 d,b
#define _B_220 d,c
#define _B_221 d,d
#define _B_222 d,e
#define _B_223 d,f
#define _B_224 e,0
#define _B_225 e,1
#define _B_226 e,2
#define _B_227 e,3
#define _B_228 e,4
#define _B_229 e,5
#define _B_230 e,6
#define _B_231 e,7
#define _B_232 e,8
#define _B_233 e,9
#define _B_234 e,a
#define _B_235 e,b
#define _B_236 e,c
#define _B_237 e,d
#define _B_238 e,e
#define _B_239 e,f
#define _B_240 f,0
#define _B_241 f,1
#define _B_242 f,2
#define _B_243 f,3
#define _B_244 f,4
#define _B_245 f,5
#define _B_246 f,6
#define _B_247 f,7
#define _B_248 f,8
#define _B_249 f,9
#define _B_250 f,a
#define _B_251 f,b
#define _B_252 f,c
#define _B_253 f,d
#define _B_254 f,e
#define _B_255 f,f


#define _B_0x0 0,0
#define _B_0x1 0,1
#define _B_0x2 0,2
#define _B_0x3 0,3
#define _B_0x4 0,4
#define _B_0x5 0,5
#define _B_0x6 0,6
#define _B_0x7 0,7
#define _B_0x8 0,8
#define _B_0x9 0,9
#define _B_0xa 0,a
#define _B_0xb 0,b
#define _B_0xc 0,c
#define _B_0xd 0,d
#define _B_0xe 0,e
#define _B_0xf 0,f

#define _B_0x00 0,0
#define _B_0x01 0,1
#define _B_0x02 0,2
#define _B_0x03 0,3
#define _B_0x04 0,4
#define _B_0x05 0,5
#define _B_0x06 0,6
#define _B_0x07 0,7
#define _B_0x08 0,8
#define _B_0x09 0,9
#define _B_0x0a 0,a
#define _B_0x0b 0,b
#define _B_0x0c 0,c
#define _B_0x0d 0,d
#define _B_0x0e 0,e
#define _B_0x0f 0,f
#define _B_0x10 1,0
#define _B_0x11 1,1
#define _B_0x12 1,2
#define _B_0x13 1,3
#define _B_0x14 1,4
#define _B_0x15 1,5
#define _B_0x16 1,6
#define _B_0x17 1,7
#define _B_0x18 1,8
#define _B_0x19 1,9
#define _B_0x1a 1,a
#define _B_0x1b 1,b
#define _B_0x1c 1,c
#define _B_0x1d 1,d
#define _B_0x1e 1,e
#define _B_0x1f 1,f
#define _B_0x20 2,0
#define _B_0x21 2,1
#define _B_0x22 2,2
#define _B_0x23 2,3
#define _B_0x24 2,4
#define _B_0x25 2,5
#define _B_0x26 2,6
#define _B_0x27 2,7
#define _B_0x28 2,8
#define _B_0x29 2,9
#define _B_0x2a 2,a
#define _B_0x2b 2,b
#define _B_0x2c 2,c
#define _B_0x2d 2,d
#define _B_0x2e 2,e
#define _B_0x2f 2,f
#define _B_0x30 3,0
#define _B_0x31 3,1
#define _B_0x32 3,2
#define _B_0x33 3,3
#define _B_0x34 3,4
#define _B_0x35 3,5
#define _B_0x36 3,6
#define _B_0x37 3,7
#define _B_0x38 3,8
#define _B_0x39 3,9
#define _B_0x3a 3,a
#define _B_0x3b 3,b
#define _B_0x3c 3,c
#define _B_0x3d 3,d
#define _B_0x3e 3,e
#define _B_0x3f 3,f
#define _B_0x40 4,0
#define _B_0x41 4,1
#define _B_0x42 4,2
#define _B_0x43 4,3
#define _B_0x44 4,4
#define _B_0x45 4,5
#define _B_0x46 4,6
#define _B_0x47 4,7
#define _B_0x48 4,8
#define _B_0x49 4,9
#define _B_0x4a 4,a
#define _B_0x4b 4,b
#define _B_0x4c 4,c
#define _B_0x4d 4,d
#define _B_0x4e 4,e
#define _B_0x4f 4,f
#define _B_0x50 5,0
#define _B_0x51 5,1
#define _B_0x52 5,2
#define _B_0x53 5,3
#define _B_0x54 5,4
#define _B_0x55 5,5
#define _B_0x56 5,6
#define _B_0x57 5,7
#define _B_0x58 5,8
#define _B_0x59 5,9
#define _B_0x5a 5,a
#define _B_0x5b 5,b
#define _B_0x5c 5,c
#define _B_0x5d 5,d
#define _B_0x5e 5,e
#define _B_0x5f 5,f
#define _B_0x60 6,0
#define _B_0x61 6,1
#define _B_0x62 6,2
#define _B_0x63 6,3
#define _B_0x64 6,4
#define _B_0x65 6,5
#define _B_0x66 6,6
#define _B_0x67 6,7
#define _B_0x68 6,8
#define _B_0x69 6,9
#define _B_0x6a 6,a
#define _B_0x6b 6,b
#define _B_0x6c 6,c
#define _B_0x6d 6,d
#define _B_0x6e 6,e
#define _B_0x6f 6,f
#define _B_0x70 7,0
#define _B_0x71 7,1
#define _B_0x72 7,2
#define _B_0x73 7,3
#define _B_0x74 7,4
#define _B_0x75 7,5
#define _B_0x76 7,6
#define _B_0x77 7,7
#define _B_0x78 7,8
#define _B_0x79 7,9
#define _B_0x7a 7,a
#define _B_0x7b 7,b
#define _B_0x7c 7,c
#define _B_0x7d 7,d
#define _B_0x7e 7,e
#define _B_0x7f 7,f
#define _B_0x80 8,0
#define _B_0x81 8,1
#define _B_0x82 8,2
#define _B_0x83 8,3
#define _B_0x84 8,4
#define _B_0x85 8,5
#define _B_0x86 8,6
#define _B_0x87 8,7
#define _B_0x88 8,8
#define _B_0x89 8,9
#define _B_0x8a 8,a
#define _B_0x8b 8,b
#define _B_0x8c 8,c
#define _B_0x8d 8,d
#define _B_0x8e 8,e
#define _B_0x8f 8,f
#define _B_0x90 9,0
#define _B_0x91 9,1
#define _B_0x92 9,2
#define _B_0x93 9,3
#define _B_0x94 9,4
#define _B_0x95 9,5
#define _B_0x96 9,6
#define _B_0x97 9,7
#define _B_0x98 9,8
#define _B_0x99 9,9
#define _B_0x9a 9,a
#define _B_0x9b 9,b
#define _B_0x9c 9,c
#define _B_0x9d 9,d
#define _B_0x9e 9,e
#define _B_0x9f 9,f
#define _B_0xa0 a,0
#define _B_0xa1 a,1
#define _B_0xa2 a,2
#define _B_0xa3 a,3
#define _B_0xa4 a,4
#define _B_0xa5 a,5
#define _B_0xa6 a,6
#define _B_0xa7 a,7
#define _B_0xa8 a,8
#define _B_0xa9 a,9
#define _B_0xaa a,a
#define _B_0xab a,b
#define _B_0xac a,c
#define _B_0xad a,d
#define _B_0xae a,e
#define _B_0xaf a,f
#define _B_0xb0 b,0
#define _B_0xb1 b,1
#define _B_0xb2 b,2
#define _B_0xb3 b,3
#define _B_0xb4 b,4
#define _B_0xb5 b,5
#define _B_0xb6 b,6
#define _B_0xb7 b,7
#define _B_0xb8 b,8
#define _B_0xb9 b,9
#define _B_0xba b,a
#define _B_0xbb b,b
#define _B_0xbc b,c
#define _B_0xbd b,d
#define _B_0xbe b,e
#define _B_0xbf b,f
#define _B_0xc0 c,0
#define _B_0xc1 c,1
#define _B_0xc2 c,2
#define _B_0xc3 c,3
#define _B_0xc4 c,4
#define _B_0xc5 c,5
#define _B_0xc6 c,6
#define _B_0xc7 c,7
#define _B_0xc8 c,8
#define _B_0xc9 c,9
#define _B_0xca c,a
#define _B_0xcb c,b
#define _B_0xcc c,c
#define _B_0xcd c,d
#define _B_0xce c,e
#define _B_0xcf c,f
#define _B_0xd0 d,0
#define _B_0xd1 d,1
#define _B_0xd2 d,2
#define _B_0xd3 d,3
#define _B_0xd4 d,4
#define _B_0xd5 d,5
#define _B_0xd6 d,6
#define _B_0xd7 d,7
#define _B_0xd8 d,8
#define _B_0xd9 d,9
#define _B_0xda d,a
#define _B_0xdb d,b
#define _B_0xdc d,c
#define _B_0xdd d,d
#define _B_0xde d,e
#define _B_0xdf d,f
#define _B_0xe0 e,0
#define _B_0xe1 e,1
#define _B_0xe2 e,2
#define _B_0xe3 e,3
#define _B_0xe4 e,4
#define _B_0xe5 e,5
#define _B_0xe6 e,6
#define _B_0xe7 e,7
#define _B_0xe8 e,8
#define _B_0xe9 e,9
#define _B_0xea e,a
#define _B_0xeb e,b
#define _B_0xec e,c
#define _B_0xed e,d
#define _B_0xee e,e
#define _B_0xef e,f
#define _B_0xf0 f,0
#define _B_0xf1 f,1
#define _B_0xf2 f,2
#define _B_0xf3 f,3
#define _B_0xf4 f,4
#define _B_0xf5 f,5
#define _B_0xf6 f,6
#define _B_0xf7 f,7
#define _B_0xf8 f,8
#define _B_0xf9 f,9
#define _B_0xfa f,a
#define _B_0xfb f,b
#define _B_0xfc f,c
#define _B_0xfd f,d
#define _B_0xfe f,e
#define _B_0xff f,f






#endif
