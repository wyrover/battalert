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


//---------------------------------------------------------------------------
// VectorForeignIterator
//---------------------------------------------------------------------------
template <class T>
inline VectorForeignIterator<T>::VectorForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline VectorForeignIterator<T>::VectorForeignIterator (T& vec)
: m_itCurrent(vec.begin())
, m_itEnd(vec.end())
{
}

template <class T>
inline bool VectorForeignIterator<T>::HasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::value_type VectorForeignIterator<T>::GetNext (void)
{
  return *m_itCurrent++;
}

template <class T>
inline typename T::value_type VectorForeignIterator<T>::PeekNext (void)
{
  return *m_itCurrent;
}

template <class T>
inline typename T::pointer VectorForeignIterator<T>::PeekNextPointer (void)
{
  return &(*m_itCurrent);
}

template <class T>
inline void VectorForeignIterator<T>::SeekNext (void)
{
  return ++m_itCurrent;
}

template <class T>
inline VectorForeignIterator<T>& VectorForeignIterator<T>::operator= (VectorForeignIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// VectorForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
inline VectorForeignConstIterator<T>::VectorForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline VectorForeignConstIterator<T>::VectorForeignConstIterator (const T& vec)
: m_itCurrent(vec.begin())
, m_itEnd(vec.end())
{
}

template <class T>
inline bool VectorForeignConstIterator<T>::HasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::value_type VectorForeignConstIterator<T>::GetNext (void)
{
  return *m_itCurrent++;
}

template <class T>
inline typename T::value_type VectorForeignConstIterator<T>::PeekNext (void) const
{
  return *m_itCurrent;
}

template <class T>
inline typename T::const_pointer VectorForeignConstIterator<T>::PeekNextPointer (void) const
{
  return &(*m_itCurrent);
}

template <class T>
inline void VectorForeignConstIterator<T>::SeekNext (void) const
{
  return ++m_itCurrent;
}

template <class T>
inline VectorForeignConstIterator<T>& VectorForeignConstIterator<T>::operator= (VectorForeignConstIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// MapForeignIterator
//---------------------------------------------------------------------------
template <class T>
inline MapForeignIterator<T>::MapForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline MapForeignIterator<T>::MapForeignIterator (T& m)
: m_itCurrent(m.begin())
, m_itEnd(m.end())
{
}

template <class T>
inline bool MapForeignIterator<T>::HasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::key_type MapForeignIterator<T>::GetNextKey (void)
{
  return (m_itCurrent++)->first;
}

template <class T>
inline typename T::mapped_type MapForeignIterator<T>::GetNextValue (void)
{
  return (m_itCurrent++)->second;
}

template <class T>
inline typename T::key_type MapForeignIterator<T>::PeekNextKey (void)
{
  return m_itCurrent->first;
}

template <class T>
inline typename T::mapped_type MapForeignIterator<T>::PeekNextValue (void)
{
  return m_itCurrent->second;
}

template <class T>
inline typename T::mapped_type* MapForeignIterator<T>::PeekNextValuePointer (void)
{
  return &(m_itCurrent->second);
}

template <class T>
inline void MapForeignIterator<T>::SeekNext (void)
{
  ++m_itCurrent;
}

template <class T>
inline MapForeignIterator<T>& MapForeignIterator<T>::operator= (MapForeignIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}


//---------------------------------------------------------------------------
// MapForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
inline MapForeignConstIterator<T>::MapForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd)
: m_itCurrent(itBegin)
, m_itEnd(itEnd)
{
}

template <class T>
inline MapForeignConstIterator<T>::MapForeignConstIterator (const T& m)
: m_itCurrent(m.begin())
, m_itEnd(m.end())
{
}

template <class T>
inline bool MapForeignConstIterator<T>::HasNext (void) const
{
  return (m_itCurrent != m_itEnd);
}

template <class T>
inline typename T::key_type MapForeignConstIterator<T>::GetNextKey (void)
{
  return (m_itCurrent++)->first;
}

template <class T>
inline typename T::mapped_type MapForeignConstIterator<T>::GetNextValue (void)
{
  return (m_itCurrent++)->second;
}

template <class T>
inline typename T::key_type MapForeignConstIterator<T>::PeekNextKey (void) const
{
  return m_itCurrent->first;
}

template <class T>
inline typename T::mapped_type MapForeignConstIterator<T>::PeekNextValue (void) const
{
  return m_itCurrent->second;
}

template <class T>
inline const typename T::mapped_type* MapForeignConstIterator<T>::PeekNextValuePointer (void) const
{
  return &(m_itCurrent->second);
}

template <class T>
inline void MapForeignConstIterator<T>::SeekNext (void) const
{
  ++m_itCurrent;
}

template <class T>
inline MapForeignConstIterator<T>& MapForeignConstIterator<T>::operator= (MapForeignConstIterator<T>& rhs)
{
  m_itCurrent = rhs.m_itCurrent;
  m_itEnd     = rhs.m_itEnd;

  return *this;
}
