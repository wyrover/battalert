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

#ifndef __nCore_NetAddress_Header__
#define __nCore_NetAddress_Header__
#if defined(NCORE_ENABLE_NETWORKING)
namespace nCore {


//---------------------------------------------------------------------------
// NetAddress
//---------------------------------------------------------------------------
class NCORE_EXPORT NetAddress
{
public :
  enum Flags
  {
    FLAG_UNSPEC     = 0,
    FLAG_IPV4_OK    = 0x0001,
#ifdef NCORE_ENABLE_NETWORKING_IPV6
    FLAG_IPV6_OK    = 0x0002,
    FLAG_IPV6_FIRST = 0x0004 | FLAG_IPV6_OK,
#endif
  };


protected :
  class Addr
  {
  public :
    NetFamily m_eFamily;
    StringA   m_strHost;
    StringA   m_strService;

    int m_nAddrLen;
    union
    {
      struct sockaddr_storage stor;
      struct sockaddr_in      sin;
#ifdef NCORE_ENABLE_NETWORKING_IPV6
      struct sockaddr_in6     sin6;
#endif
    }
    m_sa;


    Addr (void)
    {
      m_eFamily = NETFAMILY_UNSPEC;
      m_nAddrLen = 0;
    }

    Addr (const Addr& rhs)
    {
      *this = rhs;
    }

    Addr (NetFamily eFamily, const StringA& strHost, const StringA& strService, const void* sa, int nAddrLen)
    {
      XASSERT(nAddrLen <= (int)sizeof(m_sa));
      m_eFamily    = eFamily;
      m_strHost    = strHost;
      m_strService = strService;
      m_nAddrLen   = nAddrLen;
      memcpy(&m_sa, sa, nAddrLen);
    }

    void operator= (const Addr& rhs)
    {
      m_eFamily    = rhs.m_eFamily;
      m_strHost    = rhs.m_strHost;
      m_strService = rhs.m_strService;
      m_nAddrLen   = rhs.m_nAddrLen;
      memcpy(&m_sa, &rhs.m_sa, rhs.m_nAddrLen);
    }
  };

  typedef std::vector<Addr>         AddrList;
  typedef AddrList::iterator        AddrListIt;
  typedef AddrList::const_iterator  AddrListCIt;


public :
  NetAddress (void);
  NetAddress (const NetAddress& rhs);
  virtual ~NetAddress (void);


  // static utils
  static NetAddress* CreateFromRawStruct (NetFamily eFamily, const void* pSA, int nSaLen);


  // copy operator
  virtual NetAddress& operator= (const NetAddress& rhs);

  // assign / resolve
  virtual void Resolve         (const char* pszHostNameOrIP, const char* pszPortNumOrServiceName, dword dwFlags=FLAG_IPV4_OK);
  virtual void Resolve         (const char* pszHostNameOrIP, uint16 uiPortInHostOrder, dword dwFlags=FLAG_IPV4_OK);
  virtual void ResolveLocalAny (const char* pszPortNumOrServiceName, dword dwFlags=FLAG_IPV4_OK);
  virtual void ResolveLocalAny (uint16 uiPortInHostOrder, dword dwFlags=FLAG_IPV4_OK);
  virtual void AssignRawStruct (NetFamily eFamily, const void* pSA, int nSaLen);
  //virtual void AssignRawIPv4   (uint32 uiHostInNetworkOrder, uint16 uiPortInNetworkOrder);


  // list of addresses
  virtual bool IsEmpty   (void) const;
  virtual uint GetCount  (void) const;
  virtual bool SeekNext  (void);
  virtual bool SeekFirst (void);

  // properties of the current selected address in the list
  virtual NetFamily   GetFamily      (void) const;
  virtual const char* GetHostString  (bool bPreferCanonicalName=false, bool bForceRefresh=false);
  virtual const char* GetServiceName (void);
  virtual uint16      GetPort        (void) const; // could also be named GetService()
  virtual int         GetStructSize  (void) const;
  virtual const void* GetStruct      (void) const;
  virtual bool        IsLoopback     (void) const;
  virtual bool        IsAny          (void) const;


protected :
  // utils
  virtual void ResolveImpl   (const char* pszHostNameOrIP, const char* pszPortNumOrServiceName, dword dwFlags);
  virtual bool HasValidIndex (void) const;
  virtual void Reset         (void);

  // static utils
  static bool  ResolveHostName     (StringA& strOutHostName, const void* pSA, int nSaLen, bool bCanonical);
  static bool  ResolveServiceName  (StringA& strOutServiceName, const void* pSA, int nSaLen, bool bDatagram=false);
  static void* GetIpOffsetInStruct (NetFamily eFamily, const void* pSA);


protected :
  friend class NetSocket;
  virtual void* BeginAcquireRawStruct  (int& nOutMaxStructSize);
  virtual void  ReleaseRawStruct       (NetFamily eFamily, int nStructSize);
  virtual void  ReleaseRawStructFailed (void);


protected :
  AddrList m_vecList;
  int      m_nCurrentIndex;
};


#include "NetAddress.inl.h"


}
#endif // #if defined(NCORE_ENABLE_NETWORKING)
#endif // #ifndef __nCore_NetAddress_Header__
