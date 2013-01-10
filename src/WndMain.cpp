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

#define TIMER_POWERPOLL_ID       1
#define TIMER_POWERPOLL_DELAYMS  (15 * 1000)


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
  this->destroy();

  UnregisterClass(APP_UNIQUE_WNDCLASS_MAIN, g_pApp->instance());

  ms_pThis = 0;
}



//---------------------------------------------------------------------------
// open
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
  this->onPollPowerStatus();
}

//---------------------------------------------------------------------------
void WndMain::onDestroy (void)
{
  NOTIFYICONDATA nid;

  // destroy "power poll" timer
  KillTimer(m_hWnd, TIMER_POWERPOLL_ID);

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
void WndMain::onPollPowerStatus (void)
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

  // reset timer (in case we were not called because of a WM_TIMER message)
  // update much more frequently if Shell_NotifyIcon() failed
  if (!SetTimer(m_hWnd, TIMER_POWERPOLL_ID, bRes ? TIMER_POWERPOLL_DELAYMS : 1000, NULL))
    THROWEX("Failed to setup polling timer! Error %lu: %s", App::sysLastError(), App::sysLastErrorString());
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
        case TIMER_POWERPOLL_ID :
          ms_pThis->onPollPowerStatus();
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    case WM_POWERBROADCAST :
      switch (wParam)
      {
        case PBT_APMPOWERSTATUSCHANGE :
          ms_pThis->onPollPowerStatus();
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
