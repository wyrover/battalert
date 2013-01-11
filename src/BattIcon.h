//***************************************************************************
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

  HICON          getDefaultIcon  (void)       { return m_hIconDefault; }
  const StringA& getStatusString (void) const { return m_strStatus; }
  bool           isBlinking      (void) const { return m_eBlink != BLINK_OFF; }
  bool           isAlarmOn       (void) const { return m_eAlarm == ALARM_ON; }
  void           ackAlarm        (void)       { if (m_eAlarm == ALARM_ON) m_eAlarm = ALARM_ACK; }
  bool           isAcOnline      (void) const { return m_bHavePowerStatus && ((m_SPS.BatteryFlag & 8) || (m_SPS.ACLineStatus == 1)); }


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


//#include "BattIcon.inl.h"


#endif // #ifndef __BattIcon_h__
