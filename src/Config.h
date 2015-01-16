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
// Created On: 2013-01-11 17:08:00
//
//***************************************************************************

#ifndef __Config_h__
#define __Config_h__


//---------------------------------------------------------------------------
// Config
//---------------------------------------------------------------------------
class Config
{
public :
  static void setIniFile (const char* pszIniFile);

  static void load (void);
  static void save (bool bShowErrorMsgBox);

  static bool validateThreshold    (int* pnThreshold);
  static bool validateFile         (StringA& strFile);
  static bool applyLaunchAtStartup (bool bShowErrorMsgBox);


private :
  static void    iniWrite         (const char* pszName, bool b);
  static void    iniWrite         (const char* pszName, int i);
  static void    iniWrite         (const char* pszName, const char* pszValue);
  static void    iniWriteFailed   (const char* pszName, UINT uiLastError, const char* pszLastError);
  static bool    iniReadBool      (const char* pszName, bool bDefault);
  static int     iniReadThreshold (const char* pszName, int iDefault);
  static void    iniReadString    (const char* pszName, StringA& strOutput);


public :
  static StringA strIniFile;

  static bool bLaunchAtStartup;

  static bool bIconBlinking;
  static int  nIconBlinkingThreshold;

  static int     nAlertThreshold;
  static bool    bAlertMessageBox;
  static bool    bAlertSound;
  static StringA strAlertSoundFile;
};


#include "Config.inl.h"


#endif // #ifndef __Config_h__
