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
Purpose: assert macros.

Usage example:

void test()
{
    std::vector<int> v;
    v.push_back(1961);

    ZVD_ASSERT_HIGH1(v.size() == 0, ZVD_DEBUG_TEXT("Неверно, на самом деле размер: %d"), v.size());
 }

----------------------
 For developers notes
----------------------

*/


#ifndef ZVD_ASSERT_H
#define ZVD_ASSERT_H

#include "common/zvdbasedefs.h"
#include "common/zvdstaticsingleton.h"
#include "common/zvdstringify.h"

#if defined(ZVD_COMPILER_MSVC) && defined(ZVD_ARCH_X64)
#include <intrin.h>
#endif

//-----------------------------------------------------------------------------
// Debug level values
#define ZVD_DEBUG_LEVEL_NONE         0   ///< Debugging is off (for production builds).
#define ZVD_DEBUG_LEVEL_LOW          1   ///< Low cost debugging mode (for release builds).
#define ZVD_DEBUG_LEVEL_MEDIUM       2   ///< Developing mode when most of code is well tested.
#define ZVD_DEBUG_LEVEL_HIGH         3   ///< Developing mode for intensive bug catching.
#define ZVD_DEBUG_LEVEL_EXTRA        4   ///< To catch very serious bugs.
#define ZVD_DEBUG_LEVEL_PARANOIC     5   ///< Paranoidal bug catching mode.

// Default debug level setting.
#ifndef ZVD_DEBUG_LEVEL
/// Debug level - one of (NONE, LOW, MEDIUM, HIGH, EXTRA, PARANOIC with "ZVD_DEBUG_LEVEL_" prefix).
#   define ZVD_DEBUG_LEVEL ZVD_DEBUG_LEVEL_NONE
#endif

//-----------------------------------------------------------------------------
// Break point
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_LOW
#   ifdef ZVD_COMPILER_MSVC
#       if defined(ZVD_ARCH_X64)
#           define ZVD_DEBUG_BP() __debugbreak();
#       else
#           define ZVD_DEBUG_BP() do { __asm int 3 } while (0)
#       endif

#   elif defined(ZVD_COMPILER_GCC)
#       define ZVD_DEBUG_BP() do {__asm__ ("int $3\n\t"); } while (0)

#   endif

#else
#   define ZVD_DEBUG_BP(skip) { do { ; } while (false); }

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_LOW


//-----------------------------------------------------------------------------
// char type for assert messages
#ifdef ZVD_DEBUG_CHARTYPE_WIDE
#   define ZVD_ASSERT_CHARTYPE wchar_t
#   define ZVD_ASSERT_TEXT(s) L ## s
#else
#   define ZVD_ASSERT_CHARTYPE char
#   define ZVD_ASSERT_TEXT(s) s
#endif

#define ZVD_DEBUG_TEXT(s) ZVD_ASSERT_TEXT(s)
#define ZVD_DEBUG_EMPTY_TEXT ZVD_DEBUG_TEXT("")

namespace zvd
{
    namespace debug
    {
        namespace details
        {

            // to exlude <new> I use this Pseudo constructor
            class assert_point;
            struct assert_point_ctor
            {
                assert_point* operator()(void* p);
            };

            //-----------------------------------------------------------------------------
            // Auxiliary class to fix file and line where assertion was failed.
            // (Don't use it directly!)
            class assert_point : public zvd_static_singleton<
                assert_point, sizeof(void*) * 4, assert_point_ctor>
            {
            public:
                typedef ZVD_ASSERT_CHARTYPE char_type;
                typedef const char_type* const_string_type;

                friend class zvd_static_singleton<assert_point,
                    sizeof(void*) * 4, assert_point_ctor>;
                friend class assert_point_ctor;

                void set(const_string_type expr, const_string_type fileAndLine)
                {
                    m_expr = expr;
                    m_fileAndLine = fileAndLine;
                }

                void clear() { set(ZVD_ASSERT_TEXT(""), ZVD_ASSERT_TEXT("")); }

                const_string_type expr() const { return m_expr; }
                const_string_type file_and_line() const { return m_fileAndLine; }

            private:
                assert_point()
                    : m_expr(ZVD_ASSERT_TEXT(""))
                    , m_fileAndLine(ZVD_ASSERT_TEXT("")) {}

                const_string_type m_expr;
                const_string_type m_fileAndLine;
            };

            //-----------------------------------------------------------------------------
            // Shows assert related info (Don't use it directly!).
            void output_assert_message(assert_point::const_string_type fmt, ...);

        } // end of details
    } // end of debug
} // end of zvd

//-----------------------------------------------------------------------------
// Fixing assert failed point (Don't use it directly!)

#define ZVD_FIX_ASSERT_POINT(exp) { if (!(exp)) { zvd::debug::details::assert_point::instance().set(ZVD_DEBUG_STRINGIFY(exp), ZVD_FILE_AND_LINE); } }

//
#   define ZVD_ASSERT_IMPL_NOMSG(exp)               { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(""                            ); ZVD_DEBUG_BP(); } }
#   define ZVD_ASSERT_IMPL(exp,fmt)                 { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(fmt                           ); ZVD_DEBUG_BP(); } }
#   define ZVD_ASSERT_IMPL1(exp,fmt,a1)             { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(fmt, (a1)                     ); ZVD_DEBUG_BP(); } }
#   define ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)          { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(fmt, (a1), (a2)               ); ZVD_DEBUG_BP(); } }
#   define ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)       { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(fmt, (a1), (a2), (a3)         ); ZVD_DEBUG_BP(); } }
#   define ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)    { if (!(exp)) { ZVD_FIX_ASSERT_POINT(exp); zvd::debug::details::output_assert_message(fmt, (a1), (a2), (a3), (a4)   ); ZVD_DEBUG_BP(); } }


//*****************************************************************************
// Assert macros for different debugging levels [--


//-----------------------------------------------------------------------------
// Dubug level: LOW
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_LOW // [LOW, PARANOIC]

#   define ZVD_ASSERT_LOW_NOMSG(exp)               ZVD_ASSERT_IMPL_NOMSG(exp)
#   define ZVD_ASSERT_LOW(exp,fmt)                 ZVD_ASSERT_IMPL(exp,fmt)
#   define ZVD_ASSERT_LOW1(exp,fmt,a1)             ZVD_ASSERT_IMPL1(exp,fmt,a1)
#   define ZVD_ASSERT_LOW2(exp,fmt,a1,a2)          ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)
#   define ZVD_ASSERT_LOW3(exp,fmt,a1,a2,a3)       ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)
#   define ZVD_ASSERT_LOW4(exp,fmt,a1,a2,a3,a4)    ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)

#else

#   define ZVD_ASSERT_LOW_NOMSG(exp)               ((void) 0)
#   define ZVD_ASSERT_LOW(exp,fmt)                 ((void) 0)
#   define ZVD_ASSERT_LOW1(exp,fmt,a1)             ((void) 0)
#   define ZVD_ASSERT_LOW2(exp,fmt,a1,a2)          ((void) 0)
#   define ZVD_ASSERT_LOW3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZVD_ASSERT_LOW4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_LOW

//-----------------------------------------------------------------------------
// Dubug level: MEDIUM
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_MEDIUM // [MEDIUM - PARANOIC]

#   define ZVD_ASSERT_MEDIUM_NOMSG(exp)               ZVD_ASSERT_IMPL_NOMSG(exp)
#   define ZVD_ASSERT_MEDIUM(exp,fmt)                 ZVD_ASSERT_IMPL(exp,fmt)
#   define ZVD_ASSERT_MEDIUM1(exp,fmt,a1)             ZVD_ASSERT_IMPL1(exp,fmt,a1)
#   define ZVD_ASSERT_MEDIUM2(exp,fmt,a1,a2)          ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)
#   define ZVD_ASSERT_MEDIUM3(exp,fmt,a1,a2,a3)       ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)
#   define ZVD_ASSERT_MEDIUM4(exp,fmt,a1,a2,a3,a4)    ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)

#else

#   define ZVD_ASSERT_MEDIUM_NOMSG(exp)               ((void) 0)
#   define ZVD_ASSERT_MEDIUM(exp,fmt)                 ((void) 0)
#   define ZVD_ASSERT_MEDIUM1(exp,fmt,a1)             ((void) 0)
#   define ZVD_ASSERT_MEDIUM2(exp,fmt,a1,a2)          ((void) 0)
#   define ZVD_ASSERT_MEDIUM3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZVD_ASSERT_MEDIUM4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_MEDIUM

//-----------------------------------------------------------------------------
// Dubug level: HIGH
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_HIGH // [HIGH - PARANOIC]

#   define ZVD_ASSERT_HIGH_NOMSG(exp)               ZVD_ASSERT_IMPL_NOMSG(exp)
#   define ZVD_ASSERT_HIGH(exp,fmt)                 ZVD_ASSERT_IMPL(exp,fmt)
#   define ZVD_ASSERT_HIGH1(exp,fmt,a1)             ZVD_ASSERT_IMPL1(exp,fmt,a1)
#   define ZVD_ASSERT_HIGH2(exp,fmt,a1,a2)          ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)
#   define ZVD_ASSERT_HIGH3(exp,fmt,a1,a2,a3)       ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)
#   define ZVD_ASSERT_HIGH4(exp,fmt,a1,a2,a3,a4)    ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)

#else

#   define ZVD_ASSERT_HIGH_NOMSG(exp)               ((void) 0)
#   define ZVD_ASSERT_HIGH(exp,fmt)                 ((void) 0)
#   define ZVD_ASSERT_HIGH1(exp,fmt,a1)             ((void) 0)
#   define ZVD_ASSERT_HIGH2(exp,fmt,a1,a2)          ((void) 0)
#   define ZVD_ASSERT_HIGH3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZVD_ASSERT_HIGH4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_HIGH

//-----------------------------------------------------------------------------
// Dubug level: EXTRA
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_EXTRA // [EXTRA - PARANOIC]

#   define ZVD_ASSERT_EXTRA_NOMSG(exp)               ZVD_ASSERT_IMPL_NOMSG(exp)
#   define ZVD_ASSERT_EXTRA(exp,fmt)                 ZVD_ASSERT_IMPL(exp,fmt)
#   define ZVD_ASSERT_EXTRA1(exp,fmt,a1)             ZVD_ASSERT_IMPL1(exp,fmt,a1)
#   define ZVD_ASSERT_EXTRA2(exp,fmt,a1,a2)          ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)
#   define ZVD_ASSERT_EXTRA3(exp,fmt,a1,a2,a3)       ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)
#   define ZVD_ASSERT_EXTRA4(exp,fmt,a1,a2,a3,a4)    ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)

#else

#   define ZVD_ASSERT_EXTRA_NOMSG(exp)               ((void) 0)
#   define ZVD_ASSERT_EXTRA(exp,fmt)                 ((void) 0)
#   define ZVD_ASSERT_EXTRA1(exp,fmt,a1)             ((void) 0)
#   define ZVD_ASSERT_EXTRA2(exp,fmt,a1,a2)          ((void) 0)
#   define ZVD_ASSERT_EXTRA3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZVD_ASSERT_EXTRA4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_EXTRA

//-----------------------------------------------------------------------------
// Dubug level: PARANOIC
#if ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_PARANOIC // [PARANOIC]

#   define ZVD_ASSERT_PARANOIC_NOMSG(exp)               ZVD_ASSERT_IMPL_NOMSG(exp)
#   define ZVD_ASSERT_PARANOIC(exp,fmt)                 ZVD_ASSERT_IMPL(exp,fmt)
#   define ZVD_ASSERT_PARANOIC1(exp,fmt,a1)             ZVD_ASSERT_IMPL1(exp,fmt,a1)
#   define ZVD_ASSERT_PARANOIC2(exp,fmt,a1,a2)          ZVD_ASSERT_IMPL2(exp,fmt,a1,a2)
#   define ZVD_ASSERT_PARANOIC3(exp,fmt,a1,a2,a3)       ZVD_ASSERT_IMPL3(exp,fmt,a1,a2,a3)
#   define ZVD_ASSERT_PARANOIC4(exp,fmt,a1,a2,a3,a4)    ZVD_ASSERT_IMPL4(exp,fmt,a1,a2,a3,a4)

#else

#   define ZVD_ASSERT_PARANOIC_NOMSG(exp)               ((void) 0)
#   define ZVD_ASSERT_PARANOIC(exp,fmt)                 ((void) 0)
#   define ZVD_ASSERT_PARANOIC1(exp,fmt,a1)             ((void) 0)
#   define ZVD_ASSERT_PARANOIC2(exp,fmt,a1,a2)          ((void) 0)
#   define ZVD_ASSERT_PARANOIC3(exp,fmt,a1,a2,a3)       ((void) 0)
#   define ZVD_ASSERT_PARANOIC4(exp,fmt,a1,a2,a3,a4)    ((void) 0)

#endif // ZVD_DEBUG_LEVEL >= ZVD_DEBUG_LEVEL_PARANOIC

// -- ] Assert macros for different debugging levels



#endif // ZVD_ASSERT_H