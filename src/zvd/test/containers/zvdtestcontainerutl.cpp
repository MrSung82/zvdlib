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
Purpose: memory allocator intended for use in tests (implementation).

----------------------
 For developers notes
----------------------

*/

#include "zvdpch.h"
#include "test/containers/zvdtestcontainerutl.h"
#include "common/zvdstdlib.h"
#include "debug/zvdassert.h"

#include <new>
#include <unordered_map>
#include <string>
#include <cstring>

namespace zvd
{
	namespace container_test
	{

		Dummy::Dummy(const char* name, zvd_uint32 year)
			: m_year(year)
		{
			std::strcpy(m_name, name);
		}

		Dummy::Dummy(const Dummy& oth)
		{
			std::strcpy(m_name, oth.get_name());
			m_year = oth.get_year();
		}

		Dummy& Dummy::operator=(const Dummy& oth)
		{
			if (this != &oth)
			{
				std::strcpy(m_name, oth.get_name());
				m_year = oth.get_year();
			}
			return *this;
		}

		Dummy::Dummy(Dummy&& oth)
		{
			std::strcpy(m_name, oth.get_name());
			m_year = oth.get_year();
		}

		Dummy& Dummy::operator=(Dummy&& oth)
		{
			if (this != &oth)
			{
				std::strcpy(m_name, oth.get_name());
				m_year = oth.get_year();
			}
			return *this;
		}

		void Dummy::construct(void* p)
		{
			::new(p) Dummy();
		}

		void Dummy::copy_construct(void* p, const Dummy& val)
		{
			::new(p) Dummy(val);
		}
		
		void Dummy::set_name(const char* val)
		{
			std::strcpy(m_name, val);
		}

		namespace details
		{
			
			class MemBlockHeader
			{
			public:
				static const zvd_uint32 kMagic = 0xdead0bad;

				

				static zvd_uint32 allocsCount() { return m_allocsCount; }

				static MemBlockHeader* init(void* pMem, zvd_size nBytes);
				static MemBlockHeader* get_from(void* pMem);

				bool malformed() const { return kMagic != m_magic; }
				void* data_mem() { return m_pData; }
			private:
				static zvd_uint32 m_allocsCount;
				void* m_pData;
				zvd_size m_nBytes; // whole size of block in bytes
				zvd_uint32 m_magic;
			};

			const zvd_uint32 MemBlockHeader::kMagic;
			zvd_uint32 MemBlockHeader::m_allocsCount = 0;

			MemBlockHeader* MemBlockHeader::init(void* pMem, zvd_size nBytes)
			{
				MemBlockHeader* pResult = (MemBlockHeader*)pMem;
				pResult->m_pData = ((zvd_byte*)pMem) + sizeof(MemBlockHeader);
				pResult->m_nBytes = nBytes;
				pResult->m_magic = kMagic;
				++m_allocsCount;
				return pResult;
			}

			MemBlockHeader* MemBlockHeader::get_from(void* pMem)
			{
				zvd_byte* pMemBytes = (zvd_byte*)pMem;
				pMemBytes -= sizeof(MemBlockHeader);
				MemBlockHeader* pResult = (MemBlockHeader*)pMemBytes;
				return pResult;
			}

			typedef MemBlockHeader* PMemBlockHeader;
			static std::unordered_map<zvd_uint32, PMemBlockHeader> s_allocs;
			
		} // eof details

		testmemalloc::~testmemalloc()
		{

		}

		void* testmemalloc::allocate(size_type nBytes, err_type* pErrorCode)
		{
			using details::MemBlockHeader;
			using details::s_allocs;

			size_type nBlockBytes = nBytes + sizeof(MemBlockHeader);
			void* p = zvd_malloc(nBlockBytes);
			MemBlockHeader* pBlock = MemBlockHeader::init(p, nBlockBytes);
			s_allocs[MemBlockHeader::allocsCount()] = pBlock;
			return pBlock->data_mem();
		}

		void* testmemalloc::reallocate(void* p, size_type nBytes,
			err_type* pErrorCode)
		{
			return zvd_realloc(p, nBytes);
		}

		testmemalloc::err_type testmemalloc::deallocate(void* p)
		{
			using details::MemBlockHeader;
			using details::s_allocs;

			MemBlockHeader* pBlock = MemBlockHeader::get_from(p);
			zvd_free(pBlock);
			return kZVD_R_OK;
		}

		zvd_uint32 testmemalloc::is_singleton()
		{ 
			return kZVD_NO_U32;
		}

		zvd_uint32 testmemalloc::is_subsystem()
		{
			return kZVD_NO_U32;
		}

		zvd_uint32 testmemalloc::can_be_created_on_stack()
		{
			return kZVD_YES_U32;
		}

		testmemalloc::err_type testmemalloc::create_on_stack(void* pStackMem, zvd_imemalloc** ppMemAlloc)
		{
			ZVD_ASSERT_HIGH_NOMSG(ppMemAlloc);
			*ppMemAlloc = ::new(pStackMem)testmemalloc();
			return kZVD_R_OK;
		}
	} // eof container_test
} // eof zvd