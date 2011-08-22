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


//***************************************************************************
//***************************************************************************
//
// SmartPointer
//
//***************************************************************************
//***************************************************************************
//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (void)
{
  m_pSmartObject = NULL;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (const SmartPointer<T>& smartptr)
{
  T* pObj = smartptr.GetSmartPointerObject();

  if(pObj)
    pObj->AddRef();

  m_pSmartObject = pObj;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::SmartPointer (T* pObject)
{
  if(pObject)
    pObject->AddRef();
  m_pSmartObject = pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::~SmartPointer (void)
{
  if(m_pSmartObject)
    m_pSmartObject->Release();
}

//---------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::GetSmartPointerObject (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator T* (void)
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator const T* (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline T& SmartPointer<T>::operator * (void)
{
  return *m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T& SmartPointer<T>::operator * (void) const
{
  return *m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline T* SmartPointer<T>::operator -> (void)
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline const T* SmartPointer<T>::operator -> (void) const
{
  return m_pSmartObject;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>& SmartPointer<T>::operator = (T* pObject)
{
  if(m_pSmartObject != pObject)
  {
    if(m_pSmartObject)
      m_pSmartObject->Release();

    m_pSmartObject = pObject;

    if(m_pSmartObject)
      m_pSmartObject->AddRef();
  }

  return *this;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>& SmartPointer<T>::operator = (const SmartPointer<T>& smartptr)
{
  T* pObject = smartptr.GetSmartPointerObject();

  if(m_pSmartObject != pObject)
  {
    if(m_pSmartObject)
      m_pSmartObject->Release();

    m_pSmartObject = pObject;

    if(m_pSmartObject)
      m_pSmartObject->AddRef();
  }

  return *this;
}

//---------------------------------------------------------------------------
template <class T>
inline SmartPointer<T>::operator const bool (void) const
{
  return m_pSmartObject != 0;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator ! (void) const
{
  return m_pSmartObject == 0;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator == (const T* pObject) const
{
  return m_pSmartObject == pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator == (const SmartPointer<T>& smartptr) const
{
  return m_pSmartObject == smartptr.GetSmartPointerObject();
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator != (const T* pObject) const
{
  return m_pSmartObject != pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator != (const SmartPointer<T>& smartptr) const
{
  return m_pSmartObject != smartptr.GetSmartPointerObject();
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator < (const T* pObject) const
{
  return m_pSmartObject < pObject;
}

//---------------------------------------------------------------------------
template <class T>
inline bool SmartPointer<T>::operator < (const SmartPointer<T>& smartptr) const
{
  return m_pSmartObject < smartptr.GetSmartPointerObject();
}




//***************************************************************************
//***************************************************************************
//
// SmartPointerObject
//
//***************************************************************************
//***************************************************************************
//---------------------------------------------------------------------------
inline SmartPointerObject::SmartPointerObject (uint uiInitialRefCount/*=1*/)
#ifndef NCORE_ENABLE_MULTITHREADING
: m_uiRefCounter(uiInitialRefCount)
#endif
{
#ifdef NCORE_ENABLE_MULTITHREADING
  XASSERT(uiInitialRefCount < (uint)std::numeric_limits<int>::max());
  m_pAtomicRefCounter = AtomicCounter::Create((int)uiInitialRefCount);
  if(!m_pAtomicRefCounter)
    XTHROWEXCODE(EXCODE_MEMORY);
#endif
}

//---------------------------------------------------------------------------
inline SmartPointerObject::~SmartPointerObject (void)
{
#ifdef NCORE_ENABLE_MULTITHREADING
  XASSERT(m_pAtomicRefCounter->GetValue() == 0);
  delete m_pAtomicRefCounter;
#else
  XASSERT(m_uiRefCounter == 0);
#endif
}

//---------------------------------------------------------------------------
inline void SmartPointerObject::AddRef (void)
{
#ifdef NCORE_ENABLE_MULTITHREADING
  XASSERT(m_pAtomicRefCounter->GetValue() < std::numeric_limits<int>::max());
  ++(*m_pAtomicRefCounter);
#else
  XASSERT(m_uiRefCounter < std::numeric_limits<uint>::max());
  ++m_uiRefCounter;
#endif
}

//---------------------------------------------------------------------------
inline void SmartPointerObject::Release (void)
{
#ifdef NCORE_ENABLE_MULTITHREADING
  XASSERT(m_pAtomicRefCounter->GetValue() > 0);
  if(m_pAtomicRefCounter->GetValue() > 0)
  {
    if(--(*m_pAtomicRefCounter) == 0)
      delete this;
  }

#else
  XASSERT(m_uiRefCounter > 0);
  if(m_uiRefCounter > 0)
  {
    if(--m_uiRefCounter == 0)
      delete this;
  }

#endif
}

//---------------------------------------------------------------------------
inline uint SmartPointerObject::RefCount (void) const
{
#ifdef NCORE_ENABLE_MULTITHREADING
  XASSERT(m_pAtomicRefCounter->GetValue() > 0);
  return (uint)m_pAtomicRefCounter->GetValue();
#else
  XASSERT(m_uiRefCounter > 0);
  return m_uiRefCounter;
#endif
}
