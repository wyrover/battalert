
#ifndef __AppConfig_Header__
#define __AppConfig_Header__


//---------------------------------------------------------------------------
// AppConfig
//---------------------------------------------------------------------------
class AppConfig
{
private :
  static const StringA REGNAME_APP_INSTALLDIR;
  static const StringA REGNAME_APP_INSTALLVERSION;

  static const StringA SETNAME_APP_VERSION;
  static const StringA SETNAME_APP_LAUNCHATSTARTUP;

  static const dword SETFLAG_BLANK;
  static const dword SETFLAG_COMMENT;
  static const dword SETFLAG_VALUE;
  static const dword SETFLAG_REMOVE;

  struct Setting
  {
    dword   dwFlags;  // see the defined SETFLAG_* constants
    StringA strKey;
    StringA strValue;
  };

  typedef std::map<StringA,Setting*> Settings;
  typedef Settings::iterator         SettingsIt;
  typedef Settings::const_iterator   SettingsCIt;


public :
  AppConfig (void);
  ~AppConfig (void);

  // actions
  bool Load                 (void);
  bool Save                 (void);
  void RestoreDefaultValues (void);

  // settings getters
  bool GetLaunchAtStartup (void) const;
  // TODO

  // settings setters
  void SetLaunchAtStartup (bool bLaunchAtStartup);
  // TODO


private :
  void           CreateOriginalConfigFile (void);
  Setting*       GetSetting               (const StringA& strKey);
  const Setting* GetSetting               (const StringA& strKey) const;
  void           ClearSettingsMap         (void);

  // environment configuration
  bool SyncToRegistry_InstallationData (void);
  bool SyncToRegistry_LaunchAtStartup  (void);


private :
  // application data
  StringA m_strInstallDir;
  StringA m_strConfigFilePath;

  // configuration data
  Settings m_Settings;
};


#include "AppConfig.inl.h"


#endif // #ifndef __AppConfig_Header__
