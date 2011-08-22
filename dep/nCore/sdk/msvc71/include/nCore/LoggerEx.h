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

#ifndef __nCore_LoggerEx_Header__
#define __nCore_LoggerEx_Header__
#if defined(NCORE_ENABLE_LOGGEREX)
namespace nCore {


//---------------------------------------------------------------------------
// LoggerEx
//---------------------------------------------------------------------------
class NCORE_EXPORT LoggerEx : public Logger
{
public :
  enum SourceFileDisplay
  {
    SRCFILEDISP_NONE,              // output nothing
    SRCFILEDISP_COMPLETE_PATH,     // output "/root/lastdir2/lastdir1/file.cpp(line)"
    SRCFILEDISP_LASTDIR_AND_NAME,  // output "lastdir/file.cpp(line)" (DEFAULT)
    SRCFILEDISP_LASTDIR2_AND_NAME, // output "lastdir2/lastdir1/file.cpp(line)"
    SRCFILEDISP_NAME_ONLY,         // output "file.cpp(line)"
  };

  enum DateTimeFlags
  {
    DTFLAG_NONE          = 0,
    DTFLAG_DATE          = 0x0001,              // "YYYY-MM-DD"
    DTFLAG_TIME          = 0x0002,              // "hh:mm:ss"
    DTFLAG_TIME_FRACTION = 0x0004|DTFLAG_TIME,  // "hh:mm:ss.fff" (DEFAULT)
  };


public :
  LoggerEx (const char* pszTitle=NULL);
  virtual ~LoggerEx (void);

  // settings
  void SetSourceFileDisplay (SourceFileDisplay eSourceFileDisplay);
  void SetDateTimeDisplay   (dword dwDateTimeFlags);

  // settings getters
  SourceFileDisplay GetSourceFileDiplay (void) const;
  dword             GetDateTimeDisplay  (void) const;

  // output channels
  // * note that every channels are disabled by default except the WinDbg
  //   channel which enabled by default in debug mode.
  void EnableOutput_StdOut  (bool bEnable);
  void EnableOutput_StdErr  (bool bEnable, const StringA& strProcessName="", dword dwAllowedLogLevels=dword(LLWARNING|LLERROR|LLASSERTION|LLFATAL));
  void EnableOutput_TxtFile (bool bEnable, const char* pszFilePath=NULL, bool bDeletePrevious=true, uint32 uiMaxFileSize=0);
#ifdef NCORE_PLATFORM_WINDOWS
  void EnableOutput_WinDbg  (bool bEnable);
#endif

  // output channel status getters
  bool HasOutputStdOut  (void) const;
  bool HasOutputStdErr  (void) const;
  bool HasOutputTxtFile (void) const;
#ifdef NCORE_PLATFORM_WINDOWS
  bool HasOutputWinDbg  (void) const;
#endif

  // print
  void Print (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, LogLevel eLevel, const StringA& strMessage);


protected :
  // settings
  StringA           m_strTitle;           // default : <empty>
  SourceFileDisplay m_eSourceFileDisplay; // default : SRCFILEDISP_LASTDIR_AND_NAME
  dword             m_dwDateTimeFlags;    // default : DTFLAG_TIME_FRACTION

  // channel - stdout
  bool m_bStdOut;

  // channel - stderr
  dword   m_dwStdErrFilter;       // output on stderr will always use this filter *only*, not the global filter
  StringA m_strStdErrProcessName;

  // channel - txtfile
  int     m_nTxtFile;
  StringA m_strTxtFile;
  bool    m_bTxtFileEmpty;
  uint32  m_uiTxtFileMaxSize;
  bool    m_bTxtFileFull;

  // channel - windbg
#ifdef NCORE_PLATFORM_WINDOWS
  bool m_bWinDbg;
#endif
};


#include "LoggerEx.inl.h"


}
#endif // #if defined(NCORE_ENABLE_LOGGEREX)
#endif // #ifndef __nCore_LoggerEx_Header__
