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
inline void StringA::SetDefaultBoolStringType (StringA::BoolStringType eBoolStringType)
{
  ms_eBoolStringType = eBoolStringType;
}

//---------------------------------------------------------------------------
inline StringA::BoolStringType StringA::GetDefaultBoolStringType (void)
{
  return ms_eBoolStringType;
}




//---------------------------------------------------------------------------
inline StringA::StringA (void)
{
  this->Init();
}

//---------------------------------------------------------------------------
inline StringA::StringA (const StringA& rhs)
{
  this->Init();
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const StringA& rhs, uint uiCount, uint uiStart/*=0*/)
{
  this->Init();
  this->CopyCount(rhs.m_pszData, uiCount, uiStart);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const std::string& rhs)
{
  this->Init();
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const std::string& rhs, uint uiCount, uint uiStart/*=0*/)
{
  this->Init();
  this->CopyCount(rhs.c_str(), uiCount, uiStart);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const char* rhs)
{
  this->Init();
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const char* rhs, uint uiCount, uint uiStart/*=0*/)
{
  this->Init();
  this->CopyCount(rhs, uiCount, uiStart);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const bool b)
{
  this->Init();
  *this = b;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const char c)
{
  this->Init();
  *this = c;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const int32 i)
{
  this->Init();
  *this = i;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const uint32 ui)
{
  this->Init();
  *this = ui;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const int64 i)
{
  this->Init();
  *this = i;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const uint64 ui)
{
  this->Init();
  *this = ui;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const float f)
{
  this->Init();
  *this = f;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const double d)
{
  this->Init();
  *this = d;
}

//---------------------------------------------------------------------------
inline StringA::~StringA (void)
{
  this->Reset();
}




//---------------------------------------------------------------------------
inline void StringA::Init (void)
{
  m_uiLength        = 0;
  m_pszData         = (char*)&m_szBaseBuffer;
  m_uiAllocSize     = STRING_BASE_BUFFER_SIZE;
  m_szBaseBuffer[0] = 0;
}

//---------------------------------------------------------------------------
inline bool StringA::IsBufferAlloc (void) const
{
  return (m_pszData && (m_pszData != (char*)&m_szBaseBuffer));
}




//---------------------------------------------------------------------------
inline void StringA::Grow (uint uiDesiredSize, bool bKeepOldContent/*=true*/)
{
  if(uiDesiredSize > m_uiAllocSize)
    this->Resize(uiDesiredSize, bKeepOldContent);
}

//---------------------------------------------------------------------------
inline void StringA::Compact (void)
{
  if(m_uiLength < m_uiAllocSize - 1)
    this->Resize(m_uiLength + 1, true);
}




//---------------------------------------------------------------------------
inline void StringA::Clear (void)
{
  // this->Grow(1, false);
  m_pszData[0] = 0;
  m_uiLength   = 0;
}

//---------------------------------------------------------------------------
inline bool StringA::IsEmpty (void) const
{
  return (m_uiLength == 0);
}

//---------------------------------------------------------------------------
inline uint StringA::Length (void) const
{
  return m_uiLength;
}

//---------------------------------------------------------------------------
inline uint StringA::MaxLength (void) const
{
  return m_uiAllocSize - 1;
}




//---------------------------------------------------------------------------
inline const char* StringA::c_str (void) const
{
  return m_pszData;
}

//---------------------------------------------------------------------------
inline StringA::operator const char* (void) const
{
  return this->c_str();
}

//---------------------------------------------------------------------------
inline StringA::operator const char* (void)
{
  return this->c_str();
}




//---------------------------------------------------------------------------
inline char StringA::operator[] (int8 nIndex) const
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (int8 nIndex)
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (uint8 nIndex) const
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (uint8 nIndex)
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (int16 nIndex) const
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (int16 nIndex)
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (uint16 nIndex) const
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (uint16 nIndex)
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (int32 nIndex) const
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (int32 nIndex)
{
  XASSERT(nIndex >= 0);
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (uint32 nIndex) const
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (uint32 nIndex)
{
  XASSERT((uint)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (int64 nIndex) const
{
  XASSERT(nIndex >= 0);
  XASSERT(nIndex <= (int64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (int64 nIndex)
{
  XASSERT(nIndex >= 0);
  XASSERT(nIndex <= (int64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (uint64 nIndex) const
{
  XASSERT(nIndex <= (uint64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (uint64 nIndex)
{
  XASSERT(nIndex <= (uint64)m_uiLength);
  return m_pszData[nIndex];
}




//---------------------------------------------------------------------------
inline void StringA::Copy (const std::string& rhs)
{
  this->Copy(rhs.c_str());
}




//---------------------------------------------------------------------------
inline void StringA::operator= (const StringA& rhs)
{
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const std::string& rhs)
{
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const char* rhs)
{
  this->Copy(rhs);
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const char c)
{
  this->Grow(2, false);
  m_pszData[0] = c;
  m_pszData[1] = 0;
  m_uiLength = 1;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const int16 i)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%d", i);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const uint16 ui)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%u", ui);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const int32 i)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%d", i);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const uint32 ui)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%u", ui);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const int64 i)
{
  char tmp[32];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%"PRId64, i);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const uint64 ui)
{
  char tmp[32];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%"PRIu64, ui);
  this->Grow((uint)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (uint)nLen;
}




//---------------------------------------------------------------------------
inline void StringA::Append (const std::string& str)
{
  this->Append(str.c_str());
}

//---------------------------------------------------------------------------
inline void StringA::Append (const std::string& str, uint uiLength)
{
  this->Append(str.c_str(), uiLength);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const char c)
{
  this->Grow(m_uiLength + 2, true);
  m_pszData[m_uiLength++] = c;
  m_pszData[m_uiLength]   = 0;
}

//---------------------------------------------------------------------------
inline void StringA::Append (const int16 i)
{
  char tmp[16];
  sprintf((char*)&tmp, "%d", i);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const uint16 ui)
{
  char tmp[16];
  sprintf((char*)&tmp, "%u", ui);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const int32 i)
{
  char tmp[16];
  sprintf((char*)&tmp, "%d", i);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const uint32 ui)
{
  char tmp[16];
  sprintf((char*)&tmp, "%u", ui);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const int64 i)
{
  char tmp[32];
  sprintf((char*)&tmp, "%"PRId64, i);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const uint64 ui)
{
  char tmp[32];
  sprintf((char*)&tmp, "%"PRIu64, ui);
  this->Append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const float f)
{
  StringA strTemp(f);
  this->Append(strTemp);
}

//---------------------------------------------------------------------------
inline void StringA::Append (const double d)
{
  StringA strTemp(d);
  this->Append(strTemp);
}




//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const StringA& rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const std::string& rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const char* rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const bool b)
{
  this->Append(b);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const char c)
{
  this->Append(c);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const int16 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const uint16 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const int32 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const uint32 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const int64 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const uint64 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const float f)
{
  this->Append(f);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const double d)
{
  this->Append(d);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const StringA& rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const std::string& rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const char* rhs)
{
  this->Append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const bool b)
{
  this->Append(b);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const char c)
{
  this->Append(c);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const int16 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const uint16 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const int32 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const uint32 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const int64 i)
{
  this->Append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const uint64 ui)
{
  this->Append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const float f)
{
  this->Append(f);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const double d)
{
  this->Append(d);
  return *this;
}




//---------------------------------------------------------------------------
inline StringA& StringA::operator/= (const StringA& rhs)
{
  this->PathAppend(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator/= (const std::string& rhs)
{
  this->PathAppend(rhs.c_str());
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator/= (const char* rhs)
{
  this->PathAppend(rhs);
  return *this;
}




//---------------------------------------------------------------------------
inline int StringA::Compare (const char* psz) const
{
  return StringA::StrCompare(m_pszData, psz);
}

//---------------------------------------------------------------------------
inline int StringA::CompareCount (const char* psz, uint uiMaxCount) const
{
  return StringA::StrCompareCount(m_pszData, psz, uiMaxCount);
}

//---------------------------------------------------------------------------
inline int StringA::CompareI (const char* psz) const
{
  return StringA::StrCompareI(m_pszData, psz);
}

//---------------------------------------------------------------------------
inline int StringA::CompareCountI (const char* psz, uint uiMaxCount) const
{
  return StringA::StrCompareCountI(m_pszData, psz, uiMaxCount);
}

//---------------------------------------------------------------------------
inline bool StringA::HasPrefix (const char c, bool bCaseSensitive)
{
  if(m_uiLength >= 1)
  {
    if(bCaseSensitive)
      return m_pszData[0] == c;
    else
      return StringA::CharToLower(m_pszData[0]) == StringA::CharToLower(c);
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::HasPrefix (const char* psz, bool bCaseSensitive)
{
  uint uiLen = (uint)strlen(psz);
  if(m_uiLength < uiLen)
    return false;

  if(bCaseSensitive)
    return StringA::StrCompareCount(m_pszData, psz, uiLen) == 0;
  else
    return StringA::StrCompareCountI(m_pszData, psz, uiLen) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::HasSuffix (const char c, bool bCaseSensitive)
{
  if(m_uiLength >= 1)
  {
    if(bCaseSensitive)
      return m_pszData[m_uiLength - 1] == c;
    else
      return StringA::CharToLower(m_pszData[m_uiLength - 1]) == StringA::CharToLower(c);
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::HasSuffix (const char* psz, bool bCaseSensitive)
{
  uint uiLen = (uint)strlen(psz);
  if(m_uiLength < uiLen)
    return false;

  if(bCaseSensitive)
    return StringA::StrCompareCount(m_pszData + m_uiLength - uiLen, psz, uiLen) == 0;
  else
    return StringA::StrCompareCountI(m_pszData + m_uiLength - uiLen, psz, uiLen) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::Equals (const char* psz) const
{
  return this->Compare(psz) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::EqualsI (const char* psz) const
{
  return this->CompareI(psz) == 0;
}




//---------------------------------------------------------------------------
inline void StringA::ToLower (void)
{
  StringA::StrToLower(m_pszData);
}

//---------------------------------------------------------------------------
inline void StringA::ToUpper (void)
{
  StringA::StrToUpper(m_pszData);
}

//---------------------------------------------------------------------------
inline void StringA::InvertCase (void)
{
  StringA::StrInvertCase(m_pszData);
}




//---------------------------------------------------------------------------
inline StringA StringA::Left (uint uiLength) const
{
  return this->Mid(0, uiLength);
}

//---------------------------------------------------------------------------
inline const char* StringA::Left (uint uiLength, StringA& strOutResult) const
{
  return this->Mid(0, uiLength, strOutResult);
}

//---------------------------------------------------------------------------
inline StringA StringA::Right (uint uiLength) const
{
  if(uiLength > m_uiLength)
    return *this;

  return this->Mid(m_uiLength - uiLength, uiLength);
}

//---------------------------------------------------------------------------
inline const char* StringA::Right (uint uiLength, StringA& strOutResult) const
{
  if(uiLength > m_uiLength)
  {
    strOutResult = *this;
    return strOutResult;
  }

  return this->Mid(m_uiLength - uiLength, uiLength, strOutResult);
}




//---------------------------------------------------------------------------
inline StringA StringA::Substr (int nOffset, int nLength/*=0*/) const
{
  StringA strResult;

  if(!this->Substr(strResult, nOffset, nLength))
  {
    XASSERT(strResult.IsEmpty());
    strResult.Clear();
  }

  return strResult;
}




//---------------------------------------------------------------------------
inline void StringA::TrimWhitespaces (void)
{
  this->TrimLeftWhitespaces();
  this->TrimRightWhitespaces();
}

//---------------------------------------------------------------------------
inline void StringA::Trim (const char c)
{
  this->TrimLeft(c);
  this->TrimRight(c);
}

//---------------------------------------------------------------------------
inline void StringA::Trim (const char* pszCharList)
{
  this->TrimLeft(pszCharList);
  this->TrimRight(pszCharList);
}

//---------------------------------------------------------------------------
inline void StringA::TrimLeftWhitespaces (void)
{
  // "\x0B" is the ASCII 11 character : vertical tab
  this->TrimLeft(" \t\r\n\x0B");
}

//---------------------------------------------------------------------------
inline void StringA::TrimRightWhitespaces (void)
{
  // "\x0B" is the ASCII 11 character : vertical tab
  this->TrimRight(" \t\r\n\x0B");
}




//---------------------------------------------------------------------------
inline bool StringA::IsBool (bool bStrict/*=false*/, int* pnOutValue/*=NULL*/) const
{
  return StringA::StrIsBool(m_pszData, bStrict, pnOutValue);
}

//---------------------------------------------------------------------------
inline bool StringA::IsInt (bool bStrict/*=false*/) const
{
  return StringA::StrIsInt(m_pszData, bStrict);
}

//---------------------------------------------------------------------------
inline bool StringA::IsFloat (bool bStrict/*=false*/) const
{
  return StringA::StrIsFloat(m_pszData, bStrict);
}

//---------------------------------------------------------------------------
inline bool StringA::IsFullOfDigits (void) const
{
  return StringA::StrIsFullOfDigits(m_pszData);
}




//---------------------------------------------------------------------------
inline void StringA::Format (const char* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->FormatV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
inline void StringA::FormatAppend (const char* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->FormatAppendV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
inline void StringA::FormatAppendV (const char* pszFormat, va_list arglist)
{
  StringA strTemp;
  strTemp.FormatV(pszFormat, arglist);
  *this += strTemp;
}




//---------------------------------------------------------------------------
inline int StringA::FormatBytesToHuman (uint uiBytes)
{
  this->Clear();
  return this->FormatAppendBytesToHuman(uiBytes);
}

//---------------------------------------------------------------------------
inline int StringA::FormatBytesToHuman64 (uint64 uiBytes)
{
  this->Clear();
  return this->FormatAppendBytesToHuman64(uiBytes);
}




//---------------------------------------------------------------------------
inline int StringA::FindFirstOf (char cNeedle, int nStartOffset/*=0*/) const
{
  return StringA::StrFindFirstOf(m_pszData, cNeedle, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstOf (const char* pszNeedles, int nStartOffset/*=0*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::StrFindFirstOf(m_pszData, pszNeedles, true, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstOfI (char cNeedle, int nStartOffset/*=0*/) const
{
  return StringA::StrFindFirstOf(m_pszData, cNeedle, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstOfI (const char* pszNeedles, int nStartOffset/*=0*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::StrFindFirstOf(m_pszData, pszNeedles, false, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstNotOf (char cNeedless, int nStartOffset/*=0*/) const
{
  return StringA::StrFindFirstNotOf(m_pszData, cNeedless, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstNotOf (const char* pszNeedless, int nStartOffset/*=0*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::StrFindFirstNotOf(m_pszData, pszNeedless, true, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstNotOfI (char cNeedless, int nStartOffset/*=0*/) const
{
  return StringA::StrFindFirstNotOf(m_pszData, cNeedless, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindFirstNotOfI (const char* pszNeedless, int nStartOffset/*=0*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::StrFindFirstNotOf(m_pszData, pszNeedless, false, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastOf (char cNeedle, int nStartOffset/*=-1*/) const
{
  return StringA::StrFindLastOf(m_pszData, cNeedle, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastOf (const char* pszNeedles, int nStartOffset/*=-1*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::StrFindLastOf(m_pszData, pszNeedles, true, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastOfI (char cNeedle, int nStartOffset/*=-1*/) const
{
  return StringA::StrFindLastOf(m_pszData, cNeedle, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastOfI (const char* pszNeedles, int nStartOffset/*=-1*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::StrFindLastOf(m_pszData, pszNeedles, false, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastNotOf (char cNeedless, int nStartOffset/*=-1*/) const
{
  return StringA::StrFindLastNotOf(m_pszData, cNeedless, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastNotOf (const char* pszNeedless, int nStartOffset/*=-1*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::StrFindLastNotOf(m_pszData, pszNeedless, true, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastNotOfI (char cNeedless, int nStartOffset/*=-1*/) const
{
  return StringA::StrFindLastNotOf(m_pszData, cNeedless, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::FindLastNotOfI (const char* pszNeedless, int nStartOffset/*=-1*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::StrFindLastNotOf(m_pszData, pszNeedless, false, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::Find (const char cNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::StrFind(m_pszData, cNeedle, true, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::Find (const char* pszNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::StrFind(m_pszData, pszNeedle, true, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::FindI (const char cNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::StrFind(m_pszData, cNeedle, false, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::FindI (const char* pszNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::StrFind(m_pszData, pszNeedle, false, nStartOffset, nEndOffset, nEndOffset);
}




//---------------------------------------------------------------------------
inline bool StringA::Match (const char* pszPattern) const
{
  return StringA::StrMatch(m_pszData, pszPattern, true);
}

//---------------------------------------------------------------------------
inline bool StringA::MatchI (const char* pszPattern) const
{
  return StringA::StrMatch(m_pszData, pszPattern, false);
}




//---------------------------------------------------------------------------
inline uint StringA::Hash (void) const
{
  return StringA::StrHash(m_pszData);
}

//---------------------------------------------------------------------------
inline uint StringA::HashI (void) const
{
  return StringA::StrHashI(m_pszData);
}




//---------------------------------------------------------------------------
inline void StringA::PathBackSlashesToSlashes (void)
{
  this->Replace('\\', '/');
}

//---------------------------------------------------------------------------
inline void StringA::PathSlashesToBackSlashes (void)
{
  this->Replace('/', '\\');
}

//---------------------------------------------------------------------------
inline bool StringA::PathStripTrailingSeparator (void)
{
  if((m_uiLength > 0) && StringA::CharIsPathSeparator(m_pszData[m_uiLength-1]))
  {
    m_pszData[m_uiLength-1] = 0;
    --m_uiLength;
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::PathHasTrailingSeparator (void) const
{
  return (m_uiLength > 0) && StringA::CharIsPathSeparator(m_pszData[m_uiLength-1]);
}

//---------------------------------------------------------------------------
inline bool StringA::PathIsWindowsNetwork (void) const
{
  return (m_uiLength >= 3) && StringA::CharIsPathSeparator(m_pszData[0]) && StringA::CharIsPathSeparator(m_pszData[1]) && !StringA::CharIsPathSeparator(m_pszData[2]);
}




//---------------------------------------------------------------------------
inline char StringA::CharToLower (int c)
{
  if((c >= 'A') && (c <= 'Z'))
    return (c + ('a' - 'A'));

  return c;
}

//---------------------------------------------------------------------------
inline char StringA::CharToUpper (int c)
{
  if((c >= 'a') && (c <= 'z'))
    return (c - ('a' - 'A'));

  return c;
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsLower (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 'a') && (c <= 'z')) || ((c >= 0xE0) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsUpper (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c <= 'Z') && (c >= 'A')) || ((c >= 0xC0) && (c <= 0xDF));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsAlpha (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 0xC0) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsNumeric (int c)
{
  return ((c <= '9') && (c >= '0'));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsAlNum (int c)
{
  return StringA::CharIsNumeric(c) || StringA::CharIsAlpha(c);
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsPrintable (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 0x20) && (c <= 0x7E)) || ((c >= 0xA1) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsSpace (int c)
{
  return ::isspace(c) != 0; // usually returns true with 0x09 or 0x0D or 0x20
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsTab (char c)
{
  return (c == '\t');
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsNewLine (char c)
{
  return (c == '\n') || (c == '\r') || (c == '\v');
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsPathSeparator (char c)
{
  // finally, we always recognize '\\' as a valid path separator, even on
  // non-windows platforms so we keep the same behavior accross platforms,
  // which is mainly what we want here...
  // by the way, if you want to be sure your path is cleaned up, check the
  // StringA::PathBackSlashesToSlashes() and
  // StringA::PathSlashesToBackSlashes() methods.  - jcl
  return (c == '/') || (c == '\\');
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsWindowsDriveLetter (char c)
{
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsBase64 (char c)
{
  return (::isalnum(c) || (c == '+') || (c == '/'));
}

//---------------------------------------------------------------------------
inline bool StringA::CharIsHex (char c)
{
  return ( ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) );
}

//---------------------------------------------------------------------------
inline uint8 StringA::CharHexToInt (char c)
{
  if((c >= '0') && (c <= '9'))
    return c - '0';
  else if((c >= 'a') && (c <= 'f'))
    return c - 'a' + 10;
  else if((c >= 'A') && (c <= 'F'))
    return c - 'A' + 10;
  else
    return uchar(-1);
}

//---------------------------------------------------------------------------
inline void StringA::StrToLower (char* psz)
{
  char* p = psz;

  XASSERT(psz);
  if(!psz) return;

  while(*p)
  {
    if(StringA::CharIsUpper(*p))
      *p += 'a' - 'A';
    ++p;
  }
}

//---------------------------------------------------------------------------
inline void StringA::StrToUpper (char* psz)
{
  char* p = psz;

  XASSERT(psz);
  if(!psz) return;

  while(*p)
  {
    if(StringA::CharIsLower(*p))
      *p -= 'a' - 'A';
    ++p;
  }
}

//---------------------------------------------------------------------------
inline void StringA::StrInvertCase (char* psz)
{
  char* p = psz;

  XASSERT(psz);
  if(!psz) return;

  while(*p)
  {
    if(StringA::CharIsLower(*p))
      *p -= 'a' - 'A';
    else if(StringA::CharIsUpper(*p))
      *p += 'a' - 'A';
    ++p;
  }
}




//---------------------------------------------------------------------------
inline uint32 StringA::StrHash (const char* psz)
{
  // Hash method from Pr. Daniel J. Bernstein.

  uint32 uiHash = STRING_HASH_SEED;

  XASSERT(psz);
  if(!psz || !psz[0])
    return uiHash;

  while(*psz)
    uiHash = ((uiHash << 5) + uiHash) + (*psz++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline uint32 StringA::StrHash (const char* pBlock, uint uiCount)
{
  // Hash method from Pr. Daniel J. Bernstein.

  uint32 uiHash = STRING_HASH_SEED;
  uint   Ix;

  XASSERT(pBlock);
  XASSERT(uiCount);
  if(!pBlock || !uiCount)
    return uiHash;

  for(Ix=0 ; Ix<uiCount ; ++Ix)
    uiHash = ((uiHash << 5) + uiHash) + (*pBlock++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline uint32 StringA::StrHashI (const char* psz)
{
  // Hash method from Pr. Daniel J. Bernstein.

  uint32 uiHash = STRING_HASH_SEED;

  XASSERT(psz);
  if(!psz || !psz[0])
    return uiHash;

  while(*psz)
    uiHash = ((uiHash << 5) + uiHash) + StringA::CharToLower(*psz++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline uint32 StringA::StrHashI (const char* pBlock, uint uiCount)
{
  // Hash method from Pr. Daniel J. Bernstein.

  uint32 uiHash = STRING_HASH_SEED;
  uint   Ix;

  XASSERT(pBlock);
  XASSERT(uiCount);
  if(!pBlock || !uiCount)
    return uiHash;

  for(Ix=0 ; Ix<uiCount ; ++Ix)
    uiHash = ((uiHash << 5) + uiHash) + StringA::CharToLower(*pBlock++);

  return uiHash;
}




//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.Append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const std::string& rhs)
{
  StringA strRes(lhs);
  strRes.Append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const char* rhs)
{
  StringA strRes(lhs);
  XASSERT(rhs);
  if(rhs)
    strRes.Append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const std::string& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.Append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const char* lhs, const StringA& rhs)
{
  StringA strRes;
  XASSERT(lhs);
  if(lhs)
    strRes.Copy(lhs);
  strRes.Append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const bool b)
{
  StringA strRes(lhs);
  strRes.Append(b);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const char c)
{
  StringA strRes(lhs);
  strRes.Append(c);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const int32 i)
{
  StringA strRes(lhs);
  strRes.Append(i);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const uint32 ui)
{
  StringA strRes(lhs);
  strRes.Append(ui);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const int64 i)
{
  StringA strRes(lhs);
  strRes.Append(i);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const uint64 ui)
{
  StringA strRes(lhs);
  strRes.Append(ui);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const float f)
{
  StringA strRes(lhs);
  strRes.Append(f);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const double d)
{
  StringA strRes(lhs);
  strRes.Append(d);
  return strRes;
}




//---------------------------------------------------------------------------
inline StringA operator/ (const StringA& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.PathAppend(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator/ (const StringA& lhs, const std::string& rhs)
{
  StringA strRes(lhs);
  strRes.PathAppend(rhs.c_str());
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator/ (const StringA& lhs, const char* rhs)
{
  StringA strRes(lhs);
  XASSERT(rhs);
  if(rhs)
    strRes.PathAppend(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator/ (const std::string& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.PathAppend(rhs.c_str());
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator/ (const char* lhs, const StringA& rhs)
{
  StringA strRes;
  XASSERT(lhs);
  if(lhs)
    strRes.Copy(lhs);
  strRes.PathAppend(rhs);
  return strRes;
}




//---------------------------------------------------------------------------
inline bool operator== (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) == 0;
}

//---------------------------------------------------------------------------
inline bool operator== (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) == 0;
}

//---------------------------------------------------------------------------
inline bool operator== (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) == 0;
}

//---------------------------------------------------------------------------
inline bool operator== (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) == 0;
}

//---------------------------------------------------------------------------
inline bool operator== (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) == 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) != 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) != 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) != 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) != 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) != 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) < 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) < 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) < 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) < 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) < 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) > 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) > 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) > 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) > 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) > 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const StringA& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.m_pszData) >= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const StringA& lhs, const std::string& rhs)
{
  return StringA::StrCompare(lhs.m_pszData, rhs.c_str()) >= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const StringA& lhs, const char* rhs)
{
  XASSERT(rhs);
  return StringA::StrCompare(lhs.m_pszData, rhs) >= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const std::string& lhs, const StringA& rhs)
{
  return StringA::StrCompare(lhs.c_str(), rhs.m_pszData) >= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const char* lhs, const StringA& rhs)
{
  XASSERT(lhs);
  return StringA::StrCompare(lhs, rhs.m_pszData) >= 0;
}
