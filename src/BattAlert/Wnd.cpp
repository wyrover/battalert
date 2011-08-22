
#include "_Main.h"


//---------------------------------------------------------------------------
// Wnd
//---------------------------------------------------------------------------
Wnd::Wnd (Application* pApplication)
{
  XASSERT(pApplication);

  // parent application
  m_pApp = pApplication;

  // properties
  m_hWnd = NULL;
}

//---------------------------------------------------------------------------
// ~Wnd
//---------------------------------------------------------------------------
Wnd::~Wnd (void)
{
  this->Destroy();
}

//---------------------------------------------------------------------------
// Destroy
//---------------------------------------------------------------------------
void Wnd::Destroy (void)
{
  if (m_hWnd)
  {
    ::DestroyWindow(m_hWnd);
    m_hWnd = NULL;
  }
}

//---------------------------------------------------------------------------
// IsCreated
//---------------------------------------------------------------------------
bool Wnd::IsCreated (void) const
{
  return (m_hWnd && ::IsWindow(m_hWnd));
}

//---------------------------------------------------------------------------
// SetTitle
//---------------------------------------------------------------------------
void Wnd::SetTitle (const StringA& strNewTitle)
{
  XASSERT(m_hWnd);
  ::SetWindowText(m_hWnd, strNewTitle.c_str());
}

//---------------------------------------------------------------------------
// GetTitle
//---------------------------------------------------------------------------
StringA Wnd::GetTitle (void) const
{
  char szTitle[512] = "\0";

  XASSERT(m_hWnd);

  ::GetWindowText(m_hWnd, (char*)&szTitle, sizeof(szTitle));
  return StringA((char*)&szTitle);
}
