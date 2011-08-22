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

#ifndef __nCore_SmartPointer_Header__
#define __nCore_SmartPointer_Header__
namespace nCore {


//---------------------------------------------------------------------------
// SmartPointer
//---------------------------------------------------------------------------
template <class T>
class SmartPointer
{
public :
  inline SmartPointer (void);
	inline SmartPointer (const SmartPointer<T>& smartptr);
  inline SmartPointer (T* pObject);
  inline ~SmartPointer (void);

  inline T* GetSmartPointerObject (void) const;

  // implicit conversion
  inline          operator T* (void);
  inline          operator const T* (void) const;
  inline       T& operator * (void);
  inline const T& operator * (void) const;
  inline       T* operator -> (void);
  inline const T* operator -> (void) const;

  // assignment operators
  inline SmartPointer<T>& operator = (T* pObject);
  inline SmartPointer<T>& operator = (const SmartPointer<T>& smartptr);

  // comparison operators
  inline operator const bool (void) const;
  inline bool operator !  (void) const;
  inline bool operator == (const T* pObject) const;
  inline bool operator != (const T* pObject) const;
  inline bool operator <  (const T* pObject) const;
  inline bool operator == (const SmartPointer<T>& smartptr) const;
  inline bool operator != (const SmartPointer<T>& smartptr) const;
  inline bool operator <  (const SmartPointer<T>& smartptr) const;

private :
  T* m_pSmartObject;
};


//---------------------------------------------------------------------------
// SmartPointerObject
//---------------------------------------------------------------------------
class SmartPointerObject
{
protected :
  inline explicit SmartPointerObject (uint uiInitialRefCount=1);
  inline virtual ~SmartPointerObject (void);

public :
  inline virtual void AddRef  (void);
  inline virtual void Release (void); // returns the new RefCount() after Release()

  inline uint RefCount (void) const;


private :
#ifdef NCORE_ENABLE_MULTITHREADING
  AtomicCounter* m_pAtomicRefCounter;
#else
  uint           m_uiRefCounter;
#endif
};


//---------------------------------------------------------------------------
// Inline methods
//---------------------------------------------------------------------------
#include "SmartPointer.inl.h"


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
// used to declare a SmartPointer type
#define nCoreDeclareSmartPointerType(myclass)  typedef ::nCore::SmartPointer<myclass> myclass##Ptr


}
#endif // #ifndef __nCore_SmartPointer_Header__
