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
Purpose: stdlib interface implementation.

----------------------
 For developers notes
----------------------

*/

#include "common/zvdstdlib.h"

#include <cstdlib>
//#include <iostream>

void* zvd_malloc_impl(zvd_size nBytes)
{
	void* pResult = std::malloc(nBytes);
	if (!pResult)
	{
		/// @todo not implemented yet
	}
	return pResult;
}

void* zvd_realloc_impl(void* ptr, zvd_size nBytes)
{
	void* pResult = std::realloc(ptr, nBytes);
	if (!pResult)
	{
		/// @todo not implemented yet
	}
	return pResult;
}

void zvd_free_impl(void* ptr)
{
	std::free(ptr);
}

zvd_malloc_fptr zvd_malloc = zvd_malloc_impl;
zvd_realloc_fptr zvd_realloc = zvd_realloc_impl;
zvd_free_fptr zvd_free = zvd_free_impl;