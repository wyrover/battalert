//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2013-01-09 16:35:32
//
// $Id$
//
//***************************************************************************

#ifndef __stable_h__
#define __stable_h__


//---------------------------------------------------------------------------
// Branding :)
//---------------------------------------------------------------------------
#define APP_NAME         "BattAlert" // keep it as a single word (no whitespaces) !
//efine APP_VERSION_STR  "0.1"
//efine APP_TIMESTAMP    "Compiled on " __DATE__ " at " __TIME__


//---------------------------------------------------------------------------
// Constants (General Purpose)
//---------------------------------------------------------------------------
// those values must *never* be modified accross releases
// they are used to perform some IPC, detection, etc...
#define APP_UNIQUE_INSTANCE_NAME   APP_NAME "_InstanceMutex"
#define APP_UNIQUE_WNDCLASS_MAIN   (APP_NAME "_WndMainClass1")
#define APP_UNIQUE_WM_SYSTRAYICON  (WM_USER + 1)


//---------------------------------------------------------------------------
// Constants (App)
//---------------------------------------------------------------------------
// max resolution (SetTimer())
#define TIMER_MAX_RESOLUTION_MS  55

// alert mode will be turned on if remaining battery power
// is <= ALERT_BLINK_PERCENT percents in non-charging mode.
#define ALERT_BLINK_PERCENT       4
#define ALERT_BLINK_FREQUENCY_MS  750 // milliseconds

// alert sound will be played if battery power reaches ALERT_SOUND_PERCENT
// percents in non-charging mode.
#define ALERT_SOUND_PERCENT  2


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
// use this to make your code uncompilable until you remove it
#define TODO  const char TODO[-1] = "This is a TODO!"

// static assert macro (i.e.: compile-time assert)
#define MY_STATIC_ASSERT(test)  typedef char MY_CAT(_static_assert_,__LINE__)[(test) * 2 - 1]

// util macros
#define MY_VERBATIM(x)   x
#define MY_STRINGIZE(x)  MY_STRINGIZE__SUB(x)
#define MY_CAT(a,b)      MY_CAT__SUB(a,b)

// util macros subs (do not use directly)
#define MY_STRINGIZE__SUB(x)  #x
#define MY_CAT__SUB(a,b)      a##b

// assertions / log / throw exceptions
#ifdef _DEBUG
# ifdef _MSC_VER
#   define DOASSERT  _ASSERTE  // defined in <crtdbg.h>
# else
#   define DOASSERT  assert
# endif
# define LOGDBG(msg, ...)     ::Logger::print(::Logger::LLDEBUG, __FILE__, __LINE__, msg, ## __VA_ARGS__)
# define LOGINFO(msg, ...)    ::Logger::print(::Logger::LLINFO, __FILE__, __LINE__, msg, ## __VA_ARGS__)
# define LOGWARN(msg, ...)    ::Logger::print(::Logger::LLWARN, __FILE__, __LINE__, msg, ## __VA_ARGS__)
# define LOGERR(msg, ...)     ::Logger::print(::Logger::LLERROR, __FILE__, __LINE__, msg, ## __VA_ARGS__)
# define LOGFATAL(msg, ...)   ::Logger::print(::Logger::LLFATAL, __FILE__, __LINE__, msg, ## __VA_ARGS__)
# define THROWEX(msg, ...)    do { ::Logger::showNext(true); ::Logger::throwException(::Logger::LLFATAL, __FILE__, __LINE__, msg, ## __VA_ARGS__); } while(0)
#else
# define DOASSERT
# define LOGDBG(msg, ...)
# define LOGINFO(msg, ...)    ::Logger::print(::Logger::LLINFO, 0, __LINE__, msg, ## __VA_ARGS__)
# define LOGWARN(msg, ...)    ::Logger::print(::Logger::LLWARN, 0, __LINE__, msg, ## __VA_ARGS__)
# define LOGERR(msg, ...)     ::Logger::print(::Logger::LLERROR, 0, __LINE__, msg, ## __VA_ARGS__)
# define LOGFATAL(msg, ...)   ::Logger::print(::Logger::LLFATAL, 0, __LINE__, msg, ## __VA_ARGS__)
# define THROWEX(msg, ...)    do { ::Logger::showNext(true); ::Logger::throwException(::Logger::LLFATAL, 0, __LINE__, msg, ## __VA_ARGS__); } while(0)
#endif

// message box
#define MSGBOX_INFO(msg, ...)  ::Logger::showMessage(::Logger::LLINFO, 0, 0, msg, ## __VA_ARGS__)
#define MSGBOX_WARN(msg, ...)  ::Logger::showMessage(::Logger::LLWARN, 0, 0, msg, ## __VA_ARGS__)
#define MSGBOX_ERR(msg, ...)   ::Logger::showMessage(::Logger::LLERROR, 0, 0, msg, ## __VA_ARGS__)


//---------------------------------------------------------------------------
// External Headers
//---------------------------------------------------------------------------
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT  0x500  // required in order to use AddFontMemResourceEx

#ifdef _DEBUG
# ifdef _MSC_VER
#   define _CRTDBG_MAP_ALLOC
#   include <cstdlib>
#   include <crtdbg.h>
# else
#   include <cassert>
# endif
#endif

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>

#include <vector>

#include <windows.h>
#include <shellapi.h>


//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
typedef BOOL (WINAPI* FNPLAYSOUNDA)(LPCSTR,HMODULE,DWORD);


//---------------------------------------------------------------------------
// Local Headers
//---------------------------------------------------------------------------
#include "resource.h"

#include "Logger.h"
#include "StringA.h"

#include "Config.h"
#include "BattIcon.h"
#include "Wnd.h"
#include "WndMain.h"
#include "App.h"


#endif // #ifndef __stable_h__
