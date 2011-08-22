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

#ifndef __nCore_SystemTools_Header__
#define __nCore_SystemTools_Header__
namespace nCore {


//---------------------------------------------------------------------------
// System
//
// A bunch of system tools.
//
// This class is designed to be a little abstraction layer between the OS
// and our code.
//---------------------------------------------------------------------------
class NCORE_EXPORT System
{
public :
  enum Version
  {
    UNKNOWN = 0,

    OSTYPE_LINUX      = 0x0100,
    OSTYPE_MACOS      = 0x0200,
    OSTYPE_WINDOWS    = 0x0300,
    OSTYPE_WINDOWS_NT = 0x0400,
    OSTYPE__MASK      = 0xFF00, // used internally

    OSVER_WINDOWS_VANILLA = OSTYPE_WINDOWS +  0, // unknown version of windows (should never be returned)
    OSVER_WINDOWS_95      = OSTYPE_WINDOWS +  5,
    OSVER_WINDOWS_98      = OSTYPE_WINDOWS + 10,
    OSVER_WINDOWS_ME      = OSTYPE_WINDOWS + 15,

    OSVER_WINDOWS_NT_VANILLA = OSTYPE_WINDOWS_NT +  0, // unknown version of windows nt (should never be returned)
    OSVER_WINDOWS_NT4        = OSTYPE_WINDOWS_NT +  5,
    OSVER_WINDOWS_2000       = OSTYPE_WINDOWS_NT + 10,
    OSVER_WINDOWS_XP         = OSTYPE_WINDOWS_NT + 15,
    OSVER_WINDOWS_2003       = OSTYPE_WINDOWS_NT + 20,
    OSVER_WINDOWS_VISTA      = OSTYPE_WINDOWS_NT + 25,

    OSVER_LINUX_VANILLA = OSTYPE_LINUX + 0,
    OSVER_CYGWIN        = OSTYPE_LINUX + 1,
    OSVER_MINGW         = OSTYPE_LINUX + 2,

    OSVER_MACOS_VANILLA = OSTYPE_LINUX + 0,
    OSVER_MACOSX        = OSTYPE_LINUX + 10,
  };

  enum CpuCaps
  {
    CPUCAP_NONE = 0,

    // x86 / x64
#if defined(NCORE_ARCH_X86) || defined(NCORE_ARCH_X64)
    // arch
    CPUCAP_FPU      = (1 <<  0),
    CPUCAP_PRO      = (1 <<  1),
    CPUCAP_HTT      = (1 <<  2),
    CPUCAP_MMX      = (1 <<  3),
    CPUCAP_MMXEXT   = (1 <<  4),
    CPUCAP_SSE      = (1 <<  5),
    CPUCAP_SSE2     = (1 <<  6),
    CPUCAP_SSE3     = (1 <<  7),
    CPUCAP_3DNOW    = (1 <<  8),
    CPUCAP_3DNOWEXT = (1 <<  9),

    // specific instructions
    CPUCAP_TSC   = (1 << 20),  // TSC, RDTSC (Time Stamp Counter)
    CPUCAP_CMOV  = (1 << 21),  // CMOV
    CPUCAP_CX8B  = (1 << 22),  // CMPXCHG8B
    CPUCAP_CX16B = (1 << 23),  // CMPXCHG16B
#endif
  };


public :

  // os version
  static Version GetVersion  (void);
  static bool    IsWindowsNT (void);

  // cpu caps (runtime info)
  static bool           HasCpuInfo       (void);
  static const StringA& GetCpuIdentifier (void);
  static dword          GetCpuCaps       (void); // return CPUCAP_NONE if cannot read cpu caps
  static const StringA& GetCpuCapsString (void);
  static bool           HasCpuCap        (CpuCaps eCpuCap);

  // error
  static uint        LastError       (void);
  static const char* LastErrorString (void);
  static const char* GetErrorString  (uint uiOsError);

  // network subsystem error
#if defined(NCORE_ENABLE_NETWORKING)
  static NetError    LastNetError       (void);
  static const char* LastNetErrorString (void);
  static const char* GetNetErrorString  (NetError eNetError);
  static const char* GetNetErrorName    (NetError eNetError);
#endif

  // time
  static void    AppendDateTimeString (StringA& strOut, bool bFraction=false); // "YYYY-MM-DD hh:mm:ss" or "YYYY-MM-DD hh:mm:ss.fff"
  static void    AppendDateString     (StringA& strOut);                       // "YYYY-MM-DD"
  static void    AppendTimeString     (StringA& strOut, bool bFraction=false); // "hh:mm:ss" or "hh:mm:ss.fff" (24h mode)
  static StringA GetDateTimeString    (bool bFraction=false);                  // shortcut to AppendDateTimeString(), for convenience only
  static StringA GetDateString        (void);                                  // shortcut to AppendDateString(), for convenience only
  static StringA GetTimeString        (bool bFraction=false);                  // shortcut to AppendTimeString(), for convenience only
  static void    Sleep                (uint32 uiMilliseconds);
  static int64   GetUnixTime          (void);

  // byte ordering
  static bool IsBigEndian     (void);
  static void SwapBytes       (uint16 valSource, byte* pOutBytes);
  static void SwapBytes       (uint32 valSource, byte* pOutBytes);
  static void SwapBytes       (uint64 valSource, byte* pOutBytes);
  static void SwapBytes       (float valSource, byte* pOutBytes);
  static void SwapBytes       (double valSource, byte* pOutBytes);
  static void SwapBufferBytes (void* pBuffer, uint uiElemSize, uint uiElemCount);

  // process
  static uint GetCurrentProcessID (void);


private :
  static Version DetectVersion    (void);
  static int     DetectEndianness (void);


private :
  static int ms_nIsBigEndian;
};


#include "System.inl.h"


}
#endif // #ifndef __nCore_SystemTools_Header__
