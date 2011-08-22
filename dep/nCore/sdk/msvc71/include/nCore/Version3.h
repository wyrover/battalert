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

#ifndef __nCore_Version3_Header__
#define __nCore_Version3_Header__
namespace nCore {


//---------------------------------------------------------------------------
// Version3
//
// A valid Version3 string must be formatted this way :
// "M.m.R" or "M.mm.RRRR"
//
// Where :
// * "M" is the major version number
// * "m" is the minor version number
// * "R" is the revision/build/whatever-you-want version number
//
// A valid Version3 has its components' values in :
// * 0 <= Major    <= 9
// * 0 <= minor    <= 99
// * 0 <= Revision <= 9999
//
// The "combined" value is an all-in-one representation of the 3 components
// of Version3 and is computed this way :
// nCombined = (1000000 * m_nMajor) + (10000 * m_nMinor) + m_nRevision;
//---------------------------------------------------------------------------
class NCORE_EXPORT Version3
{
#if defined(NCORE_ENABLE_UTIL_VERSION2)
  friend class Version2;
#endif

public :
  Version3 (void);
  Version3 (int nMajor, int nMinor, int nRevision);
  Version3 (int nCombined);
  Version3 (const char* pszVersion);
  Version3 (const Version3& rhs);
#if defined(NCORE_ENABLE_UTIL_VERSION2)
  Version3 (const Version2& rhs);
#endif
  ~Version3 (void);

  void Init (int nMajor, int nMinor, int nRevision);
  void Init (int nCombined);
  void Init (const char* pszVersion);

  Version3& operator= (const Version3& rhs);
#if defined(NCORE_ENABLE_UTIL_VERSION2)
  Version3& operator= (const Version2& rhs);
#endif

  bool IsValid (void) const;

  bool operator== (const Version3& rhs) const;
  bool operator!= (const Version3& rhs) const;
  bool operator<  (const Version3& rhs) const;
  bool operator<= (const Version3& rhs) const;
  bool operator>  (const Version3& rhs) const;
  bool operator>= (const Version3& rhs) const;

#if defined(NCORE_ENABLE_UTIL_VERSION2)
  bool operator== (const Version2& rhs) const;
  bool operator!= (const Version2& rhs) const;
  bool operator<  (const Version2& rhs) const;
  bool operator<= (const Version2& rhs) const;
  bool operator>  (const Version2& rhs) const;
  bool operator>= (const Version2& rhs) const;
#endif

  int         GetMajor          (void) const;
  int         GetMinor          (void) const;
  int         GetRevision       (void) const;
  int         GetCombined       (void) const;
  const char* GetStrippedString (void) const; // "M.m.R" or "M.m" or "M"
  const char* GetPaddedString   (void) const; // "M.mm.RRRR"


private :
  void BuildStrings (void) const;


private :
  int32 m_nMajor;
  int32 m_nMinor;
  int32 m_nRevision;

  mutable char m_szStripped[12];
  mutable char m_szPadded[12];
};


#include "Version3.inl.h"


}
#endif // #ifndef __nCore_Version3_Header__
