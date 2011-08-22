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

#ifndef __nCore_Platform_Header__
#define __nCore_Platform_Header__


//---------------------------------------------------------------------------
// Platform
//
// TODO : Handle 64bits platforms _well_ !
//---------------------------------------------------------------------------
// Windows (64 bits)
#if defined(_WIN64)
  #define NCORE_PLATFORM_WIN64    1
  #define NCORE_PLATFORM_WINDOWS  1
  #define NCORE_PLATFORM_NAME     "Win64"

// Windows CE (32 bits)  -  NOT FULLY SUPPORTED !!!
#elif defined(_WIN32_WCE)
  #define NCORE_PLATFORM_WIN32      1
  #define NCORE_PLATFORM_WINDOWSCE  1
  #define NCORE_PLATFORM_NAME       "WinCE-32"

// Windows CE (64 bits)  -  NOT FULLY SUPPORTED !!!
#elif defined(_WIN64_WCE)
  #define NCORE_PLATFORM_WIN64      1
  #define NCORE_PLATFORM_WINDOWSCE  1
  #define NCORE_PLATFORM_NAME       "WinCE-64"

// Windows (32 bits)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #define NCORE_PLATFORM_WIN32    1
  #define NCORE_PLATFORM_WINDOWS  1
  #define NCORE_PLATFORM_NAME     "Win32"

// Cygwin
#elif defined(__CYGWIN__)
  #define NCORE_PLATFORM_CYGWIN   1
  #define NCORE_PLATFORM_LINUX    1
  #define NCORE_PLATFORM_NAME     "Cygwin"

// MinGW
#elif defined(__MINGW32__)
  #define NCORE_PLATFORM_MINGW    1
  #define NCORE_PLATFORM_LINUX    1
  #define NCORE_PLATFORM_NAME     "MinGW"
  #error Support of this platform needs to be tested ! Uncomment this line to continue :)

// Linux
#elif defined(linux) || defined(__linux) || defined(__linux__)
  #define NCORE_PLATFORM_LINUX    1
  #define NCORE_PLATFORM_NAME     "Linux"

// MacOS X  -  NOT FULLY SUPPORTED !!!
//#elif defined(__MACOSX__) || (defined(__MACH__) && defined(__APPLE__)) // don't know if this test is sure
//  #define NCORE_PLATFORM_MACOSX   1
//  #define NCORE_PLATFORM_NAME     "MacOSX"
//  #error Platform not supported yet !

// Unsupported platform
#else
  #error Platform not supported yet !
#endif


//---------------------------------------------------------------------------
// Architecture
//---------------------------------------------------------------------------
// ARM
#if defined(__arm__)
  #define NCORE_ARCH_ARM    1
  #define NCORE_ARCH_NAME   "ARM"
  #define NCORE_ARCH_32BIT  1

// PowerPC-64 (not supported)
//#elif defined(__ppc64__) || defined(__powerpc64__) || defined (__powerpc64)
//  #define NCORE_ARCH_PPC64  1
//  #define NCORE_ARCH_NAME   "PowerPC-64"
//  #define NCORE_ARCH_64BIT  1 // TODO : Be sure about that.

// PowerPC-32 (not supported)
//#elif defined(_ARCH_PPC) || defined(__ppc__) || defined(__PPC) || defined(powerpc) || defined(__PPC__)
//  #define NCORE_ARCH_PPC32  1
//  #define NCORE_ARCH_NAME   "PowerPC-32"
//  #define NCORE_ARCH_32BIT  1 // TODO : Be sure about that.

// x64 / AMD64 (not supported)
//#elif defined(__x86_64__) || defined(__x86_64) || defined(__amd64) || defined(__amd64__) || defined(_AMD64_) || defined(_M_X64)
//  #define NCORE_ARCH_X64    1
//  #define NCORE_ARCH_NAME   "x64/AMD64"
//  #define NCORE_ARCH_64BIT  1

// IA64 (not supported)
//#elif defined(__ia64__) || defined(_IA64) || defined(__ia64) || defined(_M_IA64)
//  #define NCORE_ARCH_IA64   1
//  #define NCORE_ARCH_NAME   "IA64"
//  #define NCORE_ARCH_64BIT  ??? // TODO : Duh ?!

// x86
#elif defined(__i386__) || defined(__i386) || defined(i386) || defined(_X86_) || defined(_M_IX86)
  #define NCORE_ARCH_X86    1
  #define NCORE_ARCH_NAME   "x86"
  #define NCORE_ARCH_32BIT  1

#else
#error Unsupported platform !
#endif




//---------------------------------------------------------------------------
// Architecture Endianness
//---------------------------------------------------------------------------
#if defined(__sparc) || defined(__sparc__) || defined(__powerpc__) || \
    defined(__ppc__) || defined(__hppa) || defined(_MIPSEB) || \
    defined(_POWER) || defined(_M_PPC) || defined(_M_MPPC) || \
    defined(_M_MRX000) || defined(__POWERPC) || defined(m68k) || \
    defined(powerpc) || defined(sel) || defined(pyr) || defined(mc68000) || \
    defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
    defined(MIPSEB) || defined(__convex__) || defined(DGUX) || \
    defined(hppa) || defined(apollo) || defined(_CRAY) || \
    defined(__hp9000) || defined(__hp9000s300) || defined(_AIX) || \
    defined(__AIX) || defined(__pyr__) || defined(hp9000s700) || defined(_IBMR2)

#define NCORE_ARCH_BIGENDIAN  1

#elif defined(__i386__) || defined(i386) || defined(intel) || defined(_M_IX86) || \
  defined(_M_X64) || defined(__amd64) || defined(__amd64__)	|| \
  defined(__alpha__) || defined(__alpha) || defined(__ia64) || defined(__ia64__) || \
	defined(_M_ALPHA) || defined(ns32000) || defined(__ns32000__) || \
  defined(sequent) || defined(__mips__) || defined(MIPSEL) || defined(_MIPSEL) || \
  defined(sun386) || defined(__sun386__) || defined(__arm__)

#define NCORE_ARCH_LITTLEENDIAN  1

#else
#error Unable to guess endianness target architecture !
#endif


//---------------------------------------------------------------------------
// Compiler name & version
// Do not forget that a compiler can be cross-platform :)
//---------------------------------------------------------------------------
// Microsoft Visual C++
//
// Known _MSC_VER values :
// * C Compiler 6.0          600
// * C/C++ Compiler 7.0      700
// * Visual C++ 1.0          800
// * Visual C++ 2.0          900
// * Visual C++ 2.x          900
// * Visual C++ 4.0          1000
// * Visual C++ 5.0          1100
// * Visual C++ 6.0          1200
// * Visual C++ 7.0          1300
// * Visual C++ 7.1 (2003)   1310
// * Visual C++ 8.0 (2005)   1400
#if defined(_MSC_VER)
  #define NCORE_COMPILER_MSVC
  #define NCORE_COMPILER_NAME       "Microsoft"
  #define NCORE_COMPILER_VERSION    _MSC_VER
  #define NCORE_FUNCTION            __FUNCTION__
  #define NCORE_FORCEINLINE         __forceinline

// GNU
#elif defined(__GNUC__) || defined(__GNUG__)
  #define NCORE_COMPILER_GNU
  #define NCORE_COMPILER_NAME       "GNU"

  // single version macro, format will be VVRRPP (Version, Revision, Patch)
  #ifdef __GNUC_PATCHLEVEL__
    #define NCORE_COMPILER_VERSION  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
  #else
    #define NCORE_COMPILER_VERSION  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
  #endif

  #define NCORE_FUNCTION            __func__  // __PRETTY_FUNCTION__
  #define NCORE_FORCEINLINE         inline

// Intel C Compiler (not supported)
//#elif defined(__INTEL_COMPILER)
//  #define NCORE_COMPILER_INTEL
//  #define NCORE_COMPILER_NAME       "Intel"
//  #define NCORE_COMPILER_VERSION    __INTEL_COMPILER
//  #define NCORE_FUNCTION            "?"
//  #define NCORE_FORCEINLINE         inline

// Borland Compiler (not supported)
//#elif defined(__BORLANDC__)
//  #define NCORE_COMPILER_BORLAND
//  #define NCORE_COMPILER_NAME       "Borland"
//  #define NCORE_COMPILER_VERSION    __BORLANDC__
//  #define NCORE_FUNCTION            __FUNC__
//  #define NCORE_FORCEINLINE         inline

// Unknown
#else
  #define NCORE_COMPILER_UNKNOWN
  #define NCORE_COMPILER_NAME       "Unknown"
  #define NCORE_COMPILER_VERSION    0
  #define NCORE_FUNCTION            "?"
  #define NCORE_FORCEINLINE         inline
  #error Compiler not supported !

#endif


//---------------------------------------------------------------------------
// Standard Types
//
// TODO : Handle 64bits platforms according to the "Platform" section.
//---------------------------------------------------------------------------
namespace nCore
{
  typedef unsigned char  uchar;

#ifdef NCORE_PLATFORM_WINDOWS
  typedef unsigned short ushort;
  typedef unsigned int   uint;
  typedef unsigned long  ulong;
#endif

  typedef signed   char  int8;
  typedef signed   short int16;
  typedef signed   int   int32;
  typedef unsigned char  uint8;
  typedef unsigned short uint16;
  typedef unsigned int   uint32;

#ifdef NCORE_COMPILER_MSVC
  typedef signed   __int64 int64;
  typedef unsigned __int64 uint64;
#else
  typedef signed   long long int64;
  typedef unsigned long long uint64;
#endif

  typedef uint8  byte;
  typedef uint8  ubyte;
  typedef int8   sbyte;
  typedef uint16 word;
  typedef uint16 uword;
  typedef int16  sword;
  typedef uint32 dword;
  typedef uint32 udword;
  typedef int32  sdword;
  typedef uint64 qword;
  typedef uint64 uqword;
  typedef int64  sqword;
}


#endif // #ifndef __nCore_Platform_Header__
