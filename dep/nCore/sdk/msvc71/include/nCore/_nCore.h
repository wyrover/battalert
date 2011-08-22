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

#ifndef __nCore_Package_Header__
#define __nCore_Package_Header__
#include "MemoryManagerMacrosUndef.h" // just in case...


//---------------------------------------------------------------------------
// Configuration Headers
//---------------------------------------------------------------------------
#include "nCorePlatform.h"
#include "nCoreManualConfig.h"

// get rid of those "security" warnings from msvc8+
// you can comment this line safely if you want to enable this msvc8+ "feature"...
#if defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1400) && !defined(_CRT_SECURE_NO_DEPRECATE)
  #define _CRT_SECURE_NO_DEPRECATE
#endif


//---------------------------------------------------------------------------
// Compile-Time check
//---------------------------------------------------------------------------
// define our compile-time assert() macro
#define NCORE_COMPILE_TIME_ASSERT(test)          typedef char CoreCompileTimeAssert_ ## __LINE__[(test) * 2 - 1]
#ifdef NCORE_DEBUG
  #define NCORE_COMPILE_TIME_ASSERT_DEBUG        NCORE_COMPILE_TIME_ASSERT
#else
  #define NCORE_COMPILE_TIME_ASSERT_DEBUG(test)  (void)
#endif

// check base types (maniac mode)
// note : wchar_t and off_t types are tested in the "Standard Headers"
// section (later in this header)
NCORE_COMPILE_TIME_ASSERT(sizeof(char)   == 1);
NCORE_COMPILE_TIME_ASSERT(sizeof(float)  == 4);
NCORE_COMPILE_TIME_ASSERT(sizeof(double) == 8);
NCORE_COMPILE_TIME_ASSERT(sizeof(nCore::int8)  == 1);
NCORE_COMPILE_TIME_ASSERT(sizeof(nCore::int16) == 2);
NCORE_COMPILE_TIME_ASSERT(sizeof(nCore::int32) == 4);
NCORE_COMPILE_TIME_ASSERT(sizeof(nCore::int64) == 8);



//---------------------------------------------------------------------------
// Standard Headers
//---------------------------------------------------------------------------
#if !defined(NCORE_PLATFORM_WINDOWS)
  #define _FILE_OFFSET_BITS  64  // this will make sure the off_t type to be 64 bits instead of 32 (see the File class)
#endif

// standard c headers
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <cstring>
#include <ctime>

// standard c++ headers
#include <algorithm>
#include <functional>
#include <limits>
#include <locale>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

// standard c++ headers - hash_map
// * hash_map has to be handled carefully because it does not exists in the
//   "original" stl implementation though it can be found in stlport and
//   sgi's implementation (note that gcc use the sgi's one).
#if defined(NCORE_COMPILER_GNU) && !defined(STLPORT)
  #include <ext/hash_map>
  #include <ext/hash_set>
#else
  #include <hash_map>
  #include <hash_set>
#endif

// standard c++ headers (streams)
#include <iostream>
#include <sstream>


// check the wchar_t base type
#ifdef wchar_t
  NCORE_COMPILE_TIME_ASSERT(wchar_t, (sizeof(wchar_t) == 2));
#endif

// * be sure the off_t defined in the stdio.h header is 64 bits long
//   this type is used by the File class on Unix platforms
// * if you reach this assert, please insert the following define into your
//   top header file or in your makefile so the standard c headers can
//   intercept it :
//   #define _FILE_OFFSET_BITS  64
#ifndef NCORE_PLATFORM_WINDOWS
  NCORE_COMPILE_TIME_ASSERT(sizeof(off_t) == 8);
#endif


//---------------------------------------------------------------------------
// Windows Platform Headers
//---------------------------------------------------------------------------
#ifdef NCORE_PLATFORM_WINDOWS

  // include windows.h only if user is not using mfc
  // windows.h header cannot be included when using mfc...
  #if !defined(NCORE_CLIENT_WANTS_MFC) && !defined(__AFX_H__) && !defined(__AFXWIN_H__)
    #ifndef STRICT
      #define STRICT
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
      #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
      #define NOMINMAX
    #endif

    #if defined(NCORE_ENABLE_NETWORKING)
      #include <winsock2.h>
      #include <ws2tcpip.h>

      #if defined(NCORE_COMPILER_MSVC) && ( defined(NCORE_DLL_BUILD) || !defined(NCORE_DLL_CLIENT_BUILD) )
        // link to the lib only if we are building the dll or if we are
        // building the static client application
        #pragma comment(lib, "ws2_32.lib")
      #endif
    #endif

    #include <windows.h>
    #include <io.h>
    #include <sys/types.h>
    #include <sys/timeb.h>
    #include <sys/stat.h>

    // undef some STUPID defines from the windows platform sdk
    #ifdef Yield
      #undef Yield // Yield macro comes from <winbase.h>, it collides with the Thread::Yield() method name
    #endif
    #ifdef max
      #undef max
    #endif
    #ifdef min
      #undef max
    #endif
  #endif

  #if defined(NCORE_DEBUG) && defined(NCORE_COMPILER_MSVC)
    #include <crtdbg.h> // see the XASSERT macro define from the "nCoreMacros.h" header
  #endif

#endif


//---------------------------------------------------------------------------
// Linux Platform Headers
//---------------------------------------------------------------------------
#if defined(NCORE_PLATFORM_LINUX)
  #include <wchar.h>
  #include <unistd.h>
  #include <sys/time.h>

  #if defined(NCORE_ENABLE_NETWORKING)
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <netdb.h>
  #endif
#endif


//---------------------------------------------------------------------------
// NCORE_EXPORT
//---------------------------------------------------------------------------
#if defined(NCORE_PLATFORM_WINDOWS)
  #if defined(NCORE_COMPILER_MSVC) || defined(NCORE_COMPILER_GNU)
    #if defined(NCORE_DLL_BUILD)
      #define NCORE_EXPORT  __declspec(dllexport)
    #elif defined(NCORE_DLL_CLIENT_BUILD)
      #define NCORE_EXPORT  __declspec(dllimport)
    #else
      #define NCORE_EXPORT
    #endif
  #else
    #error Compiler not supported yet !
  #endif
#else
  #define NCORE_EXPORT
#endif


//---------------------------------------------------------------------------
// Memory Management
//
// The internal memory manager is enabled only if the
// NCORE_ENABLE_INTERNAL_MEMORY_MANAGER flag is defined.
//
// WARNING
// * Must be included _after_ standard headers and _before_ local headers.
// * If you need to define additionnal external headers (ie: like third party
//   headers or standard headers) in your application, please follow the
//   following steps :
//   1. Include the <nCore/MemoryManagerMacrosUndef.h> header.
//   2. Include the external headers you want.
//   3. Include the <nCore/MemoryManagerMacros.h> header.
//---------------------------------------------------------------------------
#include "MemoryManager.h"


//---------------------------------------------------------------------------
// Project Headers
//---------------------------------------------------------------------------
#ifdef NCORE_COMPILER_MSVC
#pragma warning(disable : 4251) // non dll-interface class used as base for dll-interface class
#pragma warning(disable : 4275) // exported class X derivate from a non exported class Y
#endif

// top headers
#include "nCoreMacros.h"
#include "nCoreRawTypes.h"

// common part headers
// * note to nCore dev : everything is untidy here because of
//   inter-dependencies, but please try to respect some order in this
//   little chaos though :)  - jcl
#ifdef NCORE_ENABLE_NETWORKING
  #include "NetTypes.h"
#endif
#include "NonCopyable.h"
#include "HashMap.h"
#include "StlHelpers.h"
#include "StringA.h"
#include "ExceptionCode.h"
#include "Exception.h"
#include "Singleton.h"
#include "Timer.h"
#include "Version3.h"
#ifdef NCORE_ENABLE_MULTITHREADING
  #include "AtomicCounter.h"
#endif
#include "SmartPointer.h"
#include "SmartPointerTypes.h"
#include "System.h"
#include "FileSystem.h"

// logger
#include "Logger.h"
#include "LoggerNull.h"
#ifdef NCORE_ENABLE_LOGGEREX
  #include "LoggerEx.h"
#endif

// threading module
#ifdef NCORE_ENABLE_MULTITHREADING
  #include "ThreadMethod.h"
  #include "ThreadMutex.h"
  #include "ThreadMutexGuard.h"
  #include "Thread.h"
  #include "AtomicCounter.h"
#endif

// networking module
#ifdef NCORE_ENABLE_NETWORKING
  #include "NetTypes.h"
  #include "NetAddress.h"
  #include "NetSocket.h"
  #include "NetPollSet.h"
#endif

// networking utils
#if defined(NCORE_ENABLE_NETWORKING) && defined(NCORE_ENABLE_NETWORKING_SMTP_SENDER)
  #include "NetSmtpSender.h"
#endif

// util - stringbase class
#ifdef NCORE_ENABLE_UTIL_STRINGBASE
  #include "StringBase.h"
#endif

// util - file io
#ifdef NCORE_ENABLE_FILEIO
  #include "File.h"
  #include "FileDisk.h"
  #include "FileMem.h"
  #include "Streamer.h"
  #include "StreamerDisk.h"
  #include "StreamerMem.h"
#endif

// util - dogtags
// * for convenience, DogTags macros must not be inside the
//   "#ifdef NCORE_ENABLE_UTIL_DOGTAGS" block because the client application
//   will sometimes only want to define its dogtags but not to read/write
//   them.
#include "DogTagsMacros.h"
#ifdef NCORE_ENABLE_UTIL_DOGTAGS
  #include "DogTags.h"
#endif

// util - dynamic library/object loader
#ifdef NCORE_ENABLE_UTIL_DYNAMIC_LIBRARY
  #include "DynamicLibrary.h"
#endif

// util - build date
#ifdef NCORE_ENABLE_UTIL_BUILDDATE
  #include "BuildDate.h"
#endif

// util - build date & time
#ifdef NCORE_ENABLE_UTIL_BUILDDATETIME
  #include "BuildDateTime.h"
#endif

// util - crc8
#ifdef NCORE_ENABLE_UTIL_CRC8
  #include "Crc8.h"
#endif

// util - crc16
#ifdef NCORE_ENABLE_UTIL_CRC16
  #include "Crc16.h"
#endif

// util - crc32
#ifdef NCORE_ENABLE_UTIL_CRC32
  #include "Crc32.h"
#endif

// util - config file
#ifdef NCORE_ENABLE_UTIL_CONFIG_FILE
  #include "ConfigFile.h"
#endif

// util - file iterator
#ifdef NCORE_ENABLE_UTIL_FILE_ITERATOR
  #include "FileIterator.h"
#endif

// util - guid
#ifdef NCORE_ENABLE_UTIL_GUID
  #include "Guid.h"
#endif

// util - md5
#ifdef NCORE_ENABLE_UTIL_MD5
  #include "Md5.h"
#endif

// util - sha1
#ifdef NCORE_ENABLE_UTIL_SHA1
  #include "Sha1.h"
#endif

// util - version2
#ifdef NCORE_ENABLE_UTIL_VERSION2
  #include "Version2.h"
#endif


// library's root class
#include "nCoreRoot.h"


#ifdef NCORE_COMPILER_MSVC
#pragma warning(default : 4251) // class X needs a DLL interface to be used by Y class
#pragma warning(default : 4275) // exported class X derivate from a non exported class Y
#endif


#endif // #ifndef __nCore_Package_Header__
