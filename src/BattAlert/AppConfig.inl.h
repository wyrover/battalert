

//---------------------------------------------------------------------------
inline bool AppConfig::GetLaunchAtStartup (void) const
{
  const Setting* pSetting = this->GetSetting(SETNAME_APP_LAUNCHATSTARTUP);
  XASSERT(pSetting);
  return pSetting->strValue.ExtractBool() > 0;
}
