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
Purpose: memory allocator intended for use in tests (definition).

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_TESTCONTAINERUTL_H
#define ZVD_TESTCONTAINERUTL_H


#include "common/zvdimemalloc.h"
#include "common/zvdmemutl.h"

namespace zvd
{
	namespace container_test
	{
		class testmemalloc : public zvd_imemalloc
		{
		public:
			typedef zvd_imemalloc base_class;
			typedef typename base_class::err_type err_type;

			virtual ~testmemalloc();

			virtual void* allocate(size_type nBytes, 
				err_type* pErrorCode = kZVD_NULLPTR(err_type));

			virtual void* reallocate(void* p, size_type nBytes, 
				err_type* pErrorCode = kZVD_NULLPTR(err_type));

			virtual err_type deallocate(void* p);

			static zvd_uint32 is_singleton();

			static zvd_uint32 is_subsystem();

			static zvd_uint32 can_be_created_on_stack();

			static err_type create_on_stack(void* pStackMem, zvd_imemalloc** ppMemAlloc);

		private:
	
		};

		class Dummy
		{
		public:
			Dummy(const char* name = "", zvd_uint32 year = 0);
			Dummy(const Dummy& oth);
			Dummy& operator=(const Dummy& oth);
			Dummy(Dummy&& oth);
			Dummy& operator=(Dummy&& oth);

			static void construct(void* p);
			static void copy_construct(void* p, const Dummy& val);
			const char* get_name() const { return m_name; }
			void set_name(const char* val);
			zvd_uint32 get_year() const { return m_year; }
			void set_year(zvd_uint32 val) { m_year = val; }
		private:
			char m_name[32];
			zvd_uint32 m_year;
		};
		
		
	} // eof container_test
} // eof zvd

#endif // ZVD_TESTCONTAINERUTL_H