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

#ifndef __nCore_HashMap_Header__
#define __nCore_HashMap_Header__
//namespace nCore {


//---------------------------------------------------------------------------
// HashMap
//---------------------------------------------------------------------------
#if defined(STLPORT) && defined(_STLP_STD_NAME)
  // stlport
  #define nCoreHashMapNamespace  _STLP_STD_NAME
  #define nCoreHashMap           ::_STLP_STD_NAME::hash_map
  #define nCoreHashMultiMap      ::_STLP_STD_NAME::hash_multimap

#elif defined(NCORE_COMPILER_MSVC) && (NCORE_COMPILER_VERSION >= 1310) && !defined(STLPORT)//_STLP_MSVC)
  // msvc 7.1+ without stlport
  //   from msdn : "In Visual C++ .NET 2003, members of the <hash_map> and
  //   <hash_set> header files are no longer in the std namespace, but rather
  //   have been moved into the stdext namespace."
  #define nCoreHashMapNamespace  stdext
  #define nCoreHashMap           ::stdext::hash_map
  #define nCoreHashMultiMap      ::stdext::hash_multimap

#elif defined(NCORE_COMPILER_GNU) && (NCORE_COMPILER_VERSION >= 30100) && !defined(STLPORT)
  // gcc 3.1+ without stlport
  #define nCoreHashMapNamespace  __gnu_cxx
  #define nCoreHashMap           ::__gnu_cxx::hash_map
  #define nCoreHashMultiMap      ::__gnu_cxx::hash_multimap

#else
  // default implementation path
  #define nCoreHashMapNamespace  std
  #define nCoreHashMap           ::std::hash_map
  #define nCoreHashMultiMap      ::std::hash_multimap

#endif


//}
#endif // #ifndef __nCore_HashMap_Header__
