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
// Created On: 2013-01-11 22:52:59
//
//***************************************************************************

#ifndef __WndConfig_h__
#define __WndConfig_h__


//---------------------------------------------------------------------------
// WndConfig
//---------------------------------------------------------------------------
class WndConfig : public Wnd
{
public :
  WndConfig (void);
  ~WndConfig (void);

  void open (void);


private :
  void onCreate (void);
  void onCheckBlinking (void);
  void onSlideBlinkingThreshold (void);
  void onSlideAlertThreshold (void);
  void onCheckAlert (void);
  void onBrowseSoundFile (void);
  void onOK (void);
  void onCancel (void);
  void onPaint (void);

  static INT_PTR CALLBACK dialogProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HICON m_hIcoLogo;


private :
  static WndConfig* ms_pThis;
};


#include "WndConfig.inl.h"


#endif // #ifndef __WndConfig_h__
