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
// Created On: 2013-01-09 16:35:35
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
