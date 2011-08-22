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

#ifndef __nCore_File_Header__
#define __nCore_File_Header__
#if defined(NCORE_ENABLE_FILEIO)
namespace nCore {


//---------------------------------------------------------------------------
// File
//---------------------------------------------------------------------------
class NCORE_EXPORT File : private NonCopyable
{
  friend class Streamer;

public :
  enum OpenFlags
  {
    // access mode
    FILE_FLAG_READ            = 0x00000001,
    FILE_FLAG_WRITE           = 0x00000002,
    FILE_FLAG_READWRITE       = 0x00000003,

    // open / create mode (default behavior is to throw an exception if file does not exist)
    FILE_FLAG_CREATE_NEW      = 0x00000010,  // create file only if it does *not* exist
    FILE_FLAG_TRUNCATE        = 0x00000020,  // only in write mode
    FILE_FLAG_CREATE_ALWAYS   = FILE_FLAG_CREATE_NEW | FILE_FLAG_TRUNCATE,

    // share access (default access is "exclusive")
    FILE_FLAG_SHARE_READ      = 0x00000100,
    FILE_FLAG_SHARE_WRITE     = 0x00000200,
    FILE_FLAG_SHARE_READWRITE = 0x00000300,

    // not used
    FILEOPENFLAGS_FORCE_DWORD = 0x7FFFFFFF,
  };

  enum SeekOrigin
  {
    FILE_SEEK_BEGIN   = SEEK_SET,  // from the beginning of the file
    FILE_SEEK_CURRENT = SEEK_CUR,  // from the current position in the file
    FILE_SEEK_END     = SEEK_END,  // from the end of the file
  };

  enum BufferAccessType
  {
    BUFFERACCESSTYPE_NONE,      // no direct access to the container's buffer
    BUFFERACCESSTYPE_DIRECT,    // direct access to the container, this access type allows dynamic and automatic growing in write mode
    BUFFERACCESSTYPE_MAPPED,    // buffer access is granted by using mmap() on linux or MapViewOfFile() on windows platforms
    BUFFERACCESSTYPE_BLOCKING,  // socket or anything similar which is blocking
  };

  enum BufferAccessMode
  {
    // command
    BUFFERACCESSMODE_COMMAND_READONLY  = 1,  // get "read only" access to the buffer
    BUFFERACCESSMODE_COMMAND_READWRITE = 2,  // get writable buffer
    BUFFERACCESSMODE_COMMAND_COMMIT    = 3,  // release buffer (commit changes after a read/write command)

    // command flags
    BUFFERACCESSMODE_FLAG_DONOTGROW = 0x01000000,  // only useful with the BUFFERACCESSMODE_COMMAND_READWRITE command

    // those values are used internally
    BUFFERACCESSMODE_NONE         = 0,
    BUFFERACCESSMODE_MASK_COMMAND = 0x00FFFFFF,
    BUFFERACCESSMODE_MASK_FLAGS   = 0xFF000000,
  };


public :
  File (void);
  virtual ~File (void);

  // open / close
  virtual void  Open         (const char* pszFilePath, dword dwOpenFlags, dword dwUnused1=0, dword dwUnused2=0) = 0;
  virtual void  Close        (void);
  virtual bool  IsOpen       (void) const;
  virtual dword GetOpenFlags (void) const;

  // read / write
  virtual uint32 Read       (void* pBuffer, uint32 uiLength) = 0;
  virtual void   Write      (const void* pBuffer, uint32 uiLength) = 0;
  virtual void   Flush      (void) = 0;
  virtual bool   IsReadable (void) const;
  virtual bool   IsWritable (void) const;
  virtual bool   IsGrowable (void) const = 0;

  // seeking methods
  virtual void   SeekToBegin (void);
  virtual uint64 SeekToEnd   (void);
  virtual uint64 Seek        (SeekOrigin eOrigin, int64 nOffset) = 0;
  virtual uint64 GetPosition (void) const = 0;

  // file size
  virtual uint64 GetLength (void) const = 0;
  virtual void   SetLength (uint64 uiNewLength) = 0;


protected :
  // buffer access
  virtual BufferAccessType GetBufferAccessType      (void) const = 0;
  virtual uint32           BufferAccess             (dword dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL) = 0;
  virtual bool             IsBufferRequested        (void) const;
  virtual bool             ValidateBufferAccessMode (dword dwBufferAccessMode) const;


protected :
  // common properties
  dword m_dwOpenFlags;

  // current buffer access status
  dword m_dwBufferAccessMode;
};


#include "File.inl.h"


}
#endif // #if defined(NCORE_ENABLE_FILEIO)
#endif // #ifndef __nCore_File_Header__
