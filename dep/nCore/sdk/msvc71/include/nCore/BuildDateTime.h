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

#ifndef __nCore_BuildDateTime_Header__
#define __nCore_BuildDateTime_Header__
#if defined(NCORE_ENABLE_UTIL_BUILDDATETIME)
namespace nCore {


//---------------------------------------------------------------------------
// BuildDateTime
//
// The goal of this util class is to parse the __DATE__ and __TIME__ macros
// provided by your compiler to build a combined number from every components
// of the build date and time (year, month, day of month, hour, minutes and
// seconds).
//
// The __DATE__ string is expected to be formated this way : "Jan 1 1900".
// The __TIME__ string is expected to be formated this way : "23:59:59".
//
// If you do not want to include the value of the __TIME__ macro, please use
// the lighter BuildDate util class instead.
//---------------------------------------------------------------------------
class NCORE_EXPORT BuildDateTime
{
public :
  BuildDateTime (void);
  BuildDateTime (const char* pszBuildDateFromCompiler, const char* pszBuildTimeFromCompiler);
  ~BuildDateTime (void);

  bool Init    (const char* pszBuildDateFromCompiler, const char* pszBuildTimeFromCompiler);
  bool IsValid (void) const;

  int64 GetCombined (void) const;

  int GetYear       (void) const;
  int GetMonth      (void) const;
  int GetDayOfMonth (void) const;
  int GetHour       (void) const;
  int GetMinute     (void) const;
  int GetSecond     (void) const;


private :
  // combined
  int64 m_nCombined; // YYYYMMDDhhmmss

  // date
  int m_nYear;        // YYYY
  int m_nMonth;       // [0,12]
  int m_nDayOfMonth;  // [0,31]

  // time
  int m_nHour;    // [0,24]
  int m_nMinute;  // [0,59]
  int m_nSecond;  // [0,59]
};


#include "BuildDateTime.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_BUILDDATETIME)
#endif // #ifndef __nCore_BuildDateTime_Header__
