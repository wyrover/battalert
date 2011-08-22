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

#ifndef __nCore_StringA_Header__
#define __nCore_StringA_Header__
namespace nCore {


//---------------------------------------------------------------------------
// Types
//---------------------------------------------------------------------------
// vector<StringA>
typedef std::vector<StringA>        StringAVec;
typedef StringAVec::iterator        StringAVecIt;
typedef StringAVec::const_iterator  StringAVecCIt;

// list<StringA>
typedef std::list<StringA>           StringAList;
typedef StringAList::iterator        StringAListIt;
typedef StringAList::const_iterator  StringAListCIt;


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
static const uint   STRING_BASE_BUFFER_SIZE = 32 - sizeof(uint32) - sizeof(char*) - sizeof(uint32); // so the StringA object can be aligned on 16 bytes boundary, for comparison, sizeof(std::string) is 28 bytes on msvc7.1 so there's no major overhead
static const uint   STRING_GROW_SIZE        = 32;
static const uint32 STRING_HASH_SEED        = 0xdeadbeef;


//---------------------------------------------------------------------------
// StringA
//---------------------------------------------------------------------------
class NCORE_EXPORT StringA
{
public :
  enum BoolStringType
  {
    BOOLSTRINGTYPE_YESNO,      // representation : "yes"  / "no" (default)
    BOOLSTRINGTYPE_TRUEFALSE,  // representation : "true" / "false"
    BOOLSTRINGTYPE_ONOFF,      // representation : "on"   / "off"
    BOOLSTRINGTYPE_IO,         // representation : "0"    / "1"
  };


public :
  StringA (void);
  StringA (const StringA& rhs);
  StringA (const StringA& rhs, uint uiCount, uint uiStart=0);
  StringA (const std::string& rhs);
  StringA (const std::string& rhs, uint uiCount, uint uiStart=0);
  StringA (const char* rhs);
  StringA (const char* rhs, uint uiCount, uint uiStart=0);
  explicit StringA (const bool   b);
  explicit StringA (const char   c);
  explicit StringA (const int32  i);
  explicit StringA (const uint32 ui);
  explicit StringA (const int64  i);
  explicit StringA (const uint64 ui);
  explicit StringA (const float  f);
  explicit StringA (const double d);
  virtual ~StringA (void);


  // StringA default behavior setup
  static void           SetDefaultBoolStringType (BoolStringType eBoolStringType); // only influence Append(bool) and operator=(bool) methods. the ExtractBool() method will always recognize every supported representations.
  static BoolStringType GetDefaultBoolStringType (void);


  // memory allocation
  virtual void Grow    (uint uiDesiredSize, bool bKeepOldContent=true); // resize only if desired size is > current buffer size (do not forget : size = length + 1)
  virtual void Resize  (uint uiDesiredSize, bool bKeepOldContent=true); // force resize, this will enventually cut your string (do not forget : size = length + 1)
  virtual void Compact (void);                                          // fit buffer size to string length
  virtual void Reset   (void);                                          // complete reset : raz string and free memory (use the Clear() method if you just want make the string empty, which is faster)

  // length
  virtual void Clear     (void);       // empty string but does not free memory (this is a fast alternative to Reset())
  virtual bool IsEmpty   (void) const; // check for an empty string
  virtual uint Length    (void) const; // length of the string, this is an strlen() equivalent
  virtual uint MaxLength (void) const; // max string length the buffer is currently capable to hold (so real buffer size is MaxLength()+1)

  // direct access to the internal buffer
  // WARNING : use those methods only if you know what you are doing !
  virtual char* AcquireBuffer (uint uiMinRequiredBufferSize, bool bKeepOldContent=true); // do not forget : size = length + 1
  virtual void  ReleaseBuffer (uint uiNewStringLength=0);

  // null-terminated c-string representation
  virtual const char* c_str                (void) const;
  virtual             operator const char* (void) const;
  virtual             operator const char* (void);

  // accessors to individual characters
  virtual char  operator[] (int8   nIndex) const;
  virtual char& operator[] (int8   nIndex);
  virtual char  operator[] (uint8  nIndex) const;
  virtual char& operator[] (uint8  nIndex);
  virtual char  operator[] (int16  nIndex) const;
  virtual char& operator[] (int16  nIndex);
  virtual char  operator[] (uint16 nIndex) const;
  virtual char& operator[] (uint16 nIndex);
  virtual char  operator[] (int32  nIndex) const;
  virtual char& operator[] (int32  nIndex);
  virtual char  operator[] (uint32 nIndex) const;
  virtual char& operator[] (uint32 nIndex);
  virtual char  operator[] (int64  nIndex) const;
  virtual char& operator[] (int64  nIndex);
  virtual char  operator[] (uint64 nIndex) const;
  virtual char& operator[] (uint64 nIndex);

  // copy
  virtual void Copy      (const StringA& strSource);
  virtual void Copy      (const std::string& strSource);
  virtual void Copy      (const char* pszSource);
  virtual void CopyCount (const char* pszSource, uint uiSourceLength, uint uiSourceStartOffset=0);

  // copy operators
  virtual void operator= (const StringA& rhs);
  virtual void operator= (const std::string& rhs);
  virtual void operator= (const char*  rhs);
  virtual void operator= (const bool   b);
  virtual void operator= (const char   c);
  virtual void operator= (const int16  i);
  virtual void operator= (const uint16 ui);
  virtual void operator= (const int32  i);
  virtual void operator= (const uint32 ui);
  virtual void operator= (const int64  i);
  virtual void operator= (const uint64 ui);
  virtual void operator= (const float  f);
  virtual void operator= (const double d);

  // appending
  virtual void Append          (const StringA& str);
  virtual void Append          (const StringA& str, uint uiLength);
  virtual void Append          (const std::string& str);
  virtual void Append          (const std::string& str, uint uiLength);
  virtual void Append          (const char*  psz);
  virtual void Append          (const char*  psz, uint uiLength);
  virtual void AppendUrlEscape (const char*  psz);
  virtual void Append          (const bool   b);
  virtual void Append          (const char   c);
  virtual void Append          (const int16  i);
  virtual void Append          (const uint16 ui);
  virtual void Append          (const int32  i);
  virtual void Append          (const uint32 ui);
  virtual void Append          (const int64  i);
  virtual void Append          (const uint64 ui);
  virtual void Append          (const float  f);
  virtual void Append          (const double d);

  // appending operators
  virtual StringA& operator+= (const StringA& rhs);
  virtual StringA& operator+= (const std::string& rhs);
  virtual StringA& operator+= (const char*  rhs);
  virtual StringA& operator+= (const bool   b);
  virtual StringA& operator+= (const char   c);
  virtual StringA& operator+= (const int16  i);
  virtual StringA& operator+= (const uint16 ui);
  virtual StringA& operator+= (const int32  i);
  virtual StringA& operator+= (const uint32 ui);
  virtual StringA& operator+= (const int64  i);
  virtual StringA& operator+= (const uint64 ui);
  virtual StringA& operator+= (const float  f);
  virtual StringA& operator+= (const double d);
  virtual StringA& operator<< (const StringA& rhs);
  virtual StringA& operator<< (const std::string& rhs);
  virtual StringA& operator<< (const char*  rhs);
  virtual StringA& operator<< (const bool   b);
  virtual StringA& operator<< (const char   c);
  virtual StringA& operator<< (const int16  i);
  virtual StringA& operator<< (const uint16 ui);
  virtual StringA& operator<< (const int32  i);
  virtual StringA& operator<< (const uint32 ui);
  virtual StringA& operator<< (const int64  i);
  virtual StringA& operator<< (const uint64 ui);
  virtual StringA& operator<< (const float  f);
  virtual StringA& operator<< (const double d);

  // appending operators (path)
  // shortcuts to the PathAppend() method
  virtual StringA& operator/= (const StringA& rhs);
  virtual StringA& operator/= (const std::string& rhs);
  virtual StringA& operator/= (const char* rhs);

  // inserting
  virtual void Insert (const char c, uint uiOffset);
  virtual void Insert (const char* psz, uint uiOffset, uint uiLength=0);

  // erasing
  virtual void Erase (int nOffset, uint uiLength=0);

  // filling
  // * current content will be *discarded* by Fill() methods.
  // * use the Pad() method if you only want to append some
  //   characters to reach a given final string length.
  virtual void Fill (const char c, uint uiCount);
  virtual void Fill (const char* psz, uint uiCount);

  // padding
  virtual void Pad (const char c, uint uiDesiredLength);
  virtual void Pad (const char* psz, uint uiDesiredLength);

  // case sensitive/insensitive comparison
  virtual int  Compare       (const char* psz) const;
  virtual int  CompareCount  (const char* psz, uint uiMaxCount) const;
  virtual int  CompareI      (const char* psz) const;
  virtual int  CompareCountI (const char* psz, uint uiMaxCount) const;
  virtual bool HasPrefix     (const char c, bool bCaseSensitive);
  virtual bool HasPrefix     (const char* psz, bool bCaseSensitive);
  virtual bool HasSuffix     (const char c, bool bCaseSensitive);
  virtual bool HasSuffix     (const char* psz, bool bCaseSensitive);
  virtual bool Equals        (const char* psz) const;
  virtual bool EqualsI       (const char* psz) const;

  // case manipulation
  virtual void ToLower    (void);
  virtual void ToUpper    (void);
  virtual void InvertCase (void);

  // cutting
  virtual StringA     Left  (uint uiLength) const;
  virtual const char* Left  (uint uiLength, StringA& strOutResult) const;
  virtual StringA     Right (uint uiLength) const;
  virtual const char* Right (uint uiLength, StringA& strOutResult) const;
  virtual StringA     Mid   (uint uiOffset, uint uiLength) const;
  virtual const char* Mid   (uint uiOffset, uint uiLength, StringA& strOutResult) const;

  // a substr() emulation of the so-handy php function (check http://php.net/substr for more information)
  virtual StringA     Substr (int nOffset, int nLength=0) const;
  virtual const char* Substr (StringA& strOutResult, int nOffset, int nLength=0) const;

  // cleaning
  virtual void TrimWhitespaces      (void);
  virtual void Trim                 (const char c);
  virtual void Trim                 (const char* pszCharList);
  virtual void TrimLeft             (const char c);
  virtual void TrimLeft             (const char* pszCharList);
  virtual void TrimLeftWhitespaces  (void);
  virtual void TrimRight            (const char c);
  virtual void TrimRight            (const char* pszCharList);
  virtual void TrimRightWhitespaces (void);

  // converting
  virtual bool   IsBool              (bool bStrict=false, int* pnOutValue=NULL) const;
  virtual bool   IsInt               (bool bStrict=false) const;
  virtual bool   IsFloat             (bool bStrict=false) const;
  virtual bool   IsFullOfDigits      (void) const;
  virtual int    ExtractBool         (uint uiStartOffset=0) const;
  virtual int32  ExtractInt32        (uint uiStartOffset=0) const;
  virtual int32  ExtractInt32FromHex (uint uiStartOffset=0, uint uiMaxHexDigitsToHandle=8, bool* pbOutErrorOccurred=NULL) const;
  virtual int64  ExtractInt64        (uint uiStartOffset=0) const;
  virtual float  ExtractFloat        (uint uiStartOffset=0) const;
  virtual double ExtractDouble       (uint uiStartOffset=0) const;

  // formatting
  virtual void Format        (const char* pszFormat, ...);
  virtual void FormatV       (const char* pszFormat, va_list arglist);
  virtual void FormatAppend  (const char* pszFormat, ...);
  virtual void FormatAppendV (const char* pszFormat, va_list arglist);

  // formatting - memory / bandwidth measurement
  virtual int FormatBytesToHuman         (uint uiBytes);
  virtual int FormatBytesToHuman64       (uint64 uiBytes);
  virtual int FormatAppendBytesToHuman   (uint uiBytes, const char* pszPrefix="", const char* pszSuffix="");
  virtual int FormatAppendBytesToHuman64 (uint64 uiBytes, const char* pszPrefix="", const char* pszSuffix="");

  // encoding - url
  virtual void UrlEscape   (StringA& strOutEscaped) const;
  virtual void UrlUnescape (StringA& strOutUnescaped) const;
  virtual void UrlEscape   (void);  // only for convenience... slower than UrlEscape(StringA) since it has to copy source string before encoding it
  virtual void UrlUnescape (void);  // only for convenience... slower than UrlUnescape(StringA) since it has to copy source string before decoding it

  // encoding - utf8
  // * Utf8ToAnsi() is a very simplistic converting method since it matches
  //   only characters in the 8bit range [0,255], thus only characters in the
  //   ANSI table will be converted correctly, any other UTF8 character will
  //   be converted to a '?' character.
  // * The bComputeExactLengthFirst parameter for AnsiToUtf8() lets you to
  //   choose the best way to perform the conversion : memory footprint
  //   vs. speed. Computing the exact destination length implies an overhead
  //   because a loop is first performed on every characaters.
  // * OTOH, if bComputeExactLengthFirst is FALSE, the method will simply
  //   make a "Length * 2" operation to be sure the converted data will fit
  //   entirely in the destination buffer in every cases.
  virtual void AnsiToUtf8 (bool bComputeExactLengthFirst=false);
  virtual void Utf8ToAnsi (const uchar* pSourceUtf8String);

  // finding
  virtual int FindFirstOf     (char cNeedle,            int nStartOffset=0) const;
  virtual int FindFirstOf     (const char* pszNeedles,  int nStartOffset=0, int nNeedlesLength=-1) const;
  virtual int FindFirstOfI    (char cNeedle,            int nStartOffset=0) const;
  virtual int FindFirstOfI    (const char* pszNeedles,  int nStartOffset=0, int nNeedlesLength=-1) const;
  virtual int FindFirstNotOf  (char cNeedles,           int nStartOffset=0) const;
  virtual int FindFirstNotOf  (const char* pszNeedless, int nStartOffset=0, int nNeedlessLength=-1) const;
  virtual int FindFirstNotOfI (char cNeedles,           int nStartOffset=0) const;
  virtual int FindFirstNotOfI (const char* pszNeedless, int nStartOffset=0, int nNeedlessLength=-1) const;
  virtual int FindLastOf      (char cNeedle,            int nStartOffset=-1) const;
  virtual int FindLastOf      (const char* pszNeedles,  int nStartOffset=-1, int nNeedlesLength=-1) const;
  virtual int FindLastOfI     (char cNeedle,            int nStartOffset=-1) const;
  virtual int FindLastOfI     (const char* pszNeedles,  int nStartOffset=-1, int nNeedlesLength=-1) const;
  virtual int FindLastNotOf   (char cNeedless,          int nStartOffset=-1) const;
  virtual int FindLastNotOf   (const char* pszNeedless, int nStartOffset=-1, int nNeedlessLength=-1) const;
  virtual int FindLastNotOfI  (char cNeedless,          int nStartOffset=-1) const;
  virtual int FindLastNotOfI  (const char* pszNeedless, int nStartOffset=-1, int nNeedlessLength=-1) const;
  virtual int Find            (const char cNeedle,      int nStartOffset=0, int nEndOffset=-1) const;
  virtual int Find            (const char* pszNeedle,   int nStartOffset=0, int nEndOffset=-1) const;
  virtual int FindI           (const char cNeedle,      int nStartOffset=0, int nEndOffset=-1) const;
  virtual int FindI           (const char* pszNeedle,   int nStartOffset=0, int nEndOffset=-1) const;

  // matching
  virtual bool Match  (const char* pszPattern) const;
  virtual bool MatchI (const char* pszPattern) const;

  // replacing
  virtual void Replace  (const char cOld, const char cNew, uint uiStartOffset=0);
  virtual void ReplaceI (const char cOld, const char cNew, uint uiStartOffset=0);
  virtual void Replace  (const char* pszOld, const char* pszNew, uint uiStartOffset=0);
  virtual void ReplaceI (const char* pszOld, const char* pszNew, uint uiStartOffset=0);

  // splitting
  virtual void Explode          (StringAVec& vecOutParts, const char* pszSeparator, uint uiMaxParts=0, bool bKeepEmptyParts=false) const;
  virtual void ExplodeWithOneOf (StringAVec& vecOutParts, const char* pszSeparators, uint uiMaxParts=0, bool bKeepEmptyParts=false) const;
  virtual void Split            (const char* pszSeparator="\r\n", uint uiChunkLen=76, uint uiStartOffset=0);

  // concatenating
  virtual void Implode (const StringAVec& vecParts, const char* pszGlue);

  // hashing
  virtual uint Hash  (void) const;
  virtual uint HashI (void) const;

  // path string manipulation
  virtual void PathAppend                 (const char* pszPathElement); // this is not a simple +=, it adds a trailing slash if needed before appending new path element
  virtual void PathBackSlashesToSlashes   (void);
  virtual void PathSlashesToBackSlashes   (void);
  virtual void PathExtractRoot            (StringA& strDest) const;
  virtual void PathExtractDirectory       (StringA& strDest) const;
  virtual void PathExtractName            (StringA& strDest) const;
  virtual void PathExtractTitle           (StringA& strDest) const;
  virtual void PathExtractExtension       (StringA& strDest) const;
  virtual void PathStripDirectory         (void);
  virtual bool PathStripTrailingSeparator (void);
  virtual void PathStripName              (void);
  virtual void PathStripExtension         (void);
  virtual bool PathHasTrailingSeparator   (void) const;
  virtual void PathExplode                (StringAVec& vecPath) const;
  virtual bool PathCheckExtension         (const char* pszExtension) const; // case insensitive compare of the extension
  virtual void PathChangeExtension        (const char* pszExtension);
  virtual bool PathIsAbsolute             (void) const;
  virtual bool PathIsRoot                 (void) const;
  virtual bool PathIsWindowsNetwork       (void) const;  // i.e: it begins with "\\"
  virtual bool PathExpand                 (void); // a realpath() implementation, path must be absolute ! this method is only used to expand '.' and '..' sequences and resolve symlinks
  virtual int  PathCompare                (const char* pszPath, bool bCaseSensitive) const; // note that '/' == '\\' here !
  virtual bool PathIsInDirectory          (const char* pszDirPath, bool bCaseSensitive) const;



  // static utils - string comparison
  static int StrCompare       (const char* psz1, const char* psz2);
  static int StrCompareCount  (const char* psz1, const char* psz2, uint uiMaxCount);
  static int StrCompareI      (const char* psz1, const char* psz2);
  static int StrCompareCountI (const char* psz1, const char* psz2, uint uiMaxCount);
  static int StrComparePath   (const char* pszPath1, const char* pszPath2, bool bCaseSensitive);  // warning : this method may be slow since it makes subsequent calls to PathExpand()

  // static utils - types of characters
  static char  CharToLower              (int c);
  static char  CharToUpper              (int c);
  static bool  CharIsLower              (int c);
  static bool  CharIsUpper              (int c);
  static bool  CharIsAlpha              (int c);
  static bool  CharIsNumeric            (int c);
  static bool  CharIsAlNum              (int c);
  static bool  CharIsPrintable          (int c);
  static bool  CharIsSpace              (int c);
  static bool  CharIsTab                (char c);
  static bool  CharIsNewLine            (char c);
  static bool  CharIsPathSeparator      (char c);
  static bool  CharIsWindowsDriveLetter (char c);
  static bool  CharIsBase64             (char c);
  static bool  CharIsHex                (char c);
  static uint8 CharHexToInt             (char c);
  static void  StrToLower               (char* psz);
  static void  StrToUpper               (char* psz);
  static void  StrInvertCase            (char* psz);

  // static utils - converting
  static bool StrIsBool         (const char* psz, bool bStrict=false, int* pnOutValue=NULL);
  static bool StrIsInt          (const char* psz, bool bStrict=false);
  static bool StrIsFloat        (const char* psz, bool bStrict=false);
  static bool StrIsFullOfDigits (const char* psz);

  // static utils - finding
  static int StrFindFirstOf    (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1);
  static int StrFindFirstOf    (const char* pszHaystack, const char* pszNeedles,  bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1, int nNeedlesLength=-1);
  static int StrFindFirstNotOf (const char* pszHaystack, const char cNeedless,    bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1);
  static int StrFindFirstNotOf (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset=0,  int nHaystackLength=-1, int nNeedlessLength=-1);
  static int StrFindLastOf     (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1);
  static int StrFindLastOf     (const char* pszHaystack, const char* pszNeedles,  bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1, int nNeedlesLength=-1);
  static int StrFindLastNotOf  (const char* pszHaystack, const char cNeedless,    bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1);
  static int StrFindLastNotOf  (const char* pszHaystack, const char* pszNeedless, bool bCaseSensitive, int nStartOffset=-1, int nHaystackLength=-1, int nNeedlessLength=-1);
  static int StrFind           (const char* pszHaystack, const char cNeedle,      bool bCaseSensitive, int nStartOffset=0,  int nEndOffset=-1, int nHaystackLength=-1);
  static int StrFind           (const char* pszHaystack, const char* pszNeedle,   bool bCaseSensitive, int nStartOffset=0,  int nEndOffset=-1, int nHaystackLength=-1);

  // static utils - matching using wildcards
  static bool StrMatch (const char* pszHaystack, const char* pszPattern, bool bCaseSensitive);

  // static utils - string hashing
  static uint32 StrHash  (const char* psz);
  static uint32 StrHash  (const char* pBlock, uint uiCount);
  static uint32 StrHashI (const char* psz);
  static uint32 StrHashI (const char* pBlock, uint uiCount);



  // appending - friends operators
  friend StringA operator+ (const StringA& lhs, const StringA& rhs);
  friend StringA operator+ (const StringA& lhs, const std::string& rhs);
  friend StringA operator+ (const StringA& lhs, const char* rhs);
  friend StringA operator+ (const std::string& lhs, const StringA& rhs);
  friend StringA operator+ (const char* lhs, const StringA& rhs);
  friend StringA operator+ (const StringA& lhs, const bool   b);
  friend StringA operator+ (const StringA& lhs, const char   c);
  friend StringA operator+ (const StringA& lhs, const int32  i);
  friend StringA operator+ (const StringA& lhs, const uint32 ui);
  friend StringA operator+ (const StringA& lhs, const int64  i);
  friend StringA operator+ (const StringA& lhs, const uint64 ui);
  friend StringA operator+ (const StringA& lhs, const float  f);
  friend StringA operator+ (const StringA& lhs, const double d);

  // path appending - friends operators
  friend StringA operator/ (const StringA& lhs, const StringA& rhs);
  friend StringA operator/ (const StringA& lhs, const std::string& rhs);
  friend StringA operator/ (const StringA& lhs, const char* rhs);
  friend StringA operator/ (const std::string& lhs, const StringA& rhs);
  friend StringA operator/ (const char* lhs, const StringA& rhs);

  // case sensitive comparison - friends operators
  friend bool operator== (const StringA& lhs, const StringA& rhs);
  friend bool operator== (const StringA& lhs, const std::string& rhs);
  friend bool operator== (const StringA& lhs, const char* rhs);
  friend bool operator== (const std::string& lhs, const StringA& rhs);
  friend bool operator== (const char* lhs, const StringA& rhs);
  friend bool operator!= (const StringA& lhs, const StringA& rhs);
  friend bool operator!= (const StringA& lhs, const std::string& rhs);
  friend bool operator!= (const StringA& lhs, const char* rhs);
  friend bool operator!= (const std::string& lhs, const StringA& rhs);
  friend bool operator!= (const char* lhs, const StringA& rhs);
  friend bool operator<  (const StringA& lhs, const StringA& rhs);
  friend bool operator<  (const StringA& lhs, const std::string& rhs);
  friend bool operator<  (const StringA& lhs, const char* rhs);
  friend bool operator<  (const std::string& lhs, const StringA& rhs);
  friend bool operator<  (const char* lhs, const StringA& rhs);
  friend bool operator>  (const StringA& lhs, const StringA& rhs);
  friend bool operator>  (const StringA& lhs, const std::string& rhs);
  friend bool operator>  (const StringA& lhs, const char* rhs);
  friend bool operator>  (const std::string& lhs, const StringA& rhs);
  friend bool operator>  (const char* lhs, const StringA& rhs);
  friend bool operator<= (const StringA& lhs, const StringA& rhs);
  friend bool operator<= (const StringA& lhs, const std::string& rhs);
  friend bool operator<= (const StringA& lhs, const char* rhs);
  friend bool operator<= (const std::string& lhs, const StringA& rhs);
  friend bool operator<= (const char* lhs, const StringA& rhs);
  friend bool operator>= (const StringA& lhs, const StringA& rhs);
  friend bool operator>= (const StringA& lhs, const std::string& rhs);
  friend bool operator>= (const StringA& lhs, const char* rhs);
  friend bool operator>= (const std::string& lhs, const StringA& rhs);
  friend bool operator>= (const char* lhs, const StringA& rhs);



protected :
  void Init          (void);
  bool IsBufferAlloc (void) const;



protected :
  uint32 m_uiLength;
  char*  m_pszData;
  uint32 m_uiAllocSize;
  char   m_szBaseBuffer[STRING_BASE_BUFFER_SIZE];


protected :
  static BoolStringType ms_eBoolStringType;
};


#include "StringA.inl.h"


} // namespace nCore


//---------------------------------------------------------------------------
// hash<StringA>
//
// This specialization of the hash<> struct will allows us to use StringA as
// a key for the std::hash_* containers.
//---------------------------------------------------------------------------
namespace nCoreHashMapNamespace
{
#if defined(STLPORT)
  #if !defined(_STLP_HASH_FUN_H)
    #error At least one of the STLport <hash_*> headers must be included !
  #endif
  template<> struct hash<::nCore::StringA>
  {
    size_t operator() (const ::nCore::StringA& str) const
    {
      // __stl_hash_string() can be found in "stlport/stl/_hash_fun.h"
      //return _STLP_PRIV __stl_hash_string(str.c_str());
      return str.Hash();
    }
  };

#elif defined(NCORE_COMPILER_MSVC)
  template<> inline size_t hash_value (const ::nCore::StringA& str)
  {
    //return hash_value(str.c_str());
    return str.Hash();
  }

#elif defined(NCORE_COMPILER_GNU)
  template<> struct hash<nCore::StringA>
  {
    size_t operator() (const ::nCore::StringA& str) const
    {
      // __stl_hash_string() can be found in <ext/stl_hash_fun.h>
      //return __stl_hash_string(str.c_str());
      return str.Hash();
    }
  };

#else
#error Not implemented yet !
#endif
}


#endif // #ifndef __nCore_StringA_Header__
