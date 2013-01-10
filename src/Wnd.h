//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-10 11:03:23
//
// $Id$
//
//***************************************************************************

#ifndef __Wnd_h__
#define __Wnd_h__


//---------------------------------------------------------------------------
// Wnd
//---------------------------------------------------------------------------
class Wnd
{
public :
  Wnd (void);
  virtual ~Wnd (void);

  // open / destroy
          bool isCreated (void) const;
  virtual void destroy   (void);

  // properties
          HWND    getHandle (void) const;
  virtual void    setTitle  (const StringA& strNewTitle);
  virtual StringA getTitle  (void) const;


protected :
  HWND m_hWnd;
};


#include "Wnd.inl.h"


#endif // #ifndef __Wnd_h__
