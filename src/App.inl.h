//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-09 19:18:11
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline HINSTANCE App::instance (void)
{
  return m_hInstance;
}

//---------------------------------------------------------------------------
inline App* App::singleton (void)
{
  return ms_pSingleton;
}
