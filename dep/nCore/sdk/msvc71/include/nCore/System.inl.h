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
inline System::Version System::GetVersion (void)
{
  static const Version eVersion = System::DetectVersion();
  return eVersion;
}

//---------------------------------------------------------------------------
inline bool System::IsWindowsNT (void)
{
  static const bool bIsWindows = (dword(System::GetVersion()) & dword(OSTYPE__MASK)) == OSTYPE_WINDOWS_NT;
  return bIsWindows;
}

//---------------------------------------------------------------------------
inline StringA System::GetDateTimeString (bool bFraction/*=false*/)
{
  StringA strDateTime;
  System::AppendDateTimeString(strDateTime, bFraction);
  return strDateTime;
}

//---------------------------------------------------------------------------
inline StringA System::GetDateString (void)
{
  StringA strDate;
  System::AppendDateString(strDate);
  return strDate;
}

//---------------------------------------------------------------------------
inline StringA System::GetTimeString (bool bFraction/*=false*/)
{
  StringA strTime;
  System::AppendDateTimeString(strTime, bFraction);
  return strTime;
}

//---------------------------------------------------------------------------
inline bool System::IsBigEndian (void)
{
  if(ms_nIsBigEndian < 0)
    (void)System::DetectEndianness();
  return (ms_nIsBigEndian == 0) ? false : true;
}

//---------------------------------------------------------------------------
inline void System::SwapBytes (uint16 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint16)]; } bytes;

  XASSERT(sizeof(uint16) == 2);

  *(uint16*)&bytes = valSource;

  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::SwapBytes (uint32 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint32)]; } bytes;

  XASSERT(sizeof(uint32) == 4);

  *(uint32*)&bytes = valSource;

  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::SwapBytes (uint64 valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(uint64)]; } bytes;

  XASSERT(sizeof(uint64) == 8);

  *(uint64*)&bytes = valSource;

  *pDestBytes++ = bytes._[7];
  *pDestBytes++ = bytes._[6];
  *pDestBytes++ = bytes._[5];
  *pDestBytes++ = bytes._[4];
  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::SwapBytes (float valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(float)]; } bytes;

  XASSERT(sizeof(float) == 4);

  *(float*)&bytes = valSource;

  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}

//---------------------------------------------------------------------------
inline void System::SwapBytes (double valSource, byte* pDestBytes)
{
  struct { byte _[sizeof(double)]; } bytes;

  XASSERT(sizeof(double) == 8);

  *(double*)&bytes = valSource;

  *pDestBytes++ = bytes._[7];
  *pDestBytes++ = bytes._[6];
  *pDestBytes++ = bytes._[5];
  *pDestBytes++ = bytes._[4];
  *pDestBytes++ = bytes._[3];
  *pDestBytes++ = bytes._[2];
  *pDestBytes++ = bytes._[1];
  *pDestBytes   = bytes._[0];
}
