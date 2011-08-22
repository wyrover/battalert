//***************************************************************************
//
// [ nCore ]
//
// Copyright (c) 2001-2008 Jean-Charles Lefebvre <jcl ATNOSPAM jcl DOT name>
//
// This file is part of nCore.
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
//***************************************************************************

#ifndef __nCore_DynamicLibrary_Header__
#define __nCore_DynamicLibrary_Header__
#if defined(NCORE_ENABLE_UTIL_DYNAMIC_LIBRARY)
namespace nCore {


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#if defined(NCORE_PLATFORM_WINDOWS)
  #define NCORE_DYNLIB_HANDLE          HMODULE  //MYHMODULE
  #define NCORE_DYNLIB_LOAD(a)         ::LoadLibrary(a)
  #define NCORE_DYNLIB_GETSYMBOL(a,b)  ::GetProcAddress((HMODULE)a, b)
  #define NCORE_DYNLIB_UNLOAD(a)       !::FreeLibrary((HMODULE)a)
  //struct HINSTANCE__;
  //typedef struct HINSTANCE__* MYHMODULE;

#elif defined(NCORE_PLATFORM_LINUX)
  #define NCORE_DYNLIB_HANDLE          void*
  #define NCORE_DYNLIB_LOAD(a)         dlopen(a, RTLD_LAZY)
  #define NCORE_DYNLIB_GETSYMBOL(a,b)  dlsym(a, b)
  #define NCORE_DYNLIB_UNLOAD(a)       dlclose(a)

#else
#error This platform is not supported yet !
#endif


//---------------------------------------------------------------------------
// DynamicLibrary
//---------------------------------------------------------------------------
class NCORE_EXPORT DynamicLibrary
{
public :
  DynamicLibrary (const StringA& strLibraryName="");
  ~DynamicLibrary (void);

  // actions
  bool  Load      (const StringA& strLibraryName);
  bool  Unload    (void);
  void* GetSymbol (const StringA& strSymbolName) const;

  // properties
  bool           IsLoaded (void) const;
  const StringA& GetName  (void) const;

  // tools
  static StringA GetSystemErrorString (void);


private :
  void SetName (const StringA& strLibraryName);


private :
  StringA             m_strName;
  NCORE_DYNLIB_HANDLE m_hDynLib;
};


#include "DynamicLibrary.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_DYNAMIC_LIBRARY)
#endif // #ifndef __nCore_DynamicLibrary_Header__
