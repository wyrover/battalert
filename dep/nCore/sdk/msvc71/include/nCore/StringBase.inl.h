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
// Locally needed functors and prototypes
//---------------------------------------------------------------------------
// NOTE from CStdString author :
// A very shorthand way of applying the fix for KB problem Q172398
// (basic_string assignment bug)
#if defined(_MSC_VER) && (_MSC_VER < 1200)
  #define Q172398(x) (x).erase()
#else
  #define Q172398(x)
#endif

// NOTE from CStdString author :
//
// NCORE_LOCALE_USE_FACET macro and why we need it:
//
// Since I'm a good little Standard C++ programmer, I use locales.  Thus, I
// need to make use of the use_facet<> template function here.   Unfortunately,
// this need is complicated by the fact the MS' implementation of the Standard
// C++ Library has a non-standard version of use_facet that takes more
// arguments than the standard dictates.  Since I'm trying to write CStdString
// to work with any version of the Standard library, this presents a problem.
//
// The upshot of this is that I can't do 'use_facet' directly.  The MS' docs
// tell me that I have to use a macro, _USE() instead.  Since _USE obviously
// won't be available in other implementations, this means that I have to write
// my OWN macro -- NCORE_LOCALE_USE_FACET -- that evaluates either to _USE or
// to the standard, use_facet.
//
// If you are having trouble with the NCORE_LOCALE_USE_FACET macro, in your
// implementation of the Standard C++ Library, you can define your own
// version of NCORE_LOCALE_USE_FACET.
#define NCORE_LOCALE_USE_FACET(loc,fac)  std::use_facet<fac >(loc)
/*#ifndef NCORE_LOCALE_USE_FACET
  // NOTE from CStdString author :
	// STLPort #defines a macro (__STL_NO_EXPLICIT_FUNCTION_TMPL_ARGS) for
	// all MSVC builds, erroneously in my opinion.  It causes problems for
	// my SS_ANSI builds.  In my code, I always comment out that line.  You'll
	// find it in \stlport\config\stl_msvc.h
	#if defined(__SGI_STL_PORT) && (__SGI_STL_PORT >= 0x400 )
		#if defined(__STL_NO_EXPLICIT_FUNCTION_TMPL_ARGS) && defined(_MSC_VER)
			#pragma message(__STL_NO_EXPLICIT_FUNCTION_TMPL_ARGS defined !)
		#endif
		#define NCORE_LOCALE_USE_FACET(loc, fac) std::use_facet<fac >(loc)

  #elif defined(_MSC_VER )
	  #define NCORE_LOCALE_USE_FACET(loc, fac) std::_USE(loc, fac)

	#elif defined(_RWSTD_NO_TEMPLATE_ON_RETURN_TYPE)
    #define NCORE_LOCALE_USE_FACET(loc, fac) std::use_facet(loc, (fac*)0)

  #else
		#define NCORE_LOCALE_USE_FACET(loc, fac) std::use_facet<fac >(loc)

  #endif
#endif*/


//---------------------------------------------------------------------------
template <typename CharType>
struct _StringBase_FuncToLower : public std::binary_function<CharType, std::locale, CharType>
{
  inline CharType operator() (const CharType& c, const std::locale& loc) const
  {
    return std::tolower<CharType>(c, loc);
  }
};

//---------------------------------------------------------------------------
template <typename CharType>
struct _StringBase_FuncToUpper : public std::binary_function<CharType, std::locale, CharType>
{
  inline CharType operator() (const CharType& c, const std::locale& loc) const
  {
    return std::toupper<CharType>(c, loc);
  }
};

//---------------------------------------------------------------------------
template <typename CharType>
struct _StringBase_FuncNotSpace : public std::unary_function<CharType, bool>
{
  const std::locale m_locale;

  _StringBase_FuncNotSpace (const std::locale& loc=std::locale()) : m_locale(loc) { }
  bool operator() (CharType a) const { return !std::isspace(a, m_locale); }
};

//---------------------------------------------------------------------------
template<typename CharType>
static inline size_t _StringBase_OwnStringLen (const CharType* pszText)
{
  return pszText ? std::basic_string<CharType>::traits_type::length(pszText) : 0;
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<char>& strDst, const StringBase<char>& strSrc)
{
  if(strDst.c_str() != strSrc.c_str())
  {
    strDst.erase();
    strDst.assign(strSrc);
  }
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<wchar_t>& strDst, const StringBase<wchar_t>& strSrc)
{
  if(strDst.c_str() != strSrc.c_str())
  {
    strDst.erase();
    strDst.assign(strSrc);
  }
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<char>& strDst, const std::basic_string<char>& strSrc)
{
  if(strDst.c_str() != strSrc.c_str())
  {
    strDst.erase();
    strDst.assign(strSrc.c_str());
  }
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<wchar_t>& strDst, const std::basic_string<wchar_t>& strSrc)
{
  if(strDst.c_str() != strSrc.c_str())
  {
    strDst.erase();
    strDst.assign(strSrc.c_str());
  }
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<char>& strDst, const char* pszSrc)
{
  if(!pszSrc)
  {
    strDst.erase();
  }
	else if( (pszSrc >= strDst.c_str()) && (pszSrc <= strDst.c_str() + strDst.size()) )
	{
    // NOTE from CStdString author :
	  // If pszSrc actually points to part of strDst, we must NOT erase(), but
	  // rather take a substring
    strDst = strDst.substr( static_cast<StringBase<char>::size_type>(pszSrc - strDst.c_str()) );
	}
	else
	{
    // NOTE from CStdString author :
	  // Otherwise (most cases) apply the assignment bug fix, if applicable
	  // and do the assignment
		Q172398(strDst);
		strDst.assign(pszSrc);
	}
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAssign (StringBase<wchar_t>& strDst, const wchar_t* pszSrc)
{
  if(!pszSrc)
  {
    strDst.erase();
  }
	else if( (pszSrc >= strDst.c_str()) && (pszSrc <= strDst.c_str() + strDst.size()) )
	{
    // NOTE from CStdString author :
	  // If pszSrc actually points to part of strDst, we must NOT erase(), but
	  // rather take a substring
    strDst = strDst.substr( static_cast<StringBase<wchar_t>::size_type>(pszSrc - strDst.c_str()) );
	}
	else
	{
    // NOTE from CStdString author :
	  // Otherwise (most cases) apply the assignment bug fix, if applicable
	  // and do the assignment
		Q172398(strDst);
		strDst.assign(pszSrc);
	}
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAppend (StringBase<char>& strDst, const char* pszSrc)
{
  if(pszSrc)
  {
    // NOTE from CStdString author :
    // If the string being added is our internal string or a part of our
    // internal string, then we must NOT do any reallocation without
    // first copying that string to another object (since we're using a
    // direct pointer)

    if((pszSrc >= strDst.c_str()) && (pszSrc <= strDst.c_str() + strDst.length()))
    {
      if(strDst.capacity() <= (strDst.size() + _StringBase_OwnStringLen(pszSrc)))
        strDst.append(std::basic_string<char>(pszSrc));
      else
        strDst.append(pszSrc);
    }
    else
    {
      strDst.append(pszSrc);
    }
  }
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAppend (StringBase<wchar_t>& strDst, const wchar_t* pszSrc)
{
  if(pszSrc)
  {
    // NOTE from CStdString author :
    // If the string being added is our internal string or a part of our
    // internal string, then we must NOT do any reallocation without
    // first copying that string to another object (since we're using a
    // direct pointer)

    if((pszSrc >= strDst.c_str()) && (pszSrc <= strDst.c_str() + strDst.length()))
    {
      if(strDst.capacity() <= (strDst.size() + _StringBase_OwnStringLen(pszSrc)))
        strDst.append(std::basic_string<wchar_t>(pszSrc));
      else
        strDst.append(pszSrc);
    }
    else
    {
      strDst.append(pszSrc);
    }
  }
}

//---------------------------------------------------------------------------
template <typename CharType>
static inline int _StringBase_OwnCompareInsensitive (const CharType* psz1, const CharType* psz2)
{
  // NOTE from CStdString author :
  // Using the "C" locale = "not affected by locale"

  std::locale loc = std::locale::classic();
  const std::ctype<CharType>& ct = NCORE_LOCALE_USE_FACET(loc, std::ctype<CharType>);
  CharType f;
  CharType l;

  do
  {
    f = ct.tolower(*(psz1++));
    l = ct.tolower(*(psz2++));
  }
  while((f) && (f == l));

  return (int)(f - l);
}

//---------------------------------------------------------------------------
template <typename CharType>
static inline int _StringBase_OwnCompareInsensitive (const CharType* psz1, const CharType* psz2, size_t nCount)
{
  // NOTE from CStdString author :
  // Using the "C" locale = "not affected by locale"

  std::locale loc = std::locale::classic();
  const std::ctype<CharType>& ct = NCORE_LOCALE_USE_FACET(loc, std::ctype<CharType>);
  CharType f;
  CharType l;

  XASSERT(nCount > 0);
  if(nCount <= 0)
  {
#ifdef NCORE_DEBUG
    XTHROWSTR "Invalid nCount paramater !");
#endif
    return 0;
  }

  do
  {
    f = ct.tolower(*(psz1++));
    l = ct.tolower(*(psz2++));
  }
  while((f) && (--nCount) && (f == l));

  return (int)(f - l);
}

//---------------------------------------------------------------------------
#if defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1300)
static inline int _StringBase_OwnVSPrintF_Count (const char* pszFormat, va_list arglist)
{
  return _vscprintf(pszFormat, arglist);
}
#endif

//---------------------------------------------------------------------------
#if defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1300)
static inline int _StringBase_OwnVSPrintF_Count (const wchar_t* pszFormat, va_list arglist)
{
  return _vscwprintf(pszFormat, arglist);
}
#endif

//---------------------------------------------------------------------------
static inline int _StringBase_OwnVSNPrintF (char* pszOut, const char* pszFormat, size_t nMaxCount, va_list arglist)
{
  #if defined(NCORE_COMPILER_MSVC) && !defined(vsnprintf)
    #define vsnprintf  _vsnprintf
    #define vsnprintf_func_auto_defined
  #endif

  return vsnprintf(pszOut, nMaxCount, pszFormat, arglist);

  #ifdef vsnprintf_func_auto_defined
    #undef vsnprintf
    #undef vsnprintf_func_auto_defined
  #endif
}

//---------------------------------------------------------------------------
static inline int _StringBase_OwnVSNPrintF (wchar_t* pszOut, const wchar_t* pszFormat, size_t nMaxCount, va_list arglist)
{
  #ifdef NCORE_COMPILER_MSVC
  {
    return _vsnwprintf(pszOut, nMaxCount, pszFormat, arglist);
  }
  #else
  {
    return vswprintf(pszOut, nMaxCount, pszFormat, arglist);
  }
  #endif
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnFromBool (StringBase<char>& strDst, bool bValue, bool bAsYesNoString)
{
  if(bAsYesNoString)
    strDst = bValue ? "yes" : "no";
  else
    strDst = bValue ? "true" : "false";
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnFromBool (StringBase<wchar_t>& strDst, bool bValue, bool bAsYesNoString)
{
  if(bAsYesNoString)
    strDst = bValue ? L"yes" : L"no";
  else
    strDst = bValue ? L"true" : L"false";
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAppendBool (StringBase<char>& strDst, bool bValue, bool bAsYesNoString)
{
  if(bAsYesNoString)
    strDst += bValue ? "yes" : "no";
  else
    strDst += bValue ? "true" : "false";
}

//---------------------------------------------------------------------------
static inline void _StringBase_OwnAppendBool (StringBase<wchar_t>& strDst, bool bValue, bool bAsYesNoString)
{
  if(bAsYesNoString)
    strDst += bValue ? L"yes" : L"no";
  else
    strDst += bValue ? L"true" : L"false";
}

//---------------------------------------------------------------------------
static inline bool _StringBase_OwnToBool (const char* pszSrc, bool bDefaultValue)
{
  // the longer valid string to test is : "false" (which is 5 bytes long)
  const int c_nMaxMatchChars = 5;

  char szTemp[c_nMaxMatchChars + 1];
  int  nStart, Ix;

  for(nStart=0 ; isspace(pszSrc[nStart]) ; nStart++) { ; }

  for(Ix=nStart ; pszSrc[Ix] && !isspace(pszSrc[Ix]) && ((Ix-nStart) <= c_nMaxMatchChars) ; Ix++)
    szTemp[Ix-nStart] = tolower(pszSrc[Ix]);
  szTemp[Ix-nStart] = '\0';

  if((strcmp((char*)&szTemp, "1") == 0)
  || (strcmp((char*)&szTemp, "yes") == 0)
  || (strcmp((char*)&szTemp, "true") == 0)
  || (strcmp((char*)&szTemp, "on") == 0) )
  {
    return true;
  }
  else if((strcmp((char*)&szTemp, "0") == 0)
  ||      (strcmp((char*)&szTemp, "no") == 0)
  ||      (strcmp((char*)&szTemp, "false") == 0)
  ||      (strcmp((char*)&szTemp, "off") == 0) )
  {
    return false;
  }

#ifdef NCORE_DEBUG
  XTHROWSTR "The pszSrc parameter contains an unknown boolean string !");
#endif
  //
  // TODO
  // * Here, log a WARNING about this and remove the above assertion...
  //

  return bDefaultValue;
}

//---------------------------------------------------------------------------
static inline bool _StringBase_OwnToBool (const wchar_t* pszSrc, bool bDefaultValue)
{
  // the longer valid string to test is : "false" (which is 5 bytes long)
  const int c_nMaxMatchChars = 5;

  char szTemp[c_nMaxMatchChars + 1];

  if(wcstombs((char*)&szTemp, pszSrc, c_nMaxMatchChars + 1) == (size_t)-1)
  {
#ifdef NCORE_DEBUG
    XTHROWSTR "The pszSrc parameter could not being converted from UNICODE to multibytes string !");
#endif
    //
    // TODO
    // * Here, log a WARNING about this and remove the above assertion...
    //
    return bDefaultValue;
  }

  return _StringBase_OwnToBool((char*)&szTemp, bDefaultValue);
}

//---------------------------------------------------------------------------
static inline int32 _StringBase_OwnToInt32 (const char* pszSrc)
{
  return atoi(pszSrc);
}

//---------------------------------------------------------------------------
static inline int32 _StringBase_OwnToInt32 (const wchar_t* pszSrc)
{
  #ifdef NCORE_COMPILER_MSVC
  {
    return _wtoi(pszSrc);
  }
  #else
  {
    //
    // TODO
    // * Optimize this really-bad-and-fast-coded hack
    //

    /*
    int32  nReturn;
    size_t nSrcLen = wcslen(pszSrc);
    char*  pszTemp = new char[nSrcLen + 1];

    for(size_t Ix=0 ; Ix<=nSrcLen; Ix++)
      *(pszTemp + Ix) = (char)*(pszSrc + Ix);

    sscanf(pszTemp, "%d", &nReturn);
    delete pszTemp;

    return nReturn;
    */

    char szTemp[64];

    if(wcstombs((char*)&szTemp, pszSrc, 64) == (size_t)-1)
      return 0;

    return atoi((char*)&szTemp);
  }
  #endif
}

//---------------------------------------------------------------------------
static inline int64 _StringBase_OwnToInt64 (const char* pszSrc)
{
  #ifdef NCORE_COMPILER_MSVC
    return _atoi64(pszSrc);
  #else
    return atoll(pszSrc);
  #endif
}

//---------------------------------------------------------------------------
static inline int64 _StringBase_OwnToInt64 (const wchar_t* pszSrc)
{
  #ifdef NCORE_COMPILER_MSVC
  {
    return _wtoi64(pszSrc);
  }
  #else
  {
    //
    // TODO
    // * Optimize this really-bad-and-fast-coded hack
    //

    /*
    int64  nReturn;
    size_t nSrcLen = wcslen(pszSrc);
    char*  pszTemp = new char[nSrcLen + 1];

    for(size_t Ix=0 ; Ix<=nSrcLen; Ix++)
      *(pszTemp + Ix) = (char)*(pszSrc + Ix);

    sscanf(pszTemp, "%"PRId64, &nReturn);
    delete pszTemp;

    return nReturn;
    */

    char szTemp[64];

    if(wcstombs((char*)&szTemp, pszSrc, 64) == (size_t)-1)
      return 0;

    return atoll((char*)&szTemp);
  }
  #endif
}

//---------------------------------------------------------------------------
static inline double _StringBase_OwnToFloat (const char* pszSrc)
{
  return atof(pszSrc);
}

//---------------------------------------------------------------------------
static inline double _StringBase_OwnToFloat (const wchar_t* pszSrc)
{
  #ifdef NCORE_COMPILER_MSVC
  {
    return _wtof(pszSrc);
  }
  #else
  {
    //
    // TODO
    // * Optimize this really-bad-and-fast-coded hack
    //

    /*
    double rReturn;
    size_t nSrcLen = wcslen(pszSrc);
    char*  pszTemp = new char[nSrcLen + 1];

    for(size_t Ix=0 ; Ix<=nSrcLen; Ix++)
      *(pszTemp + Ix) = (char)*(pszSrc + Ix);

    rReturn = atof(pszTemp);
    delete pszTemp;

    return rReturn;
    */

    char szTemp[64]; // TODO : does 64 chars is big enough ???

    if(wcstombs((char*)&szTemp, pszSrc, 64) == (size_t)-1)
      return 0;

    return atof((char*)&szTemp);
  }
  #endif
}



//---------------------------------------------------------------------------
// StringBase
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>::StringBase (void)
: std::basic_string<CharType>()
{
}

template <typename CharType>
inline StringBase<CharType>::StringBase (const StringBase<CharType>& rhs)
: std::basic_string<CharType>(static_cast<const std::basic_string<CharType>&>(rhs))
{
}

template <typename CharType>
inline StringBase<CharType>::StringBase (const std::basic_string<CharType>& rhs)
: std::basic_string<CharType>(rhs)
{
}

template <typename CharType>
inline StringBase<CharType>::StringBase (const CharType* rhs)
: std::basic_string<CharType>(rhs)
{
}

//---------------------------------------------------------------------------
// operator const CharType*
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>::operator const CharType* (void) const
{
  return this->c_str();
}

//---------------------------------------------------------------------------
// operator []
//---------------------------------------------------------------------------
template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (int8 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (int8 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (uint8 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (uint8 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (int16 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (int16 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (uint16 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (uint16 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (int32 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (int32 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (uint32 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (uint32 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (int64 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (int64 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline CharType& StringBase<CharType>::operator [] (uint64 nIndex)
{
  return static_cast<std::basic_string<CharType>*>(this)->operator[](nIndex);
}

template <typename CharType>
inline const CharType& StringBase<CharType>::operator [] (uint64 nIndex) const
{
  return static_cast<const std::basic_string<CharType>*>(this)->operator[](nIndex);
}

//---------------------------------------------------------------------------
// operator =
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator = (const StringBase<CharType>& rhs)
{
  _StringBase_OwnAssign(*this, rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator = (const std::basic_string<CharType>& rhs)
{
  _StringBase_OwnAssign(*this, rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator = (const CharType* rhs)
{
  _StringBase_OwnAssign(*this, rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator = (CharType rhs)
{
  Q172398(*this);
  this->assign(1, rhs);
  return *this;
}

//---------------------------------------------------------------------------
// operator +=
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator += (const StringBase<CharType>& rhs)
{
  this->append(rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator += (const std::basic_string<CharType>& rhs)
{
  this->append(rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator += (const CharType* rhs)
{
  _StringBase_OwnAppend(*this, rhs);
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::operator += (CharType rhs)
{
  this->append(1, rhs);
  return *this;
}

//---------------------------------------------------------------------------
// operator + (global)
//---------------------------------------------------------------------------
/*inline StringBase<char> operator + (const StringBase<char>& strDst, const StringBase<char>& strSrc)
{
  StringBase<char> strReturn(strDst);
  strReturn.append(strSrc);
  return strReturn;
}*/

//---------------------------------------------------------------------------
// Equals
//---------------------------------------------------------------------------
template <typename CharType>
inline bool StringBase<CharType>::Equals (const CharType* pszCompare, bool bCaseSensitive/*=true*/) const
{
  return ( bCaseSensitive ? this->compare(pszCompare) : _StringBase_OwnCompareInsensitive(this->c_str(), pszCompare) ) == 0;
}

//---------------------------------------------------------------------------
// Compare
//---------------------------------------------------------------------------
template <typename CharType>
inline int StringBase<CharType>::Compare (const CharType* pszCompare, bool bCaseSensitive/*=true*/) const
{
  return bCaseSensitive ? this->compare(pszCompare) : _StringBase_OwnCompareInsensitive(this->c_str(), pszCompare);
}

//---------------------------------------------------------------------------
// Trim
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Trim (void)
{
  return TrimLeft().TrimRight();
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Trim (CharType cToMatch)
{
  return TrimLeft(cToMatch).TrimRight(cToMatch);
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Trim (const StringBase<CharType>& strElementsToMatch)
{
  return TrimLeft(strElementsToMatch).TrimRight(strElementsToMatch);
}

//---------------------------------------------------------------------------
// TrimLeft
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimLeft (void)
{
  this->erase(this->begin(), std::find_if(this->begin(), this->end(), _StringBase_FuncNotSpace<CharType>()));
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimLeft (CharType cToMatch)
{
  this->erase(0, this->find_first_not_of(cToMatch));
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimLeft (const StringBase<CharType>& strElementsToMatch)
{
  this->erase(0, this->find_first_not_of(strElementsToMatch));
  return *this;
}

//---------------------------------------------------------------------------
// TrimRight
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimRight (void)
{
  // NOTE from CStdString author :
  // When comparing reverse_iterators here (MYRITER), I avoid using
  // operator!=.  This is because namespace rel_ops also has a template
  // operator!= which conflicts with the global operator!= already defined
  // for reverse_iterator in the header <utility>.
  // Thanks to John James for alerting me to this.

  typename StringBase<CharType>::reverse_iterator rit;

  rit = std::find_if(this->rbegin(), this->rend(), _StringBase_FuncNotSpace<CharType>());
  if(!(this->rend() == rit))
    this->erase(this->rend() - rit);

  this->erase(!(rit == this->rend()) ? this->find_last_of(*rit) + 1 : 0);

  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimRight (CharType cToMatch)
{
  typename StringBase<CharType>::size_type nIdx = this->find_last_not_of(cToMatch);
  this->erase( (std::basic_string<CharType>::npos == nIdx) ? 0 : ++nIdx );
  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::TrimRight (const StringBase<CharType>& strElementsToMatch)
{
  typename StringBase<CharType>::size_type nIdx	= this->find_last_not_of(strElementsToMatch);
  this->erase( (std::basic_string<CharType>::npos == nIdx) ? 0 : ++nIdx );
  return *this;
}

//---------------------------------------------------------------------------
// ToLower
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::ToLower (const std::locale& loc/*=std::locale()*/)
{
  // NOTE from CStdString author :
  // If there are any MBCS character sets in which the lowercase form a
  // character takes up a different number of bytes than the uppercase form,
  // this would probably not work...

  std::transform(this->begin(), this->end(), this->begin(), std::bind2nd(_StringBase_FuncToLower<CharType>(), loc));
  return *this;
}

//---------------------------------------------------------------------------
// ToUpper
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::ToUpper (const std::locale& loc/*=std::locale()*/)
{
  // NOTE from CStdString author :
  // If there are any MBCS character sets in which the uppercase form a
  // character takes up a different number of bytes than the lowercase form,
  // this would probably not work...

  std::transform(this->begin(), this->end(), this->begin(), std::bind2nd(_StringBase_FuncToUpper<CharType>(), loc));
  return *this;
}

//---------------------------------------------------------------------------
// SplitFromSeparators
//---------------------------------------------------------------------------
template <typename CharType>
inline std::vector< StringBase<CharType> > StringBase<CharType>::SplitFromSeparators (const StringBase<CharType>& strSeparatorsList, uint uiMaxSplits/*=0*/, bool bKeepEmptyFields/*=false*/) const
{
  std::vector< StringBase<CharType> > vecReturn;
  uint   uiSplits = 0;
  size_t nStart   = 0;
  size_t nPos;

  do
  {
    nPos = this->find_first_of(strSeparatorsList, nStart);

    if(nPos == nStart)
    {
      if(bKeepEmptyFields)
        vecReturn.push_back(StringBase<CharType>());

      // go ahead
      nStart = nPos + 1;
    }
    else if((nPos == StringBase<CharType>::npos) || (uiMaxSplits && (uiSplits == uiMaxSplits)))
    {
      // copy the rest of the string
      vecReturn.push_back(this->substr(nStart));
      break;
    }
    else
    {
      // copy up to delimiter
      vecReturn.push_back(this->substr(nStart, nPos - nStart));
      nStart = nPos + 1;
    }

    // parse up to next real data
    if(!bKeepEmptyFields)
    {
      nStart = this->find_first_not_of(strSeparatorsList, nStart);
    }

    // update splits counter
    uiSplits++;
  }
  while(nPos != StringBase<CharType>::npos);

  return vecReturn;
}

//---------------------------------------------------------------------------
// SplitFromString
//---------------------------------------------------------------------------
template <typename CharType>
inline std::vector< StringBase<CharType> > StringBase<CharType>::SplitFromString (const StringBase<CharType>& strStringToMatch, uint uiMaxSplits/*=0*/, bool bKeepEmptyFields/*=false*/) const
{
  std::vector< StringBase<CharType> > vecReturn;
  uint   uiSplits = 0;
  size_t nStart   = 0;
  size_t nPos;
  StringBase<CharType> strTemp;

  do
  {
    nPos = this->find(strStringToMatch, nStart);

    if(nPos == nStart)
    {
      if(bKeepEmptyFields)
        vecReturn.push_back(StringBase<CharType>());

      // go ahead
      nStart = nPos + strStringToMatch.length();
    }
    else if((nPos == StringBase<CharType>::npos) || (uiMaxSplits && (uiSplits == uiMaxSplits)))
    {
      // copy the rest of the string
      vecReturn.push_back(this->substr(nStart));
      break;
    }
    else
    {
      // copy up to delimiter
      vecReturn.push_back(this->substr(nStart, nPos - nStart));
      nStart = nPos + strStringToMatch.length();
    }

    // parse up to next real data
    if(!bKeepEmptyFields)
    {
      while(1)
      {
        strTemp = this->substr(nStart, strStringToMatch.length());
        if(strTemp != strStringToMatch) break;
        nStart += strStringToMatch.length();
      }

      if(strTemp.empty())
        break;
    }

    // update splits counter
    uiSplits++;
  }
  while(nPos != StringBase<CharType>::npos);

  return vecReturn;
}

//---------------------------------------------------------------------------
// Cut
//---------------------------------------------------------------------------
template <typename CharType>
inline bool StringBase<CharType>::Cut (const StringBase<CharType>& strSeparator)
{
  size_t nPos = this->find(strSeparator);
  if(nPos == StringBase<CharType>::npos) return false;
  *this = this->substr(0, nPos);

  return true;
}

//---------------------------------------------------------------------------
// RCut
//---------------------------------------------------------------------------
template <typename CharType>
inline bool StringBase<CharType>::RCut (const StringBase<CharType>& strSeparator)
{
  size_t nPos = this->rfind(strSeparator);
  if(nPos == StringBase<CharType>::npos) return false;
  this->erase(0, nPos + strSeparator.length());

  return true;
}

//---------------------------------------------------------------------------
// Replace
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Replace (CharType cSearch, CharType cReplace)
{
  size_t nPos = this->find_first_of(cSearch);
  while(nPos != StringBase<CharType>::npos)
  {
    this->at(nPos) = cReplace;
    nPos = this->find_first_of(cSearch, nPos + 1);
  }

  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Replace (CharType cSearch, const StringBase<CharType>& strReplace)
{
  size_t nPos = this->find_first_of(cSearch);
  while(nPos != StringBase<CharType>::npos)
  {
    this->erase(this->begin() + nPos);
    this->insert(nPos, strReplace);

    nPos = this->find_first_of(cSearch, nPos + strReplace.length());
  }

  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Replace (const StringBase<CharType>& strSearch, CharType cReplace, size_t nReplaceRepeatCount/*=1*/)
{
  size_t nStart = 0;
  size_t nPos;
  size_t nSearchLen = strSearch.length();

  do
  {
    nPos = this->find(strSearch, nStart);
    if(nPos == StringBase<CharType>::npos) break;

    this->replace(nPos, nSearchLen, nReplaceRepeatCount, cReplace);
    nStart = nPos + nSearchLen;
  }
  while(1);

  return *this;
}

template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::Replace (const StringBase<CharType>& strSearch, const StringBase<CharType>& strReplace)
{
  size_t nStart = 0;
  size_t nPos;
  size_t nSearchLen = strSearch.length();

  do
  {
    nPos = this->find(strSearch, nStart);
    if(nPos == StringBase<CharType>::npos) break;

    this->replace(nPos, nSearchLen, strReplace);
    nStart = nPos + nSearchLen;
  }
  while(1);

  return *this;
}

//---------------------------------------------------------------------------
// StartsWith
//---------------------------------------------------------------------------
template <typename CharType>
inline bool StringBase<CharType>::StartsWith (const StringBase<CharType>& strToMatch, bool bCaseSensitive/*=true*/) const
{
  size_t nMatchLen = strToMatch.length();

  if(strToMatch.empty() || (this->length() < nMatchLen))
    return false;

  if(bCaseSensitive)
    return this->compare(0, nMatchLen, strToMatch) == 0;
  else
    return _StringBase_OwnCompareInsensitive(this->c_str(), strToMatch.c_str(), nMatchLen) == 0;
}

//---------------------------------------------------------------------------
// EndsWith
//---------------------------------------------------------------------------
template <typename CharType>
bool StringBase<CharType>::EndsWith (const StringBase<CharType>& strToMatch, bool bCaseSensitive/*=true*/) const
{
  size_t nMatchLen = strToMatch.length();

  if(strToMatch.empty() || (this->length() < nMatchLen))
    return false;

  if(bCaseSensitive)
  {
    return this->compare(this->length() - nMatchLen, nMatchLen, strToMatch) == 0;
  }
  else
  {
    const CharType* pszThis = this->c_str() + this->length() - nMatchLen;
    return _StringBase_OwnCompareInsensitive(pszThis, strToMatch.c_str(), nMatchLen) == 0;
  }

  return false;
}

template <typename CharType>
bool StringBase<CharType>::EndsWith (CharType cToMatch) const
{
  if(this->empty()) return false;
  if(this->at(this->length() - 1) == cToMatch) return true;
  return false;
}

//---------------------------------------------------------------------------
// Format
//---------------------------------------------------------------------------
template <typename CharType>
inline void StringBase<CharType>::Format (const CharType* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->FormatV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
// FormatV
//---------------------------------------------------------------------------
template <typename CharType>
inline void StringBase<CharType>::FormatV (const CharType* pszFormat, va_list arglist)
{
  #if defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1300)
  {
    size_t nLen;

    nLen = _StringBase_OwnVSPrintF_Count(pszFormat, arglist);
    if(nLen == -1) return;

    _StringBase_OwnVSNPrintF(this->GetBuffer(nLen + 1), pszFormat, nLen, arglist);
    this->ReleaseBuffer();
  }
  #elif defined(__GNUC__) && ( (__GNUC__ > 2) || ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 1)) )
  {
    // this vsnprintf() behavior requires requires glibc version >= 2.1

    if(sizeof(CharType) == 1)
    {
      static char szTemp[2];
      int nLen;

      nLen = vsnprintf((char*)&szTemp, 1, pszFormat, arglist);
      if(nLen < 0) return;
      ++nLen;

      _StringBase_OwnVSNPrintF(this->GetBuffer(nLen + 1), pszFormat, nLen + 1, arglist);
      this->ReleaseBuffer();
    }
    else
    {
      size_t nLen;
      size_t nRes;

      nLen = _StringBase_OwnStringLen(pszFormat) + 256; // arbitrary value (really bad i know...)
      while(1)
      {
        nRes = _StringBase_OwnVSNPrintF(this->GetBuffer(nLen), pszFormat, nLen-1, arglist);
        this->ReleaseBuffer();

        if(nRes != (size_t)-1) break;  // success
        if(nLen >= 0x10000) break;     // never exceed 65536 bytes (this should be enough for everyone, even for java style "coders")

        nLen += 1024; // arbitrary value
      }
    }
  }
  #else
  {
    size_t nLen;
    size_t nRes;

    nLen = _StringBase_OwnStringLen(pszFormat) + 256; // arbitrary value (really bad i know...)
    while(1)
    {
      nRes = _StringBase_OwnVSNPrintF(this->GetBuffer(nLen), pszFormat, nLen-1, arglist);
      this->ReleaseBuffer();

      if(nRes != (size_t)-1) break;  // success
      if(nLen >= 0x10000) break;     // never exceed 65536 bytes (this should be enough for everyone, even for java style "coders")

      nLen += 1024; // arbitrary value
    }
  }
  #endif
}

//---------------------------------------------------------------------------
// AppendFormat
//---------------------------------------------------------------------------
template <typename CharType>
inline void StringBase<CharType>::AppendFormat (const CharType* pszFormat, ...)
{
	va_list arglist;

	va_start(arglist, pszFormat);
	this->AppendFormatV(pszFormat, arglist);
	va_end(arglist);
}

//---------------------------------------------------------------------------
// AppendFormatV
//---------------------------------------------------------------------------
template <typename CharType>
inline void StringBase<CharType>::AppendFormatV (const CharType* pszFormat, va_list arglist)
{
  StringBase<CharType> strTemp;
  strTemp.FormatV(pszFormat, arglist);
  *this += strTemp;
}

//---------------------------------------------------------------------------
// FromBool
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromBool (bool value, bool bAsYesNoString/*=false*/)
{
  _StringBase_OwnFromBool(*this, value, bAsYesNoString);
  return *this;
}

//---------------------------------------------------------------------------
// FromInt
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromInt (int32 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// FromUInt
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromUInt (uint32 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// FromLong
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromLong (int64 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// FromULong
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromULong (uint64 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// FromFloat
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromFloat (float value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// FromDouble
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::FromDouble (double value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this = stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendBool
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendBool (bool value, bool bAsYesNoString/*=false*/)
{
  _StringBase_OwnAppendBool(*this, value, bAsYesNoString);
  return *this;
}

//---------------------------------------------------------------------------
// AppendInt
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendInt (long value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendUInt
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendUInt (unsigned long value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendLong
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendLong (int64 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendULong
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendULong (uint64 value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendFloat
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendFloat (float value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// AppendDouble
//---------------------------------------------------------------------------
template <typename CharType>
inline StringBase<CharType>& StringBase<CharType>::AppendDouble (double value)
{
  std::basic_stringstream<CharType> stream;
  stream << value;
  *this += stream.str();

  return *this;
}

//---------------------------------------------------------------------------
// ToBool
//---------------------------------------------------------------------------
template <typename CharType>
inline bool StringBase<CharType>::ToBool (bool bDefault) const
{
  return _StringBase_OwnToBool(this->c_str(), bDefault);
}

//---------------------------------------------------------------------------
// ToInt32
//---------------------------------------------------------------------------
template <typename CharType>
inline int32 StringBase<CharType>::ToInt32 (void) const
{
  return _StringBase_OwnToInt32(this->c_str());
}

//---------------------------------------------------------------------------
// ToInt64
//---------------------------------------------------------------------------
template <typename CharType>
inline int64 StringBase<CharType>::ToInt64 (void) const
{
  return _StringBase_OwnToInt64(this->c_str());
}

//---------------------------------------------------------------------------
// ToFloat
//---------------------------------------------------------------------------
template <typename CharType>
inline double StringBase<CharType>::ToFloat (void) const
{
  return _StringBase_OwnToFloat(this->c_str());
}

//---------------------------------------------------------------------------
// GetBuffer
//---------------------------------------------------------------------------
template <typename CharType>
inline CharType* StringBase<CharType>::GetBuffer (size_t nMinLength/*=-1*/)
{
  if(this->size() < nMinLength) this->resize(nMinLength);
  return this->empty() ? const_cast<CharType*>(this->data()) : &(this->at(0));
}

//---------------------------------------------------------------------------
// ReleaseBuffer
//---------------------------------------------------------------------------
template <typename CharType>
inline void StringBase<CharType>::ReleaseBuffer (size_t nNewLength/*=-1*/)
{
  this->resize( (nNewLength > (size_t)-1) ? nNewLength : _StringBase_OwnStringLen(this->c_str()) );
}
