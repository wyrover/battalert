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
// Created On : 2013-01-09 16:27:04
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
// Defaults
//---------------------------------------------------------------------------
#ifndef IDC_STATIC
# define IDC_STATIC  (-1)
#endif


//---------------------------------------------------------------------------
// Icon
//---------------------------------------------------------------------------
#define IDI_APP     101
#define IDI_NOBATT  102
#define IDI_RED     103


//---------------------------------------------------------------------------
// Menu
//---------------------------------------------------------------------------
#define IDM_POPUP_MAIN    201
#define IDM_QUIT          202
#define IDM_CONFIGDIALOG  203
#define IDM_ALARM         204


//---------------------------------------------------------------------------
// Font
//---------------------------------------------------------------------------
#define IDF_SMALL   301
#define IDF_NORMAL  302


//---------------------------------------------------------------------------
// Wave
//---------------------------------------------------------------------------
#define IDW_ALARM  401


//---------------------------------------------------------------------------
// Dialog
//---------------------------------------------------------------------------
#define IDD_CONFIG             501
#define IDC_LBL_BLINK          502
#define IDC_CHK_BLINK          503
#define IDC_SLIDER_BLINK       504
#define IDC_SLIDER_ALERT       505
#define IDC_LBL_ALERT          506
#define IDC_CHK_ALERTMSGBOX    507
#define IDC_CHK_ALERTSOUND     508
#define IDC_BTN_ALERTBROWSE    509
#define IDC_TXT_ALERT          510
#define IDC_LBL_ALERTSOUNDTIP  511
#define IDC_CHK_STARTUP        512
