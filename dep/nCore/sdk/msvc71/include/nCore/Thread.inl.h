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
inline uint Thread::GetID (void) const
{
  return m_uiID;
}

//---------------------------------------------------------------------------
inline bool Thread::IsActive (void) const
{
  return (m_eState == STATE_LAUNCHING) || (m_eState == STATE_RUNNING);
}

//---------------------------------------------------------------------------
inline bool Thread::IsJoined (void) const
{
  return (m_eState == STATE_JOINED);
}

//---------------------------------------------------------------------------
inline bool Thread::IsOvni (void) const
{
  return (m_eState == STATE_OVNI);
}

//---------------------------------------------------------------------------
inline Thread::State Thread::GetState (void) const
{
  return m_eState;
}

//---------------------------------------------------------------------------
inline Thread::Signal Thread::FetchSignal (void)
{
  return this->WaitForSignalImpl(0, true);
}

//---------------------------------------------------------------------------
inline Thread::Signal Thread::PeekSignal (void)
{
  return this->WaitForSignalImpl(0, false);
}

//---------------------------------------------------------------------------
inline bool Thread::IsSignaled (void)
{
  return this->WaitForSignalImpl(0, false) != SIGNAL_NONE;
}

//---------------------------------------------------------------------------
inline bool Thread::JoinForever (bool bSendQuitSignalFirst/*=false*/)
{
  if(!this->SendSignal(SIGNAL_QUIT, false))
    return false;

  return this->Join(uint(-1));
}

//---------------------------------------------------------------------------
inline bool Thread::operator== (const Thread& rhs) const
{
  return m_uiID == rhs.m_uiID;
}

//---------------------------------------------------------------------------
inline bool Thread::operator!= (const Thread& rhs) const
{
  return m_uiID != rhs.m_uiID;
}

//---------------------------------------------------------------------------
inline void Thread::Release (void)
{
  // do not allow user to Release() if thread is still running
  if(this->IsActive() && (this->RefCount() <= 1))
  {
    XASSERT(0 && "You must Join*() this thread before releasing it !");
    return;
  }

  SmartPointerObject::Release();
}

//---------------------------------------------------------------------------
inline Thread::Signal Thread::WaitForSignalForever (bool bFlushSignal/*=true*/)
{
  return Thread::WaitForSignal(uint(-1), bFlushSignal);
}
