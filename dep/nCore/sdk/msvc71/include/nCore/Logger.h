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

#ifndef __nCore_Logger_Header__
#define __nCore_Logger_Header__
namespace nCore {


//---------------------------------------------------------------------------
// LogLevel
//---------------------------------------------------------------------------
enum LogLevel
{
  LLTEST      = 1 << 0,
  LLDEBUG     = 1 << 1,
  LLINFO      = 1 << 2,
  LLWARNING   = 1 << 3,
  LLERROR     = 1 << 4,
  LLASSERTION = 1 << 5,
  LLFATAL     = 1 << 6,

  LLALL = 0xFFFFFFFF, // used as a filter
};


//---------------------------------------------------------------------------
// Logger
//---------------------------------------------------------------------------
class NCORE_EXPORT Logger
{
public :
  Logger (void);
  virtual ~Logger (void);

  // log-level filtering
  virtual void  SetFilter (dword dwAllowedLogLevels=dword(LLALL));
  virtual dword GetFilter (void) const;

  // print
  virtual void operator() (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const char* pszFormat, ...);
  virtual void operator() (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const StringA& strMessage);
  virtual void Print      (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const char* pszMessage, ...);
  virtual void Print      (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const StringA& strMessage) = 0;


  // static tools
  static char MapLogLevelToChar (LogLevel eLevel);


protected :
  dword m_dwLogLevelFilter;
};


#include "Logger.inl.h"


}
#endif // #ifndef __nCore_Logger_Header__
