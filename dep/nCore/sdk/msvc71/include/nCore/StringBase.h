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

#ifndef __nCore_StringBase_Header__
#define __nCore_StringBase_Header__
#if defined(NCORE_ENABLE_UTIL_STRINGBASE)
namespace nCore {


//---------------------------------------------------------------------------
// StringBase
//
// This class is intented to be as flexible and standard compliant (i.e.:
// STL compliant) as possible.
// Lot of tricks and code in this class has been taken from the CStdString
// class found at http://www.codeproject.com/
// The CharType typename can be 'char' to define a normal ASCII string or
// 'wchar_t' to be able to use UNICODE string but please, do not try to use
// this class to format/interpret MBCS data because MBCS format is very
// tricky and some of those methods should not work fine (for example : the
// ToLower() and ToUpper() methods)...
//---------------------------------------------------------------------------
template <typename CharType>
class StringBase : public std::basic_string<CharType>
{
  friend class Streamer;

public :
  StringBase (void);
  StringBase (const StringBase<CharType>& rhs);
  StringBase (const std::basic_string<CharType>& rhs);
  StringBase (const CharType* rhs);

  // additional operators
                    operator const CharType* (void) const;
        CharType&   operator []              (int8 nIndex);
  const CharType&   operator []              (int8 nIndex) const;
        CharType&   operator []              (uint8 nIndex);
  const CharType&   operator []              (uint8 nIndex) const;
        CharType&   operator []              (int16 nIndex);
  const CharType&   operator []              (int16 nIndex) const;
        CharType&   operator []              (uint16 nIndex);
  const CharType&   operator []              (uint16 nIndex) const;
        CharType&   operator []              (int32 nIndex);
  const CharType&   operator []              (int32 nIndex) const;
        CharType&   operator []              (uint32 nIndex);
  const CharType&   operator []              (uint32 nIndex) const;
        CharType&   operator []              (int64 nIndex);
  const CharType&   operator []              (int64 nIndex) const;
        CharType&   operator []              (uint64 nIndex);
  const CharType&   operator []              (uint64 nIndex) const;
        StringBase& operator =               (const StringBase<CharType>& rhs);
        StringBase& operator =               (const std::basic_string<CharType>& rhs);
        StringBase& operator =               (const CharType* rhs);
        StringBase& operator =               (CharType rhs);
        StringBase& operator +=              (const StringBase<CharType>& rhs);
        StringBase& operator +=              (const std::basic_string<CharType>& rhs);
        StringBase& operator +=              (const CharType* rhs);
        StringBase& operator +=              (CharType rhs);

  // comparison methods
  bool Equals  (const CharType* pszCompare, bool bCaseSensitive=true) const;
  int  Compare (const CharType* pszCompare, bool bCaseSensitive=true) const;

  // trim methods
  StringBase& Trim      (void);
  StringBase& Trim      (CharType cToMatch);
  StringBase& Trim      (const StringBase<CharType>& strElementsToMatch);
  StringBase& TrimLeft  (void);
  StringBase& TrimLeft  (CharType cToMatch);
  StringBase& TrimLeft  (const StringBase<CharType>& strElementsToMatch);
  StringBase& TrimRight (void);
  StringBase& TrimRight (CharType cToMatch);
  StringBase& TrimRight (const StringBase<CharType>& strElementsToMatch);

  // case changing methods
  StringBase& ToLower (const std::locale& loc=std::locale());
  StringBase& ToUpper (const std::locale& loc=std::locale());

  // splitting methods
  std::vector< StringBase<CharType> > SplitFromSeparators (const StringBase<CharType>& strSeparatorsList, uint uiMaxSplits=0, bool bKeepEmptyFields=false) const;
  std::vector< StringBase<CharType> > SplitFromString     (const StringBase<CharType>& strStringToMatch, uint uiMaxSplits=0, bool bKeepEmptyFields=false) const;

  // miscellaneous operations on string
  bool        Cut        (const StringBase<CharType>& strSeparator);
  bool        RCut       (const StringBase<CharType>& strSeparator);
  StringBase& Replace    (CharType cSearch, CharType cReplace);
  StringBase& Replace    (CharType cSearch, const StringBase<CharType>& strReplace);
  StringBase& Replace    (const StringBase<CharType>& strSearch, CharType cReplace, size_t nReplaceRepeatCount=1);
  StringBase& Replace    (const StringBase<CharType>& strSearch, const StringBase<CharType>& strReplace);
  bool        StartsWith (const StringBase<CharType>& strToMatch, bool bCaseSensitive=true) const;
  bool        EndsWith   (const StringBase<CharType>& strToMatch, bool bCaseSensitive=true) const;
  bool        EndsWith   (CharType cToMatch) const;

  // format methods
  void Format        (const CharType* pszFormat, ...);
  void FormatV       (const CharType* pszFormat, va_list arglist);
  void AppendFormat  (const CharType* pszFormat, ...);
  void AppendFormatV (const CharType* pszFormat, va_list arglist);

  // conversion from some basic types
  StringBase& FromBool     (bool value, bool bAsYesNoString=false);
  StringBase& FromInt      (int32 value);
  StringBase& FromUInt     (uint32 value);
  StringBase& FromLong     (int64 value);
  StringBase& FromULong    (uint64 value);
  StringBase& FromFloat    (float value);
  StringBase& FromDouble   (double value);
  StringBase& AppendBool   (bool value, bool bAsYesNoString=false);
  StringBase& AppendInt    (long value);
  StringBase& AppendUInt   (unsigned long value);
  StringBase& AppendLong   (int64 value);
  StringBase& AppendULong  (uint64 value);
  StringBase& AppendFloat  (float value);
  StringBase& AppendDouble (double value);

  // conversion to some basic types
  bool   ToBool  (bool bDefault) const;
  int32  ToInt32 (void) const;
  int64  ToInt64 (void) const;
  double ToFloat (void) const;


private :
  CharType* GetBuffer     (size_t nMinLength=-1);
  void      ReleaseBuffer (size_t nNewLength=-1);
};

//StringBase<char> operator + (const StringBase<char>& strDst, const StringBase<char>& strSrc);


#include "StringBase.inl.h"


//pedef StringBase<char>    StringA; // A shortcut typedef to define an ASCII string.
//pedef StringBase<wchar_t> StringW; // A shortcut typedef to define a UNICODE string (using MBCS is really not recommended).


}
#endif // #if defined(NCORE_ENABLE_UTIL_STRINGBASE)
#endif // #ifndef __nCore_StringBase_Header__
