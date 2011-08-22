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

#ifndef __nCore_Version2_Header__
#define __nCore_Version2_Header__
#if defined(NCORE_ENABLE_UTIL_VERSION2)
namespace nCore {


//---------------------------------------------------------------------------
// Version2
//
// A valid Version2 string must be formatted this way :
// "M.m" or "M.mm"
//
// Where :
// * "M" is the major version number
// * "m" is the minor version number
//
// A valid Version2 has its components' values in :
// * 0 <= Major <= 9
// * 0 <= minor <= 99
//
// The "combined" value is an all-in-one representation of the 2 components
// of Version2 and is computed this way :
// nCombined = (100 * m_nMajor) + m_nMinor;
//---------------------------------------------------------------------------
class NCORE_EXPORT Version2
{
  friend class Version3;

public :
  Version2 (void);
  Version2 (int16 nMajor, int16 nMinor);
  Version2 (int16 nCombined);
  Version2 (const char* pszVersion);
  Version2 (const Version2& rhs);
  Version2 (const Version3& rhs);
  ~Version2 (void);

  void Init (int16 nMajor, int16 nMinor);
  void Init (int16 nCombined);
  void Init (const char* pszVersion);

  Version2& operator= (const Version2& rhs);
  Version2& operator= (const Version3& rhs);

  bool IsValid (void) const;

  bool operator== (const Version2& rhs) const;
  bool operator!= (const Version2& rhs) const;
  bool operator<  (const Version2& rhs) const;
  bool operator<= (const Version2& rhs) const;
  bool operator>  (const Version2& rhs) const;
  bool operator>= (const Version2& rhs) const;

  bool operator== (const Version3& rhs) const;
  bool operator!= (const Version3& rhs) const;
  bool operator<  (const Version3& rhs) const;
  bool operator<= (const Version3& rhs) const;
  bool operator>  (const Version3& rhs) const;
  bool operator>= (const Version3& rhs) const;

  int16       GetMajor          (void) const;
  int16       GetMinor          (void) const;
  int16       GetCombined       (void) const;
  const char* GetStrippedString (void) const; // "M.m" or "M"
  const char* GetPaddedString   (void) const; // "M.mm"


private :
  void BuildStrings (void) const;


private :
  int16 m_nMajor;
  int16 m_nMinor;

  mutable char m_szStripped[6];
  mutable char m_szPadded[6];
};


#include "Version2.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_VERSION2)
#endif // #ifndef __nCore_Version2_Header__
