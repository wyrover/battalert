//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-10 11:03:27
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
Wnd::Wnd (void)
: m_hWnd(0)
{
}

//---------------------------------------------------------------------------
Wnd::~Wnd (void)
{
  this->destroy();
}

//---------------------------------------------------------------------------
void Wnd::destroy (void)
{
  if (m_hWnd)
  {
    DestroyWindow(m_hWnd);
    m_hWnd = 0;
  }
}

//---------------------------------------------------------------------------
bool Wnd::isCreated (void) const
{
  return m_hWnd && IsWindow(m_hWnd);
}

//---------------------------------------------------------------------------
void Wnd::setTitle (const StringA& strNewTitle)
{
  DOASSERT(m_hWnd);
  SetWindowText(m_hWnd, strNewTitle.c_str());
}

//---------------------------------------------------------------------------
StringA Wnd::getTitle (void) const
{
  char szTitle[1024] = "\0";

  DOASSERT(m_hWnd);
  GetWindowText(m_hWnd, (char*)&szTitle, sizeof(szTitle));
  return StringA((char*)&szTitle);
}
