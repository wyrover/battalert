//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:18:44
//
// $Id$
//
//***************************************************************************

#ifndef __Logger_h__
#define __Logger_h__


//---------------------------------------------------------------------------
// Logger
//---------------------------------------------------------------------------
class Logger
{
public :
  enum Level
  {
    LLDEBUG,
    LLINFO,
    LLWARN,
    LLERROR,
    LLFATAL,
  };


public :
  static void  showNext   (bool bEnable);
  static bool  showNext   (void);
  static void  setOutFile (const char* pszOutputFile);
  static char* outFile    (void);

  static void print          (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...);
  static void throwException (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...);

  // utils
  static char levelToChar (Level eLevel);
  static void showMessage (Level eLevel, const char* pszFile, int iLine, const char* pszFormat, ...);


private :
  static bool ms_bShowNext;
  static char ms_szOutFile[512];
};


#include "Logger.inl.h"


#endif // #ifndef __Logger_h__
