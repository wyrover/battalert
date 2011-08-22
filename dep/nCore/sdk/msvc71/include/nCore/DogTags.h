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

#ifndef __nCore_DogTags_Header__
#define __nCore_DogTags_Header__
#if defined(NCORE_ENABLE_UTIL_DOGTAGS)
namespace nCore {


//---------------------------------------------------------------------------
// DogTags
//
// What are DogTags ?
// * In nCore, a dog-tag is a key-value pair wich can be "engraved" in a
//   binary file (for example, a compiled executable file) at compile-time
//   for later reading/writing.
// * Technically, this is simply a constant you define in your source-code
//   with a specified name and length by using the NCORE_DOGTAG() macro.
// * Example to declare a DogTag in your application :
//     NCORE_DOGTAG(MyTagName, "Default\0..."); // note : no quotes around your tag name !
//   That's all ! You will need DogTags class *only* to read/write dog-tags.
// * In this example, the tag will be named "MyTagName" and its default value
//   is "Default". Here, the maximum length of its value is 12 characters
//   ("Default" is 7, '\0' is 1, "..." is 3+1 and DogTag does *not* care
//   about null termination so your value can really use the maximum space
//   allowed, otherwise, a null character will be automatically appended by
//   the DogTags class to allow easy reading).
// * The better case of use I found accross the Internet is in the BO2K
//   project (http://bo2k.sourceforge.net/) : dog-tags are used like they
//   were values from a configuration file, so values can be modified
//   directly into the target compiled file if needed.
//   This is exactly what the DogTags class allows you to do.
// * WARNING : By the way, you must know that some compiled binary file
//   formats may not support dog-tags *modifications* because they perform
//   some CRC check at runtime.
//   This is up to you to know if it will be OK.
// * It is important to understand that this class allows you only to read
//   and modify dog-tags found in a given file/buffer, but it does *not
//   allow you to create a new dog-tag.
//
// How does this class works ?
// * Dog-Tags can be found in a physical file of via a given buffer.
//   For the later, there is nothing special to explain, this class will
//   match every recognized dog-tags in it and register them internally so
//   for each one of them, you will be able to modify its value according to
//   its own maximum storage length.
// * For a physical file, this is mainly the main process...
//   File is open and *mapped* into memory, and the rest of the process is
//   the same as for a given buffer.
//   Note that file will be locked in read access only until you call the
//   Close() method.
//---------------------------------------------------------------------------
class NCORE_EXPORT DogTags
{
public :
  struct Tag
  {
    uint    uiPrefixOffset;
    StringA strName;
    StringA strValue;
    uint    uiValueMaxLen;

    bool bModified;
  };

  // why tags are stored in a vector instead of a map or even hash_map ?
  // well, in this case, it is more important to store tag a way that keep
  // the same order as they are stored in target file because this will allow
  // more efficient writing (no more seeking than it's needed).
  // more than this, there is usually not much tags into a file so finding
  // them into a vector does not cost so much...  - jcl
  typedef std::vector<Tag>     Tags;
  typedef Tags::iterator       TagsIt;
  typedef Tags::const_iterator TagsCIt;


public :
  DogTags (void);
  ~DogTags (void); // implicit call to this->Close(false);

  bool OpenFile   (const char* pszFilePath);
  bool OpenBuffer (byte* pBuffer, uint uiSize);
  bool IsOpen     (void) const;
  bool Close      (bool bWriteModifications);

  uint GetTagsCount (void) const;
  bool HasTags      (void) const;  // shortcut to GetTagsCount() == 0

  bool HasTag (const StringA& strName) const;

  bool    GetTagValue          (const StringA& strName, StringA& strOutValue) const;
  StringA GetTagValue          (const StringA& strName) const;
  uint    GetTagValueMaxLength (const StringA& strName) const;
  bool    ChangeTagValue       (const StringA& strName, const StringA& strNewValue);


private :
  void ScanForTags (void);
  bool WriteTags   (void);

  const Tag* FindTag (const StringA& strName) const;

  static byte* FindMotif (const byte* pBuffer, uint uiSize, const byte* pMotif, uint uiMotifLength);


private :
  // tags list
  Tags m_Tags;

  // file/buffer properties
  StringA m_strFilePath;
  byte*   m_pFileMap;
  uint    m_uiFileSize;
#if defined(NCORE_PLATFORM_WINDOWS)
  HANDLE  m_hFileMapping;
#elif defined(NCORE_PLATFORM_LINUX)
  int     m_hFile;
#endif


private :
  static uint ms_uiDogTagPrefixLen; // strlen(NCORE_DOGTAG_PREFIX)
  static uint ms_uiDogTagSuffixLen; // strlen(NCORE_DOGTAG_SUFFIX)
};


#include "DogTags.inl.h"


}
#endif // #if defined(NCORE_ENABLE_UTIL_DOGTAGS)
#endif // #ifndef __nCore_DogTags_Header__
