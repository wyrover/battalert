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
inline ThreadMutexGuard::ThreadMutexGuard (ThreadMutex& mutex, bool bAcquireNow/*=true*/)
: m_Mutex(mutex)
, m_bAcquired(false)
{
  if(bAcquireNow)
    this->Acquire();
}

//---------------------------------------------------------------------------
inline ThreadMutexGuard::~ThreadMutexGuard (void)
{
  if(m_bAcquired)
    this->Release();
}

//---------------------------------------------------------------------------
inline void ThreadMutexGuard::Acquire (void)
{
  XASSERT(m_bAcquired == false);

  if(!m_bAcquired)
  {
    ThreadMutex::State eThreadState = m_Mutex.Acquire();
    if(eThreadState == ThreadMutex::MUTEX_OK)
    {
      m_bAcquired = true;
    }
    else if(eThreadState == ThreadMutex::MUTEX_ALREADYLOCKED)
    {
      XASSERT(0);
      m_bAcquired = true;
    }
    else
    {
      XASSERT(0);
      XTHROWEXCODE(EXCODE_MUTEX_NOTACQUIRED);
    }
  }
}

//---------------------------------------------------------------------------
inline void ThreadMutexGuard::Release (void)
{
  XASSERT(m_bAcquired == true);

  if(m_bAcquired)
  {
#ifdef NCORE_DEBUG
    ThreadMutex::State eThreadState = m_Mutex.Release();
    XASSERT(eThreadState == ThreadMutex::MUTEX_OK);
#else
    m_Mutex.Release();
#endif

    m_bAcquired = false;
  }
}

//---------------------------------------------------------------------------
inline bool ThreadMutexGuard::IsAcquired (void) const
{
  return m_bAcquired;
}
