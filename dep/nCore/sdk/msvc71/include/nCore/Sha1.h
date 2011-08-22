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

#ifndef __nCore_Sha1_Header__
#define __nCore_Sha1_Header__
#if defined(NCORE_ENABLE_UTIL_SHA1)
namespace nCore {


//---------------------------------------------------------------------------
// Sha1
//
// This class implements code from Steve Reid's <sreid@sea-to-sky.net> public
// domain SHA-1 implementation.
//---------------------------------------------------------------------------
class NCORE_EXPORT Sha1
{
public :
  enum
  {
    DIGEST_ELEMENTS_COUNT = 20,
    DIGEST_STRING_LENGTH  = DIGEST_ELEMENTS_COUNT << 1,
  };


public :
  Sha1 (void);
  ~Sha1 (void);

  // static utils
  static int     CompareDigests (const uint8* paDigestA, const uint8* paDigestB);
  static void    DigestToString (StringA& strOutDigest, const uint8* paDigest);
  static bool    StringToDigest (uint8* paOutDigest, const StringA& strDigest);
  static StringA FileStatic     (const char* pszFilePath, uint32 uiSwapBufferSize=0x2000); // 8KB
  static StringA BlockStatic    (const byte* pBuffer, uint32 uiBufferSize);


  // compute sha1 digest from a file (digest will be finalized automatically)
  bool File (const char* pszFilePath, uint32 uiSwapBufferSize=0x2000); // 8KB
  bool File (const char* pszFilePath, byte* pSwapBuffer, uint32 uiSwapBufferSize); // this one is faster if you already have a swap buffer since no memory alloc is done here

  // compute sha1 digest from data block(s)
  void BlockUpdate   (const byte* pBuffer, uint32 uiBufferSize);
  void BlockFinalize (void);


  // final digest (digest must be finalized first)
  const uint8* GetDigest (void) const;
  void         GetDigest (StringA& strOutDigest) const;


  // state reset (in case you want to reuse the object)
  void Reset (void);


protected :
  void Transform (uint32 auiState[5], const uint8 auiIn[64]);


protected :
  // context
  uint32 m_auiState[5];
  uint32 m_auiCount[2];
  uint8  m_aBuffer[64];

  // final digest
  uint8 m_aDigest[DIGEST_ELEMENTS_COUNT];
};


// inline methods
#include "Sha1.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_SHA1)
#endif // #ifndef __nCore_Sha1_Header__
