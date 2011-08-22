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

#ifndef __nCore_NetTypes_Header__
#define __nCore_NetTypes_Header__
#if defined(NCORE_ENABLE_NETWORKING)
namespace nCore {


//---------------------------------------------------------------------------
// NetError
//---------------------------------------------------------------------------
enum NetError
{
  #define NETERROR_DEFINE(name,desc)            name,
  #define NETERROR_DEFINE_CODE(name,desc,code)  name = code,

  #include "NetErrorDefines.h"
};

#define NETSUCCESS(code)  ((code) == ::nCore::NETERR_OK)
#define NETFAILED(code)   ((code) != ::nCore::NETERR_OK)


//---------------------------------------------------------------------------
// NetFamily
//---------------------------------------------------------------------------
enum NetFamily
{
  NETFAMILY_UNSPEC = AF_UNSPEC,  // 0
  NETFAMILY_INET4  = AF_INET,    // 2

#ifdef NCORE_ENABLE_NETWORKING_IPV6
  NETFAMILY_INET6  = AF_INET6,   // 23
#endif
};


//---------------------------------------------------------------------------
// NetProtocol
//---------------------------------------------------------------------------
enum NetProtocol
{
  NETPROTO_UNSPEC = 0xFFFF,
  NETPROTO_TCP    = IPPROTO_TCP,  // 6
  NETPROTO_UDP    = IPPROTO_UDP,  // 17
//NETPROTO_SCTP   = IPPROTO_SCTP, // 126  // only for testing purpose...
};


//---------------------------------------------------------------------------
// NetSocketType
//---------------------------------------------------------------------------
enum NetSocketType
{
  NETSOCKTYPE_UNSPEC   = 0,
  NETSOCKTYPE_STREAM   = SOCK_STREAM,  // 1
  NETSOCKTYPE_DATAGRAM = SOCK_DGRAM,   // 2
  NETSOCKTYPE_RAW      = SOCK_RAW,     // 3
};


//---------------------------------------------------------------------------
// NetSocketHandle
//---------------------------------------------------------------------------
#if defined(NCORE_PLATFORM_WINDOWS)
  typedef SOCKET  NetSocketHandle;
#else
  typedef int             NetSocketHandle;
  #define INVALID_SOCKET  -1
#endif


//---------------------------------------------------------------------------
// PollFlag
//---------------------------------------------------------------------------
enum PollFlag
{
  POLL_NONE          = 0,
  POLL_READ          = 0x01,  // incomming data is ready to be read immediately
  POLL_READ_PRIORITY = 0x02,  // incomming priority data is available immediately
  POLL_WRITE         = 0x04,  // socket is ready to send data immediately
  POLL_ERROR         = 0x08,  // an error occurred on socket
  POLL_HANGUP        = 0x10,  // hangup occurred
  POLL_INVALID       = 0x20,  // invalid socket handle
};


//---------------------------------------------------------------------------
// NetCount
//---------------------------------------------------------------------------
typedef uint32  NetCount; // 32bit type allows to count up to nearly 4GB which should be a good trade enough for now


}
#endif // #if defined(NCORE_ENABLE_NETWORKING)
#endif // #ifndef __nCore_NetTypes_Header__
