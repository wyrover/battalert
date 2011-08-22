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

#if defined(NCORE_DEBUG) && defined(NCORE_ENABLE_INTERNAL_MEMORY_MANAGER) && !defined(__nCore_MemoryManagerMacros_Header__)


#define __nCore_MemoryManagerMacros_Header__

//#define new              ( ::nCore::MemoryManager::ms_MemoryManager.SetOwner(__FILE__, __LINE__, NCORE_FUNCTION), false ) ? NULL : new
//#define delete           ( ::nCore::MemoryManager::ms_MemoryManager.SetOwner(__FILE__, __LINE__, NCORE_FUNCTION), false ) ? ::nCore::MemoryManager::ms_MemoryManager.SetOwner("",0,"") : delete
#define new              new(__FILE__, __LINE__, NCORE_FUNCTION)
#define delete           ( ::nCore::MemoryManager::ms_MemoryManager.SetOwner(__FILE__, __LINE__, NCORE_FUNCTION), false ) ? ::nCore::MemoryManager::ms_MemoryManager.SetOwner("",0,"") : delete

#define malloc(sz)       ::nCore::MemoryManager::ms_MemoryManager.MemAlloc  (__FILE__, __LINE__, NCORE_FUNCTION, m_alloc_malloc,  sz,      g_uiCoreMemoryManagerProcessID)
#define calloc(sz)       ::nCore::MemoryManager::ms_MemoryManager.MemAlloc  (__FILE__, __LINE__, NCORE_FUNCTION, m_alloc_calloc,  sz,      g_uiCoreMemoryManagerProcessID)
#define realloc(ptr,sz)  ::nCore::MemoryManager::ms_MemoryManager.MemReAlloc(__FILE__, __LINE__, NCORE_FUNCTION, m_alloc_realloc, sz, ptr, g_uiCoreMemoryManagerProcessID)
#define free(ptr)        ::nCore::MemoryManager::ms_MemoryManager.MemDeAlloc(__FILE__, __LINE__, NCORE_FUNCTION, m_alloc_free,        ptr, g_uiCoreMemoryManagerProcessID)


#endif
