/*
-----------------
 Persistent info
-----------------

This file is part of the "Zv3D" project.

MIT License

Copyright (c) 2012-2099 Marat Sungatullin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

......
 Web:
......

 + https:// (for questions and help)

-------------
 Description
-------------
Purpose: base definitions.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_BASEDEFS_H
#define ZVD_BASEDEFS_H

#if defined(ZVD_HAS_STDINT_H)
#	include <stdint.h>
#endif

#ifdef ZVD_DLL_EXPORTS
#define ZVD_API __declspec(dllexport)
#else
#define ZVD_API __declspec(dllimport)
#endif

//-----------------------------------------------------------------------------
// Compilers family detection

#if defined(_MSC_VER) && !defined(ZVD_MSVC)
// Microsoft Visual C++ compiler
#	define ZVD_COMPILER_MSVC
#	define ZVD_COMPILER_VER _MSC_VER
#	define ZVD_MSVC _MSC_VER

#elif defined(__GNUC__) && !defined(ZVD_GNUC)
#	define ZVD_COMPILER_GCC
#	define ZVD_COMPILER_VER __GNUC__
#	define ZVD_GNUC __GNUC__// GNU C compilers
#	define ZVD_GNUC_VER \
(__GNUC__ * 10000  + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)
...
/* Test for GCC > 3.2.0 */
//#if ZVD_GNUC_VER > 30200

#else
#	error "No supported compiler was found."

#endif // compiler

// Test to define how much bit processor architecture has.
//--------------------------------------------------------
#if defined(ZVD_MSVC)
// test x86
#	ifdef _M_IX86
#		define ZVD_ARCH_X86
#	endif

// test x64
#	ifdef _M_X64
#		define ZVD_ARCH_X64
#	endif

#endif // how much bit processor architecture tests


// fixed size integers
//---------------------
#if defined(ZVD_HAS_STDINT_H)
typedef int8_t zvd_int8;
typedef int16_t zvd_int16;
typedef int32_t zvd_int32;

typedef uint8_t zvd_uint8;
typedef uint16_t zvd_uint16;
typedef uint32_t zvd_uint32;

#	if defined(ZVD_ARCH_X64)
typedef int64_t zvd_int64;
typedef uint64_t zvd_uint64;
#	endif

#elif defined(ZVD_MSVC)
typedef __int8 zvd_int8;
typedef __int16 zvd_int16;
typedef __int32 zvd_int32;

typedef unsigned __int8 zvd_uint8;
typedef unsigned __int16 zvd_uint16;
typedef unsigned __int32 zvd_uint32;

#	if defined(ZVD_ARCH_X64) // Нужен ли этот макрос здесь?
typedef __int64 zvd_int64;
typedef unsigned __int64 zvd_uint64;
#	endif

#endif // fixed size integers

// Human readable convenient integers
//-----------------------------------
typedef zvd_uint8 zvd_byte;

#if defined(ZVD_ARCH_X64)
typedef zvd_uint64 zvd_size;
typedef zvd_int64 zvd_ptrdiff;

#elif defined(ZVD_ARCH_X86)
typedef zvd_uint32 zvd_size;
typedef zvd_int32 zvd_ptrdiff;

#else
typedef zvd_uint16 zvd_size;
typedef zvd_int16 zvd_ptrdiff;

#endif // arch


//++++++++++++++++++++++++++++++++++++
// Language standard dependent things
//++++++++++++++++++++++++++++++++++++
#ifdef ZVD_CPP11 
#	define kZVD_NULLPTR(argType) nullptr
#	define kZVD_NULLVOID nullptr
#	define kZVD_NULLFPTR(argType) nullptr

#else
#	define kZVD_NULLPTR(argType) ((argType*)0)
#	define kZVD_NULLVOID ((void*)0)
#	define kZVD_NULLFPTR(argType) ((argType)0)
#endif

//++++++++++++++++++
// Byte utils
//++++++++++++++++++

#define ZVD_ENUM2U8(enumVal) ((zvd_uint8)(enumVal))
#define ZVD_ENUM2U32(enumVal) ((zvd_uint32)(enumVal))

const zvd_uint32 kZVD_ONE_U32 = 0x01;

#define ZVD_IS_LITTLE_ENDIAN \
((*static_cast<const zvd_uint8*>(static_cast<const void*>(&kZVD_ONE_U32))) == 0x01)


//++++++++++++++++++
// Global constants
//++++++++++++++++++

//-----------------------------------------------------------------------------
// Boolean constants

// cpp lang
const bool			kZVD_TRUE = true;
const bool			kZVD_FALSE = false;

const bool			kZVD_YES = kZVD_TRUE;
const bool			kZVD_NO = kZVD_FALSE;

// as integers
const zvd_uint32	kZVD_TRUE_U32 = 1;
const zvd_uint32	kZVD_FALSE_U32 = 0;

const zvd_uint8		kZVD_TRUE_U8 = 1;
const zvd_uint8		kZVD_FALSE_U8 = 0;

// yes/no answer constants
const zvd_uint32	kZVD_YES_U32 = kZVD_TRUE_U32;
const zvd_uint32	kZVD_NO_U32 = kZVD_FALSE_U32;

const zvd_uint8		kZVD_YES_U8 = kZVD_TRUE_U8;
const zvd_uint8		kZVD_NO_U8 = kZVD_FALSE_U8;

//-----------------------------------------------------------------------------
// Error format

// Main (control) bits of error value
// Use other 6 bit for 64 codes. Error codes must be interpreted
// in context of local operation.
enum zvd_error_control_bits
{
	/// no error (success)
	kZVD_EF_SUCCESS = 0,
	/// special marker - dependent flag
	kZVD_EF_SPECIAL = 1,
	/// error marker
	kZVD_EF_FAIL = 2,

	kZVD_EF_CODEBITS_OFFSET = 2,
};

// Packs error flags to integers
//------------------------------

#define ZVD_PACK_ERROR_U8(errorFlag) (\
(ZVD_ENUM2U8(errorFlag) << kZVD_EF_CODEBITS_OFFSET) | \
ZVD_ENUM2U8(kZVD_EF_FAIL))

#define ZVD_PACK_ERROR_S_U8(errorFlag) (\
(ZVD_ENUM2U8(errorFlag) << kZVD_EF_CODEBITS_OFFSET) | \
ZVD_ENUM2U8(kZVD_EF_FAIL) | ZVD_ENUM2U8(kZVD_EF_SPECIAL))

#endif // ZVD_BASEDEFS_H