//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-10 11:06:25
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define SYSTRAYICON_ID  1

#define TIMER_POWERPOLL                  1
#define TIMER_POWERPOLL_DELAYMS          (15 * 1000)
#define TIMER_POWERPOLL_DELAYMS_ONERROR  (3 * 1000) // must be faster than TIMER_POWERPOLL_DELAYMS

#define TIMER_STOPALARM                   2
#define TIMER_STOPALARM_DELAYMS           (20 * 1000)
#define TIMER_STOPALARM_DELAYMS_PLAYTEST  (8 * 1000)


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
WndMain* WndMain::ms_pThis = 0;


//---------------------------------------------------------------------------
WndMain::WndMain (void)
: Wnd()
{
  // pseudo-singleton
  DOASSERT(!ms_pThis);
  ms_pThis = this;

  // init members
  m_hMenuMainPopup = 0;
  m_uiLastPollTick = 0;
  m_hWinmmDll      = 0;
  m_pfnPlaySound   = 0;
  m_bSoundPlaying  = false;

  // create window's class
  {
    WNDCLASSEX wc;
    HINSTANCE  hInst = g_pApp->instance();

    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)wndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APP));
    wc.hIconSm       = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = APP_UNIQUE_WNDCLASS_MAIN;

    if (!RegisterClassEx(&wc))
      THROWEX("Error while calling RegisterClass() for main window! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
  }
}

//---------------------------------------------------------------------------
WndMain::~WndMain (void)
{
  this->stopAlarm();
  this->destroy();

  UnregisterClass(APP_UNIQUE_WNDCLASS_MAIN, g_pApp->instance());

  ms_pThis = 0;
}



//---------------------------------------------------------------------------
void WndMain::open (void)
{
  // create the main popup menu (it will be assigned to the icon of the system tray)
  m_hMenuMainPopup = GetSubMenu(LoadMenu(g_pApp->instance(), MAKEINTRESOURCE(IDM_POPUP_MAIN)), 0);
  if (!m_hMenuMainPopup)
    THROWEX("Unable to create the main popup menu! Error %u: %s", App::sysLastError(), App::sysLastErrorString());

  // prepend the application title to the popup menu
  {
    MENUITEMINFO mii;
    StringA str(App::title());

    str += " (" __DATE__ " " __TIME__ ")";
    mii.cbSize     = sizeof(MENUITEMINFO);
    mii.fMask      = MIIM_FTYPE | MIIM_STRING | MIIM_STATE;
    mii.fType      = MFT_STRING;
    mii.fState     = MFS_DISABLED;
    mii.dwTypeData = (char*)str.c_str();
    mii.cch        = str.length();

    InsertMenuItem(m_hMenuMainPopup, 0, TRUE, &mii);
  }

  // create a message-only window
  m_hWnd = CreateWindowEx(0, APP_UNIQUE_WNDCLASS_MAIN, "Main", 0, 0, 0, 0, 0, NULL, NULL, g_pApp->instance(), NULL);
  if (!m_hWnd)
    THROWEX("Failed to create main window! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
}



//---------------------------------------------------------------------------
bool WndMain::playAlarm (UINT uiLoopDurationMS/*=0*/)
{
  static bool bErrorLoggedDll = false;
  static bool bErrorLoggedSym = false;

  m_bSoundPlaying = false;

  // lazy init
  if (!m_hWinmmDll)
  {
    m_hWinmmDll = LoadLibrary("winmm.dll");
    if (!m_hWinmmDll)
    {
      if (!bErrorLoggedDll)
      {
        LOGERR("Failed to load winmm.dll dynamically! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
        bErrorLoggedDll = true;
      }
      return false;
    }

    m_pfnPlaySound = (FNPLAYSOUNDA)GetProcAddress(m_hWinmmDll, "PlaySoundA");
    if (!m_pfnPlaySound)
    {
      if (!bErrorLoggedSym)
      {
        LOGERR("Failed to load symbol from winmm.dll! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
        bErrorLoggedSym = true;
      }
      this->stopAlarm();
      return false;
    }
  }

  // play sound
  while (1)
  {
    LPCSTR  pszSound;
    DWORD   dwFlags = 2 | 8 | 1; // SND_NODEFAULT | SND_LOOP | SND_ASYNC
    HMODULE hMod;

    if (Config::strAlertSoundFile.isEmpty())
    {
      pszSound = MAKEINTRESOURCE(IDW_ALARM);
      dwFlags |= 0x40004; // SND_RESOURCE
      hMod     = g_pApp->instance();
    }
    else
    {
      pszSound = Config::strAlertSoundFile.c_str();
      dwFlags |= 0x20000; // SND_FILENAME
      hMod     = 0;
    }

    if (m_pfnPlaySound(pszSound, hMod, dwFlags))
    {
      m_bSoundPlaying = true;
      break;
    }
    else if (!Config::strAlertSoundFile.isEmpty())
    {
      LOGERR("Failed to play wave file \"%s\"!", Config::strAlertSoundFile.c_str());
      Config::strAlertSoundFile.clear();
      Config::save();
    }
    else
    {
      break;
    }
  }

  if (m_bSoundPlaying)
  {
    // setup timer to stop playing the sound later
    if (!uiLoopDurationMS)
      uiLoopDurationMS = TIMER_STOPALARM_DELAYMS;
    if (!SetTimer(m_hWnd, TIMER_STOPALARM, uiLoopDurationMS, NULL))
      THROWEX("Failed to setup stopsound timer to %u milliseconds! Error %lu: %s", uiLoopDurationMS, App::sysLastError(), App::sysLastErrorString());

    // disable "test sound" menu item
    ModifyMenu(m_hMenuMainPopup, IDM_ALARM, MF_BYCOMMAND | MF_STRING, IDM_ALARM, "Stop &Alarm");
  }
  else
  {
    this->stopAlarm();
  }

  return m_bSoundPlaying;
}

//---------------------------------------------------------------------------
void WndMain::stopAlarm (void)
{
  if (m_hWinmmDll)
  {
    if (m_pfnPlaySound)
    {
      m_pfnPlaySound(0, 0, 0);
      m_pfnPlaySound = 0;
    }

    FreeLibrary(m_hWinmmDll);
    m_hWinmmDll = 0;
  }

  m_bSoundPlaying = false;
  KillTimer(m_hWnd, TIMER_STOPALARM);
  ModifyMenu(m_hMenuMainPopup, IDM_ALARM, MF_BYCOMMAND | MF_STRING, IDM_ALARM, "Test &Alarm");
}



//---------------------------------------------------------------------------
void WndMain::onCreate (void)
{
  NOTIFYICONDATA nid;

  // set window title
  this->setTitle(App::title());

  // show default systray icon
  nid.cbSize           = sizeof(NOTIFYICONDATA);
  nid.hWnd             = m_hWnd;
  nid.uID              = SYSTRAYICON_ID;
  nid.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  nid.uCallbackMessage = APP_UNIQUE_WM_SYSTRAYICON;
  nid.hIcon            = m_BattIcon.getDefaultIcon();
  strcpy((char*)nid.szTip, this->getTitle().c_str());
  if (!Shell_NotifyIcon(NIM_ADD, &nid))
    THROWEX("Failed to show system tray icon! Error %lu: %s", App::sysLastError(), App::sysLastErrorString());

  // force refresh battery status
  this->onPollPowerStatus(true);
}

//---------------------------------------------------------------------------
void WndMain::onDestroy (void)
{
  NOTIFYICONDATA nid;

  // destroy timers
  KillTimer(m_hWnd, TIMER_POWERPOLL);
  KillTimer(m_hWnd, TIMER_STOPALARM);

  // remove systray icon
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd   = m_hWnd;
  nid.uID    = SYSTRAYICON_ID;
  Shell_NotifyIcon(NIM_DELETE, &nid);

  // quit application
  PostQuitMessage(0);
}

//---------------------------------------------------------------------------
void WndMain::onPopupMenu (const POINT& pt)
{
  // show up popup menu
  SetForegroundWindow(m_hWnd); // needed if we want our menu to hide when user is clicking outside the menu
  TrackPopupMenu(m_hMenuMainPopup, TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
  PostMessage(m_hWnd, WM_NULL, 0, 0); // idem
}

//---------------------------------------------------------------------------
void WndMain::onPollPowerStatus (bool bForceRefresh)
{
  UINT uiNow = GetTickCount();
  UINT uiNextPollMS;
  UINT uiMaximumFrequency =
    m_BattIcon.isBlinking() ?
    ALERT_BLINK_FREQUENCY_MS :
    TIMER_POWERPOLL_DELAYMS_ONERROR;

  //LOGINFO("Poll (now:%u diff:%u)", uiNow, uiNow - m_uiLastPollTick);

  // poll power status and refresh icon ?
  // first we must ensure we are not refreshing too often
  if (!bForceRefresh &&
    (uiNow >= m_uiLastPollTick) &&
    (uiNow - m_uiLastPollTick) < uiMaximumFrequency)
  {
    uiNextPollMS = uiMaximumFrequency - (uiNow - m_uiLastPollTick);
  }
  else
  {
    HICON hIcon;
    BOOL  bRes;

    // get new icon to show
    hIcon = m_BattIcon.refresh();

    // update system tray icon
    {
      NOTIFYICONDATA nid;

      nid.cbSize = sizeof(NOTIFYICONDATA);
      nid.hWnd   = m_hWnd;
      nid.uID    = SYSTRAYICON_ID;
      nid.uFlags = NIF_ICON | NIF_TIP;
      nid.hIcon  = hIcon;

      strcpy((char*)nid.szTip, m_BattIcon.getStatusString().c_str());
      strcat((char*)nid.szTip, " - ");
      strcat((char*)nid.szTip, App::title().c_str());

      bRes = Shell_NotifyIcon(NIM_MODIFY, &nid);
      if (!bRes && App::sysLastError() != ERROR_TIMEOUT)
        LOGERR("Failed to update system tray icon! Error %u: %s", App::sysLastError(), App::sysLastErrorString());
    }

    // play an alarm sound
    if (m_BattIcon.isAlarmOn())
    {
      m_BattIcon.ackAlarm();
      if (!m_bSoundPlaying)
      {
        if (Config::bAlertSound)
          this->playAlarm();
      }
      else
      {
        LOGWARN("Alarm sound is already played!");
      }
    }
    else if (m_BattIcon.isAcOnline())
    {
      this->stopAlarm();
    }

    // choose next poll's delay
    if (m_BattIcon.isBlinking())
      uiNextPollMS = ALERT_BLINK_FREQUENCY_MS;
    else if (!bRes) // in case Shell_NotifyIcon(), try more frequently than TIMER_POWERPOLL_DELAYMS
      uiNextPollMS = TIMER_POWERPOLL_DELAYMS_ONERROR;
    else
      uiNextPollMS = TIMER_POWERPOLL_DELAYMS;
  }

  m_uiLastPollTick = uiNow;

  // reset timer (in case we were not called because of a WM_TIMER message)
  // update much more frequently if Shell_NotifyIcon() failed
  if (uiNextPollMS < TIMER_MAX_RESOLUTION_MS)
    uiNextPollMS = TIMER_MAX_RESOLUTION_MS;
  if (uiNextPollMS > TIMER_POWERPOLL_DELAYMS)
    uiNextPollMS = TIMER_POWERPOLL_DELAYMS;
  if (!SetTimer(m_hWnd, TIMER_POWERPOLL, uiNextPollMS, NULL))
    THROWEX("Failed to setup polling timer to %u milliseconds! Error %lu: %s", uiNextPollMS, App::sysLastError(), App::sysLastErrorString());
}



//---------------------------------------------------------------------------
LRESULT CALLBACK WndMain::wndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  POINT pt;

  switch (uiMsg)
  {
    case WM_CREATE :
      ms_pThis->m_hWnd = hWnd;
      ms_pThis->onCreate();
      break;

    case WM_DESTROY :
      ms_pThis->onDestroy();
      break;

    case WM_COMMAND :
      switch (LOWORD(wParam))
      {
        case IDM_QUIT :
          ms_pThis->destroy();
          break;

        case IDM_ALARM :
          if (ms_pThis->isAlarmPlaying())
            ms_pThis->stopAlarm();
          else
            ms_pThis->playAlarm(TIMER_STOPALARM_DELAYMS_PLAYTEST);
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    case APP_UNIQUE_WM_SYSTRAYICON :
      switch (LOWORD(lParam)) // HIWORD(lParam) contains the icon ID.
      {
        case WM_RBUTTONUP :
          GetCursorPos(&pt);
          ms_pThis->onPopupMenu(pt);
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    case WM_ENDSESSION :
      //LOGINFO("Got WM_ENDSESSION message!");
      ms_pThis->destroy();
      break;

    case WM_TIMER :
      switch (wParam)
      {
        case TIMER_POWERPOLL :
          ms_pThis->onPollPowerStatus(false);
          break;

        case TIMER_STOPALARM :
          ms_pThis->stopAlarm();
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    case WM_POWERBROADCAST :
      switch (wParam)
      {
        case PBT_APMPOWERSTATUSCHANGE :
          ms_pThis->onPollPowerStatus(true);
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    default :
      return DefWindowProc(hWnd, uiMsg, wParam, lParam);
  }

  return 0L;
}
