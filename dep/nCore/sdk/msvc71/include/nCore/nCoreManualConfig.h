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

#ifndef __nCore_ManualConfig_Header__
#define __nCore_ManualConfig_Header__


//---------------------------------------------------------------------------
// NCORE_DEBUG
//
// * As usual, debug mode enables *tons* of miscellaneous runtime sanity
//   checks so it is *highly* recommended that you develop using debug
//   version before trying the release version.
// * It also allows you to use one of the provided memory manager strategies.
// * For convenience, X_DEBUG flag is also defined so it's less nCore
//   specific.
//---------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
  #ifndef NCORE_DEBUG
    #define NCORE_DEBUG
  #endif
  #ifndef X_DEBUG
    #define X_DEBUG
  #endif
#endif

//---------------------------------------------------------------------------
// NCORE_ENABLE_RELEASE_ASSERT
//
// * Only useful when NCORE_DEBUG is *not* defined.
// * When this flag is defined, XASSERT assertions will be kept in the code
//   and if an XASSERT test goes wrong, an nCore::Exception object will be
//   thrown.
// * Note that only the XASSERT macro is concerned here, the XRELEASEASSERT
//   asserts will *always* be included in the built binary.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_RELEASE_ASSERT

//---------------------------------------------------------------------------
// NCORE_ENABLE_MULTI_PROCESS_AWARENESS
//
// * This flag allows you to let the library knows if it needs to be careful
//   about being called from multiple process.
// * For example, if this flag is enabled, the library will take care about
//   naming log files carefully so there's no possible collision against an
//   another concurrent running process which is also using this library
//   (being dynamically linked or not !).
// * It is recommended to keep this flag *enabled* unless you are absolutely
//   sure your client application will ever have only one running instance at
//   time.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_MULTI_PROCESS_AWARENESS

//---------------------------------------------------------------------------
// NCORE_ENABLE_INTERNAL_MEMORY_MANAGER
//
// * Only used when NCORE_DEBUG is defined.
// * Allows nCore and its client applications to use the internal memory
//   manager to track memory bugs.
// * WARNING :
//   The usage of the memory manager is tricky, please read the instructions
//   from the "Memory Manager" section in the _nCore.h header !
//---------------------------------------------------------------------------
#define NCORE_ENABLE_INTERNAL_MEMORY_MANAGER

//---------------------------------------------------------------------------
// NCORE_ENABLE_LOGGEREX
//
// * Enables the LoggerEx class.
//---------------------------------------------------------------------------
#define NCORE_ENABLE_LOGGEREX

//---------------------------------------------------------------------------
// NCORE_ENABLE_FILEIO
//
// * Enables File, FileDisk, FileMem and Streamer classes.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_FILEIO

//---------------------------------------------------------------------------
// NCORE_ENABLE_MULTITHREADING
//
// * Enables Thread* classes.
// * If you are in a multithreaded context, it is recommended to *enable*
//   this feature *even* if you don't want to use nCore's Thread* classes
//   because this will protect some internal classes of nCore against
//   potential concurrent accesses (which are disabled otherwise to gain some
//   extra overhead).
// * But if you are sure your program will be single threaded, you can safely
//   disable this feature.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_MULTITHREADING

//---------------------------------------------------------------------------
// NCORE_ENABLE_NETWORKING
//
// * Allows to do network IO.
// * Implementation is wanted to be protocol independent.
// * Not supported under Cygwin because it lacks support of resolver
//   API (getnameinfo(), etc...).
//---------------------------------------------------------------------------
#ifndef NCORE_PLATFORM_CYGWIN
//#define NCORE_ENABLE_NETWORKING
#endif

//---------------------------------------------------------------------------
// NCORE_ENABLE_NETWORKING_IPV6
//
// * As the name says, this flag allows to enable IPv6 support.
// * Only used when NCORE_ENABLE_NETWORKING is defined.
// * If your platform does not support IPv6 yet (like Cygwin), you can safely
//   disable this feature if it causes you any trouble at compile-time.
//---------------------------------------------------------------------------
#ifndef NCORE_PLATFORM_CYGWIN
#define NCORE_ENABLE_NETWORKING_IPV6
#endif

//---------------------------------------------------------------------------
// NCORE_ENABLE_NETWORKING_STATS
//
// * Only used when NCORE_ENABLE_NETWORKING is defined.
// * This will enable NetSocket class to count how much bytes/packets were
//   sent/received since last Connect() or Bind().
//---------------------------------------------------------------------------
#define NCORE_ENABLE_NETWORKING_STATS

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_CONFIG_FILE
//
// * Only used when NCORE_ENABLE_NETWORKING is defined.
// * Enables the NetSmtpSender util class.
//---------------------------------------------------------------------------
#define NCORE_ENABLE_NETWORKING_SMTP_SENDER

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_BUILDDATE
//
// * Enables the BuildDate util class.
// * You'll usually want to undef the NCORE_ENABLE_UTIL_BUILDDATETIME option
//   if you use this (BuildDateTime class)...
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_BUILDDATE

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_BUILDDATETIME
//
// * Enables the BuildDateTime util class.
// * You'll usually want to undef the NCORE_ENABLE_UTIL_BUILDDATE option if
//   you use this (BuildDate class)...
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_BUILDDATETIME

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_CRC8
//
// * Enables the Crc8 util class.
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_CRC8

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_CRC16
//
// * Enables the Crc16 util class.
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_CRC16

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_CRC32
//
// * Enables the Crc32 util class.
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_CRC32

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_CONFIG_FILE
//
// * Enables the ConfigFile util class.
//---------------------------------------------------------------------------
#define NCORE_ENABLE_UTIL_CONFIG_FILE

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_DOGTAGS
//
// * Enables the DogTags util class.
// * Note that even if you disable this feature, the NCORE_DOGTAG macro will
//   be available.
// * To know more about dog-tags in nCore, be sure to read comments in the
//   <nCore/DogTags.h> header.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_DOGTAGS

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_DYNAMIC_LIBRARY
//
// * Enables the DynamicLibrary util class.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_DYNAMIC_LIBRARY

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_FILE_ITERATOR
//
// * Enables the FileIterator util class.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_FILE_ITERATOR

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_GUID
//
// * Enables the Guid util class.
// * You may experience some troubles during compile-time in static mode if
//   this feature is enabled because it uses the underlying OS API.
//   If this is the case and if you don't plan to use the Guid class, you can
//   safely disable it.
// * For informational purpose, here are the used API function according to
//   the OS for now :
//   * Win32  : UuidCreate() and UuidToString() from the <rpc.h> header
//              (rpcrt4.lib, rpcrt4.dll).
//   * Linux  : uuid_generate() and uuid_unparse_lower() from the
//              <uuid/uuid.h> header (uuid library).
//   * MacOSX : CFUUIDCreate(), CFUUIDCreateString() and CFStringGetCString()
//              from the <CoreFoundation/CFUUID.h> header.
//---------------------------------------------------------------------------
#ifndef NCORE_PLATFORM_CYGWIN
//#define NCORE_ENABLE_UTIL_GUID
#endif

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_MD5
//
// * Enables the Md5 util class.
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_MD5

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_SHA1
//
// * Enables the Sha1 util class.
// * If you don't need it, you can disable it to reduce the final size of the
//   library.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_SHA1

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_STRINGBASE
//
// * Enables StringBase class.
// * StringBase template class is an extended and derived version of the
//   std::basic_string class and is able to handle <char> or <wchar_t>
//   elements.
// * Since it's not used by default in this library, it can be safely
//   disabled.
// * Not supported under Cygwin because its libc does not implement
//   vswprintf() function.
//---------------------------------------------------------------------------
#ifndef NCORE_PLATFORM_CYGWIN
//#define NCORE_ENABLE_UTIL_STRINGBASE
#endif

//---------------------------------------------------------------------------
// NCORE_ENABLE_UTIL_VERSION2
//
// * Enables the Version2 util class.
//---------------------------------------------------------------------------
//#define NCORE_ENABLE_UTIL_VERSION2


#endif // #ifndef __nCore_ManualConfig_Header__
