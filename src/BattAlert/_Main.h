
#ifndef __AppMain_Header__
#define __AppMain_Header__


//---------------------------------------------------------------------------
// Branding :)
//---------------------------------------------------------------------------
#define APP_BRAND_NAME       "BattAlert" // keep it as a single word (no whitespaces) !
#define APP_BRAND_AUTHOR     "?"
#define APP_BRAND_WEB        "?"
#define APP_BRAND_TIMESTAMP  "Compiled on " __DATE__ // " at " __TIME__

// product version
#define APP_BRAND_VERSION_STRING  "1.01.0001" // keep it Version3() compliant !


//---------------------------------------------------------------------------
// Constants (General Purpose)
//---------------------------------------------------------------------------
// those values must *never* be modified accross releases
// they are used to perform some IPC, detection, etc...
#define APP_UNIQUE_INSTANCE_NAME   "__" APP_BRAND_NAME "_InstanceMutex__"
#define APP_UNIQUE_WNDCLASS_MAIN   (APP_BRAND_NAME "_WndMainClass1")
#define APP_UNIQUE_WM_SYSTRAYICON  (WM_USER + 1)


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define SHOWMESSAGE   Application::ShowMessage(__FILE__, __LINE__, NCORE_FUNCTION,
#define SHOWSYSERROR  Application::ShowSystemErrorMessage(__FILE__, __LINE__, NCORE_FUNCTION,


//---------------------------------------------------------------------------
// External Includes
//---------------------------------------------------------------------------
// nCore header
#include <nCore/_nCore.h>

// miscellaneous standard headers
#include <nCore/MemoryManagerMacrosUndef.h>
//#include <queue>
#include <nCore/MemoryManagerMacros.h>

// miscellaneous windows headers
#include <nCore/MemoryManagerMacrosUndef.h>
#include <shellapi.h>
//#include <shlobj.h>
//#include <wininet.h>
//#include <commctrl.h>
#include <nCore/MemoryManagerMacros.h>


//---------------------------------------------------------------------------
// Raw Types
//---------------------------------------------------------------------------
// application
class AppConfig;
class Application;
class OwnLogger;
class Wnd;
class WndMain;


//---------------------------------------------------------------------------
// Local Includes
//---------------------------------------------------------------------------
using namespace nCore;

#include "resource.h"

// common code
#include "OwnLogger.h"
#include "Wnd.h"

// application
#include "AppConfig.h"
#include "Application.h"
#include "WndMain.h"



#endif // #ifndef __AppMain_Header__
