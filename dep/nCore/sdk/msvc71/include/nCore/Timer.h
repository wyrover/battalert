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

#ifndef __nCore_Timer_Header__
#define __nCore_Timer_Header__
namespace nCore {


//---------------------------------------------------------------------------
// Timer
//
// Performance Timer class.
//---------------------------------------------------------------------------
class NCORE_EXPORT Timer
{
public :
  Timer (void);
  ~Timer (void);

  // reset
  // * Reset() will setup the "start" time so every subsequent call to one
  //   of the Get*() methods will return time passed from the "start" time
  //   offset.
  // * note that the Timer() constructor calls it for you so you'll want to
  //   call it only if you want to update the "start" time offset.
  void Reset (void);

  // get elapsed time since ctor or last reset
  float  GetSecondsFloat   (void) const;
  double GetSecondsDouble  (void) const;
  uint32 GetMilliseconds32 (void) const;
  uint64 GetMilliseconds64 (void) const;
  uint64 GetMicroseconds   (void) const;


private :
#if defined(NCORE_PLATFORM_WINDOWS)
  void GetPerformanceCount             (LARGE_INTEGER& nOutPerfCount, uint32& uiOutTicksElapsed) const;
  void GetCorrectPerformanceValue      (LARGE_INTEGER& nPerfCountElapsed) const;
  void GetCorrectPerformanceValueMilli (uint32& uiPerfCountElapsedMilli) const;
#endif


private :
#if defined(NCORE_PLATFORM_WINDOWS)
          LARGE_INTEGER m_nFrequency;
  mutable LARGE_INTEGER m_nStart;
          DWORD         m_dwStartTicks;
          DWORD_PTR     m_dwTimerAffinityMask;
  mutable LONGLONG      m_nLastPerfCountElapsed;
#elif defined(NCORE_PLATFORM_LINUX)
  struct timeval m_tvStart;
#endif
};


}
#endif // #ifndef __nCore_Timer_Header__
