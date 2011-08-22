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
inline const char* Exception::what (void) const throw()
{
  return m_strDescription.c_str();
}

//---------------------------------------------------------------------------
inline const char* Exception::GetDescription (void) const throw()
{
  return m_strDescription.c_str();
}

//---------------------------------------------------------------------------
inline const char* Exception::GetFilePath (void) const
{
  return m_pszFilePath;
}

//---------------------------------------------------------------------------
inline uint Exception::GetFileLine (void) const
{
  return m_uiFileLine;
}

//---------------------------------------------------------------------------
inline const char* Exception::GetFunction (void) const
{
  return m_pszFunction;
}

//---------------------------------------------------------------------------
inline uint Exception::GetCode (void) const
{
  return m_uiCode;
}


//---------------------------------------------------------------------------
inline bool Exception::IsUserExCode (uint uiExCode)
{
  return (uiExCode > uint(EXCODE_USER_BASE));
}
