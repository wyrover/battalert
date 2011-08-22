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

#ifndef __nCore_ConfigFile_Header__
#define __nCore_ConfigFile_Header__
#if defined(NCORE_ENABLE_UTIL_CONFIG_FILE)
namespace nCore {


//---------------------------------------------------------------------------
// ConfigFile
//
// Load/Save a configuration file.
//
// Run-time configuration settings class.
// Configuration data is loaded from a file. The file format used here is
// basically the same as most of the configuration files on Unix platforms.
// You can insert comments by starting them using a '#' or ';' character.
//
// Configuration file example :
// <pre>
//   # Some settings
//   Phrase      My value with some space characters inside...  # comment
//   ThreadsNum  123    # comment
//   Range       0.45   ; comment
//
//   ; A new comment
//   DBHost  localhost      # comment
//   DBName  "WeirdDbName with spaces   " ; The '"' characters allow you define values with spaces inside
//   DBUser  HarryPotter
//   DBPass  PezPoilu
//
//   # Some settings with the same key
//   LoadModule  Module0  # uiIndex is 0
//   LoadModule  Module1  # uiIndex is 1
//   LoadModule  Module2  # uiIndex is 2
// </pre>
//---------------------------------------------------------------------------
class NCORE_EXPORT ConfigFile
{
public :
  enum
  {
    MAX_RAWDATA_SIZE = 0xFFFF, // this allows us to limit security attacks a *little*
  };

  enum LineType
  {
    LINE_BLANK = 0,
    LINE_COMMENT,
    LINE_INCLUDE,
    LINE_SETTING,

    LINE_FORCE_DWORD = 0x7FFFFFFF,
  };

  enum LineState
  {
    LINESTATE_NONE = 0,  // nothing special, line was loaded while reading file
    LINESTATE_MODIFIED,  // this line has a key-value pair (a setting) and its value part has been modified
    LINESTATE_NEW,       // this line did not exist at loading time and was added after
    LINESTATE_REMOVED,   // this line was loaded but removed after

    LINESTATE_FORCE_DWORD = 0x7FFFFFFF,
  };

  struct File;
  struct Line
  {
    File*     pFile;           // parent file entity of this line
    LineType  eType;           // type of the line
    LineState eState;          // state of the line
    StringA   strValue;        // the content of the line, but if type is LINE_SETTING, represents only the "value" part of the line
    StringA   strKey;          // the name of the setting, only used if if line type is LINE_SETTING
    StringA   strComment;      // the trailing comment, only used if if line type is LINE_SETTING
  //StringA   strIncludePath;  // the complete path of the pointed configuration file, only used if if line type is LINE_INCLUDE
    bool      bHasQuotes;

    // constructor
    Line (void)
    {
      pFile      = NULL;
      eType      = LINE_BLANK;
      eState     = LINESTATE_NONE;
      bHasQuotes = false;
    }
  };

  typedef std::list<Line*>        Lines;
  typedef Lines::iterator         LinesIt;
  typedef Lines::const_iterator   LinesCIt;
  typedef Lines::reverse_iterator LinesRIt;

  struct File
  {
    File*   pParentFile;
    StringA strFilePath;  // config file path to keep trace of it in case of rewrite
    StringA strFileName;  // file name extracted from given path
    Lines   vecLines;     // lines of the loaded file

    // constructor
    File (void)
    {
      pParentFile = NULL;
    }
  };

  typedef std::list<File*>       Files;
  typedef Files::iterator        FilesIt;
  typedef Files::const_iterator  FilesCIt;

  typedef std::multimap<StringA,Line*> Settings; // <key_name,line>
  typedef Settings::iterator           SettingsIt;
  typedef Settings::const_iterator     SettingsCIt;



public :
  ConfigFile (void);
  ~ConfigFile (void);

  // create new
  void CreateNew (const StringA& strFilePath);

  // load / save
  uint Load       (const StringA& strFilePath, bool bCreateNewIfNotExist=false);
  bool IsLoaded   (void) const;  // Only if an *existing* file has been loaded
  bool IsModified (void) const;  // If a value has been added/modified after loading
  bool Save       (void);

  // last error occurred while loading
  bool           HasError (void) const;
  const StringA& GetError (void) const;

  // settings
  bool    HasValue              (const StringA& strKey, uint uiIndex=0) const;
  StringA GetValueAsString      (const StringA& strKey, const StringA& strDefaultValue="", uint uiIndex=0) const;
  bool    GetValueAsRawData     (const StringA& strKey, byte* pDestBuffer, uint uiMaxDataSize, uint uiIndex=0) const;
  uint    GetValueAsRawDataSize (const StringA& strKey, uint uiIndex=0) const;

  // add/change/remove a value
  bool AddValue    (const StringA& strKey, const StringA& strValue);
  bool AddValue    (const StringA& strKey, const byte* pRawDataBuffer, uint uiBufferSize);
  bool ChangeValue (const StringA& strKey, const StringA& strNewValue, uint uiIndex=0);
  bool ChangeValue (const StringA& strKey, const byte* pBuffer, uint uiBufferSize, uint uiIndex=0);
  bool RemoveValue (const StringA& strKey, uint uiIndex=0);


private :
  void Reset (void);

  void LoadFile           (const StringA& strFilePath, File* pParentFile);
  bool ParseFile          (File& file, FILE* hFile);
  bool ParseLine          (Line& line, uint uiCurrentLine);
  bool HasLoadedFileNamed (const StringA& strFileName);
  void UnregisterFile     (File* pFile);
  bool SaveFile           (const File& file);

        Line* GetSetting (const StringA& strKey, uint uiIndex=0);
  const Line* GetSetting (const StringA& strKey, uint uiIndex=0) const;

  static bool   IsComment          (const StringA& strLine, int nStartFromPos=0);
  static void   SetValue           (Line& line, const StringA& strValue);
  static size_t ValueLength        (const Line& line);
  static void   PrepareRawData     (StringA& strOutEncodedData, const byte* pBuffer, uint uiBufferSize);
  static bool   ExtractRawData     (const StringA& strLineValue, byte* pDestBuffer, uint uiMaxDataSize);
  static uint   ExtractRawDataSize (const StringA& strLineValue);


private :
  Files    m_Files;         // "files -> settings" representation
  Settings m_Settings;      // flat representation of all loaded settings
  bool     m_bLoaded;
  StringA  m_strLastError;
};


#include "ConfigFile.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_CONFIG_FILE)
#endif // #ifndef __nCore_ConfigFile_Header__
