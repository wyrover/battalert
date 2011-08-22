
#include "_Main.h"


//---------------------------------------------------------------------------
// Application DogTags
//---------------------------------------------------------------------------
NCORE_DOGTAG(AppName,    APP_BRAND_NAME);
NCORE_DOGTAG(AppVersion, APP_BRAND_VERSION_STRING);


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
Application* Application::ms_pApplication = NULL;


//---------------------------------------------------------------------------
// Application
//---------------------------------------------------------------------------
Application::Application (HINSTANCE hInstance, const char* pszArgv0)
{
  XASSERT(m_hInstance);
  XASSERT(pszArgv0);

  // pseudo-singleton
  XASSERT(!ms_pApplication);
  ms_pApplication = this;


  // init nCore root object and logger
  nCoreRoot::Init(pszArgv0);


  // application properties
  m_hInstance = hInstance;
  m_Version.Init(APP_BRAND_VERSION_STRING);
  m_strTitle  = StringA(APP_BRAND_NAME " v") + m_Version.GetStrippedString();
#ifdef X_DEBUG
  m_strTitle += " (DEBUG)";
#endif

  // application properties - logger
  m_pLogger = new OwnLogger(g_Core().GetApplicationDirectory(), g_Core().GetApplicationFileName(false), m_strTitle);
  g_Core().BindExternalLogger(m_pLogger);

  // dynamic configuration settings
  m_pAppConfig = NULL;

  // windows / dialogs
  m_pWndMain = NULL;
}

//---------------------------------------------------------------------------
// ~Application
//---------------------------------------------------------------------------
Application::~Application (void)
{
  this->Uninit();

  // logger
  g_Core().BindExternalLogger(NULL);
  XDELETE(m_pLogger);

  // uninit nCore
  nCoreRoot::Uninit();

  // pseudo-singleton
  ms_pApplication = NULL;
}

//---------------------------------------------------------------------------
// Uninit
//---------------------------------------------------------------------------
void Application::Uninit (void)
{
  // windows / dialogs
  XDELETE(m_pWndMain);

  // save config settings and destroy object
  if (m_pAppConfig)
  {
    m_pAppConfig->Save();
    XDELETE(m_pAppConfig);
  }
}

//---------------------------------------------------------------------------
// Init
//---------------------------------------------------------------------------
int Application::Init (int argc, const char** argv)
{
  // dynamic configuration settings
  m_pAppConfig = new AppConfig();
  if (!m_pAppConfig->Load())
    return 1;

  // create windows & dialogs
  m_pWndMain = new WndMain(this);
  m_pWndMain->Open();

  return 0;
}




//---------------------------------------------------------------------------
// Run
//---------------------------------------------------------------------------
int Application::Run (void)
{
  BOOL bRes;
  MSG  msg;

  // windows messages loop
  while ((bRes = ::GetMessage(&msg, NULL, 0, 0)) != 0)
  {
    if (!bRes)
    {
      SHOWSYSERROR NULL, "System loop error !");
      return -1;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  return 0;
}




//---------------------------------------------------------------------------
// GetApplication
//---------------------------------------------------------------------------
Application* Application::GetApplication (void)
{
  return ms_pApplication;
}

//---------------------------------------------------------------------------
// ShowMessage
//---------------------------------------------------------------------------
int Application::ShowMessage (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceMethod, HWND hParentWnd, UINT uiType, const char* pszFormat, ...)
{
  va_list arglist;
  StringA strMessage;

  va_start(arglist, pszFormat);
  strMessage.FormatV(pszFormat, arglist);
  va_end(arglist);

  if ((uiType == MB_ICONERROR) || (uiType == MB_ICONEXCLAMATION) || (uiType == MB_ICONASTERISK))
  {
    LogLevel eLogLevel = (uiType == MB_ICONERROR) ? LLERROR : (uiType == MB_ICONEXCLAMATION) ? LLWARNING : LLINFO;
    nCoreRoot::Log(pszSourceFile, uiSourceLine, pszSourceMethod, eLogLevel, strMessage);
  }

  return ::MessageBox(hParentWnd, strMessage, Application::GetApplication()->GetTitle().c_str(), uiType);
}

//---------------------------------------------------------------------------
// ShowSystemErrorMessage
//---------------------------------------------------------------------------
void Application::ShowSystemErrorMessage (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceMethod, HWND hParentWnd, const char* pszFormat, ...)
{
  va_list arglist;
  StringA strMessage;

  va_start(arglist, pszFormat);
  strMessage.FormatV(pszFormat, arglist);
  va_end(arglist);

  if (System::LastError() != 0)
    strMessage.FormatAppend(" Error (%lu) : %s", System::LastError(), System::LastErrorString());

  Application::ShowMessage(pszSourceFile, uiSourceLine, pszSourceMethod, hParentWnd, MB_ICONERROR, strMessage.c_str());
}
