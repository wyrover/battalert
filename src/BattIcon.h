//***************************************************************************
//
// BattAlert
// A minimalistic battery monitor for Win32.
//
// Copyright (C) 2011-2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
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
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-10 15:26:13
//
// $Id$
//
//***************************************************************************

#ifndef __BattIcon_h__
#define __BattIcon_h__


//---------------------------------------------------------------------------
// BattIcon
//---------------------------------------------------------------------------
class BattIcon
{
public :
  enum BlinkMode
  {
    BLINK_OFF,    // no blinking
    BLINK_STEP1,  // blinking icon
    BLINK_STEP2,  // normal icon
  };

  enum AlarmMode
  {
    ALARM_OFF,  // no alarm
    ALARM_ON,   // alarm mode is enabled
    ALARM_ACK,  // alarm mode is enabled and has been acknowledged
  };


public :
  BattIcon (void);
  ~BattIcon (void);

  HICON refresh (void);

  HICON          getDefaultIcon  (void);
  const StringA& getStatusString (void) const;
  bool           isBlinking      (void) const;
  bool           isAlarmOn       (void) const;
  void           ackAlarm        (void);
  bool           isAcOnline      (void) const;


private :
  // system power status
  bool                m_bHavePowerStatus;
  SYSTEM_POWER_STATUS m_SPS;
  StringA             m_strStatus;
  BlinkMode           m_eBlink;
  AlarmMode           m_eAlarm;
  BYTE                m_cAlarmPrevBattLife;

  // loaded icons
  HICON m_hIconDefault;
  HICON m_hIconNoBatt;
  HICON m_hIconBlink;

  // the system tray icon
  HDC         m_hDcIcon;
  BITMAPINFO* m_pBmiIcon;
  HBITMAP     m_hBmpIcon;
  PBYTE       m_pBmpBitsIcon;
  HICON       m_hIcon;

  // fonts
  HANDLE m_hFontNormalRsrc;
  HFONT  m_hFontNormal;
  HANDLE m_hFontSmallRsrc;
  HFONT  m_hFontSmall;
};


#include "BattIcon.inl.h"


#endif // #ifndef __BattIcon_h__
