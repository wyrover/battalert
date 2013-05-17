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
// Created On : 2013-01-09 19:14:03
//
// $Id$
//
//***************************************************************************

#ifndef __App_h__
#define __App_h__


//---------------------------------------------------------------------------
// App
//---------------------------------------------------------------------------
class App
{
public :
  App (HINSTANCE hInstance);
  ~App (void);

  // properties
  HINSTANCE      instance (void);
  const StringA& exePath  (void) const;
  WndMain*       wndMain  (void);

  // main methods
  int  init   (int argc, char** argv);
  int  run    (void);
  void uninit (void);

  // static utils
  static App*           singleton          (void);
  static const StringA& name               (void);
  static unsigned       sysLastError       (void);
  static const char*    sysLastErrorString (void);
  static const char*    sysGetErrorString  (unsigned uiOsError);


private :
  HINSTANCE m_hInstance;
  StringA   m_strExePath;
  WndMain*  m_pWndMain;


private :
  static App* ms_pSingleton;
};


#define g_App   (*App::singleton())
#define g_pApp  (App::singleton())


#include "App.inl.h"


#endif // #ifndef __App_h__
