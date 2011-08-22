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

#ifndef __nCore_NetPollSet_Header__
#define __nCore_NetPollSet_Header__
#if defined(NCORE_ENABLE_NETWORKING)
namespace nCore {


//---------------------------------------------------------------------------
// NetPollSet
//---------------------------------------------------------------------------
class NCORE_EXPORT NetPollSet
{
public :
  typedef struct PollResult_
  {
    dword      dwPollFlags;
    NetSocket* pSocket;
  }
  PollResult;


public :
  virtual ~NetPollSet (void);

  // factory
  static NetPollSet* Create (int nMaxCapacity);

  // add / remove sockets
  virtual bool AddSocket        (NetSocket* pSocket);
  virtual bool RemoveSocket     (NetSocket* pSocket);
  virtual void RemoveAllSockets (void);

  // poll
  virtual NetError Poll (dword dwPollFlags, int* pnOutCount, const PollResult** ppOutResult, int nTimeoutMS=-1) = 0;

  // utils
  virtual int GetRegisteredSocketIndex (const NetSocket* pSocket) const;


protected :
  explicit NetPollSet (int nMaxCapacity);


protected :
  NetSocket** m_ppaSockets;    // registered sockets
  PollResult* m_paResult;      // result array
  int         m_nMaxCapacity;  // maximum set capacity
  int         m_nCount;        // current number of registered sockets
};


#include "NetPollSet.inl.h"


}
#endif // #if defined(NCORE_ENABLE_NETWORKING)
#endif // #ifndef __nCore_NetPollSet_Header__
