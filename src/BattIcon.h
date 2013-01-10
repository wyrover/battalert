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
  BattIcon (void);
  ~BattIcon (void);

  HICON refresh (void);

  HICON          getDefaultIcon  (void) { return m_hIconDefault; }
  const StringA& getStatusString (void) { return m_strStatus; }


private :
  // system power status
  bool                m_bHavePowerStatus;
  SYSTEM_POWER_STATUS m_SPS;
  StringA             m_strStatus;

  // loaded icons
  HICON m_hIconDefault;
  HICON m_hIconNoBatt;

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
