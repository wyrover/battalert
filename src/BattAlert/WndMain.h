
#ifndef __WndMain_Header__
#define __WndMain_Header__


//---------------------------------------------------------------------------
// WndMain
//---------------------------------------------------------------------------
class WndMain : public Wnd
{
public :
  WndMain (Application* pApplication);
  virtual ~WndMain (void);

  void Open  (void);
  void Close (void);


private :
  // window's events
  void OnCreate    (void);
  void OnDestroy   (void);
  void OnPopupMenu (const POINT& ptPos);

  // window's events switcher
  static LRESULT CALLBACK WndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HMENU m_hMenuMainPopup;


private :
  static WndMain* ms_pThis;
};


#include "WndMain.inl.h"


#endif // #ifndef __WndMain_Header__
