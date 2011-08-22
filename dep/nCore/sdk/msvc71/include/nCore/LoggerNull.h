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

#ifndef __nCore_LoggerNull_Header__
#define __nCore_LoggerNull_Header__
namespace nCore {


//---------------------------------------------------------------------------
// LoggerNull
//
// This is a dummy logger so you can really tell nCore to shut up completely.
// This is not recommended though...
//
// To use it, simply create an instance of this class and give it to the
// g_Core().BindExternalLogger() method. Be aware that you'll have to keep
// this pointer to release it at the end of your application :
// <code>
//   // create your null logger
//   LoggerNull* pNullLogger = new LoggerNull();
//
//   // init nCore
//   // * you can use g_Core().BindExternalLogger(pNullLogger); after the call
//   //   to nCore::Init() if you don't want to create your logger first
//   nCore::Init(argv[0], pNullLogger);
//
//   //
//   // ... code of your application ...
//   //
//
//   // end of your application
//   // * calling g_Core().BindExternalLogger(NULL) is important so nCore
//   //   will not use the dumb pointer if you delete if.
//   g_Core().BindExternalLogger(NULL);
//   delete pNullLogger;
//
//   uninit nCore (do that as late as possible)
//   nCoreRoot::Uninit();
// </code>
//---------------------------------------------------------------------------
class NCORE_EXPORT LoggerNull : public Logger
{
public :
  LoggerNull (void) : Logger() { }
  ~LoggerNull (void) { }

  // print
  void Print (const char*, uint, const char*, LogLevel, const char*) { }
};


}
#endif // #ifndef __nCore_Logger_Header__
