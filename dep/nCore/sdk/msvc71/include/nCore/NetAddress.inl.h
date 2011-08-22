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
inline bool NetAddress::IsEmpty (void) const
{
  return m_vecList.empty();
}

//---------------------------------------------------------------------------
inline uint NetAddress::GetCount (void) const
{
  return (uint)m_vecList.size();
}

//---------------------------------------------------------------------------
inline bool NetAddress::SeekNext (void)
{
  if(!m_vecList.empty())
  {
    if(m_nCurrentIndex < int(m_vecList.size() - 1))
    {
      ++m_nCurrentIndex;
      return true;
    }
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool NetAddress::SeekFirst (void)
{
  if(m_vecList.empty())
    return false;

  m_nCurrentIndex = 0;
  return true;
}




//---------------------------------------------------------------------------
inline NetFamily NetAddress::GetFamily (void) const
{
  XASSERT(this->HasValidIndex());
  return m_vecList[m_nCurrentIndex].m_eFamily;
}

//---------------------------------------------------------------------------
inline int NetAddress::GetStructSize (void) const
{
  XASSERT(this->HasValidIndex());
  return m_vecList[m_nCurrentIndex].m_nAddrLen;
}

//---------------------------------------------------------------------------
inline const void* NetAddress::GetStruct (void) const
{
  XASSERT(this->HasValidIndex());
  return &m_vecList[m_nCurrentIndex].m_sa;
}




//---------------------------------------------------------------------------
inline bool NetAddress::HasValidIndex (void) const
{
  return !m_vecList.empty() && (m_nCurrentIndex >= 0) && (m_nCurrentIndex < (int)m_vecList.size());
}




//---------------------------------------------------------------------------
inline void* NetAddress::GetIpOffsetInStruct (NetFamily eFamily, const void* pSA)
{
  switch(eFamily)
  {
    case NETFAMILY_INET4 :
      return (char*)pSA + sizeof(short) + sizeof(short);

#ifdef NCORE_ENABLE_NETWORKING_IPV6
    case NETFAMILY_INET6 :
      return (char*)pSA + sizeof(short) + sizeof(short) + sizeof(long);
#endif

    default :
      XASSERT(0);
      XTHROWEXCODE(EXCODE_UNKNOWN);
  }
}
