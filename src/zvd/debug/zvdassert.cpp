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
Purpose: assert implementation.

----------------------
 For developers notes
----------------------

*/

#include "zvdpch.h"
#include <assert.h>
#include <stdio.h>

#ifndef ZVD_USE_TEMPLATE_ARGS_FOR_PRINT
#   include <cstdarg>
#else
#   error "Feature is not implemented yet"
#endif

#include "debug/zvdassert.h"

#ifdef ZVD_USE_MSGBOX_FOR_ASSERT
#   ifndef ZVD_OS_WIN
#	   error "message box for assert messaging is unsupported on nowindows platforms"
#   endif
#   include <windows.h>
#endif // ZVD_USE_MSGBOX_FOR_ASSERT

#include <new>
namespace zvd
{
	namespace debug
	{
		namespace details
		{
			//-----------------------------------------------------------------------------
			// Implementation of assert point ctor.
			assert_point* assert_point_ctor::operator()(void* p)
			{
				return ::new(p) assert_point();
			}

			template<typename T, zvd_uint32 KSize>
			class debug_string
			{
			public:
				typedef zvd_uint32 size_type;
				debug_string() : m_len(0)
				{
					m_buffer[0] = 0;
				}

				size_type raw_length(const T* rawStr)
				{
					size_type result = 0;
					if (!rawStr)
						return 0;
					while (*rawStr++)
					{
						++result;
						if (result >= space_left())
							return space_left();
					}
					return result;
				}

				void append(const T* rawStr)
				{
					size_type nToCopy = raw_length(rawStr);
					for (size_type i = 0; i < nToCopy; ++i)
					{
						m_buffer[m_len + i] = rawStr[i];
					}

					m_len += nToCopy;
					m_buffer[m_len] = 0;
				}

				debug_string<T, KSize>& operator +=(const T* rawStr)
				{
					append(rawStr);
					return *this;
				}

				const T* c_str()
				{
					return &m_buffer[0];
				}

			private:
				size_type space_left()
				{
					return KSize - (m_len + 1);
				}

				T m_buffer[KSize];
				size_type m_len;
			};

			//-----------------------------------------------------------------------------
			// Implementation of output assert message function.
#ifndef ZVD_USE_TEMPLATE_ARGS_FOR_PRINT
			void output_assert_message(assert_point::const_string_type fmt, ...)
			{
				const zvd_uint32 kFmtStaticBufSize = 256;
				typedef assert_point::char_type char_type;
				typedef debug_string<assert_point::char_type, 1024 + kFmtStaticBufSize> string_type;

				if (!assert_point::instance().expr())
				{
					return;
				}

				string_type strMsg;
				strMsg += ZVD_ASSERT_TEXT("[ASSERT]: \"");
				strMsg += assert_point::instance().expr();
				strMsg += ZVD_ASSERT_TEXT("\" failed in \"");
				strMsg += assert_point::instance().file_and_line();
				strMsg += ZVD_ASSERT_TEXT(".\n");
				assert_point::instance().clear();

				if (fmt)
				{
					va_list args;
					va_start(args, fmt);

					// evaluating buffer size for "details"
					va_list args2;
					va_copy(args2, args);
#ifdef ZVD_DEBUG_CHARTYPE_WIDE
					int nBufSize = _vscwprintf(fmt, args2) + 1;
#else
					int nBufSize = _vscprintf(fmt, args2) + 1;
#endif
					va_end(args2);

					
					char_type staticBuf[kFmtStaticBufSize] = {};
					char_type* pFormattedText = staticBuf;
					bool fStaticBuf = true;
					if (nBufSize > kFmtStaticBufSize)
					{
						pFormattedText = new char_type[nBufSize];
						fStaticBuf = false;
					}
#ifdef ZVD_DEBUG_CHARTYPE_WIDE
#	ifdef ZVD_USE_SAFE_VSNPRINTF
					nBufSize = _vsnwprintf_s(pFormattedText, nBufSize, nBufSize, fmt, args);
#	else
					nBufSize = _vsnwprintf(pFormattedText, nBufSize, fmt, args);
#endif
#else
#	ifdef ZVD_USE_SAFE_VSNPRINTF
					nBufSize = _vsnprintf_s(pFormattedText, nBufSize, nBufSize, fmt, args);
#	else
					nBufSize = _vsnprintf(pFormattedText, nBufSize, fmt, args);
#	endif
#endif
					va_end(args);

					if (nBufSize == -1)
					{
						if (!fStaticBuf)
							delete[] pFormattedText;
						return;
					}

					strMsg += ZVD_ASSERT_TEXT("Details: ");
					strMsg += pFormattedText;
					strMsg += ZVD_ASSERT_TEXT("\n");
					if (!fStaticBuf)
						delete[] pFormattedText;
				}

#ifdef ZVD_USE_MSGBOX_FOR_ASSERT
#   ifdef ZVD_DEBUG_CHARTYPE_WIDE
				::MessageBoxW(0, strMsg.c_str(), ZVD_ASSERT_TEXT("Assert failed!"),
					MB_ICONERROR | MB_OK | MB_TOPMOST);
#   else
				::MessageBoxA(0, strMsg.c_str(), ZVD_ASSERT_TEXT("Assert failed!"),
					MB_ICONERROR | MB_OK | MB_TOPMOST);
#   endif
#else
#   ifdef ZVD_DEBUG_CHARTYPE_WIDE
				wprintf(strMsg.c_str());
#   else
				printf(strMsg.c_str());
#   endif
#endif // ZVD_USE_MSGBOX_FOR_ASSERT

			}
#endif // ZVD_USE_TEMPLATE_ARGS_FOR_PRINT
		} // end of details
	} // end of debug
} // end of zvd
