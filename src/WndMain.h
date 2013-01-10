//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-10 11:06:20
//
// $Id$
//
//***************************************************************************

#ifndef __WndMain_h__
#define __WndMain_h__


//---------------------------------------------------------------------------
// WndMain
//---------------------------------------------------------------------------
class WndMain : public Wnd
{
public :
  struct PowerStatus
  {
    bool                bValid;
    unsigned            uiTimestamp;
    SYSTEM_POWER_STATUS sps;
  };


public :
  WndMain (void);
  ~WndMain (void);

  void open (void);


private :
  // window's events
  void onCreate          (void);
  void onDestroy         (void);
  void onPopupMenu       (const POINT& ptPos);
  void onPollPowerStatus (bool bForceRefresh);

  // window's events switcher
  static LRESULT CALLBACK wndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HMENU    m_hMenuMainPopup;
  BattIcon m_BattIcon;
  UINT     m_uiLastPollTick;


private :
  static WndMain* ms_pThis;
};


#include "WndMain.inl.h"


#endif // #ifndef __WndMain_h__
