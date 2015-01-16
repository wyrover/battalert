//***************************************************************************
//
// BattAlert
// A minimalistic battery monitor for Win32.
//
// Copyright 2011-2013 Jean-Charles Lefebvre <polyvertex@gmail.com>
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
// Created On: 2013-01-10 11:06:25
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
#define TIMER_STOPALARM_DELAYMS           (30 * 1000)
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
    StringA str(App::name());

    str += " " APP_VERSION_STR;
    mii.cbSize     = sizeof(MENUITEMINFO);
    mii.fMask      = MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_STATE;
    mii.fType      = MFT_STRING;
    mii.fState     = MFS_ENABLED;
    mii.wID        = IDM_ABOUT;
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
  StringA strSoundFile;

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

  // determine the location of the user's alert sound file
  strSoundFile = Config::strAlertSoundFile;
  if (!strSoundFile.isEmpty() && !strSoundFile.pathHasSeparators())
  {
    strSoundFile = g_pApp->exePath();
    strSoundFile.pathStripName();
    strSoundFile /= Config::strAlertSoundFile;
  }

  // play sound
  while (1)
  {
    LPCSTR  pszSound;
    DWORD   dwFlags = 2 | 8 | 1; // SND_NODEFAULT | SND_LOOP | SND_ASYNC
    HMODULE hMod;

    if (strSoundFile.isEmpty())
    {
      pszSound = MAKEINTRESOURCE(IDW_ALARM);
      dwFlags |= 0x40004; // SND_RESOURCE
      hMod     = g_pApp->instance();
    }
    else
    {
      pszSound = strSoundFile.c_str();
      dwFlags |= 0x20000; // SND_FILENAME
      hMod     = 0;
    }

    if (m_pfnPlaySound(pszSound, hMod, dwFlags))
    {
      m_bSoundPlaying = true;
      break;
    }
    else if (!strSoundFile.isEmpty())
    {
      LOGERR("Failed to play wave file \"%s\"!", strSoundFile.c_str());
      strSoundFile.clear();
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
void WndMain::alertMessage (void)
{
  MSGBOXPARAMS mbp;
  StringA strMsg;

  strMsg.format(
    "%s would kindly like to remind you that your system will very probably "
    "run out of battery soon and that if you do nothing about it, the world "
    "will end in the worst way ever!\n"
    "(Just plug-in your computer on AC power, it will be just fine...)",
    App::name().c_str());

  ZeroMemory(&mbp, sizeof(mbp));
  mbp.cbSize      = sizeof(mbp);
  mbp.hwndOwner   = m_hWnd;
  mbp.hInstance   = g_pApp->instance();
  mbp.lpszText    = strMsg.c_str();
  mbp.lpszCaption = App::name().c_str();
  mbp.dwStyle     = MB_USERICON | MB_SYSTEMMODAL | MB_SETFOREGROUND | MB_TOPMOST;
  mbp.lpszIcon    = MAKEINTRESOURCE(IDI_APP);
  if (MessageBoxIndirect(&mbp) != 0 && ms_pThis->isAlarmPlaying())
    ms_pThis->stopAlarm();
}



//---------------------------------------------------------------------------
void WndMain::onCreate (void)
{
  NOTIFYICONDATA nid;

  // set window title
  this->setTitle(App::name());

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

  this->stopAlarm();

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
void WndMain::onAbout (void)
{
    ShellExecute(m_hWnd, "open", APP_URL, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void WndMain::onConfigDialog (void)
{
  WndConfig wndConfig;
  wndConfig.open();
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
  bool bShowAlertMessage = false;

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
    static UINT uiLastModifyIconError = 0;

    HICON   hIcon;
    BOOL    bRes;
    StringA strTip;

    // get new icon to show
    hIcon = m_BattIcon.refresh();

    // prepare tip text
    strTip  = m_BattIcon.getStatusString();
    strTip += " - ";
    strTip += App::name();

    // update system tray icon
    {
      NOTIFYICONDATA nid;

      // remove systray icon first
      if (bForceRefresh)
      {
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd   = m_hWnd;
        nid.uID    = SYSTRAYICON_ID;
        nid.uFlags = NIF_ICON;
        Shell_NotifyIcon(NIM_DELETE, &nid);

        nid.cbSize           = sizeof(NOTIFYICONDATA);
        nid.hWnd             = m_hWnd;
        nid.uID              = SYSTRAYICON_ID;
        nid.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE;
        nid.uCallbackMessage = APP_UNIQUE_WM_SYSTRAYICON;
        nid.hIcon            = hIcon;
        strcpy((char*)nid.szTip, strTip.c_str());
        bRes = Shell_NotifyIcon(NIM_ADD, &nid);

        // reset last error state since we deleted the icon first
        uiLastModifyIconError = 0;
      }
      else
      {
        // if Shell_NotifyIcon() is used with a more recent version of the
        // NOTIFYICONDATA structure, the tip member always has to be
        // specified (i.e.: even if its content doesn't have to be modified)
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd   = m_hWnd;
        nid.uID    = SYSTRAYICON_ID;
        nid.uFlags = NIF_ICON | NIF_TIP;
        nid.hIcon  = hIcon;
        strcpy((char*)nid.szTip, strTip.c_str());
        bRes = Shell_NotifyIcon(NIM_MODIFY, &nid);
      }

      if (bRes)
      {
        if (uiLastModifyIconError != 0)
        {
          LOGINFO("System tray icon is now up-to-date.");
          uiLastModifyIconError = 0;
        }
      }
      else
      {
        UINT uiSysError = App::sysLastError();
        if (uiSysError != ERROR_TIMEOUT)
        {
          if (uiSysError != uiLastModifyIconError)
          {
            LOGERR("Failed to update system tray icon! Error %u: %s", uiSysError, App::sysGetErrorString(uiSysError));
            uiLastModifyIconError = uiSysError;
          }
        }
      }
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
      if (Config::bAlertMessageBox)
        bShowAlertMessage = true;
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

  if (bShowAlertMessage)
    this->alertMessage();
}



//---------------------------------------------------------------------------
LRESULT CALLBACK WndMain::wndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  static UINT uiMsgTaskbarCreated = 0;

  POINT pt;

  switch (uiMsg)
  {
    case WM_CREATE :
      // the "TaskbarCreated" message is sent by the shell when the task
      // tray has been created. we need to catch it in case exporer.exe
      // crashed, to force display our icon again.
      uiMsgTaskbarCreated = RegisterWindowMessage(TEXT("TaskbarCreated"));

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

        case IDM_ABOUT :
          ms_pThis->onAbout();
          break;

        case IDM_CONFIGDIALOG :
          ms_pThis->onConfigDialog();
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
        case WM_LBUTTONUP :
          if (ms_pThis->isAlarmPlaying())
            ms_pThis->stopAlarm();
          break;

        case WM_LBUTTONDBLCLK :
          ms_pThis->onConfigDialog();
          break;

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

    default:
      if (uiMsg == uiMsgTaskbarCreated)
      {
        LOGDBG("Received TaskbarCreated system message");
        ms_pThis->onPollPowerStatus(true);
      }
      return DefWindowProc(hWnd, uiMsg, wParam, lParam);
  }

  return 0L;
}
