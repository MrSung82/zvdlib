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
Purpose: standard memory allocator definition.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_STDMEMALLOC_H
#define ZVD_STDMEMALLOC_H


#include "common/zvdimemalloc.h"

class zvd_stdmemalloc : public zvd_imemalloc
{
public:

	virtual ~zvd_stdmemalloc() {}

	virtual void* allocate(size_type nBytes, 
		zvd_uint32* pErrorCode = kZVD_NULLPTR(zvd_uint32));

	virtual void* reallocate(void* p, size_type nBytes, 
		zvd_uint32* pErrorCode = kZVD_NULLPTR(zvd_uint32));

	virtual zvd_uint32 deallocate(void* p);

	virtual bool is_singleton() const;

	/// If true this memory allocator is subsystem of some larger system (engine).
	virtual bool is_subsystem() const;

	virtual bool can_be_created_on_stack() const;
};

#endif // ZVD_STDMEMALLOC_H