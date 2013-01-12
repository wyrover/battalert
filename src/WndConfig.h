//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-11 22:52:59
//
// $Id$
//
//***************************************************************************

#ifndef __WndConfig_h__
#define __WndConfig_h__


//---------------------------------------------------------------------------
// WndConfig
//---------------------------------------------------------------------------
class WndConfig : public Wnd
{
public :
  WndConfig (void);
  ~WndConfig (void);

  void open (void);


private :
  void onCreate (void);
  void onCheckBlinking (void);
  void onSlideBlinkingThreshold (void);
  void onSlideAlertThreshold (void);
  void onCheckAlert (void);
  void onBrowseSoundFile (void);
  void onOK (void);
  void onCancel (void);
  void onPaint (void);

  static INT_PTR CALLBACK dialogProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HICON m_hIcoLogo;


private :
  static WndConfig* ms_pThis;
};


#include "WndConfig.inl.h"


#endif // #ifndef __WndConfig_h__
