//***************************************************************************
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
