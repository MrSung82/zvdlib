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
Purpose: memory management relative utils.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_MEMUTL_H
#define ZVD_MEMUTL_H

#include "common/zvdbasedefs.h"


template <typename TVal>
class zvd_memutil_default
{
public:
	typedef zvd_size size_type;

	typedef void (*construct_fn)(void* p);
	typedef void (*copy_construct_fn)(void* p, const TVal& val);
	enum Constants
	{
		kMIN_CAP = 4
	};

	static size_type grow_capacity(size_type nNewCount, size_type nCap)
	{
		if (nNewCount > nCap)
		{
			size_type nNewCap = nCap;
			if (nNewCap < kMIN_CAP)
				nNewCap = kMIN_CAP;

			while (nNewCap < nNewCount)
			{
				nNewCap *= 2;
			}
			nCap = nNewCap;
		}
		return nCap;
	}

	static construct_fn construct;

	static copy_construct_fn copy_construct;

	static void destroy(TVal* p)
	{
		p->~TVal();
	}
};

template <typename TVal>
typename zvd_memutil_default<TVal>::construct_fn
zvd_memutil_default<TVal>::construct = 
	kZVD_NULLFPTR(typename zvd_memutil_default<TVal>::construct_fn);

template <typename TVal>
typename zvd_memutil_default<TVal>::copy_construct_fn
zvd_memutil_default<TVal>::copy_construct = 
	kZVD_NULLFPTR(typename zvd_memutil_default<TVal>::copy_construct_fn);

#endif // ZVD_MEMUTL_H