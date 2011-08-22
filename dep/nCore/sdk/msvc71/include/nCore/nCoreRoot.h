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

#ifndef __nCore_nCoreRoot_Header__
#define __nCore_nCoreRoot_Header__
namespace nCore {


//---------------------------------------------------------------------------
// nCoreRoot
//
// The nCore's root object.
//---------------------------------------------------------------------------
class NCORE_EXPORT nCoreRoot : public Singleton<nCoreRoot>, private NonCopyable
{
private :
  explicit nCoreRoot (const char* pszArgv0, Logger* pUserLoggerToBind);
  virtual ~nCoreRoot (void);

public :
  // init & uninit nCore'
  static void Init   (const char* pszArgv0, Logger* pUserLoggerToBind=NULL);
  static bool IsInit (void);
  static void Uninit (void);

  // bind external logger (RECOMMENDED)
  // * first, you should be aware that nCore will create a log file and
  //   append log message each it will need it if you do not provide your own
  //   Logger object.
  // * if you think this is an annoying feature, please be sure to check the
  //   <nCore/LoggerNull.h> header to create a dummy Logger object.
  // * othwerwise, you may want to enable the NCORE_ENABLE_LOGGEREX feature
  //   in order to gain access to the LoggerEx class which is convenient in
  //   many cases.
  // * if you are a brave coder and think nCore::LoggerEx is worthless, you
  //   may want to create your own logging facility by simply derive a class
  //   from the nCore::Logger interface.
  // * In every case, be sure to check the <nCore/LoggerNull.h> header to have
  //   an example of use.
  //   You can also read the source code of nCoreTester (the testing
  //   application bound to the nCore's source code).
  void    BindExternalLogger (Logger* pUserLogger);
  Logger* GetExternalLogger  (void);

  // logger methods (please use the CORELOG macro instead)
  static void Log (const char* pszSourcePath, uint uiSourceLine, const char* pszSourceMethod, LogLevel eLevel, const char* pszFormat, ...);
  static void Log (const char* pszSourcePath, uint uiSourceLine, const char* pszSourceMethod, LogLevel eLevel, const StringA& strMessage);

  // current client application location
  const StringA& GetApplicationPath      (void) const;
  const StringA& GetApplicationDirectory (void) const;
  const StringA& GetApplicationFileName  (bool bWithExtension=true) const;

  // about this product
  static const StringA&  GetLibraryTitle     (void); // returns a "PRODNAME vPRODVERSION" string.
  static const StringA&  GetLibraryName      (void); // returns the name of this library.
  static const Version3& GetLibraryVersion3  (void); // returns version number of this library.
  static const StringA&  GetLibraryTimeStamp (void); // returns the compile-time generated timestamp of the library "__DATE__ @ __TIME__".
  static const StringA&  GetLibraryWebLink   (void); // returns an http link to the home page of this library.
  static const StringA&  GetLibraryAuthor    (void); // returns the name of the author of this library (just in case :) ).

  // base64 encoding / decoding
  static void Base64Encode (StringA& strOutEncodedData, const byte* pSourceData, uint uiSourceDataSize, bool bClearOutputFirst);
  static void Base64Decode (std::vector<byte>& vecOutputData, const byte* pSourceData, uint uiSourceDataSize);
  static void Base64Decode (byte* pDestBuffer, uint uiMaxDestDataSize, const byte* pSourceData, uint uiSourceDataSize);

  // randomness
                         static uint32 Random32        (void);                   // generate a random number in a 32bit word
                         static uint64 Random64        (void);                   // generate a random number in a 64bit word
  template <class RealT> static RealT  RandomUnit      (void);                   // generate a random floating point value in [0,1]
  template <class RealT> static RealT  RandomSymmetric (void);                   // generate a random floating point value in [-1,1]
  template <class RealT> static RealT  RandomRange     (RealT rMin, RealT rMax); // generate a random floating point value in [rMin,rMax]

  // singleton stuff
  static nCoreRoot& GetSingleton    (void);
  static nCoreRoot* GetSingletonPtr (void);


private :
  // default logger's sink
  static void LogToDefaultSink (const char* pszSourcePath, uint uiSourceLine, const char* pszSourceMethod, LogLevel eLevel, const StringA& strMessage);


private :
  // application location
  StringA m_strAppPath;
  StringA m_strAppDir;
  StringA m_strAppFileName;
  StringA m_strAppFileTitle;

  // user's logger
  Logger* m_pUserLogger;


public :
  static const char BASE64CHARS[];     // "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
  static const int  BASE64CHARS_COUNT; // (sizeof(nCoreRoot::BASE64CHARS) / sizeof(nCoreRoot::BASE64CHARS[0])) - 1
};


// shortcuts
static inline nCoreRoot& g_Core  (void)  { return ::nCore::nCoreRoot::GetSingleton(); }
static inline nCoreRoot* g_pCore (void)  { return ::nCore::nCoreRoot::GetSingletonPtr(); }

// inline methods
#include "nCoreRoot.inl.h"


}
#endif // #ifndef __nCore_nCoreRoot_Header__
