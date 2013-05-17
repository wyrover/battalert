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
// Created On : 2013-01-11 17:08:02
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
static const char* c_pszIniSection                  = "main";
static const char* c_pszValue_LaunchAtStartup       = "LaunchAtStartup";
static const char* c_pszValue_IconBlinkingEnabled   = "IconBlinkingEnabled";
static const char* c_pszValue_IconBlinkingThreshold = "IconBlinkingThreshold";
static const char* c_pszValue_AlertThreshold        = "AlertThreshold";
static const char* c_pszValue_AlertMsgBoxEnabled    = "AlertMsgBoxEnabled";
static const char* c_pszValue_AlertSoundEnabled     = "AlertSoundEnabled";
static const char* c_pszValue_AlertSoundFile        = "AlertSoundFile";


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
StringA Config::strIniFile;

bool    Config::bLaunchAtStartup       = true;
bool    Config::bIconBlinking          = true;
int     Config::nIconBlinkingThreshold = ALERT_BLINK_PERCENT;
int     Config::nAlertThreshold        = ALERT_SOUND_PERCENT;
bool    Config::bAlertMessageBox       = true;
bool    Config::bAlertSound            = true;
StringA Config::strAlertSoundFile;



//---------------------------------------------------------------------------
void Config::setIniFile (const char* pszIniFile)
{
  Config::strIniFile = pszIniFile;
}

//---------------------------------------------------------------------------
void Config::load (void)
{
  // read values if ini file is found
  if (GetFileAttributes(Config::strIniFile) != INVALID_FILE_ATTRIBUTES)
  {
    Config::bLaunchAtStartup       = Config::iniReadBool(c_pszValue_LaunchAtStartup, Config::bLaunchAtStartup);
    Config::bIconBlinking          = Config::iniReadBool(c_pszValue_IconBlinkingEnabled, Config::bIconBlinking);
    Config::nIconBlinkingThreshold = Config::iniReadThreshold(c_pszValue_IconBlinkingThreshold, Config::nIconBlinkingThreshold);
    Config::nAlertThreshold        = Config::iniReadThreshold(c_pszValue_AlertThreshold, Config::nAlertThreshold);
    Config::bAlertMessageBox       = Config::iniReadBool(c_pszValue_AlertMsgBoxEnabled, Config::bAlertMessageBox);
    Config::bAlertSound            = Config::iniReadBool(c_pszValue_AlertSoundEnabled, Config::bAlertSound);
    Config::iniReadString(c_pszValue_AlertSoundFile, Config::strAlertSoundFile);
  }

  Config::applyLaunchAtStartup(false);
}

//---------------------------------------------------------------------------
void Config::save (bool bShowErrorMsgBox)
{
  // delete ini file to ensure we generate a clean config
  if (GetFileAttributes(Config::strIniFile) != INVALID_FILE_ATTRIBUTES)
    ::remove(Config::strIniFile);

  // write values
  Config::iniWrite(c_pszValue_LaunchAtStartup, Config::bLaunchAtStartup);
  Config::iniWrite(c_pszValue_IconBlinkingEnabled, Config::bIconBlinking);
  Config::iniWrite(c_pszValue_IconBlinkingThreshold, Config::nIconBlinkingThreshold);
  Config::iniWrite(c_pszValue_AlertThreshold, Config::nAlertThreshold);
  Config::iniWrite(c_pszValue_AlertMsgBoxEnabled, Config::bAlertMessageBox);
  Config::iniWrite(c_pszValue_AlertSoundEnabled, Config::bAlertSound);
  Config::iniWrite(c_pszValue_AlertSoundFile, Config::strAlertSoundFile);

  Config::applyLaunchAtStartup(bShowErrorMsgBox);
}



//---------------------------------------------------------------------------
bool Config::validateThreshold (int* pnThreshold)
{
  if (*pnThreshold < 1)
  {
    *pnThreshold = 1;
    return false;
  }
  else if (*pnThreshold > 99)
  {
    *pnThreshold = 90;
    return false;
  }

  return true;
}

//---------------------------------------------------------------------------
bool Config::validateFile (StringA& strFile)
{
  if (!strFile.isEmpty())
  {
    DWORD dwAttr = GetFileAttributes(strFile.c_str());
    if (dwAttr == INVALID_FILE_ATTRIBUTES)
    {
      strFile.clear(); // file not found
      return false;
    }
  }

  return true;
}

//---------------------------------------------------------------------------
bool Config::applyLaunchAtStartup (bool bShowErrorMsgBox)
{
  const StringA c_strKeyPathStartup = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  const StringA c_strKeyNameStartup = APP_NAME;

  HKEY  hKey;
  LONG  nRes;
  DWORD dwDisposition = 0;

  // open registry key
  nRes = RegCreateKeyEx(HKEY_CURRENT_USER, c_strKeyPathStartup.c_str(), 0,
    NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
  if (nRes != ERROR_SUCCESS)
  {
    if (bShowErrorMsgBox)
      Logger::showNext(true);
    LOGERR("Error while openning Registry key ! Error %d: %s",
      c_strKeyPathStartup.c_str(), nRes, App::sysLastErrorString());
    return false;
  }

  // write/delete value according to our configuration data
  if (Config::bLaunchAtStartup)
  {
    StringA strStartupValue("\"" + g_pApp->exePath() + "\" --autostart");

    nRes = RegSetValueEx(hKey, c_strKeyNameStartup.c_str(), 0, REG_SZ,
      (const BYTE*)strStartupValue.c_str(), DWORD(strStartupValue.length()+1));
    if (nRes != ERROR_SUCCESS)
    {
      if (bShowErrorMsgBox)
        Logger::showNext(true);
      LOGERR("Error while writing into the Registry (key: HKEY_CURRENT_USER\\%s\\%s)! Error %d: %s",
        c_strKeyPathStartup.c_str(), c_strKeyNameStartup.c_str(), nRes, App::sysLastErrorString());
      RegCloseKey(hKey);
      return false;
    }
  }
  else
  {
    nRes = RegDeleteValue(hKey, c_strKeyNameStartup.c_str());
    if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_FILE_NOT_FOUND))
    {
      if (bShowErrorMsgBox)
        Logger::showNext(true);
      LOGERR("Error while removing key from the Registry (key: HKEY_CURRENT_USER\\%s\\%s)! Error %d: %s",
        c_strKeyPathStartup.c_str(), c_strKeyNameStartup.c_str(), nRes, App::sysLastErrorString());
      RegCloseKey(hKey);
      return false;
    }
  }

  // close registry key
  RegCloseKey(hKey);

  return true;
}



//---------------------------------------------------------------------------
void Config::iniWrite (const char* pszName, bool b)
{
  if (!WritePrivateProfileString(c_pszIniSection, pszName, (b ? "true" : "false"), Config::strIniFile))
    Config::iniWriteFailed(pszName, App::sysLastError(), App::sysLastErrorString());
}

//---------------------------------------------------------------------------
void Config::iniWrite (const char* pszName, int i)
{
  StringA str(i);
  if (!WritePrivateProfileString(c_pszIniSection, pszName, str.c_str(), Config::strIniFile))
    Config::iniWriteFailed(pszName, App::sysLastError(), App::sysLastErrorString());
}

//---------------------------------------------------------------------------
void Config::iniWrite (const char* pszName, const char* pszValue)
{
  if (!WritePrivateProfileString(c_pszIniSection, pszName, pszValue, Config::strIniFile))
    Config::iniWriteFailed(pszName, App::sysLastError(), App::sysLastErrorString());
}

//---------------------------------------------------------------------------
void Config::iniWriteFailed (const char* pszName, UINT uiLastError, const char* pszLastError)
{
  THROWEX("Failed to write value \"%s\" into INI file \"%s\"! Error %u: %s",
    pszName, Config::strIniFile.c_str(), uiLastError, pszLastError);
}

//---------------------------------------------------------------------------
bool Config::iniReadBool (const char* pszName, bool bDefault)
{
  StringA str;

  Config::iniReadString(pszName, str);
  if (str.isEmpty())
  {
    return bDefault;
  }
  else
  {
    int nRes = str.extractBool();
    return (nRes == -1) ? bDefault : (nRes != 0);
  }
}

//---------------------------------------------------------------------------
int Config::iniReadThreshold (const char* pszName, int iDefault)
{
  StringA str;

  Config::iniReadString(pszName, str);
  if (str.isEmpty() || !str.isFullOfDigits())
  {
    return iDefault;
  }
  else
  {
    int nRes = str.extractInt32();
    Config::validateThreshold(&nRes);
    return nRes;
  }
}

//---------------------------------------------------------------------------
void Config::iniReadString (const char* pszName, StringA& strOutput)
{
  strOutput.clear();
  GetPrivateProfileString(c_pszIniSection, pszName, "",
    strOutput.acquireBuffer(1024, false), 1024-1, Config::strIniFile);
  strOutput.releaseBuffer();
  strOutput.trimWhitespaces();
}
