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

#ifndef __nCore_FileSystem_Header__
#define __nCore_FileSystem_Header__
namespace nCore {


//---------------------------------------------------------------------------
// FileSystem
//---------------------------------------------------------------------------
class NCORE_EXPORT FileSystem
{
public :
  // file existency
  static bool DoesExist (const char* pszPath);
  static bool Delete    (const char* pszPath);

  // file type
  static bool IsDirectory (const char* pszPath);

  // file attributes
  static uint64 GetSize      (const char* pszPath);
  static time_t GetModifTime (const char* pszPath); // returns unix time (utc)

  // directories specific
  static bool CreateDirectory (const char* pszPath, bool bRecursive=false);

  // temp files
  static StringA GetTempDirectory (void);
  static StringA CreateTempFile   (const char* pszPrefix="", const char* pszExtension="");

  // extract file content to memory
  // * this method is just here for convenience, if you are searching for
  //   something more complete, try the FileMem::OpenFromDisk() method.
  static uint32 ExtractContent (const char* pszFilePath, byte* pBuffer, uint32 uiBufferSize);

  // windows specifics
#if defined(NCORE_PLATFORM_WINDOWS)
  static int     Win_IsUtcFileSystem       (const char* pszPath);
  static time_t  Win_UtcFileTimeToUnixTime (const FILETIME& ftUTC);
#endif
};


#include "FileSystem.inl.h"


}
#endif // #ifndef __nCore_FileSystem_Header__
