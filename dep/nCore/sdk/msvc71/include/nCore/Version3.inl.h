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
inline Version3::Version3 (void)
: m_nMajor(-1)
, m_nMinor(-1)
, m_nRevision(-1)
{
  m_szStripped[0] = 0;
  m_szPadded[0]   = 0;
}

//---------------------------------------------------------------------------
inline Version3::Version3 (int nMajor, int nMinor, int nRevision)
{
  this->Init(nMajor, nMinor, nRevision);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (int nCombined)
{
  this->Init(nCombined);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (const char* pszVersion)
{
  this->Init(pszVersion);
}

//---------------------------------------------------------------------------
inline Version3::Version3 (const Version3& rhs)
{
  *this = rhs;
}

//---------------------------------------------------------------------------
inline Version3::~Version3 (void)
{
}

//---------------------------------------------------------------------------
inline Version3& Version3::operator= (const Version3& rhs)
{
  m_nMajor    = rhs.m_nMajor;
  m_nMinor    = rhs.m_nMinor;
  m_nRevision = rhs.m_nRevision;

  strcpy((char*)&m_szStripped, (char*)&rhs.m_szStripped);
  strcpy((char*)&m_szPadded, (char*)&rhs.m_szPadded);

  return *this;
}

//---------------------------------------------------------------------------
inline bool Version3::IsValid (void) const
{
  return
    (m_nMajor    >= 0) && (m_nMajor    <= 9) &&
    (m_nMinor    >= 0) && (m_nMinor    <= 99) &&
    (m_nRevision >= 0) && (m_nRevision <= 9999);
}

//---------------------------------------------------------------------------
inline int Version3::GetMajor (void) const
{
  XASSERT(this->IsValid());
  return m_nMajor;
}

//---------------------------------------------------------------------------
inline int Version3::GetMinor (void) const
{
  XASSERT(this->IsValid());
  return m_nMinor;
}

//---------------------------------------------------------------------------
inline int Version3::GetRevision (void) const
{
  XASSERT(this->IsValid());
  return m_nRevision;
}

//---------------------------------------------------------------------------
inline int Version3::GetCombined (void) const
{
  XASSERT(this->IsValid());
  return (1000000 * m_nMajor) + (10000 * m_nMinor) + m_nRevision;
}

//---------------------------------------------------------------------------
inline const char* Version3::GetStrippedString (void) const
{
  XASSERT(this->IsValid());

  if(!m_szStripped[0] && this->IsValid())
    this->BuildStrings();

  return (const char*)&m_szStripped;
}

//---------------------------------------------------------------------------
inline const char* Version3::GetPaddedString (void) const
{
  XASSERT(this->IsValid());

  if(!m_szPadded[0] && this->IsValid())
    this->BuildStrings();

  return (const char*)&m_szPadded;
}

//---------------------------------------------------------------------------
inline bool Version3::operator== (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  return (m_nMajor == rhs.m_nMajor) && (m_nMinor == rhs.m_nMinor) && (m_nRevision == rhs.m_nRevision);
}

//---------------------------------------------------------------------------
inline bool Version3::operator!= (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  return (m_nMajor != rhs.m_nMajor) || (m_nMinor != rhs.m_nMinor) || (m_nRevision != rhs.m_nRevision);
}

//---------------------------------------------------------------------------
inline bool Version3::operator< (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  if(m_nMajor < rhs.m_nMajor)
  {
    return true;
  }
  else if(m_nMajor > rhs.m_nMajor)
  {
    return false;
  }
  else
  {
    if(m_nMinor < rhs.m_nMinor)
    {
      return true;
    }
    else if(m_nMinor > rhs.m_nMinor)
    {
      return false;
    }
    else
    {
      if(m_nRevision < rhs.m_nRevision)
        return true;
      else
        return false;
    }
  }
}

//---------------------------------------------------------------------------
inline bool Version3::operator> (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  if(m_nMajor > rhs.m_nMajor)
  {
    return true;
  }
  else if(m_nMajor < rhs.m_nMajor)
  {
    return false;
  }
  else
  {
    if(m_nMinor > rhs.m_nMinor)
    {
      return true;
    }
    else if(m_nMinor < rhs.m_nMinor)
    {
      return false;
    }
    else
    {
      if(m_nRevision > rhs.m_nRevision)
        return true;
      else
        return false;
    }
  }
}

//---------------------------------------------------------------------------
inline bool Version3::operator<= (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  return !(*this > rhs);
}

//---------------------------------------------------------------------------
inline bool Version3::operator>= (const Version3& rhs) const
{
  XASSERT(this->IsValid());
  XASSERT(rhs.IsValid());

  return !(*this < rhs);
}
