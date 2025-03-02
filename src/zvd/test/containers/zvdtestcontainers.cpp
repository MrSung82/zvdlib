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
Purpose: container test suites (implementation).

----------------------
 For developers notes
----------------------

*/

#include "zvdpch.h"
#include "test/containers/zvdtestcontainerutl.h"
#include "common/zvdstdlib.h"
#include "debug/zvdassert.h"
#include "common/zvdmemutl.h"
#include "common/zvdmemutlnew.h"

#include <new>
#include <unordered_map>
#include <string>
#include <cstring>

#include <gtest/gtest.h>
#include <gmock/gmock.h>



// Tests factorial of 0.
TEST(zvd_darray, push_back_size) {
	typedef zvd::container_test::Dummy DummyType;

	typedef zvd_darray<DummyType,
		zvd::container_test::testmemalloc,
		zvd_new_based_memutil<DummyType, 4>
	> DummiesArrayType;

	typedef DummiesArrayType::size_type SizeType;

	DummiesArrayType dummies_array;
	SizeType nInitSize = dummies_array.size();

	dummies_array.push_back(zvd::container_test::Dummy("Tolstoy", 1828));
	SizeType nSizeAfterPush1 = dummies_array.size();

	ASSERT_TRUE(nInitSize == 0);
	ASSERT_TRUE(nSizeAfterPush1 == 1);
}