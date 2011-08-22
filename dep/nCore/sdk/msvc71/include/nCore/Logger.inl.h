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


//---------------------------------------------------------------------------
inline Logger::Logger (void)
{
  m_dwLogLevelFilter = LLALL;
}

//---------------------------------------------------------------------------
inline Logger::~Logger (void)
{
}

//---------------------------------------------------------------------------
inline void Logger::SetFilter (dword dwAllowedLogLevels/*=dword(LLALL)*/)
{
  m_dwLogLevelFilter = dwAllowedLogLevels;
}

//---------------------------------------------------------------------------
inline dword Logger::GetFilter (void) const
{
  return m_dwLogLevelFilter;
}

//---------------------------------------------------------------------------
inline void Logger::operator() (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const StringA& strMessage)
{
  // filter message
  if(!(m_dwLogLevelFilter & (dword)eLevel))
    return;

  // print
  this->Print(pszSourceFile, uiSourceLine, pszSourceFunction, eLevel, strMessage);
}

//---------------------------------------------------------------------------
inline char Logger::MapLogLevelToChar (LogLevel eLevel)
{
  switch(eLevel)
  {
    case LLTEST      : return '.';
    case LLDEBUG     : return 'd';
    case LLINFO      : return 'i';
    case LLWARNING   : return 'W';
    case LLERROR     : return 'E';
    case LLASSERTION : return 'A';
    case LLFATAL     : return 'X';

    default :
      XTHROW EXCODE_UNKNOWN, "Unknown LogLevel value : %u !", (uint)eLevel);
  }
}
