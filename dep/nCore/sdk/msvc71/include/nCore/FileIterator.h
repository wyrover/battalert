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

#ifndef __nCore_FileIterator_Header__
#define __nCore_FileIterator_Header__
#if defined(NCORE_ENABLE_UTIL_FILE_ITERATOR)
namespace nCore {


//---------------------------------------------------------------------------
// FileIterator
//---------------------------------------------------------------------------
class NCORE_EXPORT FileIterator
{
public :
  enum FileType
  {
    TYPE_UNKNOWN,
    TYPE_FILE,
    TYPE_DIRECTORY,
    TYPE_SYMLINK,
  };

  enum QueryFlags
  {
    QUERY_NAMEONLY  = 0,
    QUERY_TYPE      = 0x00000001,
    QUERY_SIZE      = 0x00000002,
    QUERY_MODIFTIME = 0x00000004,

    QUERY_ALL       = 0xFFFFFFFF,
  };

  struct FileInfo
  {
    // query flags
    // * this value must be setup using flags from the QueryFlags enum
    //   *before* being sent to the OpenDir() or GetNext() method.
    // * this value will never be reset by the OpenDir() or GetNext() method
    //   so you'll only need to setup it the first time you use it.
    dword dwQueryFlags;

    // file properties
    StringA  strName;     // file name (i.e.: "myfile.ext"), this field can be empty when GetNext() reaches the end of dir
    FileType eType;       // file type, only if (dwQueryFlags & QUERY_TYPE) != 0, *but* it's always setup on Windows because it's costless to get it
    uint64   uiSize;      // file size, only if (dwQueryFlags & QUERY_SIZE) != 0
    time_t   nModifTime;  // modification unix time, only if (dwQueryFlags & QUERY_MODIFTIME) != 0
  };


public :
  FileIterator (void);
  ~FileIterator (void);

  // open
  bool OpenDir (const char* pszDirectory, FileInfo& infoFirst);
  bool IsOpen  (void) const;
  bool IsEmpty (void) const;

  // iterator properties
  const StringA& GetDirectory (void) const;

  // iterate
  bool GetNext     (FileInfo& info);
  bool SeekToFirst (FileInfo& info);

  // close
  void Close (void);


protected :
  StringA m_strDir;
  void*   m_pHandle;
  bool    m_bEmpty;
};


#include "FileIterator.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_FILE_ITERATOR)
#endif // #ifndef __nCore_FileIterator_Header__
