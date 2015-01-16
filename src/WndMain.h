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
// Created On: 2013-01-10 11:06:20
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

  // alert
  bool playAlarm      (UINT uiLoopDurationMS=0);
  bool isAlarmPlaying (void) const;
  void stopAlarm      (void);
  void alertMessage   (void);


private :
  // window's events
  void onCreate          (void);
  void onDestroy         (void);
  void onPopupMenu       (const POINT& ptPos);
  void onAbout           (void);
  void onConfigDialog    (void);
  void onPollPowerStatus (bool bForceRefresh);

  // window's events switcher
  static LRESULT CALLBACK wndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HMENU    m_hMenuMainPopup;
  BattIcon m_BattIcon;
  UINT     m_uiLastPollTick;

  // sound
  HMODULE      m_hWinmmDll;
  FNPLAYSOUNDA m_pfnPlaySound;
  bool         m_bSoundPlaying;


private :
  static WndMain* ms_pThis;
};


#include "WndMain.inl.h"


#endif // #ifndef __WndMain_h__
