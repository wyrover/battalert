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
inline File* Streamer::GetFile (void)
{
  return m_pFile;
}

//---------------------------------------------------------------------------
inline dword Streamer::GetStreamMode (void) const
{
  return m_dwStreamMode;
}

//---------------------------------------------------------------------------
inline bool Streamer::IsLoading (void) const
{
  return (m_dwStreamMode & STREAM_STORE) == 0;
}

//---------------------------------------------------------------------------
inline bool Streamer::IsStoring (void) const
{
  return (m_dwStreamMode & STREAM_STORE) != 0;
}

//---------------------------------------------------------------------------
inline bool Streamer::IsLittleEndian (void) const
{
  return (m_dwStreamMode & STREAM_BIGENDIAN) == 0;
}

//---------------------------------------------------------------------------
inline bool Streamer::IsNetworkEndian (void) const
{
  return (m_dwStreamMode & STREAM_NETWORKENDIAN) != 0;
}

//---------------------------------------------------------------------------
inline bool Streamer::IsTargetReadable (void) const
{
  return (m_pFile != NULL) && m_pFile->IsReadable();
}

//---------------------------------------------------------------------------
inline bool Streamer::IsTargetWritable (void) const
{
  return (m_pFile != NULL) && m_pFile->IsWritable();
}



//---------------------------------------------------------------------------
inline Streamer& Streamer::operator>> (int8& val)
{
  return Streamer::operator>>((uint8&)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator>> (int16& val)
{
  return Streamer::operator>>((uint16&)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator>> (int32& val)
{
  return Streamer::operator>>((uint32&)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator>> (int64& val)
{
  return Streamer::operator>>((uint64&)val);
}



//---------------------------------------------------------------------------
inline Streamer& Streamer::operator<< (int8 val)
{
  return Streamer::operator<<((uint8)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator<< (int16 val)
{
  return Streamer::operator<<((uint16)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator<< (int32 val)
{
  return Streamer::operator<<((uint32)val);
}

//---------------------------------------------------------------------------
inline Streamer& Streamer::operator<< (int64 val)
{
  return Streamer::operator<<((uint64)val);
}
