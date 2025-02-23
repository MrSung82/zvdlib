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
Purpose: stringification macros.

----------------------
 For developers notes
----------------------

*/

#ifndef ZVD_STRINGIFY_H
#define ZVD_STRINGIFY_H

//-----------------------------------------------------------------------------
// Widen predefined macro and stringification support
#define ZVD_WIDECHAR(x)          ZVD_WIDECHAR_(x)
#define ZVD_WIDECHAR_(x)         L ## x
#define ZVD_NARROWCHAR(x)        x

#define ZVD_STRINGIFY_IMPL(x, t)     ZVD_STRINGIFY_IMPL_(x, t)
#define ZVD_STRINGIFY_IMPL_(x, t)    t(#x)

#define ZVD_FILE_AND_LINE_(t)  t(__FILE__) t("[") ZVD_STRINGIFY_IMPL(__LINE__, t) t("]")

#ifdef ZVD_DEBUG_CHARTYPE_WIDE
#   define ZVD_FILE_AND_LINE       ZVD_FILE_AND_LINE_(ZVD_WIDECHAR)
#   define ZVD_DEBUG_STRINGIFY(x)  ZVD_STRINGIFY_IMPL(x,ZVD_WIDECHAR)
#else
#   define ZVD_FILE_AND_LINE       ZVD_FILE_AND_LINE_(ZVD_NARROWCHAR)
#   define ZVD_DEBUG_STRINGIFY(x)  ZVD_STRINGIFY_IMPL(x,ZVD_NARROWCHAR)
#endif

#define ZVD_STRINGIFY(x)  ZVD_STRINGIFY_IMPL(x,ZVD_NARROWCHAR)
#define ZVD_STRINGIFY_W(x)  ZVD_STRINGIFY_IMPL(x,ZVD_WIDECHAR)
#endif // ZVD_STRINGIFY_H