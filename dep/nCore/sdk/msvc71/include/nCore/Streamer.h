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

#ifndef __nCore_Streamer_Header__
#define __nCore_Streamer_Header__
#if defined(NCORE_ENABLE_FILEIO)
namespace nCore {


//---------------------------------------------------------------------------
// Streamer
//
// * The Streamer class allows you to perform *buffered* i/o operations
//   through an attached File-derived object (FileDisk or FileMem for
//   example), according to a given endianess flavor.
// * Why not directly integrate those features in the File class ?
//   Well, sometimes, writing (or reading) to a physical support can become a
//   very expensive task (time consuming, battery consuming for embedded
//   development, etc...), especially if we need to perform lots of small i/o
//   operations.
//   In those cases, it's would be a better idea to create your data chunks
//   into a buffer, and then upload them to the final physical support.
// * This is what the Streamer class offers to you : an intermediate layer
//   between your application's logic of reading/writing (which has better to
//   be sequential though !) and the physical support (hard-drive, dvd,
//   sd-card, network, or whatever support which needs streaming i/o).
// * This is also why the convenient << and >> operators are not declared in
//   the File interface, this forces user to think about it :)
//   See it as an action to promote good programming practices !
// * As a side note, you will not be able to make both read & write operation
//   types with the *same* Streamer object.
//   For example, if you Open() a Streamer object with the STREAM_LOAD
//   StreamMode, you will only be able to perform *read* operations.
//---------------------------------------------------------------------------
class NCORE_EXPORT Streamer : private NonCopyable
{
public :
  enum
  {
    BUFFER_MIN_SIZE     = 128,
    BUFFER_DEFAULT_SIZE = 4092,
  };

  enum StreamMode
  {
    STREAM_LOAD                  = 0,  // read only (mutually exclusive with STREAM_STORE)
    STREAM_STORE                 = 1,  // write only (mutually exclusive with STREAM_LOAD)

    STREAM_BIGENDIAN             = 0x00000100,        // means load/store data in big endian. little endian is the default behavior
    STREAM_NETWORKENDIAN         = STREAM_BIGENDIAN,  // alias
    STREAM_NOFLUSHONCLOSE        = 0x00000200,

    STREAM_AUTODELETE_FILEOBJECT = 0x00010000,  // pFile object given to Open() will be deleted by this Streamer
  };


public :
  Streamer (void);
  virtual ~Streamer (void);

  // open / close
  virtual void Open  (File* pFile, dword dwStreamMode=STREAM_LOAD, uint32 uiSwapBufferSize=BUFFER_DEFAULT_SIZE, byte* pSwapBuffer=NULL);
  virtual void Close (void);

  // properties accessors
  File* GetFile         (void);
  dword GetStreamMode   (void) const;
  bool  IsLoading       (void) const;
  bool  IsStoring       (void) const;
  bool  IsLittleEndian  (void) const;
  bool  IsNetworkEndian (void) const;

  // read
  uint32 Read       (void* pDestBuffer, uint32 uiMaxBytesToRead);
  void   ReadSize   (uint32& uiSize);
  void   ReadSize   (uint64& uiSize);
  uint32 ReadString (char* pszDest, uint32 uiMaxStringLength, bool* pbNeedMore=NULL);  // uiMaxStringLength should be one less than the size of the destination buffer
  uint32 ReadString (wchar_t* pszDest, uint32 uiMaxStringLength, bool* pbNeedMore=NULL);
  void   ReadString (StringA& strDest);
#ifdef NCORE_ENABLE_UTIL_STRINGBASE
  void   ReadString (StringBase<char>& strDest);
  void   ReadString (StringBase<wchar_t>& strDest);
#endif

  // write
  void Write       (const void* pSourceBuffer, uint32 uiBytesToWrite);
  void WriteSize   (uint32 uiSize);
  void WriteSize   (uint64 uiSize);
  void WriteString (const char* pszSource);
  void WriteString (const wchar_t* pszSource);

  // read operators
  Streamer& operator>> (bool& val);
  Streamer& operator>> (int8& val);
  Streamer& operator>> (uint8& val);
  Streamer& operator>> (int16& val);
  Streamer& operator>> (uint16& val);
  Streamer& operator>> (int32& val);
  Streamer& operator>> (uint32& val);
  Streamer& operator>> (int64& val);
  Streamer& operator>> (uint64& val);
  Streamer& operator>> (float& val);
  Streamer& operator>> (double& val);

  // write operators
  Streamer& operator<< (bool val);
  Streamer& operator<< (int8 val);
  Streamer& operator<< (uint8 val);
  Streamer& operator<< (int16 val);
  Streamer& operator<< (uint16 val);
  Streamer& operator<< (int32 val);
  Streamer& operator<< (uint32 val);
  Streamer& operator<< (int64 val);
  Streamer& operator<< (uint64 val);
  Streamer& operator<< (float val);
  Streamer& operator<< (double val);


protected :
  bool IsTargetReadable (void) const;
  bool IsTargetWritable (void) const;

  void FillBuffer  (uint32 uiBytesNeeded);
  void FlushBuffer (bool bGetNextBuffer=true);


protected :
  // streamer properties
  File* m_pFile;
  dword m_dwStreamMode;
  bool  m_bDirectMapping;
  bool  m_bBlocking;
  bool  m_bBigEndian;

  // buffer properties
  byte*  m_pBuffer;
  uint32 m_uiBufferSize;
  bool   m_bUserBuffer;

  // buffer cursors
  byte* m_pBufferCursor;
  byte* m_pBufferMax;
};


#include "Streamer.inl.h"


}
#endif // #if defined(NCORE_ENABLE_FILEIO)
#endif // #ifndef __nCore_Streamer_Header__
