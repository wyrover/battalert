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

#ifndef __nCore_FileMem_Header__
#define __nCore_FileMem_Header__
#if defined(NCORE_ENABLE_FILEIO)
namespace nCore {


//---------------------------------------------------------------------------
// FileMem
//---------------------------------------------------------------------------
class NCORE_EXPORT FileMem : public File
{
public :
  FileMem (void);
  FileMem (uint32 uiGrowBytes); // implicit call to OpenNew()
  FileMem (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes=0); // implicit call to Open()
  virtual ~FileMem (void);

  // open / close
  // * A FileMem buffer is always growable except in one case : when you
  //   provide your own buffer to the Open() method. this is because the
  //   FileMem class will not be able to reallocate buffer if needed since
  //   there is no way to know, in standard c++, which alloc method you used
  //   (new[] or malloc, etc...).
  // * OpenNew() is a convenient shortcut for a Open(0,0,0,uiGrowBytes) call
  //   to initialize an empty and automatically growable memory file.
  virtual void  Open         (const char* pBuffer, uint32 uiBufferSize, uint32 uiInitialFileSize, uint32 uiGrowBytes=0);
  virtual void  OpenNew      (uint32 uiGrowBytes=1024);
  virtual void  OpenFromDisk (const char* pszFilePath, uint32 uiMaxLengthToReadFromDisk=0, int64 nOffset=0, uint32 uiGrowBytes=0);
  virtual void  Close        (void);
  virtual bool  IsOpen       (void) const;

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

  // direct access to the buffer
  // * use this only to *read* operations so you can get the amount of data
  //   with the GetLength() method.
  // * remember the GetLength() method does not return the capacity of the
  //   buffer but the data effectively written to this buffer !
  // * in any case, prefer to use the Read/Write methods or even consider to
  //   use the Streamer class. this method is only here to provide a way to
  //   get instant acces to the internal buffer so you can send it to an
  //   third party API if needed.
  virtual const byte* GetDirectBuffer (void) const;


  // FileMem specific methods
  virtual void WriteToDiskFile (const char* pszFilePath) const; // existing file will be truncated


protected :
  // buffer access
  virtual BufferAccessType GetBufferAccessType      (void) const;
  virtual uint32           BufferAccess             (dword dwBufferAccessMode, uint32 uiLength=0, byte** ppBufferBegin=NULL, byte** ppBufferEnd=NULL);

  // memory file helpers
  virtual void GrowMemFile (uint32 uiNewSize);

  // memory helpers
  virtual byte* BlockAlloc   (uint32 uiSize);
  virtual byte* BlockRealloc (byte* pBlock, uint32 uiNewSize);
  virtual void  BlockCopy    (byte* pTargetBlock, const byte* pSourceBlock, uint32 uiSize);
  virtual void  BlockFree    (byte* pBlock);


protected :
  byte*  m_pBuffer;
  uint32 m_uiBufferSize;
  uint32 m_uiFileSize;
  uint32 m_uiGrowBytes;
  uint32 m_uiPosition;
  bool   m_bAutoDelete;
};


#include "FileMem.inl.h"


}
#endif // #if defined(NCORE_ENABLE_FILEIO)
#endif // #ifndef __nCore_FileMem_Header__
