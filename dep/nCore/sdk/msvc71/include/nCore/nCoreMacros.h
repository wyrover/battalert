//***************************************************************************
//
// [ nCore ]
//
// Copyright (c) 2001-2008 Jean-Charles Lefebvre <jcl ATNOSPAM jcl DOT name>
//
// This file is part of nCore.
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************

#ifndef __nCore_Macros_Header__
#define __nCore_Macros_Header__


//---------------------------------------------------------------------------
// TODO mark
//---------------------------------------------------------------------------
#define XTODO  const char TODO[-1] = "This section needs to be implemented !";  // this line will lead to a compile-time error


//---------------------------------------------------------------------------
// Memory Management
//---------------------------------------------------------------------------
#define XDELETE(obj)       do{ if((obj))   { delete   (obj);    (obj)=NULL; } }while(0)
#define XDELETEARRAY(arr)  do{ if((arr))   { delete[] (arr);    (arr)=NULL; } }while(0)
#define XFREE(block)       do{ if((block)) { free(block);     (block)=NULL; } }while(0)
#define XRELEASE(obj)      do{ if((obj))   { (obj)->Release();  (obj)=NULL; } }while(0)


//---------------------------------------------------------------------------
// Assertions
//---------------------------------------------------------------------------
// debug mode, normal assertions
#if defined(NCORE_DEBUG)

  #if defined(NCORE_COMPILER_MSVC)
    // <crtdbg.h> header is required for the _ASSERTE macro
    #if defined(NCORE_ENABLE_MULTITHREADING)
      #define XASSERT(exp)  do{ if(!(exp)) { _ASSERTE(exp); ::DebugBreak(); } }while(0)
    #else
      #define XASSERT       _ASSERTE
    #endif
  #else
    #define XASSERT         assert
  #endif

  #define XRELEASEASSERT  XASSERT

// non-debug mode, but we keep every assertions types (they are thrown as exceptions)
#elif defined(NCORE_ENABLE_RELEASE_ASSERT)
  #define XASSERT(exp)    do{ if(!(exp)) { throw ::nCore::Exception(__FILE__, __LINE__, NCORE_FUNCTION, ::nCore::EXCODE_ASSERT, #exp); } }while(0)
  #define XRELEASEASSERT  XASSERT

// pure release mode, only release assertions are thrown
#else
  #define XASSERT(exp)         ((void)0)
  #define XRELEASEASSERT(exp)  do{ if(!(exp)) { throw ::nCore::Exception(__FILE__, __LINE__, NCORE_FUNCTION, ::nCore::EXCODE_ASSERT, #exp); } }while(0)

#endif


//---------------------------------------------------------------------------
// Exceptions
//---------------------------------------------------------------------------
#define XTHROW                throw ::nCore::Exception(__FILE__, __LINE__, NCORE_FUNCTION, (uint)
#define XTHROWEXCODE(excode)  throw ::nCore::Exception(__FILE__, __LINE__, NCORE_FUNCTION, (uint)(excode), NULL)

#ifdef NCORE_DEBUG
  #define XTHROWSTR  throw (__FILE__ ": "
#else
  #define XTHROWSTR  throw (
#endif


//---------------------------------------------------------------------------
// Logger
//---------------------------------------------------------------------------
#define CORELOG  ::nCore::nCoreRoot::Log(__FILE__, __LINE__, NCORE_FUNCTION,


//---------------------------------------------------------------------------
// Debugger
//---------------------------------------------------------------------------
#if defined(NCORE_DEBUG) && defined(NCORE_PLATFORM_WINDOWS)
  #define XLAUNCHDEBUGGER  ::DebugBreak();
#else
  #define XLAUNCHDEBUGGER  ((void)0)
#endif


//---------------------------------------------------------------------------
// "Deprecated API" flag
//---------------------------------------------------------------------------
#if defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1300) // 7.0+
  #define NCORE_DEPRECATED       __declspec(deprecated)
  #define NCORE_DEPRECATED_CTOR  explicit NCORE_DEPRECATED
#elif defined(NCORE_COMPILER_GNU) && (NCORE_COMPILER_VERSION >= 30200) // 3.2+
  #define NCORE_DEPRECATED       __attribute__((deprecated))
  #define NCORE_DEPRECATED_CTOR  explicit NCORE_DEPRECATED
#else
  #define NCORE_DEPRECATED
  #define NCORE_DEPRECATED_CTOR
#endif


//---------------------------------------------------------------------------
// Miscellaneous
//---------------------------------------------------------------------------
#if defined(NCORE_PLATFORM_WINDOWS)
  #ifndef snprintf
    #define snprintf  _snprintf
  #endif
  #ifndef vsnprintf
    #define vsnprintf  _vsnprintf
  #endif
#elif defined(NCORE_PLATFORM_LINUX)
  #ifndef stricmp
    #define stricmp  strcasecmp
  #endif
#endif


// hack for <inttypes.h> replacement
//   TOFIX : make those macros *really* 64bit compliant at compile-time.
//   if sizeof(int)==sizeof(int64), those macros must reflect this.
#if defined(NCORE_PLATFORM_WINDOWS)
  #ifndef PRId64
    #define PRId64  "I64d"
  #endif
  #ifndef PRIi64
    #define PRIi64  "I64i"
  #endif
  #ifndef PRIu64
    #define PRIu64  "I64u"
  #endif
#else
  #ifndef PRId64
    #define PRId64  "lld"
  #endif
  #ifndef PRIi64
    #define PRIi64  "lli"
  #endif
  #ifndef PRIu64
    #define PRIu64  "llu"
  #endif
#endif


#endif // #ifndef __nCore_Macros_Header__
