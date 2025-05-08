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
 */


//#define IFEQ_ZERO(__value, __block) _IFEQ_ZERO##__value(__block)
//#define _IFEQ_ZERO_0(__block) block

//#define IFNEQ_ZERO(__value, __block) _IFNEQ_ZERO_##__value(__block)
//#define _IFNEQ_ZERO_0(__block) block

// would this even be useful for anything?
//#define EVAL(__n) __n





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
// Will discard any extra params.
// T/ODO: consider enforcing only two params to help with finding errors
//#define _CONCAT(__0, ...) __0 ## __VA_ARGS__
#define _CONCAT(__0, __1) __0 ## __1





#define NOT(__b) CONCAT(_NOT_, __b))
#define _NOT_true false
#define _NOT_false true


// I don't remember what I was planning for this either
//#define NONE none


#define _IT(...) IFTHEN(__VA_ARGS__)

// alright. I think the itterator gets one value, and returns two.

#define WHILE(__init, __itter, __block) _W(__itter(__init), __block)
// value, condition, itterator, block
#define _W(_v,_c,_i,_b) _W0() _IT(_)
#define _W0(_v,_c,_i,_b) _W1(_ITE(_c(_i)), __b) __b
#define _W1(_v,_c,_i,_b) _W2(_ITE(_c(_i)), __b) __b

// loop itterator constructor
#define LOOP_ITTER(__start, __stop, __inc) __start, __stop, __inc _LOOP_ITTER
// loop itterator
#define _LOOP_ITTER(__start, __stop, __inc, __itter) 



#define IS_ZERO(__n) AND(_IS_ZERO)

// T/ODO: Yeah, just replace this with the updated system eventually
/*#define IS_ZERO(__n) CONCAT(_IS_ZERO_, __n)
#define _IS_ZERO_0 true
#define _IS_ZERO_1 false
#define _IS_ZERO_2 false
#define _IS_ZERO_3 false
#define _IS_ZERO_4 false
#define _IS_ZERO_5 false
#define _IS_ZERO_6 false
#define _IS_ZERO_7 false
#define _IS_ZERO_8 false
#define _IS_ZERO_9 false
#define _IS_ZERO_10 false
#define _IS_ZERO_11 false
#define _IS_ZERO_12 false
#define _IS_ZERO_13 false
#define _IS_ZERO_14 false
#define _IS_ZERO_15 false
#define _IS_ZERO_16 false
#define _IS_ZERO_17 false
#define _IS_ZERO_18 false
#define _IS_ZERO_19 false
#define _IS_ZERO_20 false
#define _IS_ZERO_21 false
#define _IS_ZERO_22 false
#define _IS_ZERO_23 false
#define _IS_ZERO_24 false
#define _IS_ZERO_25 false
#define _IS_ZERO_26 false
#define _IS_ZERO_27 false
#define _IS_ZERO_28 false
#define _IS_ZERO_29 false
#define _IS_ZERO_30 false
#define _IS_ZERO_31 false
#define _IS_ZERO_32 false
#define _IS_ZERO_33 false
#define _IS_ZERO_34 false
#define _IS_ZERO_35 false
#define _IS_ZERO_36 false
#define _IS_ZERO_37 false
#define _IS_ZERO_38 false
#define _IS_ZERO_39 false
#define _IS_ZERO_40 false
#define _IS_ZERO_41 false
#define _IS_ZERO_42 false
#define _IS_ZERO_43 false
#define _IS_ZERO_44 false
#define _IS_ZERO_45 false
#define _IS_ZERO_46 false
#define _IS_ZERO_47 false
#define _IS_ZERO_48 false
#define _IS_ZERO_49 false
#define _IS_ZERO_50 false
#define _IS_ZERO_51 false
#define _IS_ZERO_52 false
#define _IS_ZERO_53 false
#define _IS_ZERO_54 false
#define _IS_ZERO_55 false
#define _IS_ZERO_56 false
#define _IS_ZERO_57 false
#define _IS_ZERO_58 false
#define _IS_ZERO_59 false
#define _IS_ZERO_60 false
#define _IS_ZERO_61 false
#define _IS_ZERO_62 false
#define _IS_ZERO_63 false
#define _IS_ZERO_64 false*/


#define ISNZERO(__n) NOT(ISZERO(__n))



#define IFTHEN(__cond, __block) CONCAT(_IFTHEN_, __cond(__block))
#define _IFTHEN_true(__block) __block
#define _IFTHEN_false(__block)


#define IFNTHEN(__cond, __block) IFTHEN(NOT(__cond), __block)


#define IFTHENELSE(__cond, __then, __else) \
    IFTHEN(__cond, __then) \
    IFNTHEN(__cond, __else)



#define IFZERO(__n, __block) IFTHEN(ISZERO(__n), __block)
#define IFNZERO(__n, __block) IFTHEN(ISNZERO(__n), __block)



#define NUMBER(...) _NUMBER( stuff here


// TODO: yeah, lets just stick with number loops for now
// with maybe some behavior enums
// or maybe we can just have an itterator where we pass a macro function
//FOR(GTR(), "stuff")

#define LOOP(__cond, block) _D_##__cond
#define _D_


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

#define BLANK

// T/ODO: Have concat accept variable number of parameters
//#define CONCAT(__a, __b) _CONCAT(__a, __b)
//#define _CONCAT(__a, __b) __a ## __b
//#define CONCAT(...) _CONCAT(__a, __b)
//#define _CONCAT(...) __a ## __b


#define ALLOW_SECOND(...) _ALLOW_SECOND(__VA_ARGS__, BLANK, BLANK)
#define _ALLOW_SECOND(__n, __m, ...) __m

/*#define RETURN_DEFINED(__n) ALLOW_SECOND(CONCAT(_RETURN_DEFINED_, __n))
#define _RETURN_DEFINED_void BLANK, "void"
#define _RETURN_DEFINED_bog BLANK, "bog"*/

// usage PARAM(3, __VA_ARGS__) or PARAM(3, (__VA_ARGS__))
#define PARAM(__n, ...) CONCAT(_PARAM_, __n)(UNPACK(__VA_ARGS__))
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


// TODO: Fix PARAM macro and replace ALLOW_SECOND with it
#define DECAST(__n) ALLOW_SECOND(_DECAST_ __n, __n)
//#define DECAST(__n) PARAM(1 , _DECAST_ __n, __n)
#define _DECAST_(__n) BLANK, __n

//#define ISVOID(__n) _ISVOID_ADJUST(ALLOW_SECOND(CONCAT(_ISVOID_, __n)))
#define ISVOID(__n) _ISVOID_ADJUST(ALLOW_SECOND(CONCAT(_ISVOID_, DECAST(__n))))
#define _ISVOID_void NONE, true
// note that _CONCAT is neccissary instead of CONCAT
#define _ISVOID_ADJUST(__n) _CONCAT(__ISVOID_ADJUST_, __n)
#define __ISVOID_ADJUST_true true
#define __ISVOID_ADJUST_ false



// TODO: finish this function
// this will return true if it is any of the known types of values or parameters
// otherwise it will return false
//#define ISEXIST(__n) OR(ISVOID(__n), ISNUM(__n), ISBOOL(__n))
#define ISDEF(__n) OR(ISVOID(__n), ISNUM(__n), ISBOOL(__n), ISBLANK(__n))




// Say, with my new trick, is it possible to check if a value is defined
// by basically comparing the name of the value with the definition?
// I suppose not, as that would require the hashtag, and can't be generated



// !!!!!!!!!
// TODO: I should consider encouraging myself to use _CONCAT instead of CONCAT
// for internal usage of all of my macros. Because I believe it would still all
// work while also avoiding certain issues. And just leave the CONCAT for use
// outside of these macro definitions
// !!!!!!!!!


// TODO: create a macro test file, and run the c preprocessor on it to test
// that everything is working


#define _TRUTH_ADJUST(__n) _CONCAT(__TRUTH_ADJUST_, __n)
#define __TRUTH_ADJUST_ false
#define __TRUTH_ADJUST_true true


// this only works for macros defined as blank. Undefined macros
// will return false rather than true
// TODO: replace ALLOW_SECOND with PARAM when you get that working
#define ISBLANK(__n) _TRUTH_ADJUST(ALLOW_SECOND(_CONCAT(_ISBLANK_, __n)))
#define _ISBLANK_ BLANK, true

// get number of parameters (up to 9 for now
// TODO: replace with a while loop when you get that set up
// TODO: Finish writing this macro
// TODO: use this for determining size of number
// to allow for different number sizes rather than static
#define PLEN(...) _PLEN(_PARAM_ADJUST(__VA_ARGS__, BLANK))
#define _PLEN()


// will return true zero params if first param is blank
// TODO: use PARAM to get first param when you get that working
// TODO: figure out if _CONCAT can be used in place of CONCAT
#define _PARAM_ADJUST(...) __PARAM_ADJUST(__VA_ARGS__, BLANK)
#define __PARAM_ADJUST(__0, ...) CONCAT(___PARAM_ADJUST_, ISBLANK(__0))(__0, ...)
#define ___PARAM_ADJUST_true(...)
#define ___PARAM_ADJUST_false(...) return __VA_ARGS__



// basically, if true then true. If anything else, then false
#define ISTRUTHY(__n) _TRUTH_ADJUST(_CONCAT(_ISTRUTHY_, __n))


#define SURVIVE_SECOND(__n) BLANK, __n


// number format:
// p w 2 3 a 7


// TODO: replace the BLANK, true system with something a little
// more intuitive, such as a truthy adjust function so that I don't have to
// worry about second parameters for everything
#define _ISSIGNED(...) __ISSIGNED(__VA_ARGS__, BLANK)
#define __ISSIGNED(__0, ...) _TRUTH_ADJUST(ALLOW_SECOND(_CONCAT(___ISSIGNED_, __0)))
#define ___ISSIGNED_n BLANK, true
#define ___ISSIGNED_p BLANK, true


#define NEG(__n) CONCAT(_NEG_, PARAM(0, UNPACK(__n)))(UNPACK(__n))
// should always receive at least three parameters
#define _NEG_true(__sign, ...) _NEG_##__sign(__VA_ARGS__)
#define _NEG_false(__n) _NEG_true(BYTE(__n))
#define _NEG_p(...) n,__VA_ARGS__
#define _NEG_n(...) p,__VA_ARGS__


// used by PARAM to address both __VA_ARGS__ and a packed array
#define ISPACKED(...) _ISPACKED(__VA_ARGS__, BLANK. BLANK)
#define _ISPACKED(__0, __1, ...) AND(ISBLANK(__1), ISTRUTHY(CONCAT(__ISPACKED, __0)))
#define __ISPACKED(...) true


#define PACK(...) (__VA_ARGS__)

#define UNPACK(__n) _CONCAT(_UNPACK, __n)
#define _UNPACK(...) __VA_ARGS__


#define NOP(...)


// to prevent recursively expanding every call in a loop
// this will only call a macro if the condition is true
// the last value is the params
#define CCALL(__cond, __macro, ...) CONCAT(IFTHENELSE(__cond, __macro, NOP)(__VA_ARGS__))


#define REMOVE_LEADING_ZEROES(__n) PACK(_RLZ_0(UNPACK(__n)))
#define _RLZ_0(__zero, ...) IFZERO(__zero)


#define UNSIGN(__n) PACK(_UNSIGN(UNPACK(__n)))
#define _UNSIGN(__sign, ...) __VA_ARGS__


// will discard the sign!!!
#define UNBYTE(__n) CONCAT(0x, UNPACK(REMOVE_LEADING_ZEROES(UNSIGN(__n))))
//#define _UNBYTE(__sign, ...) CONCAT(0x, __VA_ARGS__)


// accepts packed byte, returns packed byte
// probably would benefit from the while loop
#define ADD(__a, __b) 


#define _ADC(__0, __1) CONCAT(_A_, __0, __1)


//#define BYTE(__n) p b _B_##__n
#define BYTE(__n) PACK(p,CONCAT(_B_,__n))

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

#define _A_00 0,0
#define _A_01 0,1
#define _A_02 0,2
#define _A_03 0,3
#define _A_04 0,4
#define _A_05 0,5
#define _A_06 0,6
#define _A_07 0,7
#define _A_08 0,8
#define _A_09 0,9
#define _A_0a 0,a
#define _A_0b 0,b
#define _A_0c 0,c
#define _A_0d 0,d
#define _A_0e 0,e
#define _A_0f 0,f

#define _A_10 0,1
#define _A_11 0,2
#define _A_12 0,3
#define _A_13 0,4
#define _A_14 0,5
#define _A_15 0,6
#define _A_16 0,7
#define _A_17 0,8
#define _A_18 0,9
#define _A_19 0,a
#define _A_1a 0,b
#define _A_1b 0,c
#define _A_1c 0,d
#define _A_1d 0,e
#define _A_1e 0,f
#define _A_1f 1,0

#define _A_20 0,2
#define _A_21 0,3
#define _A_22 0,4
#define _A_23 0,5
#define _A_24 0,6
#define _A_25 0,7
#define _A_26 0,8
#define _A_27 0,9
#define _A_28 0,a
#define _A_29 0,b
#define _A_2a 0,c
#define _A_2b 0,d
#define _A_2c 0,e
#define _A_2d 0,f
#define _A_2e 1,0
#define _A_2f 1,1

#define _A_30 0,3
#define _A_31 0,4
#define _A_32 0,5
#define _A_33 0,6
#define _A_34 0,7
#define _A_35 0,8
#define _A_36 0,9
#define _A_37 0,a
#define _A_38 0,b
#define _A_39 0,c
#define _A_3a 0,d
#define _A_3b 0,e
#define _A_3c 0,f
#define _A_3d 1,0
#define _A_3e 1,1
#define _A_3f 1,2

#define _A_40 0,4
#define _A_41 0,5
#define _A_42 0,6
#define _A_43 0,7
#define _A_44 0,8
#define _A_45 0,9
#define _A_46 0,a
#define _A_47 0,b
#define _A_48 0,c
#define _A_49 0,d
#define _A_4a 0,e
#define _A_4b 0,f
#define _A_4c 1,0
#define _A_4d 1,1
#define _A_4e 1,2
#define _A_4f 1,3

#define _A_50 0,5
#define _A_51 0,6
#define _A_52 0,7
#define _A_53 0,8
#define _A_54 0,9
#define _A_55 0,a
#define _A_56 0,b
#define _A_57 0,c
#define _A_58 0,d
#define _A_59 0,e
#define _A_5a 0,f
#define _A_5b 1,0
#define _A_5c 1,1
#define _A_5d 1,2
#define _A_5e 1,3
#define _A_5f 1,4

#define _A_60 0,6
#define _A_61 0,7
#define _A_62 0,8
#define _A_63 0,9
#define _A_64 0,a
#define _A_65 0,b
#define _A_66 0,c
#define _A_67 0,d
#define _A_68 0,e
#define _A_69 0,f
#define _A_6a 1,0
#define _A_6b 1,1
#define _A_6c 1,2
#define _A_6d 1,3
#define _A_6e 1,4
#define _A_6f 1,5

#define _A_70 0,7
#define _A_71 0,8
#define _A_72 0,9
#define _A_73 0,a
#define _A_74 0,b
#define _A_75 0,c
#define _A_76 0,d
#define _A_77 0,e
#define _A_78 0,f
#define _A_79 1,0
#define _A_7a 1,1
#define _A_7b 1,2
#define _A_7c 1,3
#define _A_7d 1,4
#define _A_7e 1,5
#define _A_7f 1,6

#define _A_80 0,8
#define _A_81 0,9
#define _A_82 0,a
#define _A_83 0,b
#define _A_84 0,c
#define _A_85 0,d
#define _A_86 0,e
#define _A_87 0,f
#define _A_88 1,0
#define _A_89 1,1
#define _A_8a 1,2
#define _A_8b 1,3
#define _A_8c 1,4
#define _A_8d 1,5
#define _A_8e 1,6
#define _A_8f 1,7

#define _A_90 0,9
#define _A_91 0,a
#define _A_92 0,b
#define _A_93 0,c
#define _A_94 0,d
#define _A_95 0,e
#define _A_96 0,f
#define _A_97 1,0
#define _A_98 1,1
#define _A_99 1,2
#define _A_9a 1,3
#define _A_9b 1,4
#define _A_9c 1,5
#define _A_9d 1,6
#define _A_9e 1,7
#define _A_9f 1,8

#define _A_a0 0,a
#define _A_a1 0,b
#define _A_a2 0,c
#define _A_a3 0,d
#define _A_a4 0,e
#define _A_a5 0,f
#define _A_a6 1,0
#define _A_a7 1,1
#define _A_a8 1,2
#define _A_a9 1,3
#define _A_aa 1,4
#define _A_ab 1,5
#define _A_ac 1,6
#define _A_ad 1,7
#define _A_ae 1,8
#define _A_af 1,9

#define _A_b0 0,b
#define _A_b1 0,c
#define _A_b2 0,d
#define _A_b3 0,e
#define _A_b4 0,f
#define _A_b5 1,0
#define _A_b6 1,1
#define _A_b7 1,2
#define _A_b8 1,3
#define _A_b9 1,4
#define _A_ba 1,5
#define _A_bb 1,6
#define _A_bc 1,7
#define _A_bd 1,8
#define _A_be 1,9
#define _A_bf 1,a

#define _A_c0 0,c
#define _A_c1 0,d
#define _A_c2 0,e
#define _A_c3 0,f
#define _A_c4 1,0
#define _A_c5 1,1
#define _A_c6 1,2
#define _A_c7 1,3
#define _A_c8 1,4
#define _A_c9 1,5
#define _A_ca 1,6
#define _A_cb 1,7
#define _A_cc 1,8
#define _A_cd 1,9
#define _A_ce 1,a
#define _A_cf 1,b

#define _A_d0 0,d
#define _A_d1 0,e
#define _A_d2 0,f
#define _A_d3 1,0
#define _A_d4 1,1
#define _A_d5 1,2
#define _A_d6 1,3
#define _A_d7 1,4
#define _A_d8 1,5
#define _A_d9 1,6
#define _A_da 1,7
#define _A_db 1,8
#define _A_dc 1,9
#define _A_dd 1,a
#define _A_de 1,b
#define _A_df 1,c

#define _A_e0 0,e
#define _A_e1 0,f
#define _A_e2 0,0
#define _A_e3 1,1
#define _A_e4 1,2
#define _A_e5 1,3
#define _A_e6 1,4
#define _A_e7 1,5
#define _A_e8 1,6
#define _A_e9 1,7
#define _A_ea 1,8
#define _A_eb 1,9
#define _A_ec 1,a
#define _A_ed 1,b
#define _A_ee 1,c
#define _A_ef 1,d

#define _A_f0 0,f
#define _A_f1 0,0
#define _A_f2 0,1
#define _A_f3 1,2
#define _A_f4 1,3
#define _A_f5 1,4
#define _A_f6 1,5
#define _A_f7 1,6
#define _A_f8 1,7
#define _A_f9 1,8
#define _A_fa 1,9
#define _A_fb 1,a
#define _A_fc 1,b
#define _A_fd 1,c
#define _A_fe 1,d
#define _A_ff 1,e



#endif


// http://jhnet.co.uk/articles/cpp_magic
// https://www.iar.com/knowledge/learn/programming/advanced-preprocessor-tips-and-tricks/

// still trying to find resource that describes macro order of operation
// it is probably located in one of my earlier projects somewhere.
