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

#ifndef __nCore_MemoryManager_Header__
#define __nCore_MemoryManager_Header__
#if defined(NCORE_DEBUG) && defined(NCORE_ENABLE_INTERNAL_MEMORY_MANAGER)


//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------
// allocation types
namespace nCore
{
  enum MemAllocType
  {
    m_alloc_unknown      = 0,
    m_alloc_new          = 1,
    m_alloc_new_array    = 2,
    m_alloc_malloc       = 3,
    m_alloc_calloc       = 4,
    m_alloc_realloc      = 5,
    m_alloc_delete       = 6,
    m_alloc_delete_array = 7,
    m_alloc_free         = 8
  };
}


//---------------------------------------------------------------------------
// Variations of global new and delete operators
//---------------------------------------------------------------------------
void* operator new      (size_t nReportedSize);
void* operator new[]    (size_t nReportedSize);
void  operator delete   (void* pReportedAddress);
void  operator delete[] (void* pReportedAddress);
void* operator new      (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);
void* operator new[]    (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);
void  operator delete   (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);
void  operator delete[] (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);


//---------------------------------------------------------------------------
// MemoryManager
//---------------------------------------------------------------------------
namespace nCore {
class NCORE_EXPORT MemoryManager
{
public :
  MemoryManager (void);
  ~MemoryManager (void);

  // malloc / calloc / realloc / free
  void* MemAlloc   (const char* sourceFile, const unsigned int sourceLine, const char* sourceFunc, const unsigned int allocationType, const size_t reportedSize, unsigned int uiProcessID);
  void* MemReAlloc (const char* sourceFile, const unsigned int sourceLine, const char* sourceFunc, const unsigned int reallocationType, const size_t reportedSize, void* reportedAddress, unsigned int uiProcessID);
  void  MemDeAlloc (const char* sourceFile, const unsigned int sourceLine, const char* sourceFunc, const unsigned int deallocationType, const void* reportedAddress, unsigned int uiProcessID);

  // tools
  //   Simply call this routine with the address of an allocated block of RAM,
  //   to cause it to force a breakpoint when it is reallocated.
  bool& BreakOnRealloc (void* reportedAddress);
  //   Simply call this routine with the address of an allocated block of RAM,
  //   to cause it to force a breakpoint when it is deallocated.
  bool& BreakOnDealloc (void* reportedAddress);
  //   When tracking down a difficult bug, use this routine to force a
  //   breakpoint on a specific allocation count.
  void BreakOnAllocation (unsigned int count);

  bool ValidateAddress       (const void* pReportedAddress);
  bool ValidateAllAllocUnits (void);

  // unused ram calculations
  unsigned int CalcAllUnused (void);

  // logging and reporting
  void DumpLeakReport   (void);
  void DumpMemoryReport (const bool bOverwrite=true);


public : // protected :
  // wrapper for standard new/delete
  void SetOwner (const char* pszFile, const size_t nLine, const char* pszFunc);


protected :
  friend void* ::operator new      (size_t nReportedSize);
  friend void* ::operator new[]    (size_t nReportedSize);
  friend void  ::operator delete   (void* pReportedAddress);
  friend void  ::operator delete[] (void* pReportedAddress);
  friend void* ::operator new      (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);
  friend void* ::operator new[]    (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* sourpszSourceFuncceFunc);
  friend void  ::operator delete   (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);
  friend void  ::operator delete[] (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc);

  // standard new/delete
  void* OpNew         (size_t nReportedSize, unsigned int uiProcessID);
  void* OpNewArray    (size_t nReportedSize, unsigned int uiProcessID);
  void  OpDelete      (void* pReportedAddress, unsigned int uiProcessID);
  void  OpDeleteArray (void* pReportedAddress, unsigned int uiProcessID);

  // placement new/delete
  void* OpNew         (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc, unsigned int uiProcessID);
  void* OpNewArray    (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc, unsigned int uiProcessID);
  void  OpDelete      (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc, unsigned int uiProcessID);
  void  OpDeleteArray (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc, unsigned int uiProcessID);

  // create a new process id for every modules linked to this library
  unsigned int GetProcessID (void);


private :
  unsigned int m_uiProcessID;


public :
  // the memory manager object itself
  static MemoryManager ms_MemoryManager;
};
} // namespace nCore


//---------------------------------------------------------------------------
// MemoryManager ProcessID
//
// This variable exists separately in each module that links to this library
// and is used to track the ID of the current process from the perspective of
// the memory manager.
//---------------------------------------------------------------------------
static unsigned int g_uiCoreMemoryManagerProcessID;


//---------------------------------------------------------------------------
// Overridden global standard new and delete operators
//---------------------------------------------------------------------------
// go to unmanaged c++ in msvc 8.0+
#if defined(__cplusplus_cli)
#pragma managed(push, off)
#endif

inline void* operator new (size_t nReportedSize)
{
  if(!g_uiCoreMemoryManagerProcessID)
    g_uiCoreMemoryManagerProcessID = nCore::MemoryManager::ms_MemoryManager.GetProcessID();
  return nCore::MemoryManager::ms_MemoryManager.OpNew(nReportedSize, g_uiCoreMemoryManagerProcessID);
}

inline void* operator new[] (size_t nReportedSize)
{
  if(!g_uiCoreMemoryManagerProcessID)
    g_uiCoreMemoryManagerProcessID = nCore::MemoryManager::ms_MemoryManager.GetProcessID();
  return nCore::MemoryManager::ms_MemoryManager.OpNewArray(nReportedSize, g_uiCoreMemoryManagerProcessID);
}

inline void operator delete (void* pReportedAddress)
{
  nCore::MemoryManager::ms_MemoryManager.OpDelete(pReportedAddress, g_uiCoreMemoryManagerProcessID);
}

inline void operator delete[] (void* pReportedAddress)
{
  nCore::MemoryManager::ms_MemoryManager.OpDeleteArray(pReportedAddress, g_uiCoreMemoryManagerProcessID);
}

#if defined(__cplusplus_cli)
#pragma managed(push, on)
#endif


//---------------------------------------------------------------------------
// Overridden global placement new & delete operators
//---------------------------------------------------------------------------
// go to unmanaged c++ in msvc 8.0+
#if defined(__cplusplus_cli)
#pragma managed(push, off)
#endif

inline void* operator new (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc)
{
  if(!g_uiCoreMemoryManagerProcessID)
    g_uiCoreMemoryManagerProcessID = nCore::MemoryManager::ms_MemoryManager.GetProcessID();
  return nCore::MemoryManager::ms_MemoryManager.OpNew(nReportedSize, pszSourceFile, nSourceLine, pszSourceFunc, g_uiCoreMemoryManagerProcessID);
}

inline void* operator new[] (size_t nReportedSize, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc)
{
  if(!g_uiCoreMemoryManagerProcessID)
    g_uiCoreMemoryManagerProcessID = nCore::MemoryManager::ms_MemoryManager.GetProcessID();
  return nCore::MemoryManager::ms_MemoryManager.OpNewArray(nReportedSize, pszSourceFile, nSourceLine, pszSourceFunc, g_uiCoreMemoryManagerProcessID);
}

inline void operator delete (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc)
{
  nCore::MemoryManager::ms_MemoryManager.OpDelete(pReportedAddress, pszSourceFile, nSourceLine, pszSourceFunc, g_uiCoreMemoryManagerProcessID);
}

inline void operator delete[] (void* pReportedAddress, const char* pszSourceFile, int nSourceLine, const char* pszSourceFunc)
{
  nCore::MemoryManager::ms_MemoryManager.OpDeleteArray(pReportedAddress, pszSourceFile, nSourceLine, pszSourceFunc, g_uiCoreMemoryManagerProcessID);
}

#if defined(__cplusplus_cli)
#pragma managed(push, on)
#endif




#endif // #if defined(NCORE_DEBUG) && defined(NCORE_ENABLE_INTERNAL_MEMORY_MANAGER)


#include "MemoryManagerMacros.h"


#endif // #ifndef __nCore_MemoryManager_Header__
