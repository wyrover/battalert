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

#ifndef __nCore_ThreadMethod_Header__
#define __nCore_ThreadMethod_Header__
#if defined(NCORE_ENABLE_MULTITHREADING)
namespace nCore {


//---------------------------------------------------------------------------
// ThreadMethod
//
// Thread entry point representation.
//---------------------------------------------------------------------------
class ThreadMethod
{
public :
  ThreadMethod (void) { }
  virtual ~ThreadMethod (void) { }

  // thread entry point you'll have to implement
  virtual void ThreadEntry (Thread* pThreadSelf, void* pUserParam) = 0;
};


}
#endif // #if defined(NCORE_ENABLE_MULTITHREADING)
#endif // #ifndef __nCore_ThreadMethod_Header__
