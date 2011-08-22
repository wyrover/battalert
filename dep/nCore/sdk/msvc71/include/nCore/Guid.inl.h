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
inline Guid::Guid (void)
{
  // minimal initializing for performance purpose
  m_szGuid[0] = 0;
}

//---------------------------------------------------------------------------
inline Guid::Guid (const Guid& rhs)
{
  *this = rhs;
}

//---------------------------------------------------------------------------
inline Guid::Guid (const char* rhs)
{
  *this = rhs;
}




//---------------------------------------------------------------------------
inline Guid::~Guid (void)
{
}




//---------------------------------------------------------------------------
inline bool Guid::IsNull (void) const
{
  return !m_szGuid[0];
}




//---------------------------------------------------------------------------
inline void Guid::Set (const char* pszGuid)
{
  if(!pszGuid || !pszGuid[0])
  {
    m_szGuid[0] = 0;
  }
  else
  {
    XASSERT(strlen(pszGuid) == (size_t)GUID_STRING_LENGTH);
    strcpy((char*)&m_szGuid, pszGuid);
  }
}

//---------------------------------------------------------------------------
inline void Guid::operator= (const Guid& rhs)
{
  this->Set((char*)&rhs.m_szGuid);
}

//---------------------------------------------------------------------------
inline void Guid::operator= (const char* rhs)
{
  this->Set(rhs);
}




//---------------------------------------------------------------------------
inline const char* Guid::Get (void) const
{
  if(!m_szGuid[0])
    return (char*)&ms_szNullGuid;
  else
    return (char*)&m_szGuid;
}

//---------------------------------------------------------------------------
inline const char* Guid::c_str (void) const
{
  return this->Get();
}

//---------------------------------------------------------------------------
inline Guid::operator const char* (void) const
{
  return this->Get();
}




//---------------------------------------------------------------------------
inline bool Guid::operator== (const Guid& rhs) const
{
  return StringA::StrCompareCountI((char*)&m_szGuid, (char*)&rhs.m_szGuid, (uint)GUID_STRING_LENGTH) == 0;
}

//---------------------------------------------------------------------------
inline bool Guid::operator== (const char* rhs) const
{
  XASSERT(rhs);
  return StringA::StrCompareCountI((char*)&m_szGuid, rhs, (uint)GUID_STRING_LENGTH) == 0;
}

//---------------------------------------------------------------------------
inline bool Guid::operator!= (const Guid& rhs) const
{
  return !(*this == rhs);
}

//---------------------------------------------------------------------------
inline bool Guid::operator!= (const char* rhs) const
{
  return !(*this == rhs);
}
