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

#ifndef __nCore_Crc16_Header__
#define __nCore_Crc16_Header__
#if defined(NCORE_ENABLE_UTIL_CRC16)
namespace nCore {


//---------------------------------------------------------------------------
// Crc16
//---------------------------------------------------------------------------
class NCORE_EXPORT Crc16
{
public :
  enum
  {
    INIT_VALUE = 0xFFFF,
    XOR_VALUE  = 0x0000,
  };


public :
  Crc16 (void);
  ~Crc16 (void);

  // static utils
  static uint16 BlockStatic (const byte* pBlock, uint uiBlockSize);

  // actions
  void   Begin  (void);
  void   Update (byte b);
  void   Update (const byte* pBlock, uint uiBlockSize);
  uint16 Finish (void); // returns the finalized crc

  // get finalized crc
  uint16 GetCrc (void) const;


protected :
  uint16 uiCrc;
};


// inline methods
#include "Crc16.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_CRC16)
#endif // #ifndef __nCore_Crc16_Header__
