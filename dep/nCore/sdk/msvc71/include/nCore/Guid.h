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

#ifndef __nCore_Guid_Header__
#define __nCore_Guid_Header__
#if defined(NCORE_ENABLE_UTIL_GUID)
namespace nCore {


//---------------------------------------------------------------------------
// Guid
//---------------------------------------------------------------------------
class NCORE_EXPORT Guid
{
public :
  enum
  {
    GUID_STRING_LENGTH = 36,  // "00000000-0000-0000-0000-000000000000"
  };


public :
  Guid (void);
  Guid (const Guid& rhs);
  Guid (const char* rhs);
  ~Guid (void);

  // get object status
  bool IsNull (void) const;

  // copy
  void Set       (const char* pszGuid);
  void operator= (const Guid& rhs);
  void operator= (const char* rhs);

  // generate
  void Generate (void);

  // get a null-terminated c-string representation
  const char* Get      (void) const;
  const char* c_str    (void) const;
  operator const char* (void) const;

  // compare
  bool operator== (const Guid& rhs) const;
  bool operator== (const char* rhs) const;
  bool operator!= (const Guid& rhs) const;
  bool operator!= (const char* rhs) const;


private :
  char m_szGuid[GUID_STRING_LENGTH + 1];


private :
  static char ms_szNullGuid[GUID_STRING_LENGTH + 1]; // "00000000-0000-0000-0000-000000000000"
};


#include "Guid.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_GUID)
#endif // #ifndef __nCore_Guid_Header__
