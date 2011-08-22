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


//---------------------------------------------------------------------------
inline NetSocket::NetSocket (void)
{
  // basic properties
  m_Handle    = INVALID_SOCKET;
  m_eFamily   = NETFAMILY_UNSPEC;
  m_eType     = NETSOCKTYPE_UNSPEC;
  m_eProtocol = NETPROTO_UNSPEC;

  // addresses
  m_pLocalAddr  = NULL;
  m_pRemoteAddr = NULL;

  // options
  m_bAutoClose = true;
  m_dwOptions  = 0;
  m_nTimeoutMS = -1; // default is blocking mode...

  // statistics
#ifdef NCORE_ENABLE_NETWORKING_STATS
  this->ResetStats();
#endif
}

//---------------------------------------------------------------------------
inline void NetSocket::SetAutoClose (bool bEnableAutoClose)
{
  m_bAutoClose = bEnableAutoClose;
}

//---------------------------------------------------------------------------
inline bool NetSocket::GetAutoClose (void) const
{
  return m_bAutoClose;
}

//---------------------------------------------------------------------------
inline NetFamily NetSocket::GetFamily (void) const
{
  return m_eFamily;
}

//---------------------------------------------------------------------------
inline NetSocketType NetSocket::GetType (void) const
{
  return m_eType;
}

//---------------------------------------------------------------------------
inline NetProtocol NetSocket::GetProtocol (void) const
{
  return m_eProtocol;
}

//---------------------------------------------------------------------------
inline NetSocketHandle NetSocket::GetRawHandle (void) const
{
  return m_Handle;
}

//---------------------------------------------------------------------------
inline bool NetSocket::GetOptionFlag (NetSocket::Option eOption) const
{
  return (m_dwOptions & eOption) == (dword)eOption;
}

//---------------------------------------------------------------------------
inline bool NetSocket::IsBlocking (void) const
{
  return !this->GetOptionFlag(OPT_NONBLOCK);
}

//---------------------------------------------------------------------------
inline int NetSocket::GetTimeout (void) const
{
  return m_nTimeoutMS;
}

//---------------------------------------------------------------------------
#ifdef NCORE_ENABLE_NETWORKING_STATS
inline NetCount NetSocket::GetBytesSent (void) const
{
  return m_nBytesSent;
}
#endif

//---------------------------------------------------------------------------
#ifdef NCORE_ENABLE_NETWORKING_STATS
inline NetCount NetSocket::GetBytesReceived (void) const
{
  return m_nBytesRecv;
}
#endif

//---------------------------------------------------------------------------
#ifdef NCORE_ENABLE_NETWORKING_STATS
inline NetCount NetSocket::GetPacketsSent (void) const
{
  return m_nPacketsSent;
}
#endif

//---------------------------------------------------------------------------
#ifdef NCORE_ENABLE_NETWORKING_STATS
inline NetCount NetSocket::GetPacketsReceived (void) const
{
  return m_nPacketsRecv;
}
#endif
