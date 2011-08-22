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

#ifndef __nCore_ThreadMutex_Header__
#define __nCore_ThreadMutex_Header__
#if defined(NCORE_ENABLE_MULTITHREADING)
namespace nCore {


//---------------------------------------------------------------------------
// ThreadMutex
//
// Intra-Process MUTually EXclusive object.
//---------------------------------------------------------------------------
class NCORE_EXPORT ThreadMutex
{
public :
  enum State
  {
    MUTEX_OK = 0,         // Successful operation
    MUTEX_ERROR,          // An unknown error occurred
    MUTEX_NOTINIT,        // Mutex was not initialized
    MUTEX_ALREADYLOCKED,  // Mutex is already locked by an another thread
  };


public :
  ThreadMutex (void);
  ~ThreadMutex (void);

  State Acquire    (void);
  State TryAcquire (void);
  State Release    (void);


private :
  void* m_pHandle;
};


#include "ThreadMutex.inl.h"


}
#endif // #if defined(NCORE_ENABLE_MULTITHREADING)
#endif // #ifndef __nCore_ThreadMutex_Header__
