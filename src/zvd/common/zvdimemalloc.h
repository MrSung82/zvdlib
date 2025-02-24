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
Purpose: memory allocator interface.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_IMEMALLOC_H
#define ZVD_IMEMALLOC_H


#include "debug/zvderrorcodes.h"

class zvd_imemalloc
{
public:
	typedef zvd_size size_type;
	typedef zvd_uint8 err_type;

	virtual ~zvd_imemalloc() {}

	virtual void* allocate(size_type nBytes, 
		err_type* pErrorCode = kZVD_NULLPTR(err_type)) = 0;

	virtual void* reallocate(void* p, size_type nBytes, 
		err_type* pErrorCode = kZVD_NULLPTR(err_type)) = 0;

	virtual err_type deallocate(void* p) = 0;

	// Methods to be used when allocator is template parameter
	//--------------------------------------------------------
	static zvd_uint32 is_singleton() { return kZVD_NO_U32; }
	// If singleton implement these methods:
	static zvd_imemalloc* instance() { return kZVD_NULLPTR(zvd_imemalloc); }

	/// If true this memory allocator is subsystem of some larger system (engine).
	static zvd_uint32 is_subsystem() { return kZVD_NO_U32; }
	// If subsystem implement these methods:
	// 
	//	You can request to engine class for example 
	static zvd_imemalloc* as_subsystem() { return kZVD_NULLPTR(zvd_imemalloc); }
		//
	virtual zvd_uint32 is_ready() const { return kZVD_NO_U32; }
	virtual zvd_uint32 is_inited() const { return kZVD_NO_U32; }

	static zvd_uint32 can_be_created_on_stack() { return kZVD_YES_U32; }
	// If subsystem implement these methods:
	static err_type create_on_stack(void* pStackMem, zvd_imemalloc** ppMemAlloc)
	{
		return kZVD_E_UNACCEPTABLE;
	}
};

#endif // ZVD_IMEMALLOC_H