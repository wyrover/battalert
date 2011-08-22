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

#ifndef __nCore_FileDisk_Header__
#define __nCore_FileDisk_Header__
#if defined(NCORE_ENABLE_FILEIO)
namespace nCore {


//---------------------------------------------------------------------------
// FileDisk
//
// Operations on a physical file.
//
// FileDisk class always use the binary mode to open physical files so if you
// want to explicitely use the text mode, you'll have to implement a class
// derived from it but be careful while implementing methods like GetLength()
// or Seek() for example, since it may become a mess if you don't pay enough
// attention (remember the "\r\n" vs. "\n" length for example !)...
//---------------------------------------------------------------------------
class NCORE_EXPORT FileDisk : public File
{
public :
  FileDisk (void);
  virtual ~FileDisk (void);

  // open / close
  virtual void           Open         (const char* pszFilePath, dword dwOpenFlags, dword dwUnused1=0, dword dwUnused2=0);
  virtual void           Close        (void);
  virtual const StringA& GetPath      (void) const;

  // read / write
  virtual uint32 Read       (void* pBuffer, uint32 uiLength);
  virtual void   Write      (const void* pBuffer, uint32 uiLength);
  virtual void   Flush      (void);
  virtual bool   IsGrowable (void) const;

  // seeking methods
  virtual uint64 Seek        (SeekOrigin eOrigin, int64 nOffset);
  virtual uint64 GetPosition (void) const;

  // file size
  virtual uint64 GetLength (void) const;
  virtual void   SetLength (uint64 uiNewLength);


protected :
  // buffer access
  virtual BufferAccessType GetBufferAccessType      (void) const;
  virtual uint32           BufferAccess             (dword dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL);


protected :
  // path
  StringA m_strPath;

  // current buffer access status
  byte*  m_pMappedBuffer;
  uint32 m_uiMappedBufferLength;

  // handles
#if defined(NCORE_PLATFORM_WINDOWS)
  HANDLE m_hFile;
  HANDLE m_hFileMap;
#else
  FILE* m_pFile;
#endif
};


#include "FileDisk.inl.h"


}
#endif // #if defined(NCORE_ENABLE_FILEIO)
#endif // #ifndef __nCore_FileDisk_Header__
