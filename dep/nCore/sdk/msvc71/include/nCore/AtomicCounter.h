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

#ifndef __nCore_AtomicCounter_Header__
#define __nCore_AtomicCounter_Header__
#if defined(NCORE_ENABLE_MULTITHREADING)
namespace nCore {


//---------------------------------------------------------------------------
// AtomicCounter
//
// Please use the ::nCore::AtomicCounter::Create() function to create an
// AtomicCounter object.
// You'll be responsible to release it by using the delete operator or the
// convenient XDELETE macro.
//---------------------------------------------------------------------------
class AtomicCounter : private NonCopyable
{
public :
  AtomicCounter (void) { }
  virtual ~AtomicCounter (void) { }

  // factory
  NCORE_EXPORT static AtomicCounter* Create (int nInitialCounterValue=0);

  // properties
  virtual int GetValue (void) const = 0;

  // prefix increment / decrement
  virtual int operator++ (void);
  virtual int operator-- (void);

  // postfix increment / decrement
  //   note : for now, in every internal implementations of the AtomicCounter
  //   class, "postfix" operators are a *very* little bit slower than their
  //   "prefix" equivalent. this is mainly because of the nature of the
  //   underlying called os' functions.
  //   so, in most cases, be sure to prefer "prefix" operators when it's
  //   possible...  - jcl
  virtual int operator++ (int);
  virtual int operator-- (int);


private :
  virtual int Increment (void) = 0;
  virtual int Decrement (void) = 0;
};


#include "AtomicCounter.inl.h"


}
#endif // #if defined(NCORE_ENABLE_MULTITHREADING)
#endif // #ifndef __nCore_AtomicCounter_Header__
