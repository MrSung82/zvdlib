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
Purpose: dynamic array.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_DARRAY_H
#define ZVD_DARRAY_H

#include "common/zvdmemutl.h"
#include "common/zvdimemalloc.h"
#include "debug/zvdassert.h"
#include "debug/zvderrorcodes.h"



template <typename TElem, typename TMemAlloc,
	typename TUtil = zvd_memutil_default<TElem> >
class zvd_darray
{
public:
	typedef zvd_size size_type;
	typedef TElem* iterator;
	typedef typename TMemAlloc::err_type ret_type;

	zvd_darray()
		: m_pData(kZVD_NULLPTR(TElem))
		, m_nCount(0)
		, m_nCapacity(0)
	{

	}

	~zvd_darray()
	{
		clear();
		ret_type retVal = resize_memory(0);
		if (kZVD_R_OK != retVal)
		{
			/// @todo
			// abort program
		}
	}

	void clear()
	{
		for (size_type i = 0; i < m_nCount; ++i)
		{
			TUtil::destroy(&m_pData[i]);
		}
		m_nCount = 0;
	}

	ret_type grow(size_type nGrowBy = 1)
	{
		ret_type retVal = grow_memory(nGrowBy);
		if (kZVD_R_OK != retVal)
		{
			return retVal;
		}

		size_type nNewCount = m_nCount + nGrowBy;
		for (size_type i = m_nCount; i < nNewCount; ++i)
		{
			TUtil::construct(m_pData + i);
		}
		m_nCount = nNewCount;
		return kZVD_R_OK;
	}

	ret_type push_back(const TElem& val)
	{
		ret_type retVal = grow_memory(1);
		if (kZVD_R_OK != retVal)
		{
			return retVal;
		}
		TUtil::copy_construct(m_pData + m_nCount, val);
		++m_nCount;
		return kZVD_R_OK;
	}

	ret_type reserve(size_type nNewCap)
	{
		if (nNewCap <= m_nCapacity)
			return kZVD_R_OK;
		return resize_memory(nNewCap);
	}

	template<typename EqPred>
	size_type find(const TElem& val, const EqPred& pred,
		size_type startIdx = 0, size_type endIdx = m_nCount) const
	{
		if (endIdx > m_nCount)
			endIdx = m_nCount;

		for (size_type idx = startIdx; idx < endIdx; ++idx)
		{
			if (pred(m_pData[idx], val))
				return idx;
		}
		return m_nCount;
	}

	TElem* at(size_type idx)
	{
		if (idx >= m_nCount)
			return kZVD_NULLPTR(TElem);
		return m_pData + idx;
	}

	const TElem* at(size_type idx) const
	{
		if (idx >= m_nCount)
			return kZVD_NULLPTR(TElem);
		return m_pData + idx;
	}

	iterator begin()
	{
		return m_pData;
	}

	iterator end()
	{
		return m_pData + m_nCount;
	}

	size_type size() const
	{
		return m_nCount;
	}

	size_type capacity() const
	{
		return m_nCapacity;
	}

	ret_type get_allocator(zvd_imemalloc** ppMemAlloc)
	{
		return m_get_mem_alloc(ppMemAlloc, kZVD_NO_U32);
	}
private:

	ret_type m_get_mem_alloc(zvd_imemalloc** ppMemAlloc,
		zvd_uint32 bUsedByThisClass = kZVD_YES_U32,
		void* pStackMem = kZVD_NULLVOID)
	{
		ZVD_ASSERT_HIGH_NOMSG(ppMemAlloc);
		*ppMemAlloc = kZVD_NULLPTR(zvd_imemalloc);

		ret_type retVal = kZVD_R_OK;

		zvd_imemalloc* pMemAlloc = kZVD_NULLPTR(zvd_imemalloc);
		if (TMemAlloc::is_singleton() == kZVD_YES_U32)
		{
			pMemAlloc = TMemAlloc::instance();
			ZVD_ASSERT_MEDIUM_NOMSG(pMemAlloc);
			if (!pMemAlloc)
			{
				retVal = kZVD_E_NOPRECOND;
				return retVal;
			}
		}
#ifndef ZVD_USE_ZVD_AS_LIB_ONLY
		else if (TMemAlloc::is_subsystem())
		{
			pMemAlloc = TMemAlloc::as_subsystem();
			if (!pMemAlloc)
			{
				retVal = kZVD_E_NOPRECOND;
				return retVal;
			}

			if (pMemAlloc->is_ready() == kZVD_NO_U32)
			{
				if (pMemAlloc->is_inited() == kZVD_NO_U32)
				{
					retVal = kZVD_E_NOPRECOND;
				}
				else
				{
					retVal = kZVD_E_NOPRECOND;
				}
				return retVal;
			}
		}
#endif // ! zvd as lib only
		else
		{
			// create mem alloc on stack of called function

			ZVD_ASSERT_HIGH_NOMSG(kZVD_YES_U32 == bUsedByThisClass);
			ZVD_ASSERT_HIGH_NOMSG(pStackMem);

			if (TMemAlloc::can_be_created_on_stack() == kZVD_NO_U32)
			{
				retVal = kZVD_E_NOPRECOND;
				return retVal;
			}
			retVal = TMemAlloc::create_on_stack(pStackMem, &pMemAlloc);
			if (kZVD_R_OK != retVal)
			{
				return retVal;
			}
		}

		*ppMemAlloc = pMemAlloc;
		return kZVD_R_OK;
	}

	ret_type grow_memory(size_type nGrowBy)
	{
		size_type nNewCount = m_nCount + nGrowBy;

		if (nNewCount > m_nCapacity)
		{
			size_type nNewCap = TUtil::grow_capacity(nNewCount, m_nCapacity);

			ret_type retVal = resize_memory(nNewCap);
			if (kZVD_R_OK != retVal)
			{
				return retVal;
			}
		}

		return kZVD_R_OK;
	}

	ret_type m_resize_memory_if_data(size_type nNewCap, zvd_imemalloc* pMemAlloc)
	{
		ret_type retVal = kZVD_R_OK;

		ZVD_ASSERT_HIGH_NOMSG(pMemAlloc);
		ZVD_ASSERT_HIGH_NOMSG(m_pData);

		typename TMemAlloc::err_type errVal;

		if (0 == nNewCap)
		{
			clear();
			errVal = pMemAlloc->deallocate(m_pData);
			if (kZVD_R_OK != errVal)
			{
				return kZVD_E_ABORT;
			}
			m_pData = kZVD_NULLPTR(TElem);
			m_nCapacity = 0;
			return kZVD_R_OK;
		}

		ZVD_ASSERT_HIGH_NOMSG(nNewCap);
		size_type nReqBytes = sizeof(TElem) * nNewCap;
		void* pMem = pMemAlloc->allocate(nReqBytes);
		if (!pMem)
		{
			return kZVD_E_NOMEMORY;
		}

		TElem* pMemAsData = (TElem*)pMem;
		for (size_type i = 0; i < m_nCount; ++i)
		{
			TUtil::copy_construct(pMemAsData + i, m_pData[i]);
		}

		for (size_type i = 0; i < m_nCount; ++i)
		{
			TUtil::destroy(&m_pData[i]);
		}

		errVal = pMemAlloc->deallocate(m_pData);
		if (kZVD_R_OK != errVal)
		{
			return kZVD_E_ABORT;
		}
		
		m_pData = pMemAsData;
		m_nCapacity = nNewCap;
		return kZVD_R_OK;
	}

	ret_type resize_memory(size_type nNewCap)
	{
		ret_type retVal = kZVD_R_OK;

		zvd_imemalloc* pMemAlloc = kZVD_NULLPTR(zvd_imemalloc);
		zvd_byte allocStorage[sizeof(TMemAlloc)];
		retVal = m_get_mem_alloc(&pMemAlloc, kZVD_YES_U32, &allocStorage[0]);
		if (kZVD_R_OK != retVal)
		{
			return retVal;
		}
		ZVD_ASSERT_MEDIUM_NOMSG(pMemAlloc);

		if (m_pData)
		{
			retVal = m_resize_memory_if_data(nNewCap, pMemAlloc);
			return retVal;
		}

		if (nNewCap)
		{
			size_type nReqBytes = sizeof(TElem) * nNewCap;
			void* pMem = pMemAlloc->allocate(nReqBytes);
			if (!pMem)
			{
				return kZVD_E_NOMEMORY;
			}

			m_pData = (TElem*)pMem;
			m_nCapacity = nNewCap;
			return kZVD_R_OK;
		}
		return kZVD_E_UNACCEPTABLE;
	}
// Data members
private:
	TElem* m_pData;
	size_type m_nCount;
	size_type m_nCapacity;
#ifndef ZVD_USE_TDK_AS_LIB_ONLY
	TMemAlloc* m_pMemAlloc; // cached pointer to allocator
#endif
};


#endif // ZVD_DARRAY_H