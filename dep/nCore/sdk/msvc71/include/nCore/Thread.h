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

#ifndef __nCore_Thread_Header__
#define __nCore_Thread_Header__
#if defined(NCORE_ENABLE_MULTITHREADING)
namespace nCore {


//---------------------------------------------------------------------------
// Thread
//
// * Please use the Thread::Create() static method to create a Thread object.
//   Since Thread class derives from SmartPointerObject, you'll be able to
//   release it by make a pThread->Release() for example or by using the
//   XRELEASE macro. Example : XRELEASE(pThread);
// * Before releasing a thread, ensure you've joined it by calling the Join()
//   or JoinForever() method.
// * To get the object corresponding to the current running thread, use the
//   Thread::GetCurrent() static method.
//   If thread was not created by you (i.e.: it is the main thread or it was
//   not created by this library), you will not need to release it, join it
//   or whatever. Actually, you must *not* perform any operations on it !
// * This kind of threads are called "ovni" threads here.
//---------------------------------------------------------------------------
class NCORE_EXPORT Thread : public SmartPointerObject, private NonCopyable
{
public :
  enum Priority
  {
    PRIORITY_LOW = 1,
    PRIORITY_NORMAL,  // default
    PRIORITY_HIGH,
  };

  enum State
  {
    STATE_OVNI = 1,   // this thread is recognized as a valid *running* thread but it was not created by this library
    STATE_LAUNCHING,  // being initialized and launched
    STATE_RUNNING,    // running
    STATE_JOINED,     // client method returned
  };

  enum Signal
  {
    SIGNAL_NONE,     // can be used to cancel a previous sent signal if it has not been handled yet by the target thread (risky)
    SIGNAL_USER1,    // you can do whatever you want with this signal
    SIGNAL_USER2,    // you can do whatever you want with this signal
    SIGNAL_USER3,    // you can do whatever you want with this signal
    SIGNAL_USER4,    // you can do whatever you want with this signal
    SIGNAL_USER5,    // you can do whatever you want with this signal
    SIGNAL_SLEEP,    // finish the current task and do not accept new incomming tasks until a WAKEUP signal
    SIGNAL_SLEEPNOW, // *interrupt* current task and do not accept new incomming tasks until a WAKEUP signal (of course this is supposed to be a very strong signal than SIGNAL_SLEEP)
    SIGNAL_WAKEUP,   // this signal is usually sent after a SLEEP or SLEEPNOW signal to resume the thread, you may also received this signal if you are trying to join an another thread (i.e.: via a Join*() method for example)
    SIGNAL_QUIT,     // thread must terminate so it can be joined as soon as possible
  };


  typedef std::deque<Thread*>     Threads;
  typedef Threads::iterator       ThreadsIt;
  typedef Threads::const_iterator ThreadsCIt;


public :
  Thread (void);
  virtual ~Thread (void);

  // factory
  static Thread* GetCurrent (void);
  static Thread* Create     (ThreadMethod* pUserMethod, void* pUserParam=NULL, uint uiInitialStackSize=0);


  // state
  virtual uint  GetID      (void) const;
  virtual bool  IsActive   (void) const; // is thread STATE_LAUNCHING or STATE_RUNNING now ?
  virtual bool  IsJoined   (void) const; // is thread is STATE_JOINED ?
  virtual bool  IsOvni     (void) const; // is thread created by using this library (i.e. this Thread interface) ?
  virtual State GetState   (void) const;

  // priority
  virtual bool     SetPriority (Priority ePriority) const = 0;
  virtual Priority GetPriority (void) const = 0;

  // send signal
  // * sending a signal to a distant thread is useful to control its behavior
  //   (suspend, resume, terminate, etc...).
  // * the Signal enum defines some common signal types but the ThreadMethod
  //   implementation does *not* need to handle all of them (actually, this
  //   Thread does not care too much about them so it is up to the
  //   ThreadMethod implementation to know what to do about it).
  // * only SIGNAL_QUIT *has* to be handled properly because it is used
  //   internally by nCore (for example : by the GetState() method).
  // * note that a thread cannot send a signal to itself.
  // * the bJustTry parameter allows you to make this call non-blocking.
  virtual bool SendSignal (Signal eSignal, bool bJustTry) = 0;

  // fetch signal
  // * this method is usually meant to be called from a ThreadMethod
  //   implementation to check if current thread has been signaled.
  // * this call is not blocking, you can use the WaitForSignal() or
  //   WaitForSignalForever() method to block until a signal has been
  //   received.
  // * WARNING : after FetchSignal() returned, the internal "received signal"
  //   slot of the thread will be *reset* ! thus, two subsequent calls to the
  //   FetchSignal() method for the same thread object will probably not
  //   return the same value.
  //   If you really want to keep the signal for further use, call the
  //   PeekSignal() method instead.
  virtual Signal FetchSignal (void);
  virtual Signal PeekSignal  (void);
  virtual bool   IsSignaled  (void);

  // join / wait
  // * joining a thread means waiting for its termination.
  // * this is usually the next step after sending a QUIT signal.
  // * note that you can use the bSendQuitSignalFirst parameter of the
  //   JoinForever() method to both send a QUIT signal and then wait for its
  //   termination in a single call.
  virtual bool Join        (uint uiMilliseconds);
  virtual bool JoinForever (bool bSendQuitSignalFirst=false);  // shortcut to Thread::Join(-1)

  // comparison operators
  bool operator== (const Thread& rhs) const;
  bool operator!= (const Thread& rhs) const;

  // derived from SmartPointerObject
  void Release (void);


  // static utils
  static void   JoinAllThreads       (bool bSendQuitSignalFirst=false);              // join *every* non-ovni running threads
  static Signal WaitForSignal        (uint uiMilliseconds, bool bFlushSignal=true);
  static Signal WaitForSignalForever (bool bFlushSignal=true);                       // alias for Thread::WaitForSignal(-1)
  static Signal Sleep                (uint uiMilliseconds, bool bFlushSignal=false); // enhanced version of Thread::WaitForSignal() : you can decide not to flush the incomming signal in order to retrieve it later from the FetchSignal() method
  static void   Yield                (void);                                         // cause the current thread to yield
  static uint   GetCurrentThreadID   (void);


protected :
  // protected utils
  virtual Signal WaitForSignalImpl (uint uiMilliseconds, bool bFlushSignal) = 0;
  virtual Signal FetchSignalImpl   (void);
  virtual void   RawJoin           (void) = 0;

  // primary entry point
  static void PrimaryEntryPoint    (Thread* pThread, ThreadMethod* pUserMethod, void* pUserParam);
  static void PrimaryEntryPointEnd (Thread* pThread);


protected :
  // properties
  State m_eState;
  uint  m_uiID;

  // common mutex to serialize internal access to members
  ThreadMutex m_Mutex;

  // joiners
  Threads m_Joiners;

  // signal
  Signal m_eSignal;
};


//---------------------------------------------------------------------------
// Inline Methods
//---------------------------------------------------------------------------
#include "Thread.inl.h"


}
#endif // #if defined(NCORE_ENABLE_MULTITHREADING)
#endif // #ifndef __nCore_Thread_Header__
