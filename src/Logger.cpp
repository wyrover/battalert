//***************************************************************************
//
// BattAlert
// A minimalistic battery monitor for Win32.
//
// Copyright (C) 2011-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
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
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:31:28
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// Static Variables
//---------------------------------------------------------------------------
bool Logger::ms_bShowNext   = false;
char Logger::ms_szOutFile[] = { 0 };


//---------------------------------------------------------------------------
char Logger::levelToChar (Level eLevel)
{
  switch (eLevel)
  {
    case LLDEBUG  : return 'd';
    case LLINFO   : return 'i';
    case LLWARN   : return 'W';
    case LLERROR  : return 'E';
    case LLFATAL  : return 'X';
    default       : return '?';
  }
}

//---------------------------------------------------------------------------
void Logger::setOutFile (const char* pszOutputFile)
{
  if (pszOutputFile && *pszOutputFile)
  {
    size_t len = strlen(pszOutputFile);
    strncpy((char*)&ms_szOutFile, pszOutputFile,
      (len + 1 > sizeof(ms_szOutFile) ? sizeof(ms_szOutFile)-1 : len));
  }
  else
  {
    memset(&ms_szOutFile, 0, sizeof(ms_szOutFile));
  }
}

//---------------------------------------------------------------------------
void Logger::print (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...)
{
  time_t  t = time(0);
  tm*     ptm = localtime(&t);
  va_list arglist;
  StringA strMessage;
  StringA strLog;

  // format message if needed
  if (pszFormat && *pszFormat)
  {
    va_start(arglist, pszFormat);
    strMessage.formatV(pszFormat, arglist);
    va_end(arglist);
  }

  // prepare log line
  strLog.format(
    "%04d-%02d-%02d %02d:%02d:%02d [%c] ",
    1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday,
    ptm->tm_hour, ptm->tm_min, ptm->tm_sec,
    Logger::levelToChar(eLevel));
  strLog += strMessage;

  // output to file if we can
  if (ms_szOutFile[0])
  {
    FILE* pFile = fopen((char*)&ms_szOutFile, "a");
    if (pFile)
    {
      fputs(strLog.c_str(), pFile);

#ifdef _DEBUG
      StringA strFileName(pszFile);
      strFileName.pathStripDirectory();
      fprintf(pFile, "  in %s line %i.\n", strFileName.c_str(), iLine);
#else
      fputs("\n", pFile);
#endif

      fclose(pFile);
    }
  }

  // debug output for win32
#if ( defined(_WIN32) || defined(_WIN64) ) && defined(_DEBUG)
  if (pszFile && *pszFile)
    strLog.formatPrepend("%s(%i): ", pszFile, iLine);
  strLog += '\n';
  OutputDebugStringA(strLog.c_str());
#endif

  // message box ?
  if (ms_bShowNext)
  {
    Logger::showMessage(eLevel, pszFile, iLine, strMessage.c_str());
    ms_bShowNext = false;
  }
}

//---------------------------------------------------------------------------
void Logger::throwException (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...)
{
  va_list arglist;
  StringA strMessage;

  if (pszFormat && *pszFormat)
  {
    va_start(arglist, pszFormat);
    strMessage.formatV(pszFormat, arglist);
    va_end(arglist);
  }

  Logger::print(eLevel, pszFile, iLine, strMessage.c_str());
  throw strMessage.c_str();
}

//---------------------------------------------------------------------------
void Logger::showMessage (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...)
{
#if defined(_WIN32) || defined(_WIN64)
  StringA strText;
  UINT    uiType;

  switch (eLevel)
  {
    case LLFATAL :
    case LLERROR :
      uiType = MB_ICONERROR;
      break;

    case LLWARN :
      uiType = MB_ICONEXCLAMATION;
      break;

    default :
      uiType = MB_ICONINFORMATION;
      break;
  }

#ifdef _DEBUG
  if (pszFile && *pszFile)
  {
    StringA strFileName(pszFile);
    strFileName.pathStripDirectory();
    strText.formatPrepend("%s at line %i:\n", strFileName.c_str(), iLine);
  }
#endif

  {
    va_list arglist;
    StringA strFormat;

    if (pszFormat && *pszFormat)
    {
      va_start(arglist, pszFormat);
      strFormat.formatV(pszFormat, arglist);
      va_end(arglist);
    }

    strText += strFormat;
  }

  MessageBoxA(NULL, strText.c_str(), App::name().c_str(), uiType);
#endif
}
