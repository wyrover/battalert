//***************************************************************************
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
  if (GetFileAttributes(Config::strIniFile) == INVALID_FILE_ATTRIBUTES)
    return; // ini file not found

  // read values
  Config::bIconBlinking          = Config::iniReadBool(c_pszValue_IconBlinkingEnabled, Config::bIconBlinking);
  Config::nIconBlinkingThreshold = Config::iniReadThreshold(c_pszValue_IconBlinkingThreshold, Config::nIconBlinkingThreshold);
  Config::nAlertThreshold        = Config::iniReadThreshold(c_pszValue_AlertThreshold, Config::nAlertThreshold);
  Config::bAlertMessageBox       = Config::iniReadBool(c_pszValue_AlertMsgBoxEnabled, Config::bAlertMessageBox);
  Config::bAlertSound            = Config::iniReadBool(c_pszValue_AlertSoundEnabled, Config::bAlertSound);
  Config::strAlertSoundFile      = Config::iniReadFilePath(c_pszValue_AlertSoundFile);
}

//---------------------------------------------------------------------------
void Config::save (void)
{
  // delete ini file to ensure we generate a clean config
  if (GetFileAttributes(Config::strIniFile) != INVALID_FILE_ATTRIBUTES)
    ::remove(Config::strIniFile);

  // write values
  Config::iniWrite(c_pszValue_IconBlinkingEnabled, Config::bIconBlinking);
  Config::iniWrite(c_pszValue_IconBlinkingThreshold, Config::nIconBlinkingThreshold);
  Config::iniWrite(c_pszValue_AlertThreshold, Config::nAlertThreshold);
  Config::iniWrite(c_pszValue_AlertMsgBoxEnabled, Config::bAlertMessageBox);
  Config::iniWrite(c_pszValue_AlertSoundEnabled, Config::bAlertSound);
  Config::iniWrite(c_pszValue_AlertSoundFile, Config::strAlertSoundFile);
}



//---------------------------------------------------------------------------
bool Config::adjustThreshold (int* pnThreshold)
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
bool Config::adjustFile (StringA& strFile)
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
  str.trimWhitespaces();
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
  str.trimWhitespaces();
  if (str.isEmpty() || !str.isFullOfDigits())
  {
    return iDefault;
  }
  else
  {
    int nRes = str.extractInt32();
    Config::adjustThreshold(&nRes);
    return nRes;
  }
}

//---------------------------------------------------------------------------
StringA Config::iniReadFilePath (const char* pszName)
{
  StringA str;

  Config::iniReadString(pszName, str);
  str.trimWhitespaces();
  if (GetFileAttributes(str.c_str()) == INVALID_FILE_ATTRIBUTES)
    str.clear();

  return str;
}

//---------------------------------------------------------------------------
void Config::iniReadString (const char* pszName, StringA& strOutput)
{
  strOutput.clear();
  GetPrivateProfileString(c_pszIniSection, pszName, "",
    strOutput.acquireBuffer(1024, false), 1024-1, Config::strIniFile);
  strOutput.releaseBuffer();
}
