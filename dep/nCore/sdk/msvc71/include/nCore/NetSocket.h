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

#ifndef __nCore_NetSocket_Header__
#define __nCore_NetSocket_Header__
#if defined(NCORE_ENABLE_NETWORKING)
namespace nCore {


//---------------------------------------------------------------------------
// NetSocket
//---------------------------------------------------------------------------
class NCORE_EXPORT NetSocket
{
public :
  enum Option
  {
    OPT_BROADCAST = 0x0001,  // enable/disable broadcasting support for this socket (messages)
    OPT_DEBUG     = 0x0002,  // enable/disable sockets layer debugging
    OPT_KEEPALIVE = 0x0004,  // enable/disable sending of "keepalive probes" packets (RFC says every 2 hours, but i don't know on platforms)
    OPT_REUSEADDR = 0x0008,  // enable/disable address reusability (useful for listening)
    OPT_NODELAY   = 0x0010,  // enable nodelay = disable nagle algorithm, connection-oriented sockets only
    OPT_NONBLOCK  = 0x0020,  // non-blocking socket = async socket
    OPT_LINGER    = 0x0040,  // enable/disable lingering on disconnect
  };

  enum ShutdownHow
  {
    SHUTDOWN_RECEIVE,  // 0
    SHUTDOWN_SEND,     // 1
    SHUTDOWN_BOTH,     // 2
  };

  enum Buffer
  {
    BUF_SEND = 1,
    BUF_RECV,
    BUF_BOTH,
  };


public :
  virtual ~NetSocket (void);

  // create
  static NetError Create (NetSocket** ppOutSocket, NetFamily eFamily, NetSocketType eType, NetProtocol eProtocol);


  // autoclose
  // * the "autoclose" feature allows you to tell this NetSocket instance
  //   to not reset/close the internal "raw" socket handle during NetSocket
  //   destructor.
  // * this is useful if you only want to use NetSocket to create socket
  //   and open connection but then using your own low-level routines to
  //   manage your network connections.
  //   thus, call the GetRawHandle() method right before destroying the
  //   NetSocket object.
  void SetAutoClose (bool bEnableAutoClose); // default : enabled
  bool GetAutoClose (void) const;

  // shutdown / close
  NetError Shutdown (ShutdownHow eShutdownHow);
  NetError Close    (bool bIgnoreErrors=false);

  // action
  NetError Bind    (NetAddress& naLocal);
  NetError Listen  (int nBackLog=-1);
  NetError Accept  (NetSocket** ppOutNewSocket, int nTimeoutMS=-1);
  NetError Connect (NetAddress& naPeer, int nTimeoutMS=-1);

  // send / recv
  NetError Send    (const byte* pBuffer, uint32* puiLength);
  NetError Receive (byte* pBuffer, uint32* puiLength);

  // forced send / recv
  // * use those methods to force send or receive a given amount of bytes.
  // * using those method is a non-sense in async mode.
  // * even in case of error, the total number of bytes sent/received before
  //   the error occurs will be passed through the puiLength parameter.
  NetError SendForced    (const byte* pBuffer, uint32* puiLength);
  NetError ReceiveForced (byte* pBuffer, uint32* puiLength);

  // sendto / recvfrom
  NetError SendTo      (const NetAddress& naTo, const byte* pBuffer, uint32* puiLength);
  NetError ReceiveFrom (NetAddress& naFrom, byte* pBuffer, uint32* puiLength);

  // utils
  NetError SinglePoll (dword& dwInOutPollFlags, int nTimeoutMS=-1) const;
  bool     IsReadable (int nTimeoutMS=-1) const;  // convenience shortcut to SinglePoll(POLL_READ)
  bool     IsWritable (int nTimeoutMS=-1) const;  // convenience shortcut to SinglePoll(POLL_WRITE)


  // properties
  NetFamily         GetFamily        (void) const;
  NetSocketType     GetType          (void) const;
  NetProtocol       GetProtocol      (void) const;
  const NetAddress* GetLocalAddress  (void);
  const NetAddress* GetRemoteAddress (void);
  NetSocketHandle   GetRawHandle     (void) const;


  // socket's options
  NetError SetOptionFlag (Option eOption, bool bEnable);
  bool     GetOptionFlag (Option eOption) const;
  bool     IsBlocking    (void) const;

  // send & recv timeout
  // How to setup the nTimeoutMS parameter :
  // * <0 : Total blocking mode (synchronous)
  // * =0 : Total non-blocking mode (asynchronous)
  // * >0 : Blocking mode, but timeout is enabled on Send & Recv operations *only*
  //        Timeout value is in milliseconds.
  NetError SetTimeout (int nTimeoutMS);
  int      GetTimeout (void) const;

  // sizeof send/recv subsystem's buffer (note : this class does not implement any buffering system itself)
  NetError SetBuffersSize (Buffer eWhichBuffer, int nBufferSize);
  NetError GetBuffersSize (Buffer eWhichBuffer, int& nBufferSize) const;
  int      GetBuffersSize (Buffer eWhichBuffer) const;

  // statistics
#ifdef NCORE_ENABLE_NETWORKING_STATS
  NetCount GetBytesSent       (void) const;
  NetCount GetBytesReceived   (void) const;
  NetCount GetPacketsSent     (void) const;
  NetCount GetPacketsReceived (void) const;
#endif


protected :
  // protected constructors
  NetSocket (void);

  // utils
#ifdef NCORE_ENABLE_NETWORKING_STATS
  void ResetStats (void);
#endif


protected :
  // basic properties
  NetSocketHandle m_Handle;
  NetFamily       m_eFamily;
  NetSocketType   m_eType;
  NetProtocol     m_eProtocol;

  // addresses
  NetAddress* m_pLocalAddr;
  NetAddress* m_pRemoteAddr;

  // options
  bool  m_bAutoClose;
  dword m_dwOptions;
  int   m_nTimeoutMS;  // timeout on send/recv operations

  // statistics
#ifdef NCORE_ENABLE_NETWORKING_STATS
  NetCount m_nBytesSent;
  NetCount m_nBytesRecv;
  NetCount m_nPacketsSent;
  NetCount m_nPacketsRecv;
#endif
};


#include "NetSocket.inl.h"


}
#endif // #if defined(NCORE_ENABLE_NETWORKING)
#endif // #ifndef __nCore_NetSocket_Header__
