//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-09 16:35:35
//
// $Id$
//
//***************************************************************************

#include "stable.h"


//---------------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // setup debugging
#if defined(_MSC_VER) && defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
  //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  //_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
  //_CrtSetBreakAlloc(197);
#endif

  // handle duplicated instances
  //   the system will close the mutex handle automatically when the process
  //   will terminate, so there's no need to CloseHandle() after that.
  CreateMutex(NULL, FALSE, APP_UNIQUE_INSTANCE_NAME
#ifdef _DEBUG
    "_DEBUG"
#endif
    );
  if (GetLastError() == ERROR_ALREADY_EXISTS)
  {
    MessageBoxA(NULL, "An instance of " APP_NAME " is already running!", APP_NAME, MB_ICONEXCLAMATION);
    return 2;
  }

  try
  {
    App* pApp = new App(hInstance);
    int iRes = pApp->init(__argc, __argv);
    if (!iRes)
      iRes = pApp->run();
    pApp->uninit();
    delete pApp;
    return iRes;
  }
  catch (const char*)
  {
    // probably already logged by Logger::throwException
    return 1;
  }
  catch (std::exception& e)
  {
    Logger::showNext(true);
    LOGFATAL("EXCEPTION (std): %s", e.what());
    return 1;
  }
  catch (...)
  {
    LOGFATAL("EXCEPTION (unknown)!");
    DOASSERT(0);
    return 1;
  }
}
