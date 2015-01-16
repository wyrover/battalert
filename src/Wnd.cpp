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
// Created On: 2013-01-10 11:03:27
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
