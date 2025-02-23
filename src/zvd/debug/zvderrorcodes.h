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
Purpose: error codes.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_ERRORCODES_H
#define ZVD_ERRORCODES_H

#include "common/zvdbasedefs.h"

//-----------------------------------------------------------------------------
// Error constants

// main (control) bits of error value
enum zvd_error_flags
{
	kZVD_EF_UNKNOWN = 0,
	kZVD_EFX_SPEC0 = 1,
	kZVD_EFX_INVALID = 1,
	kZVD_EFX_SPEC1 = 2,
	kZVD_EFX_DATA = 2,
	kZVD_EFX_SPEC2 = 4,
	kZVD_EFX_MEMORY = 4,
	kZVD_EFX_DATAINMEM = kZVD_EFX_MEMORY | kZVD_EFX_DATA,
	kZVD_EFX_SPEC3 = 8,
	kZVD_EFX_FILE = 8,
	kZVD_EFX_OPERATION = 16,
	kZVD_EFX_CTX = 32,

	kZVD_EF_NODATA = kZVD_EFX_DATA,
	kZVD_EF_BADDATA = kZVD_EFX_DATA | kZVD_EFX_INVALID,
	kZVD_EF_NOMEMORY = kZVD_EFX_MEMORY,
	kZVD_EF_OUTOFMEMORY = kZVD_EFX_MEMORY | kZVD_EFX_INVALID,
	kZVD_EF_CANTBECREATED = kZVD_EFX_DATAINMEM,
	kZVD_EF_BADMEMDATA = kZVD_EFX_DATAINMEM | kZVD_EFX_INVALID,
	kZVD_EF_NOFILEORDIR = kZVD_EFX_FILE,
	kZVD_EF_BADPATH = kZVD_EFX_FILE | kZVD_EFX_INVALID,
	kZVD_EF_BADFORMAT = kZVD_EFX_FILE | kZVD_EF_NODATA,
	kZVD_EF_BADDATAINFILE = kZVD_EFX_FILE | kZVD_EF_BADDATA,
	kZVD_EF_NOACCESSTOFILE = kZVD_EFX_FILE | kZVD_EFX_SPEC2,
	kZVD_EF_CANTBELOADED = kZVD_EFX_FILE | kZVD_EFX_DATAINMEM,

	kZVD_EF_OPFAILED = kZVD_EFX_OPERATION,
	kZVD_EF_NOPRECOND = kZVD_EFX_OPERATION | kZVD_EFX_SPEC0,
	kZVD_EF_DONTREPEAT = kZVD_EFX_OPERATION | kZVD_EFX_SPEC1,
	kZVD_EF_BADARG = kZVD_EFX_OPERATION | kZVD_EF_BADDATA,
	kZVD_EF_SYSTEM = kZVD_EFX_OPERATION | kZVD_EFX_SPEC2,

	kZVD_EF_RESERVED1 = 13,
	kZVD_EF_RESERVED2 = 15,

	kZVD_EF_CTX_FIRST = 21,
	kZVD_EF_CTX_LAST = 62,
	kZVD_EF_NOTFOUND = 0x3F,

};

// Base error codes (its cover more frequent cases)
// "E" means error
// "R" means return value (result)
//-------------------------------------------------

/// somethin was not found in search operation
const zvd_uint8 kZVD_R_NOTFOUND = ZVD_PACK_ERROR_S_U8(kZVD_EF_NOTFOUND);
/// no error (success)
const zvd_uint8 kZVD_R_OK = kZVD_EF_SUCCESS;
/// there is no need to continue operation
const zvd_uint8 kZVD_R_CANCEL = kZVD_EF_SPECIAL;
/// Not enough memory to continue operation
const zvd_uint8 kZVD_E_NOMEMORY = ZVD_PACK_ERROR_U8(kZVD_EF_NOMEMORY);


#endif // ZVD_ERRORCODES_H