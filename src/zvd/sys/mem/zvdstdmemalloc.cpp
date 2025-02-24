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
Purpose: standard memory allocator implementation.

----------------------
 For developers notes
----------------------

*/

#include "sys/mem/zvdstdmemalloc.h"
#include "common/zvdstdlib.h"
#include "debug/zvdassert.h"

#include <new>

void* zvd_stdmemalloc::allocate(size_type nBytes, err_type* pErrorCode)
{
	return zvd_malloc(nBytes);
}

void* zvd_stdmemalloc::reallocate(void* p, size_type nBytes,
	err_type* pErrorCode)
{
	return zvd_realloc(p, nBytes);
}

zvd_stdmemalloc::err_type zvd_stdmemalloc::deallocate(void* p)
{
	zvd_free(p);
	return kZVD_R_OK;
}

zvd_uint32 zvd_stdmemalloc::is_singleton()
{ 
	return kZVD_NO_U32;
}

zvd_uint32 zvd_stdmemalloc::is_subsystem()
{
	return kZVD_NO_U32;
}

zvd_uint32 zvd_stdmemalloc::can_be_created_on_stack()
{
	return kZVD_YES_U32;
}

zvd_stdmemalloc::err_type zvd_stdmemalloc::create_on_stack(void* pStackMem, zvd_imemalloc** ppMemAlloc)
{
	ZVD_ASSERT_HIGH_NOMSG(ppMemAlloc);
	*ppMemAlloc = ::new(pStackMem)zvd_stdmemalloc();
	return kZVD_R_OK;
}