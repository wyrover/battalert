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

#ifndef __nCore_Singleton_Header__
#define __nCore_Singleton_Header__
namespace nCore {


//---------------------------------------------------------------------------
// Singleton
//
// Automatic Singleton Implementation by Scott Bilas.
// http://www.drizzle.com/~scottb/publish/gpgems1_singleton.htm
//
// CREDITS
// * Original code by Scott Bilas.
// * Modified by Jean-Charles Lefebvre to fit nCore's needs.
//---------------------------------------------------------------------------
template <typename T>
class Singleton
{
protected :
  static T* ms_pSingleton;


public :
  Singleton (void)
  {
    XASSERT(!ms_pSingleton);
    ms_pSingleton = static_cast<T*>(this);
  }

  ~Singleton (void)
  {
    XASSERT(ms_pSingleton);
    ms_pSingleton = NULL;
  }

  static T& GetSingleton (void)
  {
    XASSERT(ms_pSingleton);
    return *ms_pSingleton;
  }

  static T* GetSingletonPtr (void)
  {
    return ms_pSingleton;
  }
};

template <typename T> T* Singleton<T>::ms_pSingleton = NULL;


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define NCORE_IMPLEMENT_SINGLETON(classname) \
  classname& classname::GetSingleton (void) \
  { \
    XASSERT(ms_pSingleton); \
    return *ms_pSingleton; \
  } \
  \
  classname* classname::GetSingletonPtr (void) \
  { \
    return ms_pSingleton; \
  }
/*#define NCORE_IMPLEMENT_SINGLETON(classname) \
  classname##& classname##::GetSingleton (void) \
  { \
    XASSERT(ms_pSingleton); \
    return *ms_pSingleton; \
  } \
  \
  classname##* classname##::GetSingletonPtr (void) \
  { \
    return ms_pSingleton; \
  }*/


}
#endif // #ifndef __nCore_Singleton_Header__
