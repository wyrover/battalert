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

#ifndef __nCore_DogTagsMacros_Header__
#define __nCore_DogTagsMacros_Header__
//namespace nCore {


//---------------------------------------------------------------------------
// Constants
//
// You do not need to use those constants directly, use the macros defined
// later instead !
//---------------------------------------------------------------------------
#define NCORE_DOGTAG_PREFIX  "#!@NCT->"
#define NCORE_DOGTAG_SUFFIX  "<-NCT@!#"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#define NCORE_DOGTAG(name,strvalue) \
  static const char  __szCoreDogTag_##name [] = NCORE_DOGTAG_PREFIX #name "=" strvalue NCORE_DOGTAG_SUFFIX; \
  static const void* __szCoreDogTagRef_##name = (const void*)&__szCoreDogTag_##name;


//}
#endif // #ifndef __nCore_DogTagsMacros_Header__
