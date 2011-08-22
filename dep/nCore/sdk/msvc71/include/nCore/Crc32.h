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
//
// This class implements the CRC-32 algorithm from Mark Adler.
// Copyright (C) 1995-1998 Mark Adler.
//
//***************************************************************************

#ifndef __nCore_Crc32_Header__
#define __nCore_Crc32_Header__
#if defined(NCORE_ENABLE_UTIL_CRC32)
namespace nCore {


//---------------------------------------------------------------------------
// Crc32
//---------------------------------------------------------------------------
class NCORE_EXPORT Crc32
{
public :
  enum
  {
    INIT_VALUE = 0xFFFFFFFFL,
    XOR_VALUE  = 0xFFFFFFFFL,
  };


public :
  Crc32 (void);
  ~Crc32 (void);

  // static utils
  static uint32 BlockStatic (const byte* pBlock, uint uiBlockSize);
  static uint32 FileStatic  (const char* pszFilePath, uint uiSwapBufferSize=0x2000); // 8KB

  // actions
  void   Begin  (void);
  void   Update (byte b);
  void   Update (const byte* pBlock, uint uiBlockSize);
  uint32 Finish (void); // returns the finalized crc

  // get finalized crc
  uint32 GetCrc (void) const;


protected :
  uint32 uiCrc;
};


// inline methods
#include "Crc32.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_CRC32)
#endif // #ifndef __nCore_Crc32_Header__
