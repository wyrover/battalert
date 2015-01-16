//***************************************************************************
//
// BattAlert
// A minimalistic battery monitor for Win32.
//
// Copyright 2011-2013 Jean-Charles Lefebvre <polyvertex@gmail.com>
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
// Created On: 2011-10-07 18:18:44
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
