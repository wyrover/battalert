
#ifndef __WndMain_Header__
#define __WndMain_Header__


//---------------------------------------------------------------------------
// WndMain
//---------------------------------------------------------------------------
class WndMain : public Wnd
{
public :
  struct PowerStatus
  {
    bool                bValid;
    uint32              uiTimestamp;
    SYSTEM_POWER_STATUS sps;
  };


public :
  WndMain (Application* pApplication);
  virtual ~WndMain (void);

  void Open  (void);
  void Close (void);


private :
  // window's events
  void OnCreate          (void);
  void OnDestroy         (void);
  void OnPopupMenu       (const POINT& ptPos);
  void OnPollPowerStatus (void);

  // window's events switcher
  static LRESULT CALLBACK WndProc (HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


private :
  HMENU    m_hMenuMainPopup;
  BattIcon m_BattIcon;


private :
  static WndMain* ms_pThis;
};


#include "WndMain.inl.h"


#endif // #ifndef __WndMain_Header__
