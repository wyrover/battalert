
#include "_Main.h"


//---------------------------------------------------------------------------
// Constant Members
//---------------------------------------------------------------------------
// registry key names
const StringA AppConfig::REGNAME_APP_INSTALLDIR     = "InstallPath";
const StringA AppConfig::REGNAME_APP_INSTALLVERSION = "InstallVersion";

// settings names
const StringA AppConfig::SETNAME_APP_VERSION         = "AppVersion";
const StringA AppConfig::SETNAME_APP_LAUNCHATSTARTUP = "AppLaunchAtStartup";
// TODO

// settings flags
const dword AppConfig::SETFLAG_BLANK   = 0x00000001;
const dword AppConfig::SETFLAG_COMMENT = 0x00000002;
const dword AppConfig::SETFLAG_VALUE   = 0x00000004;
const dword AppConfig::SETFLAG_REMOVE  = 0x00010000;


//---------------------------------------------------------------------------
// AppConfig
//---------------------------------------------------------------------------
AppConfig::AppConfig (void)
{
  // application data
  {
    // install directory
    m_strInstallDir = g_Core().GetApplicationDirectory();
    m_strInstallDir.PathStripTrailingSeparator();

    // configuration file path
    m_strConfigFilePath = m_strInstallDir + "/" + g_Core().GetApplicationFileName(false) + ".conf";
  }

  // init default configuration values
  this->RestoreDefaultValues();

  // be sure the registry is being up to date
  this->SyncToRegistry_InstallationData();
  this->SyncToRegistry_LaunchAtStartup();
}

//---------------------------------------------------------------------------
// ~AppConfig
//---------------------------------------------------------------------------
AppConfig::~AppConfig (void)
{
  this->ClearSettingsMap();
}


//---------------------------------------------------------------------------
// RestoreDefaultValues
//---------------------------------------------------------------------------
void AppConfig::RestoreDefaultValues (void)
{
  Setting* pSetting;

  this->ClearSettingsMap();

  // application - version
  pSetting = new Setting();
  pSetting->dwFlags  = SETFLAG_VALUE;
  pSetting->strKey   = SETNAME_APP_VERSION;
  pSetting->strValue = APP_BRAND_VERSION_STRING;
  m_Settings.insert(Settings::value_type(pSetting->strKey, pSetting));

  // application - launch at startup
  pSetting = new Setting();
  pSetting->dwFlags  = SETFLAG_VALUE;
  pSetting->strKey   = SETNAME_APP_LAUNCHATSTARTUP;
  pSetting->strValue = "yes";
  m_Settings.insert(Settings::value_type(pSetting->strKey, pSetting));
#ifdef X_DEBUG
  pSetting->strValue = "no";
#endif
}

//---------------------------------------------------------------------------
// Load
//---------------------------------------------------------------------------
bool AppConfig::Load (void)
{
  //Setting* pSetting;

  // if config file does not already exist create it first
  if (!FileSystem::DoesExist(m_strConfigFilePath.c_str()))
  {
#ifdef X_DEBUG
    CORELOG LLDEBUG, "Configuration file \"%s\" doesn't exist : creating a new one with current values...", m_strConfigFilePath.c_str());
#endif
    this->CreateOriginalConfigFile();
  }

  // load configuration data from file
  {
    ConfigFile cfg;

    // load configuration file
    if (cfg.Load(m_strConfigFilePath) == 0)
      XTHROW EXCODE_SYSTEM_GENERIC, "Unable to load configuration file \"%s\" ! Error : %s", m_strConfigFilePath.c_str(), cfg.GetError().c_str());

    // gather values
    for (SettingsIt itSetting=m_Settings.begin() ; itSetting!=m_Settings.end() ; itSetting++)
    {
      Setting& set = *itSetting->second;

      if (!(set.dwFlags & SETFLAG_VALUE)) continue;

      if (set.strKey == SETNAME_APP_VERSION)
      {
        StringA strValue = cfg.GetValueAsString(set.strKey, set.strValue);
        if (strValue != set.strValue)
        {
          // if needed, handle special case when application has
          // been updated but not the configuration file yet.
        }
      }
      else
      {
        set.strValue = cfg.GetValueAsString(set.strKey, set.strValue);
      }
    }
  }


  // check loaded configuration values
  //
  // TODO
  //
  //pSetting = const_cast<Setting*>(this->GetSetting(SETNAME_APP_LAUNCHATSTARTUP));
  //if (pSetting->strValue.ToBool() != MY_DEFAULT_VALUE)
  //  pSetting->strValue.FromBool(MY_DEFAULT_VALUE);


  // be sure the registry is being up to date
  //this->SyncToRegistry_InstallationData();
  this->SyncToRegistry_LaunchAtStartup();

  return true;
}

//---------------------------------------------------------------------------
// Save
//---------------------------------------------------------------------------
bool AppConfig::Save (void)
{
  ConfigFile cfg;

  // if config file does not already exist create it first
  if (!FileSystem::DoesExist(m_strConfigFilePath.c_str()))
  {
    XASSERT(0); // should never get here since the Load() method must be called before
    this->CreateOriginalConfigFile();
  }

  // load configuration file so we can update its values
  if (cfg.Load(m_strConfigFilePath) == 0)
    XTHROW EXCODE_SYSTEM_GENERIC, "Unable to load configuration file \"%s\" ! Error : %s", m_strConfigFilePath.c_str(), cfg.GetError().c_str());

  // update values
  for (SettingsCIt itSetting=m_Settings.begin() ; itSetting!=m_Settings.end() ; itSetting++)
  {
    const Setting& set = *itSetting->second;

    if (set.dwFlags & SETFLAG_VALUE)
    {
      if (set.dwFlags & SETFLAG_REMOVE)
      {
        cfg.RemoveValue(set.strKey);
      }
      else
      {
        if (!cfg.HasValue(set.strKey))
          cfg.AddValue(set.strKey, set.strValue);

        cfg.ChangeValue(set.strKey, set.strValue);
      }
    }
  }

  // write configuration file
  if (!cfg.Save())
  {
    CORELOG LLERROR, "Error while saving configuration file \"%s\" ! Error : %s", m_strConfigFilePath.c_str(), cfg.GetError().c_str());
    return false;
  }

  return true;
}


//---------------------------------------------------------------------------
// CreateOriginalConfigFile
//---------------------------------------------------------------------------
void AppConfig::CreateOriginalConfigFile (void)
{
  FILE* hFile;

  hFile = fopen(m_strConfigFilePath.c_str(), "wt");
  if (!hFile) XTHROW EXCODE_SYSTEM_GENERIC, "Unable to create configuration file \"%s\" ! Error : %s", m_strConfigFilePath.c_str(), System::LastErrorString());

  for (SettingsCIt itSetting=m_Settings.begin() ; itSetting!=m_Settings.end() ; itSetting++)
  {
    const Setting& set = *itSetting->second;

    if ((set.dwFlags & SETFLAG_VALUE) && !set.strValue.IsEmpty())
      fprintf(hFile, "%s  %s\n", set.strKey.c_str(), set.strValue.c_str());
  }

  fclose(hFile);
}

//---------------------------------------------------------------------------
// GetSetting
//---------------------------------------------------------------------------
AppConfig::Setting* AppConfig::GetSetting (const StringA& strKey)
{
  SettingsIt itSetting = m_Settings.find(strKey);
  if (itSetting == m_Settings.end()) return NULL;
  return itSetting->second;
}

//---------------------------------------------------------------------------
// GetSetting
//---------------------------------------------------------------------------
const AppConfig::Setting* AppConfig::GetSetting (const StringA& strKey) const
{
  SettingsCIt itSetting = m_Settings.find(strKey);
  if (itSetting == m_Settings.end()) return NULL;
  return itSetting->second;
}

//---------------------------------------------------------------------------
// ClearSettingsMap
//---------------------------------------------------------------------------
void AppConfig::ClearSettingsMap (void)
{
  for (SettingsIt itSetting=m_Settings.begin() ; itSetting!=m_Settings.end() ; itSetting++)
    delete itSetting->second;
  m_Settings.clear();
}

//---------------------------------------------------------------------------
// SyncToRegistry_InstallationData
//---------------------------------------------------------------------------
bool AppConfig::SyncToRegistry_InstallationData (void)
{
  /*
  const StringA c_strKeyPath = "Software\\" APP_BRAND_NAME;

  HKEY    hKey;
  LONG    nRes;
  DWORD   dwDisposition = 0;
  StringA strTemp;


  // open registry key
  nRes = RegCreateKeyEx(HKEY_CURRENT_USER, c_strKeyPath.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
  if (nRes != ERROR_SUCCESS)
  {
    CORELOG LLERROR, "Error while openning the Registry ! Error %d : %s", nRes, System::LastErrorString());
    return false;
  }


  // write - install path
  nRes = RegSetValueEx(hKey, AppConfig::REGNAME_APP_INSTALLDIR.c_str(), 0, REG_SZ, (const byte*)m_strInstallDir.c_str(), DWORD(m_strInstallDir.length()+1));
  if (nRes != ERROR_SUCCESS)
  {
    CORELOG LLERROR, "Error while writing into the Registry ! Error %d : %s", nRes, System::LastErrorString());
    RegCloseKey(hKey);
    return false;
  }

  // write - install version
  strTemp = Application::GetApplication()->GetVersion().GetString();
  nRes = RegSetValueEx(hKey, AppConfig::REGNAME_APP_INSTALLVERSION.c_str(), 0, REG_SZ, (const byte*)strTemp.c_str(), DWORD(strTemp.length()+1));
  if (nRes != ERROR_SUCCESS)
  {
    CORELOG LLERROR, "Error while writing into the Registry ! Error %d : %s", nRes, System::LastErrorString());
    RegCloseKey(hKey);
    return false;
  }


  // close registry key
  RegCloseKey(hKey);
  */

  return true;
}

//---------------------------------------------------------------------------
// SyncToRegistry_LaunchAtStartup
//---------------------------------------------------------------------------
bool AppConfig::SyncToRegistry_LaunchAtStartup (void)
{
  /*
  const StringA c_strKeyPathStartup = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  const StringA c_strKeyNameStartup = APP_BRAND_NAME;

  HKEY  hKey;
  LONG  nRes;
  DWORD dwDisposition = 0;

  // open registry key
  nRes = RegCreateKeyEx(HKEY_CURRENT_USER, c_strKeyPathStartup.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition);
  if (nRes != ERROR_SUCCESS)
  {
    CORELOG LLERROR, "Error while openning the Registry ! Error %d : %s", nRes, System::LastErrorString());
    return false;
  }

  // write/delete value according to our configuration data
  if (this->GetLaunchAtStartup())
  {
    StringA strStartupValue;
    strStartupValue = "\"" + g_Core().GetApplicationPath() + "\" --autostart";

    nRes = RegSetValueEx(hKey, c_strKeyNameStartup.c_str(), 0, REG_SZ, (const byte*)strStartupValue.c_str(), DWORD(strStartupValue.length()+1));
    if (nRes != ERROR_SUCCESS)
    {
      CORELOG LLERROR, "Error while writing into the Registry ! Error %d : %s", nRes, System::LastErrorString());
      RegCloseKey(hKey);
      return false;
    }
  }
  else
  {
    nRes = RegDeleteValue(hKey, c_strKeyNameStartup.c_str());
    if ((nRes != ERROR_SUCCESS) && (nRes != ERROR_FILE_NOT_FOUND))
    {
      CORELOG LLERROR, "Error while removing key \"%s\" from the Registry ! Error %d : %s", c_strKeyNameStartup.c_str(), nRes, System::LastErrorString());
      RegCloseKey(hKey);
      return false;
    }
  }

  // close registry key
  RegCloseKey(hKey);
  */

  return true;
}



//---------------------------------------------------------------------------
// Settings Setters
//---------------------------------------------------------------------------
void AppConfig::SetLaunchAtStartup (bool bLaunchAtStartup)
{
  Setting* pSetting = this->GetSetting(SETNAME_APP_LAUNCHATSTARTUP);
  XASSERT(pSetting);
  pSetting->strValue = bLaunchAtStartup ? "yes" : "no";

  this->SyncToRegistry_LaunchAtStartup();
}
