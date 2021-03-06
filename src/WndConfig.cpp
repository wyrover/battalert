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
// Created On: 2013-01-11 22:53:04
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
// Static Members
//---------------------------------------------------------------------------
WndConfig* WndConfig::ms_pThis = 0;


//---------------------------------------------------------------------------
WndConfig::WndConfig (void)
: Wnd()
{
  // pseudo-singleton
  DOASSERT(!ms_pThis);
  ms_pThis = this;

  m_hIcoLogo = (HICON)LoadImage(g_pApp->instance(), MAKEINTRESOURCE(IDI_APP), IMAGE_ICON, 128, 128, LR_DEFAULTCOLOR);
}

//---------------------------------------------------------------------------
WndConfig::~WndConfig (void)
{
  this->onCancel();
  DeleteObject(m_hIcoLogo);

  ms_pThis = 0;
}



//---------------------------------------------------------------------------
void WndConfig::open (void)
{
  DialogBox(g_pApp->instance(), MAKEINTRESOURCE(IDD_CONFIG),
    g_pApp->wndMain()->getHandle(), WndConfig::dialogProc);
}



//---------------------------------------------------------------------------
void WndConfig::onCreate (void)
{
  this->setTitle(App::name() + " Configuration");

  CheckDlgButton(m_hWnd, IDC_CHK_BLINK, (Config::bIconBlinking ? BST_CHECKED : BST_UNCHECKED));
  SendDlgItemMessage(m_hWnd, IDC_SLIDER_BLINK, TBM_SETRANGE, TRUE, MAKELONG(1, 99));
  SendDlgItemMessage(m_hWnd, IDC_SLIDER_BLINK, TBM_SETPOS, TRUE, Config::nIconBlinkingThreshold);
  this->onSlideBlinkingThreshold();
  this->onCheckBlinking();

  CheckDlgButton(m_hWnd, IDC_CHK_ALERTMSGBOX, (Config::bAlertMessageBox ? BST_CHECKED : BST_UNCHECKED));
  CheckDlgButton(m_hWnd, IDC_CHK_ALERTSOUND, (Config::bAlertSound ? BST_CHECKED : BST_UNCHECKED));
  SendDlgItemMessage(m_hWnd, IDC_SLIDER_ALERT, TBM_SETRANGE, TRUE, MAKELONG(1, 99));
  SendDlgItemMessage(m_hWnd, IDC_SLIDER_ALERT, TBM_SETPOS, TRUE, Config::nAlertThreshold);
  SetDlgItemText(m_hWnd, IDC_TXT_ALERT, Config::strAlertSoundFile);
  this->onSlideAlertThreshold();
  this->onCheckAlert();

  CheckDlgButton(m_hWnd, IDC_CHK_STARTUP, (Config::bLaunchAtStartup ? BST_CHECKED : BST_UNCHECKED));

  SetForegroundWindow(m_hWnd);
  SetFocus(GetDlgItem(m_hWnd, IDC_CHK_BLINK));
}

//---------------------------------------------------------------------------
void WndConfig::onCheckBlinking (void)
{
  BOOL bChecked = (IsDlgButtonChecked(m_hWnd, IDC_CHK_BLINK) == BST_CHECKED) ? TRUE : FALSE;

  EnableWindow(GetDlgItem(m_hWnd, IDC_SLIDER_BLINK), bChecked);
  EnableWindow(GetDlgItem(m_hWnd, IDC_LBL_BLINK), bChecked);
}

//---------------------------------------------------------------------------
void WndConfig::onSlideBlinkingThreshold (void)
{
  StringA str;
  LRESULT nRes;

  nRes = SendDlgItemMessage(m_hWnd, IDC_SLIDER_BLINK, TBM_GETPOS, 0, 0);
  str.format("%i%%", nRes);
  SetDlgItemText(m_hWnd, IDC_LBL_BLINK, str);
}

//---------------------------------------------------------------------------
void WndConfig::onSlideAlertThreshold (void)
{
  StringA str;
  LRESULT nRes;

  nRes = SendDlgItemMessage(m_hWnd, IDC_SLIDER_ALERT, TBM_GETPOS, 0, 0);
  str.format("%i%%", nRes);
  SetDlgItemText(m_hWnd, IDC_LBL_ALERT, str);
}

//---------------------------------------------------------------------------
void WndConfig::onCheckAlert (void)
{
  BOOL bCheckedMsgBox = (IsDlgButtonChecked(m_hWnd, IDC_CHK_ALERTMSGBOX) == BST_CHECKED) ? TRUE : FALSE;
  BOOL bCheckedSound  = (IsDlgButtonChecked(m_hWnd, IDC_CHK_ALERTSOUND) == BST_CHECKED) ? TRUE : FALSE;
  bool bAtLeastOneChecked = bCheckedMsgBox || bCheckedSound;

  EnableWindow(GetDlgItem(m_hWnd, IDC_BTN_ALERTBROWSE), bCheckedSound);
  EnableWindow(GetDlgItem(m_hWnd, IDC_TXT_ALERT), bCheckedSound);
  EnableWindow(GetDlgItem(m_hWnd, IDC_LBL_ALERTSOUNDTIP), bCheckedSound);

  EnableWindow(GetDlgItem(m_hWnd, IDC_SLIDER_ALERT), bAtLeastOneChecked);
  EnableWindow(GetDlgItem(m_hWnd, IDC_LBL_ALERT), bAtLeastOneChecked);
}

//---------------------------------------------------------------------------
void WndConfig::onBrowseSoundFile (void)
{
  OPENFILENAME ofn;
  StringA strNewFile(Config::strAlertSoundFile);
  StringA strDir;
  WORD wOffsetName = 0;
  WORD wOffsetExt = 0;
  BOOL bRes;

  if (strNewFile.isEmpty())
  {
    strDir = g_pApp->exePath();
    strDir.pathStripName();
  }
  else
  {
    wOffsetName = (WORD)(strNewFile.length() - strNewFile.pathExtractName().length());
    wOffsetExt  = (WORD)(strNewFile.length() - strNewFile.pathExtractExtension().length());
  }

  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize     = sizeof(OPENFILENAME);
  ofn.hwndOwner       = m_hWnd;
  ofn.hInstance       = g_pApp->instance();
  ofn.lpstrFilter     = "Wave Files (*.wav)\0*.wav\0Other Files (*.*)\0*.*\0\0";
  ofn.lpstrFile       = strNewFile.acquireBuffer(MAX_PATH, true);
  ofn.nMaxFile        = MAX_PATH - 1;
  ofn.lpstrInitialDir = strDir.isEmpty() ? NULL : strDir.c_str();
  ofn.lpstrTitle      = "Choose a alarm wave file";
  ofn.Flags           = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_READONLY;
  ofn.nFileOffset     = wOffsetName;
  ofn.nFileExtension  = wOffsetExt;

  bRes = GetOpenFileName(&ofn);
  strNewFile.releaseBuffer();
  if (bRes && !strNewFile.isEmpty())
  {
    Config::validateFile(strNewFile);
    if (strNewFile.isEmpty())
    {
      MessageBox(m_hWnd, "Selected file not found!", App::name(), MB_ICONERROR);
    }
    else
    {
      Config::strAlertSoundFile = strNewFile;
      SetDlgItemText(m_hWnd, IDC_TXT_ALERT, strNewFile);
    }
  }
}

//---------------------------------------------------------------------------
void WndConfig::onOK (void)
{
  Config::bIconBlinking = IsDlgButtonChecked(m_hWnd, IDC_CHK_BLINK) == BST_CHECKED;
  Config::nIconBlinkingThreshold = (int)SendDlgItemMessage(m_hWnd, IDC_SLIDER_BLINK, TBM_GETPOS, 0, 0);

  Config::nAlertThreshold = (int)SendDlgItemMessage(m_hWnd, IDC_SLIDER_ALERT, TBM_GETPOS, 0, 0);
  Config::bAlertMessageBox = IsDlgButtonChecked(m_hWnd, IDC_CHK_ALERTMSGBOX) == BST_CHECKED;
  Config::bAlertSound = IsDlgButtonChecked(m_hWnd, IDC_CHK_ALERTSOUND) == BST_CHECKED;

  GetDlgItemText(m_hWnd, IDC_TXT_ALERT, Config::strAlertSoundFile.acquireBuffer(MAX_PATH, false), MAX_PATH-1);
  Config::strAlertSoundFile.releaseBuffer();
  Config::validateFile(Config::strAlertSoundFile);

  Config::bLaunchAtStartup = IsDlgButtonChecked(m_hWnd, IDC_CHK_STARTUP) == BST_CHECKED;

  Config::save(true);
  EndDialog(m_hWnd, 1);
}

//---------------------------------------------------------------------------
void WndConfig::onCancel (void)
{
  EndDialog(m_hWnd, 0);
}

//---------------------------------------------------------------------------
void WndConfig::onPaint (void)
{
  HDC hWndDC;
  PAINTSTRUCT psWnd;

  hWndDC = BeginPaint(m_hWnd, &psWnd);
  DrawIconEx(hWndDC, 3, 3, m_hIcoLogo, 128, 128, 0, NULL, DI_NORMAL);
  EndPaint(m_hWnd, &psWnd);
}



//---------------------------------------------------------------------------
INT_PTR CALLBACK WndConfig::dialogProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uiMsg)
  {
    case WM_INITDIALOG :
      ms_pThis->m_hWnd = hWnd;
      ms_pThis->onCreate();
      return FALSE; // FALSE means : do not focus to default control

    case WM_CLOSE :
      ms_pThis->onCancel();
      break;

    case WM_DESTROY :
      ms_pThis->m_hWnd = 0;
      break;

    case WM_COMMAND :
      switch (LOWORD(wParam))
      {
        case IDOK :
          ms_pThis->onOK();
          break;

        case IDCANCEL :
          ms_pThis->onCancel();
          break;

        case IDC_CHK_BLINK :
          ms_pThis->onCheckBlinking();
          break;

        case IDC_CHK_ALERTMSGBOX :
        case IDC_CHK_ALERTSOUND :
          ms_pThis->onCheckAlert();
          break;

        case IDC_BTN_ALERTBROWSE :
          ms_pThis->onBrowseSoundFile();
          break;

        default :
          return FALSE;
      }
      break;

    case WM_NOTIFY :
      switch (wParam)
      {
        case IDC_SLIDER_BLINK :
          //if (((NMHDR*)lParam)->code == TRBN_THUMBPOSCHANGING)
            ms_pThis->onSlideBlinkingThreshold();
          //else
          //  return FALSE;
          break;

        case IDC_SLIDER_ALERT :
          //if (((NMHDR*)lParam)->code == TRBN_THUMBPOSCHANGING)
            ms_pThis->onSlideAlertThreshold();
          //else
          //  return FALSE;
          break;

        default :
          return FALSE;
      }
      break;

    case WM_PAINT :
      ms_pThis->onPaint();
      return FALSE;

    default :
      return FALSE;
  }

  return TRUE;
}
