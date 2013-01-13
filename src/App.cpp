//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-09 19:14:05
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// Static Variables
//---------------------------------------------------------------------------
App* App::ms_pSingleton = 0;


//---------------------------------------------------------------------------
App::App (HINSTANCE hInstance)
: m_hInstance(hInstance)
, m_pWndMain(0)
{
  DOASSERT(!ms_pSingleton);
  ms_pSingleton = this;

  // get current exe file path and remove its extension part
  if (!::GetModuleFileNameA(::GetModuleHandleA(0), m_strExePath.acquireBuffer(MAX_PATH, false), MAX_PATH))
    THROWEX("GetModuleFileName() failed! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
  m_strExePath.releaseBuffer();
}

//---------------------------------------------------------------------------
App::~App (void)
{
  ms_pSingleton = 0;
}



//---------------------------------------------------------------------------
void App::uninit (void)
{
  Config::applyLaunchAtStartup(false);
  delete m_pWndMain;
  m_pWndMain = 0;
}

//---------------------------------------------------------------------------
int App::init (int argc, char** argv)
{
  StringA strExePathNoExt;

  InitCommonControls();

  strExePathNoExt = m_strExePath;
  strExePathNoExt.pathStripExtension();

  // setup logger's output file
  Logger::setOutFile(strExePathNoExt + ".log");

  // load config
  Config::setIniFile(strExePathNoExt + ".ini");
  Config::load();

  // main window
  m_pWndMain = new WndMain();
  m_pWndMain->open();

  return 0;
}

//---------------------------------------------------------------------------
int App::run (void)
{
  BOOL bRes;
  MSG  msg;

  // windows messages loop
  while ((bRes = ::GetMessage(&msg, NULL, 0, 0)) != 0)
  {
    if (!bRes)
    {
      MSGBOX_ERR("System loop error !");
      return -1;
    }

    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }

  return 0;
}



//---------------------------------------------------------------------------
const StringA& App::name (void)
{
  static const StringA str(APP_NAME);
  return str;
}

//---------------------------------------------------------------------------
unsigned App::sysLastError (void)
{
  return ::GetLastError();
}

//---------------------------------------------------------------------------
const char* App::sysLastErrorString (void)
{
  return App::sysGetErrorString(App::sysLastError());
}

//---------------------------------------------------------------------------
const char* App::sysGetErrorString (unsigned uiOsError)
{
  // CAUTION: this code is not thread-safe!

  static char szError[1024];

  DWORD dwRes = ::FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
    0, uiOsError, 0, (char*)&szError, sizeof(szError), 0);

  if (dwRes == 0)
  {
    szError[0] = 0;
    LOGERR("Error while calling FormatMessage() ! Error %u.", ::GetLastError());
  }
  else
  {
    DOASSERT(dwRes == (DWORD)strlen((char*)&szError));

    if ((szError[dwRes-2] == '\n') || (szError[dwRes-2] == '\r'))
      szError[dwRes-2] = '\0';
    else if ((szError[dwRes-1] == '\n') || (szError[dwRes-1] == '\r'))
      szError[dwRes-1] = '\0';
  }

  return (const char*)&szError;
}
