

//---------------------------------------------------------------------------
inline HINSTANCE Application::GetInstanceHandle (void) const
{
  return m_hInstance;
}

//---------------------------------------------------------------------------
inline const Version3& Application::GetVersion (void) const
{
  return m_Version;
}

//---------------------------------------------------------------------------
inline const StringA& Application::GetTitle (void) const
{
  return m_strTitle;
}

//---------------------------------------------------------------------------
inline AppConfig* Application::GetConfig (void)
{
  return m_pAppConfig;
}

//---------------------------------------------------------------------------
inline WndMain* Application::GetMainWindow (void)
{
  return m_pWndMain;
}
