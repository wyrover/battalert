
#ifndef __OwnLogger_Header__
#define __OwnLogger_Header__


//---------------------------------------------------------------------------
// OwnLogger
//---------------------------------------------------------------------------
class OwnLogger :
#ifdef NCORE_ENABLE_LOGGEREX
  public nCore::LoggerEx
#else
  public nCore::Logger
#endif
{
public :
  OwnLogger (const char* pszBaseDir, const char* pszBaseName, const char* pszTitle);

#ifndef NCORE_ENABLE_LOGGEREX
  // print
  void Print (const char* pszSourceFile, uint uiSourceLine, const char* pszSourceFunction, nCore::LogLevel eLevel, const nCore::String8& strMessage);
#endif
};


#endif // #ifndef __OwnLogger_Header__
