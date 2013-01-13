//***************************************************************************
//
// Author     : Jean-Charles Lefebvre
// Created On : 2011-10-07 18:46:18
//
// $Id$
//
//***************************************************************************


//---------------------------------------------------------------------------
inline void StringA::setDefaultBoolStringType (StringA::BoolStringType eBoolStringType)
{
  ms_eBoolStringType = eBoolStringType;
}

//---------------------------------------------------------------------------
inline StringA::BoolStringType StringA::getDefaultBoolStringType (void)
{
  return ms_eBoolStringType;
}




//---------------------------------------------------------------------------
inline StringA::StringA (void)
{
  this->init();
}

//---------------------------------------------------------------------------
inline StringA::StringA (const StringA& rhs)
{
  this->init();
  this->copy(rhs);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const StringA& rhs, STRA_UINT uiCount, STRA_UINT uiStart/*=0*/)
{
  this->init();
  this->copyCount(rhs.m_pszData, uiCount, uiStart);
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA::StringA (const std::string& rhs)
{
  this->init();
  this->copy(rhs);
}
#endif

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA::StringA (const std::string& rhs, STRA_UINT uiCount, STRA_UINT uiStart/*=0*/)
{
  this->init();
  this->copyCount(rhs.c_str(), uiCount, uiStart);
}
#endif

//---------------------------------------------------------------------------
inline StringA::StringA (const char* rhs)
{
  this->init();
  this->copy(rhs);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const char* rhs, STRA_UINT uiCount, STRA_UINT uiStart/*=0*/)
{
  this->init();
  this->copyCount(rhs, uiCount, uiStart);
}

//---------------------------------------------------------------------------
inline StringA::StringA (const bool b)
{
  this->init();
  *this = b;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const char c)
{
  this->init();
  *this = c;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const STRA_INT32 i)
{
  this->init();
  *this = i;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const STRA_UINT32 ui)
{
  this->init();
  *this = ui;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const STRA_INT64 i)
{
  this->init();
  *this = i;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const STRA_UINT64 ui)
{
  this->init();
  *this = ui;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const float f)
{
  this->init();
  *this = f;
}

//---------------------------------------------------------------------------
inline StringA::StringA (const double d)
{
  this->init();
  *this = d;
}

//---------------------------------------------------------------------------
inline StringA::~StringA (void)
{
  this->reset();
}




//---------------------------------------------------------------------------
inline void StringA::init (void)
{
  m_uiLength        = 0;
  m_pszData         = (char*)&m_szBaseBuffer;
  m_uiAllocSize     = STRING_BASE_BUFFER_SIZE;
  m_szBaseBuffer[0] = 0;
}

//---------------------------------------------------------------------------
inline bool StringA::isBufferAlloc (void) const
{
  return (m_pszData && (m_pszData != (char*)&m_szBaseBuffer));
}




//---------------------------------------------------------------------------
inline void StringA::grow (STRA_UINT uiDesiredSize, bool bKeepOldContent/*=true*/)
{
  if (uiDesiredSize > m_uiAllocSize)
    this->resize(uiDesiredSize, bKeepOldContent);
}

//---------------------------------------------------------------------------
inline void StringA::compact (void)
{
  if (m_uiLength < m_uiAllocSize - 1)
    this->resize(m_uiLength + 1, true);
}




//---------------------------------------------------------------------------
inline void StringA::clear (void)
{
  // this->grow(1, false);
  m_pszData[0] = 0;
  m_uiLength   = 0;
}

//---------------------------------------------------------------------------
inline bool StringA::isEmpty (void) const
{
  return (m_uiLength == 0);
}

//---------------------------------------------------------------------------
inline STRA_UINT StringA::length (void) const
{
  return m_uiLength;
}

//---------------------------------------------------------------------------
inline STRA_UINT StringA::maxLength (void) const
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
inline char StringA::operator[] (STRA_INT8 nIndex) const
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_INT8 nIndex)
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_UINT8 nIndex) const
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_UINT8 nIndex)
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_INT16 nIndex) const
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_INT16 nIndex)
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_UINT16 nIndex) const
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_UINT16 nIndex)
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_INT32 nIndex) const
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_INT32 nIndex)
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_UINT32 nIndex) const
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_UINT32 nIndex)
{
  STRA_ASSERT((STRA_UINT)nIndex <= m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_INT64 nIndex) const
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT(nIndex <= (STRA_INT64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_INT64 nIndex)
{
  STRA_ASSERT(nIndex >= 0);
  STRA_ASSERT(nIndex <= (STRA_INT64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char StringA::operator[] (STRA_UINT64 nIndex) const
{
  STRA_ASSERT(nIndex <= (STRA_UINT64)m_uiLength);
  return m_pszData[nIndex];
}

//---------------------------------------------------------------------------
inline char& StringA::operator[] (STRA_UINT64 nIndex)
{
  STRA_ASSERT(nIndex <= (STRA_UINT64)m_uiLength);
  return m_pszData[nIndex];
}




//---------------------------------------------------------------------------
#ifdef _STRING_
inline void StringA::copy (const std::string& rhs)
{
  this->copy(rhs.c_str());
}
#endif




//---------------------------------------------------------------------------
inline void StringA::operator= (const StringA& rhs)
{
  this->copy(rhs);
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline void StringA::operator= (const std::string& rhs)
{
  this->copy(rhs);
}
#endif

//---------------------------------------------------------------------------
inline void StringA::operator= (const char* rhs)
{
  this->copy(rhs);
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const char c)
{
  this->grow(2, false);
  m_pszData[0] = c;
  m_pszData[1] = 0;
  m_uiLength = 1;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_INT16 i)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%d", i);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_UINT16 ui)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%u", ui);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_INT32 i)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%d", i);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_UINT32 ui)
{
  char tmp[16];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%u", ui);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_INT64 i)
{
  char tmp[32];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%I64d", i);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}

//---------------------------------------------------------------------------
inline void StringA::operator= (const STRA_UINT64 ui)
{
  char tmp[32];
  int  nLen;

  nLen = sprintf((char*)&tmp, "%I64u", ui);
  this->grow((STRA_UINT)nLen + 1, false);
  memcpy(m_pszData, (char*)&tmp, size_t(nLen + 1));
  m_uiLength = (STRA_UINT)nLen;
}




//---------------------------------------------------------------------------
#ifdef _STRING_
inline void StringA::append (const std::string& str)
{
  this->append(str.c_str());
}
#endif

//---------------------------------------------------------------------------
#ifdef _STRING_
inline void StringA::append (const std::string& str, STRA_UINT uiLength)
{
  this->append(str.c_str(), uiLength);
}
#endif

//---------------------------------------------------------------------------
inline void StringA::append (const char c)
{
  this->grow(m_uiLength + 2, true);
  m_pszData[m_uiLength++] = c;
  m_pszData[m_uiLength]   = 0;
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_INT16 i)
{
  char tmp[16];
  sprintf((char*)&tmp, "%d", i);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_UINT16 ui)
{
  char tmp[16];
  sprintf((char*)&tmp, "%u", ui);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_INT32 i)
{
  char tmp[16];
  sprintf((char*)&tmp, "%d", i);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_UINT32 ui)
{
  char tmp[16];
  sprintf((char*)&tmp, "%u", ui);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_INT64 i)
{
  char tmp[32];
  sprintf((char*)&tmp, "%I64d", i);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const STRA_UINT64 ui)
{
  char tmp[32];
  sprintf((char*)&tmp, "%I64u", ui);
  this->append((char*)&tmp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const float f)
{
  StringA strTemp(f);
  this->append(strTemp);
}

//---------------------------------------------------------------------------
inline void StringA::append (const double d)
{
  StringA strTemp(d);
  this->append(strTemp);
}




//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const StringA& rhs)
{
  this->append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA& StringA::operator+= (const std::string& rhs)
{
  this->append(rhs);
  return *this;
}
#endif

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const char* rhs)
{
  this->append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const bool b)
{
  this->append(b);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const char c)
{
  this->append(c);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_INT16 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_UINT16 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_INT32 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_UINT32 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_INT64 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const STRA_UINT64 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const float f)
{
  this->append(f);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator+= (const double d)
{
  this->append(d);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const StringA& rhs)
{
  this->append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA& StringA::operator<< (const std::string& rhs)
{
  this->append(rhs);
  return *this;
}
#endif

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const char* rhs)
{
  this->append(rhs);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const bool b)
{
  this->append(b);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const char c)
{
  this->append(c);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_INT16 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_UINT16 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_INT32 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_UINT32 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_INT64 i)
{
  this->append(i);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const STRA_UINT64 ui)
{
  this->append(ui);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const float f)
{
  this->append(f);
  return *this;
}

//---------------------------------------------------------------------------
inline StringA& StringA::operator<< (const double d)
{
  this->append(d);
  return *this;
}




//---------------------------------------------------------------------------
inline StringA& StringA::operator/= (const StringA& rhs)
{
  this->pathAppend(rhs);
  return *this;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA& StringA::operator/= (const std::string& rhs)
{
  this->pathAppend(rhs.c_str());
  return *this;
}
#endif

//---------------------------------------------------------------------------
inline StringA& StringA::operator/= (const char* rhs)
{
  this->pathAppend(rhs);
  return *this;
}




//---------------------------------------------------------------------------
inline int StringA::compare (const char* psz) const
{
  return StringA::strCompare(m_pszData, psz);
}

//---------------------------------------------------------------------------
inline int StringA::compareCount (const char* psz, STRA_UINT uiMaxCount) const
{
  return StringA::strCompareCount(m_pszData, psz, uiMaxCount);
}

//---------------------------------------------------------------------------
inline int StringA::compareI (const char* psz) const
{
  return StringA::strCompareI(m_pszData, psz);
}

//---------------------------------------------------------------------------
inline int StringA::compareCountI (const char* psz, STRA_UINT uiMaxCount) const
{
  return StringA::strCompareCountI(m_pszData, psz, uiMaxCount);
}

//---------------------------------------------------------------------------
inline bool StringA::hasPrefix (const char c, bool bCaseSensitive)
{
  if (m_uiLength >= 1)
  {
    if (bCaseSensitive)
      return m_pszData[0] == c;
    else
      return StringA::charToLower(m_pszData[0]) == StringA::charToLower(c);
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::hasPrefix (const char* psz, bool bCaseSensitive)
{
  STRA_UINT uiLen = (STRA_UINT)strlen(psz);
  if (m_uiLength < uiLen)
    return false;

  if (bCaseSensitive)
    return StringA::strCompareCount(m_pszData, psz, uiLen) == 0;
  else
    return StringA::strCompareCountI(m_pszData, psz, uiLen) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::hasSuffix (const char c, bool bCaseSensitive)
{
  if (m_uiLength >= 1)
  {
    if (bCaseSensitive)
      return m_pszData[m_uiLength - 1] == c;
    else
      return StringA::charToLower(m_pszData[m_uiLength - 1]) == StringA::charToLower(c);
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::hasSuffix (const char* psz, bool bCaseSensitive)
{
  STRA_UINT uiLen = (STRA_UINT)strlen(psz);
  if (m_uiLength < uiLen)
    return false;

  if (bCaseSensitive)
    return StringA::strCompareCount(m_pszData + m_uiLength - uiLen, psz, uiLen) == 0;
  else
    return StringA::strCompareCountI(m_pszData + m_uiLength - uiLen, psz, uiLen) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::equals (const char* psz) const
{
  return this->compare(psz) == 0;
}

//---------------------------------------------------------------------------
inline bool StringA::equalsI (const char* psz) const
{
  return this->compareI(psz) == 0;
}




//---------------------------------------------------------------------------
inline void StringA::toLower (void)
{
  StringA::strToLower(m_pszData);
}

//---------------------------------------------------------------------------
inline void StringA::toUpper (void)
{
  StringA::strToUpper(m_pszData);
}

//---------------------------------------------------------------------------
inline void StringA::invertCase (void)
{
  StringA::strInvertCase(m_pszData);
}




//---------------------------------------------------------------------------
inline StringA StringA::left (STRA_UINT uiLength) const
{
  return this->mid(0, uiLength);
}

//---------------------------------------------------------------------------
inline const char* StringA::left (STRA_UINT uiLength, StringA& strOutResult) const
{
  return this->mid(0, uiLength, strOutResult);
}

//---------------------------------------------------------------------------
inline StringA StringA::right (STRA_UINT uiLength) const
{
  if (uiLength > m_uiLength)
    return *this;

  return this->mid(m_uiLength - uiLength, uiLength);
}

//---------------------------------------------------------------------------
inline const char* StringA::right (STRA_UINT uiLength, StringA& strOutResult) const
{
  if (uiLength > m_uiLength)
  {
    strOutResult = *this;
    return strOutResult;
  }

  return this->mid(m_uiLength - uiLength, uiLength, strOutResult);
}




//---------------------------------------------------------------------------
inline StringA StringA::substr (int nOffset, int nLength/*=0*/) const
{
  StringA strResult;

  if (!this->substr(strResult, nOffset, nLength))
  {
    STRA_ASSERT(strResult.isEmpty());
    strResult.clear();
  }

  return strResult;
}




//---------------------------------------------------------------------------
inline void StringA::trimWhitespaces (void)
{
  this->trimLeftWhitespaces();
  this->trimRightWhitespaces();
}

//---------------------------------------------------------------------------
inline void StringA::trim (const char c)
{
  this->trimLeft(c);
  this->trimRight(c);
}

//---------------------------------------------------------------------------
inline void StringA::trim (const char* pszCharList)
{
  this->trimLeft(pszCharList);
  this->trimRight(pszCharList);
}

//---------------------------------------------------------------------------
inline void StringA::trimLeftWhitespaces (void)
{
  // "\x0B" is the ASCII 11 character : vertical tab
  this->trimLeft(" \t\r\n\x0B");
}

//---------------------------------------------------------------------------
inline void StringA::trimRightWhitespaces (void)
{
  // "\x0B" is the ASCII 11 character : vertical tab
  this->trimRight(" \t\r\n\x0B");
}




//---------------------------------------------------------------------------
inline bool StringA::isBool (bool bStrict/*=false*/, int* pnOutValue/*=NULL*/) const
{
  return StringA::strIsBool(m_pszData, bStrict, pnOutValue);
}

//---------------------------------------------------------------------------
inline bool StringA::isInt (bool bStrict/*=false*/) const
{
  return StringA::strIsInt(m_pszData, bStrict);
}

//---------------------------------------------------------------------------
inline bool StringA::isFloat (bool bStrict/*=false*/) const
{
  return StringA::strIsFloat(m_pszData, bStrict);
}

//---------------------------------------------------------------------------
inline bool StringA::isFullOfDigits (void) const
{
  return StringA::strIsFullOfDigits(m_pszData);
}

//---------------------------------------------------------------------------
inline float StringA::extractFloat (STRA_UINT uiStartOffset/*=0*/) const
{
  return (float)this->extractDouble(uiStartOffset);
}




//---------------------------------------------------------------------------
inline void StringA::format (const char* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->formatV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
inline void StringA::formatAppend (const char* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->formatAppendV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
inline void StringA::formatAppendV (const char* pszFormat, va_list arglist)
{
  StringA strTemp;
  strTemp.formatV(pszFormat, arglist);
  *this += strTemp;
}

//---------------------------------------------------------------------------
inline void StringA::formatPrepend (const char* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->formatPrependV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
inline void StringA::formatPrependV (const char* pszFormat, va_list arglist)
{
  StringA strTemp;
  strTemp.formatV(pszFormat, arglist);
  this->insert(strTemp, 0);
}




//---------------------------------------------------------------------------
inline int StringA::formatBytesToHuman (STRA_UINT uiBytes)
{
  this->clear();
  return this->formatAppendBytesToHuman(uiBytes);
}

//---------------------------------------------------------------------------
inline int StringA::formatBytesToHuman64 (STRA_UINT64 uiBytes)
{
  this->clear();
  return this->formatAppendBytesToHuman64(uiBytes);
}




//---------------------------------------------------------------------------
inline int StringA::findFirstOf (char cNeedle, int nStartOffset/*=0*/) const
{
  return StringA::strFindFirstOf(m_pszData, cNeedle, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstOf (const char* pszNeedles, int nStartOffset/*=0*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::strFindFirstOf(m_pszData, pszNeedles, true, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstOfI (char cNeedle, int nStartOffset/*=0*/) const
{
  return StringA::strFindFirstOf(m_pszData, cNeedle, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstOfI (const char* pszNeedles, int nStartOffset/*=0*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::strFindFirstOf(m_pszData, pszNeedles, false, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstNotOf (char cNeedless, int nStartOffset/*=0*/) const
{
  return StringA::strFindFirstNotOf(m_pszData, cNeedless, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstNotOf (const char* pszNeedless, int nStartOffset/*=0*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::strFindFirstNotOf(m_pszData, pszNeedless, true, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstNotOfI (char cNeedless, int nStartOffset/*=0*/) const
{
  return StringA::strFindFirstNotOf(m_pszData, cNeedless, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findFirstNotOfI (const char* pszNeedless, int nStartOffset/*=0*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::strFindFirstNotOf(m_pszData, pszNeedless, false, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastOf (char cNeedle, int nStartOffset/*=-1*/) const
{
  return StringA::strFindLastOf(m_pszData, cNeedle, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastOf (const char* pszNeedles, int nStartOffset/*=-1*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::strFindLastOf(m_pszData, pszNeedles, true, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastOfI (char cNeedle, int nStartOffset/*=-1*/) const
{
  return StringA::strFindLastOf(m_pszData, cNeedle, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastOfI (const char* pszNeedles, int nStartOffset/*=-1*/, int nNeedlesLength/*=-1*/) const
{
  return StringA::strFindLastOf(m_pszData, pszNeedles, false, nStartOffset, (int)m_uiLength, nNeedlesLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastNotOf (char cNeedless, int nStartOffset/*=-1*/) const
{
  return StringA::strFindLastNotOf(m_pszData, cNeedless, true, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastNotOf (const char* pszNeedless, int nStartOffset/*=-1*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::strFindLastNotOf(m_pszData, pszNeedless, true, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastNotOfI (char cNeedless, int nStartOffset/*=-1*/) const
{
  return StringA::strFindLastNotOf(m_pszData, cNeedless, false, nStartOffset, (int)m_uiLength);
}

//---------------------------------------------------------------------------
inline int StringA::findLastNotOfI (const char* pszNeedless, int nStartOffset/*=-1*/, int nNeedlessLength/*=-1*/) const
{
  return StringA::strFindLastNotOf(m_pszData, pszNeedless, false, nStartOffset, (int)m_uiLength, nNeedlessLength);
}

//---------------------------------------------------------------------------
inline int StringA::find (const char cNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::strFind(m_pszData, cNeedle, true, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::find (const char* pszNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::strFind(m_pszData, pszNeedle, true, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::findI (const char cNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::strFind(m_pszData, cNeedle, false, nStartOffset, nEndOffset, nEndOffset);
}

//---------------------------------------------------------------------------
inline int StringA::findI (const char* pszNeedle, int nStartOffset/*=0*/, int nEndOffset/*=-1*/) const
{
  return StringA::strFind(m_pszData, pszNeedle, false, nStartOffset, nEndOffset, nEndOffset);
}




//---------------------------------------------------------------------------
inline bool StringA::match (const char* pszPattern) const
{
  return StringA::strMatch(m_pszData, pszPattern, true);
}

//---------------------------------------------------------------------------
inline bool StringA::matchI (const char* pszPattern) const
{
  return StringA::strMatch(m_pszData, pszPattern, false);
}




//---------------------------------------------------------------------------
inline STRA_UINT StringA::hash (void) const
{
  return StringA::strHash(m_pszData);
}

//---------------------------------------------------------------------------
inline STRA_UINT StringA::hashI (void) const
{
  return StringA::strHashI(m_pszData);
}




//---------------------------------------------------------------------------
inline void StringA::pathBackSlashesToSlashes (void)
{
  this->replace('\\', '/');
}

//---------------------------------------------------------------------------
inline void StringA::pathSlashesToBackSlashes (void)
{
  this->replace('/', '\\');
}

//---------------------------------------------------------------------------
inline bool StringA::pathStripTrailingSeparator (void)
{
  if ((m_uiLength > 0) && StringA::charIsPathSeparator(m_pszData[m_uiLength-1]))
  {
    m_pszData[m_uiLength-1] = 0;
    --m_uiLength;
    return true;
  }

  return false;
}

//---------------------------------------------------------------------------
inline bool StringA::pathHasTrailingSeparator (void) const
{
  return (m_uiLength > 0) && StringA::charIsPathSeparator(m_pszData[m_uiLength-1]);
}

//---------------------------------------------------------------------------
inline bool StringA::pathIsWindowsNetwork (void) const
{
  return (m_uiLength >= 3) && StringA::charIsPathSeparator(m_pszData[0]) && StringA::charIsPathSeparator(m_pszData[1]) && !StringA::charIsPathSeparator(m_pszData[2]);
}




//---------------------------------------------------------------------------
inline char StringA::charToLower (int c)
{
  if ((c >= 'A') && (c <= 'Z'))
    return (c + ('a' - 'A'));

  return c;
}

//---------------------------------------------------------------------------
inline char StringA::charToUpper (int c)
{
  if ((c >= 'a') && (c <= 'z'))
    return (c - ('a' - 'A'));

  return c;
}

//---------------------------------------------------------------------------
inline bool StringA::charIsLower (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 'a') && (c <= 'z')) || ((c >= 0xE0) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsUpper (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c <= 'Z') && (c >= 'A')) || ((c >= 0xC0) && (c <= 0xDF));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsAlpha (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 0xC0) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsNumeric (int c)
{
  return ((c <= '9') && (c >= '0'));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsAlNum (int c)
{
  return StringA::charIsNumeric(c) || StringA::charIsAlpha(c);
}

//---------------------------------------------------------------------------
inline bool StringA::charIsPrintable (int c)
{
  // test for regular ascii and western European high-ascii chars
  return ((c >= 0x20) && (c <= 0x7E)) || ((c >= 0xA1) && (c <= 0xFF));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsSpace (int c)
{
  return ::isspace(c) != 0; // usually returns true with 0x09 or 0x0D or 0x20
}

//---------------------------------------------------------------------------
inline bool StringA::charIsTab (char c)
{
  return (c == '\t');
}

//---------------------------------------------------------------------------
inline bool StringA::charIsNewLine (char c)
{
  return (c == '\n') || (c == '\r') || (c == '\v');
}

//---------------------------------------------------------------------------
inline bool StringA::charIsPathSeparator (char c)
{
  // we always recognize '\\' as a valid path separator, even on non-windows
  // platforms so we keep the same behavior accross platforms, which is
  // mainly what we want here...
  // if you want to be sure your path is cleaned up, check the
  // StringA::pathBackSlashesToSlashes() and
  // StringA::pathSlashesToBackSlashes() methods.  - jcl
  return (c == '/') || (c == '\\');
}

//---------------------------------------------------------------------------
inline bool StringA::charIsWindowsDriveLetter (char c)
{
  return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsBase64 (char c)
{
  return (::isalnum(c) || (c == '+') || (c == '/'));
}

//---------------------------------------------------------------------------
inline bool StringA::charIsHex (char c)
{
  return ( ((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) );
}

//---------------------------------------------------------------------------
inline STRA_UINT8 StringA::charHexToInt (char c)
{
  if ((c >= '0') && (c <= '9'))
    return c - '0';
  else if ((c >= 'a') && (c <= 'f'))
    return c - 'a' + 10;
  else if ((c >= 'A') && (c <= 'F'))
    return c - 'A' + 10;
  else
    return STRA_UCHAR(-1);
}

//---------------------------------------------------------------------------
inline void StringA::strToLower (char* psz)
{
  char* p = psz;

  STRA_ASSERT(psz);
  if (!psz) return;

  while (*p)
  {
    if (StringA::charIsUpper(*p))
      *p += 'a' - 'A';
    ++p;
  }
}

//---------------------------------------------------------------------------
inline void StringA::strToUpper (char* psz)
{
  char* p = psz;

  STRA_ASSERT(psz);
  if (!psz) return;

  while (*p)
  {
    if (StringA::charIsLower(*p))
      *p -= 'a' - 'A';
    ++p;
  }
}

//---------------------------------------------------------------------------
inline void StringA::strInvertCase (char* psz)
{
  char* p = psz;

  STRA_ASSERT(psz);
  if (!psz) return;

  while (*p)
  {
    if (StringA::charIsLower(*p))
      *p -= 'a' - 'A';
    else if (StringA::charIsUpper(*p))
      *p += 'a' - 'A';
    ++p;
  }
}




//---------------------------------------------------------------------------
inline STRA_UINT32 StringA::strHash (const char* psz)
{
  // Hash method from Pr. Daniel J. Bernstein.

  STRA_UINT32 uiHash = STRING_HASH_SEED;

  STRA_ASSERT(psz);
  if (!psz || !psz[0])
    return uiHash;

  while (*psz)
    uiHash = ((uiHash << 5) + uiHash) + (*psz++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline STRA_UINT32 StringA::strHash (const char* pBlock, STRA_UINT uiCount)
{
  // Hash method from Pr. Daniel J. Bernstein.

  STRA_UINT32 uiHash = STRING_HASH_SEED;
  STRA_UINT   Ix;

  STRA_ASSERT(pBlock);
  STRA_ASSERT(uiCount);
  if (!pBlock || !uiCount)
    return uiHash;

  for (Ix = 0; Ix < uiCount; ++Ix)
    uiHash = ((uiHash << 5) + uiHash) + (*pBlock++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline STRA_UINT32 StringA::strHashI (const char* psz)
{
  // Hash method from Pr. Daniel J. Bernstein.

  STRA_UINT32 uiHash = STRING_HASH_SEED;

  STRA_ASSERT(psz);
  if (!psz || !psz[0])
    return uiHash;

  while (*psz)
    uiHash = ((uiHash << 5) + uiHash) + StringA::charToLower(*psz++);

  return uiHash;
}

//---------------------------------------------------------------------------
inline STRA_UINT32 StringA::strHashI (const char* pBlock, STRA_UINT uiCount)
{
  // Hash method from Pr. Daniel J. Bernstein.

  STRA_UINT32 uiHash = STRING_HASH_SEED;
  STRA_UINT   Ix;

  STRA_ASSERT(pBlock);
  STRA_ASSERT(uiCount);
  if (!pBlock || !uiCount)
    return uiHash;

  for (Ix = 0; Ix < uiCount; ++Ix)
    uiHash = ((uiHash << 5) + uiHash) + StringA::charToLower(*pBlock++);

  return uiHash;
}




//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA operator+ (const StringA& lhs, const std::string& rhs)
{
  StringA strRes(lhs);
  strRes.append(rhs);
  return strRes;
}
#endif

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const char* rhs)
{
  StringA strRes(lhs);
  STRA_ASSERT(rhs);
  if (rhs)
    strRes.append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA operator+ (const std::string& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.append(rhs);
  return strRes;
}
#endif

//---------------------------------------------------------------------------
inline StringA operator+ (const char* lhs, const StringA& rhs)
{
  StringA strRes;
  STRA_ASSERT(lhs);
  if (lhs)
    strRes.copy(lhs);
  strRes.append(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const bool b)
{
  StringA strRes(lhs);
  strRes.append(b);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const char c)
{
  StringA strRes(lhs);
  strRes.append(c);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const STRA_INT32 i)
{
  StringA strRes(lhs);
  strRes.append(i);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const STRA_UINT32 ui)
{
  StringA strRes(lhs);
  strRes.append(ui);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const STRA_INT64 i)
{
  StringA strRes(lhs);
  strRes.append(i);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const STRA_UINT64 ui)
{
  StringA strRes(lhs);
  strRes.append(ui);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const float f)
{
  StringA strRes(lhs);
  strRes.append(f);
  return strRes;
}

//---------------------------------------------------------------------------
inline StringA operator+ (const StringA& lhs, const double d)
{
  StringA strRes(lhs);
  strRes.append(d);
  return strRes;
}




//---------------------------------------------------------------------------
inline StringA operator/ (const StringA& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.pathAppend(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA operator/ (const StringA& lhs, const std::string& rhs)
{
  StringA strRes(lhs);
  strRes.pathAppend(rhs.c_str());
  return strRes;
}
#endif

//---------------------------------------------------------------------------
inline StringA operator/ (const StringA& lhs, const char* rhs)
{
  StringA strRes(lhs);
  STRA_ASSERT(rhs);
  if (rhs)
    strRes.pathAppend(rhs);
  return strRes;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline StringA operator/ (const std::string& lhs, const StringA& rhs)
{
  StringA strRes(lhs);
  strRes.pathAppend(rhs.c_str());
  return strRes;
}
#endif

//---------------------------------------------------------------------------
inline StringA operator/ (const char* lhs, const StringA& rhs)
{
  StringA strRes;
  STRA_ASSERT(lhs);
  if (lhs)
    strRes.copy(lhs);
  strRes.pathAppend(rhs);
  return strRes;
}




//---------------------------------------------------------------------------
inline bool operator== (const StringA& lhs, const StringA& rhs)
{
  return
    lhs.m_uiLength == rhs.m_uiLength &&
    StringA::strCompare(lhs.m_pszData, rhs.m_pszData) == 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator== (const StringA& lhs, const std::string& rhs)
{
  return
    (size_t)lhs.m_uiLength == rhs.length() &&
    StringA::strCompare(lhs.m_pszData, rhs.c_str()) == 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator== (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) == 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator== (const std::string& lhs, const StringA& rhs)
{
  return
    lhs.length() == (size_t)rhs.m_uiLength &&
    StringA::strCompare(lhs.c_str(), rhs.m_pszData) == 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator== (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) == 0;
}

//---------------------------------------------------------------------------
inline bool operator!= (const StringA& lhs, const StringA& rhs)
{
  return
    lhs.m_uiLength != rhs.m_uiLength ||
    StringA::strCompare(lhs.m_pszData, rhs.m_pszData) != 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator!= (const StringA& lhs, const std::string& rhs)
{
  return
    (size_t)lhs.m_uiLength != rhs.length() ||
    StringA::strCompare(lhs.m_pszData, rhs.c_str()) != 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator!= (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) != 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator!= (const std::string& lhs, const StringA& rhs)
{
  return
    lhs.length() != (size_t)rhs.m_uiLength ||
    StringA::strCompare(lhs.c_str(), rhs.m_pszData) != 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator!= (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) != 0;
}

//---------------------------------------------------------------------------
inline bool operator< (const StringA& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.m_pszData) < 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator< (const StringA& lhs, const std::string& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.c_str()) < 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator< (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) < 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator< (const std::string& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.c_str(), rhs.m_pszData) < 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator< (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) < 0;
}

//---------------------------------------------------------------------------
inline bool operator> (const StringA& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.m_pszData) > 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator> (const StringA& lhs, const std::string& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.c_str()) > 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator> (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) > 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator> (const std::string& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.c_str(), rhs.m_pszData) > 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator> (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) > 0;
}

//---------------------------------------------------------------------------
inline bool operator<= (const StringA& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.m_pszData) <= 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator<= (const StringA& lhs, const std::string& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.c_str()) <= 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator<= (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) <= 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator<= (const std::string& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.c_str(), rhs.m_pszData) <= 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator<= (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) <= 0;
}

//---------------------------------------------------------------------------
inline bool operator>= (const StringA& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.m_pszData) >= 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator>= (const StringA& lhs, const std::string& rhs)
{
  return StringA::strCompare(lhs.m_pszData, rhs.c_str()) >= 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator>= (const StringA& lhs, const char* rhs)
{
  STRA_ASSERT(rhs);
  return StringA::strCompare(lhs.m_pszData, rhs) >= 0;
}

//---------------------------------------------------------------------------
#ifdef _STRING_
inline bool operator>= (const std::string& lhs, const StringA& rhs)
{
  return StringA::strCompare(lhs.c_str(), rhs.m_pszData) >= 0;
}
#endif

//---------------------------------------------------------------------------
inline bool operator>= (const char* lhs, const StringA& rhs)
{
  STRA_ASSERT(lhs);
  return StringA::strCompare(lhs, rhs.m_pszData) >= 0;
}
