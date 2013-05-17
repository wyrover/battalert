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
inline const StringA& App::exePath (void) const
{
  return m_strExePath;
}

//---------------------------------------------------------------------------
inline WndMain* App::wndMain (void)
{
  return m_pWndMain;
}

//---------------------------------------------------------------------------
inline App* App::singleton (void)
{
  return ms_pSingleton;
}
