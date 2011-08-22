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

#ifndef __nCore_Exception_Header__
#define __nCore_Exception_Header__
namespace nCore {


//---------------------------------------------------------------------------
// Exception
//
// Exception class
//
// A class to handle exceptions across the code. Thrown exception can be
// identified by getting its exception code (use the GetCode() method).
//---------------------------------------------------------------------------
class NCORE_EXPORT Exception : public ::std::exception
{
public :
  Exception (void);
  Exception (const char* pszMessage);
  Exception (const Exception& e);
  Exception (const ::std::exception& e);
  Exception (const char* pszFilePath, uint uiFileLine, const char* pszFunction, uint uiExCode, const char* pszMessageFormat, ...);
  virtual ~Exception (void) throw();

  // operators
  Exception& operator= (const Exception& e);
  Exception& operator= (const ::std::exception& e);

  // description
  virtual const char* what           (void) const throw();
  virtual const char* GetDescription (void) const throw(); // alias for what()

  // miscellaneous properties getters
  virtual const char* GetFilePath (void) const;
  virtual uint        GetFileLine (void) const;
  virtual const char* GetFunction (void) const;
  virtual uint        GetCode     (void) const;

  // utilities
  virtual void    Log                     (void) const;
  virtual StringA GetFileName             (void) const;
  virtual StringA GetCodeString           (void) const;
  virtual StringA GetDescriptionMultiline (void) const;


  // static utils
  static bool        IsUserExCode                (uint uiExCode);
  static bool        IsStandardExCode            (ExceptionCode eExCode);
  static const char* StandardExCodeToName        (ExceptionCode eExCode);
  static const char* StandardExCodeToDescription (ExceptionCode eExCode);


protected :
  // description
  StringA m_strDescription;

  // miscellaneous properties
  const char* m_pszFilePath;
  uint        m_uiFileLine;
  const char* m_pszFunction;
  uint        m_uiCode;
};

#include "Exception.inl.h"


}
#endif // #ifndef __nCore_Exception_Header__
