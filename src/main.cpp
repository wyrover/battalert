
#include "_Main.h"


//---------------------------------------------------------------------------
// _ParseWinCommandLine
//---------------------------------------------------------------------------
static void _ParseWinCommandLine (int* out_argc, char*** out_argv)
{
  char* pszCommandLine = ::GetCommandLine();
  static std::vector<char*> pos;
  bool new_arg = true;

	for ( ; *pszCommandLine ; pszCommandLine++)
	{
    if (*pszCommandLine == '"')
    {
      new_arg = true;
      pszCommandLine++;
      char* start_arg = pszCommandLine;
      for ( ; *pszCommandLine && *pszCommandLine!='"' ; ++pszCommandLine)
      {
        if (!isspace(*pszCommandLine))
          new_arg = false;
      }
      if (new_arg == false)
        pos.push_back(start_arg);
      if (*pszCommandLine == '"')
        *pszCommandLine = 0;
      else if (*pszCommandLine == 0)
        break;
      pszCommandLine++;
      if (*pszCommandLine == 0)
        break;
      new_arg = true;
    }

    if (new_arg && !isspace(*pszCommandLine))
    {
      new_arg = false;
      pos.push_back(pszCommandLine);
    }
    else if (!new_arg && isspace(*pszCommandLine))
    {
      new_arg = true;
      *pszCommandLine = 0;
    }
	}

	int num_words = (int)pos.size();
	char** words = new char*[num_words + 1];
	int i;

  for (i=0 ; i<num_words ; i++)
    words[i] = pos[i];
  words[i] = NULL;

  *out_argc = num_words;
  *out_argv = words;
}

//---------------------------------------------------------------------------
// m a i n
//---------------------------------------------------------------------------
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  int    nReturn = 0;
  int    nArgc = 0;
  char** pArgv = NULL;
  Application* pApp;

  try
  {
    // handle duplicated instances
    // * uncomment this if needed
    // * the system will close the mutex handle automatically when the
    //   process will terminate, so there's no need to CloseHandle() after
    //   that.
    ::CreateMutex(NULL, FALSE, APP_UNIQUE_INSTANCE_NAME
#ifdef X_DEBUG
      "DEBUG"
#endif
      );
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
      return 2;

    // parse command line
    _ParseWinCommandLine(&nArgc, &pArgv);

    // init application
    pApp = new Application(hInstance, pArgv[0]);
    nReturn = pApp->Init(nArgc, (const char**)pArgv);

    // run application
    if (nReturn == 0)
      nReturn = pApp->Run();

    // uninit application
    delete pApp;  // implicit call to pApp->Uninit()

    // destroy parsed command line result
    if (pArgv) delete[] pArgv;
  }
  catch (::nCore::Exception& e)
  {
    e.Log();
    ::MessageBox(NULL, e.GetDescription(), APP_BRAND_NAME, MB_ICONEXCLAMATION);
    nReturn = 99;
  }
  catch (const ::std::exception& e)
  {
    CORELOG LLFATAL, "EXCEPTION : %s", e.what());
    ::MessageBox(NULL, e.what(), APP_BRAND_NAME, MB_ICONEXCLAMATION);
    nReturn = 99;
  }
  catch (const char* pszMessage)
  {
    CORELOG LLFATAL, "EXCEPTION : %s", pszMessage);
    ::MessageBox(NULL, pszMessage, APP_BRAND_NAME, MB_ICONEXCLAMATION);
    nReturn = 99;
  }
  catch (...)
  {
    ::MessageBox(NULL, "Unknown exception !", APP_BRAND_NAME, MB_ICONEXCLAMATION);
    XASSERT(0);
    nReturn = 99;
  }

  return nReturn;
}
