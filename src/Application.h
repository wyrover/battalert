
#ifndef __Application_Header__
#define __Application_Header__


//---------------------------------------------------------------------------
// Application
//---------------------------------------------------------------------------
class Application
{
public :
  Application (HINSTANCE hInstance, const char* pszArgv0);
  ~Application (void);

  // static utils
  static Application* GetApplication         (void);
  static int          ShowMessage            (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceMethod, HWND hParentWnd, UINT uiType, const char* pszFormat, ...);
  static void         ShowSystemErrorMessage (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceMethod, HWND hParentWnd, const char* pszFormat, ...);

  // application
  int  Init   (int argc, const char** argv);
  int  Run    (void);
  void Uninit (void);

  // properties
  HINSTANCE       GetInstanceHandle (void) const;
  const Version3& GetVersion        (void) const;
  const StringA&  GetTitle          (void) const;
  AppConfig*      GetConfig         (void);
  WndMain*        GetMainWindow     (void);


private :
  // application properties
  HINSTANCE  m_hInstance;
  Version3   m_Version;
  StringA    m_strTitle;
  OwnLogger* m_pLogger;

  // dynamic configuration settings
  AppConfig* m_pAppConfig;

  // windows / dialogs
  WndMain* m_pWndMain;


private :
  static Application* ms_pApplication;
};


// shortcuts
static inline Application& g_App  (void)  { return *(::Application::GetApplication()); }
static inline Application* g_pApp (void)  { return ::Application::GetApplication(); }

// inline methods
#include "Application.inl.h"


#endif // #ifndef __Application_Header__
