
#include "_Main.h"


//---------------------------------------------------------------------------
// OwnLogger *with* LoggerEx
//---------------------------------------------------------------------------
#ifdef NCORE_ENABLE_LOGGEREX

OwnLogger::OwnLogger (const char* pszBaseDir, const char* pszBaseName, const char* pszTitle)
: nCore::LoggerEx(pszTitle)
{
  const bool c_bDeletePrevious  = false;
  const uint c_uiMaxLogFileSize = 0; //512000; //500KB

  nCore::StringA strLogFile;

  // check parameters
  XASSERT(pszBaseDir && pszBaseDir[0]);
  XASSERT(pszBaseName && pszBaseName[0]);

  // setup logger behavior
#ifdef X_DEBUG
  this->SetFilter(nCore::LLALL);
  this->SetSourceFileDisplay(nCore::LoggerEx::SRCFILEDISP_LASTDIR_AND_NAME);
  this->SetDateTimeDisplay(nCore::LoggerEx::DTFLAG_TIME_FRACTION);
#else
  this->SetFilter(nCore::LLINFO | nCore::LLWARNING | nCore::LLERROR | nCore::LLFATAL);
  this->SetSourceFileDisplay(nCore::LoggerEx::SRCFILEDISP_LASTDIR_AND_NAME); //nCore::LoggerEx::SRCFILEDISP_NAME_ONLY);
  this->SetDateTimeDisplay(nCore::LoggerEx::DTFLAG_TIME_FRACTION);
#endif

  // build log file path
  strLogFile << pszBaseDir << '/' << pszBaseName;
//#ifdef NCORE_ENABLE_MULTI_PROCESS_AWARENESS
//  strLogFile << '_' << nCore::System::GetCurrentProcessID();
//#endif
  strLogFile += ".log";


  // enable windbg channel
#if defined(NCORE_PLATFORM_WINDOWS) && defined(X_DEBUG)
  this->EnableOutput_WinDbg(true);
#endif

  // enable stdout channel
#if !defined(NCORE_PLATFORM_WINDOWS)
  this->EnableOutput_StdOut(true);
#endif

  // enable stderr channel
#if !defined(NCORE_PLATFORM_WINDOWS) && !defined(X_DEBUG)
  this->EnableOutput_StdErr(true, pszBaseName, getpid());
#endif

  // enable text file channel
  this->EnableOutput_TxtFile(true, strLogFile, c_bDeletePrevious, c_uiMaxLogFileSize);
}


//---------------------------------------------------------------------------
// OwnLogger *without* LoggerEx
//---------------------------------------------------------------------------
#else // #ifdef NCORE_ENABLE_LOGGEREX

OwnLogger::OwnLogger (const char* pszBaseDir, const char* pszBaseName, const char* pszTitle)
: nCore::Logger()
{
}

void OwnLogger::Print (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, nCore::LogLevel eLevel, const nCore::String8& strMessage)
{
  nCore::String8 strOutput;

  // format message
  strOutput.Format("[%c] %s\n", nCore::Logger::MapLogLevelToChar(eLevel), strMessage.c_str());

  // print message to stdout
  fputs(strOutput.c_str(), stdout);

  // print message to windbg
#if defined(NCORE_PLATFORM_WINDOWS) && defined(X_DEBUG)
  nCore::String8 strTemp;
  strTemp << pszSourceFile << "(" << uiSourceLine << "): ";
  strOutput.Insert(strTemp, 0);
  OutputDebugString(strOutput.c_str());
#endif

}

#endif // #ifdef NCORE_ENABLE_LOGGEREX
