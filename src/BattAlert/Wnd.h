
#ifndef __Wnd_Header__
#define __Wnd_Header__


//---------------------------------------------------------------------------
// Wnd
//---------------------------------------------------------------------------
class Wnd
{
public :
  explicit Wnd (Application* pApplication);
  virtual ~Wnd (void);

  // open / destroy
          bool IsCreated (void) const;
  virtual void Destroy   (void);

  // properties
          Application* GetApplication (void);
          HWND         GetHandle      (void) const;
  virtual void         SetTitle       (const StringA& strNewTitle);
  virtual StringA      GetTitle       (void) const;


protected :
  // parent application
  Application* m_pApp;

  // properties
  HWND m_hWnd;
};


#include "Wnd.inl.h"


#endif // #ifndef __Wnd_Header__
