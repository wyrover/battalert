//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-11 16:05:44
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline HICON BattIcon::getDefaultIcon (void)
{
  return m_hIconDefault;
}

//---------------------------------------------------------------------------
inline const StringA& BattIcon::getStatusString (void) const
{
  return m_strStatus;
}

//---------------------------------------------------------------------------
inline bool BattIcon::isBlinking (void) const
{
  return m_eBlink != BLINK_OFF;
}

//---------------------------------------------------------------------------
inline bool BattIcon::isAlarmOn (void) const
{
  return m_eAlarm == ALARM_ON;
}

//---------------------------------------------------------------------------
inline void BattIcon::ackAlarm (void)
{
  if (m_eAlarm == ALARM_ON)
    m_eAlarm = ALARM_ACK;
}

//---------------------------------------------------------------------------
inline bool BattIcon::isAcOnline (void) const
{
  return m_bHavePowerStatus && ((m_SPS.BatteryFlag & 8) || (m_SPS.ACLineStatus == 1));
}
