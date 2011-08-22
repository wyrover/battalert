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

#ifndef __nCore_StlHelpers_Header__
#define __nCore_StlHelpers_Header__
namespace nCore {


//---------------------------------------------------------------------------
// VectorForeignIterator
//
// * A "foreign" iterator allows you to iterate through a given STL container
//   without the need to have any access to this container.
// * Note that by using this "foreign iterator", you are still dependent of
//   any changes made to the underlying container while iterating on it
//   (inserts, removes, etc...), since there are big chances that any
//   modification will unvalidate you iterator.
// * This also means instances of foreign iterator *must not* be stored, thus
//   they must be used at a given time only.
//---------------------------------------------------------------------------
template <class T>
class VectorForeignIterator
{
public :
  VectorForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd);
  explicit VectorForeignIterator (T& vec);

  bool HasNext (void) const;

  typename T::value_type GetNext         (void);
  typename T::value_type PeekNext        (void);
  typename T::pointer    PeekNextPointer (void);
  void                   SeekNext        (void);

  VectorForeignIterator<T>& operator= (VectorForeignIterator<T>& rhs);

private :
  VectorForeignIterator (void) { XASSERT(0); }

private  :
  typename T::iterator m_itCurrent;
  typename T::iterator m_itEnd;
};


//---------------------------------------------------------------------------
// VectorForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
class VectorForeignConstIterator
{
public :
  VectorForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd);
  explicit VectorForeignConstIterator (const T& vec);

  bool HasNext (void) const;

  typename T::value_type    GetNext         (void);
  typename T::value_type    PeekNext        (void) const;
  typename T::const_pointer PeekNextPointer (void) const;
  void                      SeekNext        (void) const;

  VectorForeignConstIterator<T>& operator= (VectorForeignConstIterator<T>& rhs);

private :
  VectorForeignConstIterator (void) { XASSERT(0); }

private  :
  typename T::const_iterator m_itCurrent;
  typename T::const_iterator m_itEnd;
};


//---------------------------------------------------------------------------
// MapForeignIterator
//---------------------------------------------------------------------------
template <class T>
class MapForeignIterator
{
public :
  MapForeignIterator (typename T::iterator itBegin, typename T::iterator itEnd);
  explicit MapForeignIterator (T& m);

  bool HasNext (void) const;

  typename T::key_type    GetNextKey   (void);
  typename T::mapped_type GetNextValue (void);

  typename T::key_type     PeekNextKey          (void);
  typename T::mapped_type  PeekNextValue        (void);
  typename T::mapped_type* PeekNextValuePointer (void);

  void SeekNext (void);

  MapForeignIterator<T>& operator= (MapForeignIterator<T>& rhs);

private :
  MapForeignIterator (void) { XASSERT(0); }

private  :
  typename T::iterator m_itCurrent;
  typename T::iterator m_itEnd;
};


//---------------------------------------------------------------------------
// MapForeignConstIterator
//---------------------------------------------------------------------------
template <class T>
class MapForeignConstIterator
{
public :
  MapForeignConstIterator (typename T::const_iterator itBegin, typename T::const_iterator itEnd);
  explicit MapForeignConstIterator (const T& m);

  bool HasNext (void) const;

  typename T::key_type    GetNextKey   (void);
  typename T::mapped_type GetNextValue (void);

  typename T::key_type           PeekNextKey          (void) const;
  typename T::mapped_type        PeekNextValue        (void) const;
  const typename T::mapped_type* PeekNextValuePointer (void) const;

  void SeekNext (void) const;

  MapForeignConstIterator<T>& operator= (MapForeignConstIterator<T>& rhs);

private :
  MapForeignConstIterator (void) { XASSERT(0); }

private  :
  typename T::const_iterator m_itCurrent;
  typename T::const_iterator m_itEnd;
};


#include "StlHelpers.inl.h"


}
#endif // #ifndef __nCore_StlHelpers_Header__
