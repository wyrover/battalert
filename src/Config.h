//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-11 17:08:00
//
// $Id$
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
  static void save (void);

  static bool validateThreshold (int* pnThreshold);
  static bool validateFile      (StringA& strFile);


private :
  static void    iniWrite         (const char* pszName, bool b);
  static void    iniWrite         (const char* pszName, int i);
  static void    iniWrite         (const char* pszName, const char* pszValue);
  static void    iniWriteFailed   (const char* pszName, UINT uiLastError, const char* pszLastError);
  static bool    iniReadBool      (const char* pszName, bool bDefault);
  static int     iniReadThreshold (const char* pszName, int iDefault);
  static StringA iniReadFilePath  (const char* pszName);
  static void    iniReadString    (const char* pszName, StringA& strOutput);


public :
  static StringA strIniFile;

  static bool bIconBlinking;
  static int  nIconBlinkingThreshold;

  static int     nAlertThreshold;
  static bool    bAlertMessageBox;
  static bool    bAlertSound;
  static StringA strAlertSoundFile;
};


#include "Config.inl.h"


#endif // #ifndef __Config_h__
