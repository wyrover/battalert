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

#ifndef __nCore_Md5_Header__
#define __nCore_Md5_Header__
#if defined(NCORE_ENABLE_UTIL_MD5)
namespace nCore {


//---------------------------------------------------------------------------
// Md5
//
// Implements the MD5 Message Digest Algorithm (RFC 1321).
//
// Notes about this MD5 implementation :
// * The algorithm is due to Ron Rivest.
// * This implementation is based on he code written by Colin Plumb in 1993
//   and no copyright is claimed for it.
// * It is fully compatible with the RSA implementation.
// * RFC : http://www.freesoft.org/CIE/RFC/1321/
//---------------------------------------------------------------------------
class NCORE_EXPORT Md5
{
public :
  enum
  {
    DIGEST_ELEMENTS_COUNT = 16,
    DIGEST_STRING_LENGTH  = DIGEST_ELEMENTS_COUNT << 1,
  };


public :
  Md5 (void);
  ~Md5 (void);

  // static utils
  static int     CompareDigests (const uint8* paDigestA, const uint8* paDigestB);
  static void    DigestToString (StringA& strOutDigest, const uint8* paDigest);
  static bool    StringToDigest (uint8* paOutDigest, const StringA& strDigest);
  static StringA FileStatic     (const char* pszFilePath, uint32 uiSwapBufferSize=0x2000); // 8KB
  static StringA BlockStatic    (const uchar* pBuffer, uint32 uiBufferSize);


  // compute md5 digest from a file (digest will be finalized automatically)
  bool File (const char* pszFilePath, uint32 uiSwapBufferSize=0x2000); // 8KB
  bool File (const char* pszFilePath, uchar* pSwapBuffer, uint32 uiSwapBufferSize); // this one is faster if you already have a swap buffer since no memory alloc is done here

  // compute md5 digest from data block(s)
  void BlockUpdate   (const uchar* pBuffer, uint32 uiBufferSize);
  void BlockFinalize (void);


  // final digest (digest must be finalized first)
  const uint8* GetDigest (void) const;
  void         GetDigest (StringA& strOutDigest) const;


  // state reset (in case you want to reuse the object)
  void Reset (void);


protected :
  void Transform (uint32 auiState[4], uint32 auiIn[16]);


protected :
  // context
  uint32 m_auiState[4];
  uint32 m_auiBits[2];
  uint8  m_aBuffer[64];

  // final digest
  uint8 m_aDigest[DIGEST_ELEMENTS_COUNT];
};


// inline methods
#include "Md5.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_MD5)
#endif // #ifndef __nCore_Md5_Header__
