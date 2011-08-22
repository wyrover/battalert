
#include "_Main.h"


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
#define SYSTRAYICON_STATUS  1


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
WndMain* WndMain::ms_pThis = NULL;


//---------------------------------------------------------------------------
// WndMain
//---------------------------------------------------------------------------
WndMain::WndMain (Application* pApplication)
: Wnd(pApplication)
{
  // pseudo-singleton
  XASSERT(!ms_pThis);
  ms_pThis = this;

  // init members
  m_hMenuMainPopup = NULL;

  // create window's class
  {
    WNDCLASSEX wc;
    HINSTANCE  hInst = (HINSTANCE)::GetModuleHandle(NULL); // g_App().GetInstance();

    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = ::LoadIcon(hInst, MAKEINTRESOURCE(IDI_APP));
    wc.hIconSm       = NULL;
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = APP_UNIQUE_WNDCLASS_MAIN;

    if (!RegisterClassEx(&wc))
      XTHROW EXCODE_SYSTEM_GENERIC, "Error while calling RegisterClass() for main window ! Error %u : %s", System::LastError(), System::LastErrorString());
  }
}

//---------------------------------------------------------------------------
// ~WndMain
//---------------------------------------------------------------------------
WndMain::~WndMain (void)
{
  this->Destroy();

  ::UnregisterClass(APP_UNIQUE_WNDCLASS_MAIN, g_App().GetInstanceHandle());

  ms_pThis = NULL;
}



//---------------------------------------------------------------------------
// Open
//---------------------------------------------------------------------------
void WndMain::Open (void)
{
  // create the main popup menu (it will be assigned to the icon of the system tray)
  m_hMenuMainPopup = ::GetSubMenu(::LoadMenu(g_App().GetInstanceHandle(), MAKEINTRESOURCE(IDM_POPUP_MAIN)), 0);
  if (!m_hMenuMainPopup)
    XTHROW EXCODE_SYSTEM_GENERIC, "Unable to create the main popup menu ! Error %u : %s", System::LastError(), System::LastErrorString());

  // create a message-only window
  m_hWnd = ::CreateWindowEx(0, APP_UNIQUE_WNDCLASS_MAIN, "Main", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, g_App().GetInstanceHandle(), NULL);
  if (!m_hWnd)
    XTHROW EXCODE_SYSTEM_GENERIC, "Failed to create main window ! Error %u : %s", System::LastError(), System::LastErrorString());
}

//---------------------------------------------------------------------------
// Close
//---------------------------------------------------------------------------
void WndMain::Close (void)
{
  if (m_hWnd)
  {
    ::DestroyWindow(m_hWnd);
    m_hWnd = NULL;
  }
}



//---------------------------------------------------------------------------
// OnCreate
//---------------------------------------------------------------------------
void WndMain::OnCreate (void)
{
  NOTIFYICONDATA nid;

  // set window title
  this->SetTitle(m_pApp->GetTitle());

  // show default systray icon
  nid.cbSize           = sizeof(NOTIFYICONDATA);
  nid.hWnd             = m_hWnd;
  nid.uID              = SYSTRAYICON_STATUS;
  nid.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE;
  nid.uCallbackMessage = APP_UNIQUE_WM_SYSTRAYICON;
  nid.hIcon            = ::LoadIcon(g_App().GetInstanceHandle(), MAKEINTRESOURCE(IDI_APP));
  strcpy((char*)nid.szTip, this->GetTitle().c_str());
  if (!Shell_NotifyIcon(NIM_ADD, &nid))
    XTHROW EXCODE_SYSTEM_GENERIC, "Failed to show system tray icon ! Error (%lu) : %s", System::LastError(), System::LastErrorString());
}

//---------------------------------------------------------------------------
// OnDestroy
//---------------------------------------------------------------------------
void WndMain::OnDestroy (void)
{
  NOTIFYICONDATA nid;

  // remove systray icon
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd   = m_hWnd;
  nid.uID    = SYSTRAYICON_STATUS;
  Shell_NotifyIcon(NIM_DELETE, &nid);

  // quit application
  ::PostQuitMessage(0);
}

//---------------------------------------------------------------------------
// OnPopupMenu
//---------------------------------------------------------------------------
void WndMain::OnPopupMenu (const POINT& pt)
{
  // show up popup menu
  SetForegroundWindow(m_hWnd); // needed if we want our menu to hide when user is clicking outside the menu
  TrackPopupMenu(m_hMenuMainPopup, TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, m_hWnd, NULL);
  PostMessage(m_hWnd, WM_NULL, 0, 0); // idem
}



//---------------------------------------------------------------------------
// WndProc
//---------------------------------------------------------------------------
LRESULT CALLBACK WndMain::WndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  POINT pt;

  switch(uiMsg)
  {
    case WM_CREATE :
      ms_pThis->m_hWnd = hWnd;
      ms_pThis->OnCreate();
      break;

    case WM_DESTROY :
      ms_pThis->OnDestroy();
      break;

    case WM_COMMAND :
      switch(LOWORD(wParam))
      {
        case IDM_QUIT :
          ms_pThis->Close();
          break;

        default :
          return DefWindowProc(hWnd, uiMsg, wParam, lParam);
      }
      break;

    case APP_UNIQUE_WM_SYSTRAYICON :
      switch(LOWORD(lParam)) // HIWORD(lParam) contains the icon ID.
      {
        case WM_RBUTTONUP :
          GetCursorPos(&pt);
          ms_pThis->OnPopupMenu(pt);
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
