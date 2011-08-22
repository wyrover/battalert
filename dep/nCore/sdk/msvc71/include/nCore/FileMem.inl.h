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
inline bool FileMem::IsOpen (void) const
{
  return (m_uiBufferSize != 0) || (m_uiGrowBytes != 0);
}

//---------------------------------------------------------------------------
inline bool FileMem::IsGrowable (void) const
{
  return this->IsWritable() && (m_uiGrowBytes > 0);
}

//---------------------------------------------------------------------------
inline uint64 FileMem::GetPosition (void) const
{
  XASSERT(m_pBuffer);
  return m_uiPosition;
}

//---------------------------------------------------------------------------
inline uint64 FileMem::GetLength (void) const
{
  return m_uiFileSize;
}

//---------------------------------------------------------------------------
inline const byte* FileMem::GetDirectBuffer (void) const
{
  return m_pBuffer;
}

//---------------------------------------------------------------------------
inline File::BufferAccessType FileMem::GetBufferAccessType (void) const
{
  return BUFFERACCESSTYPE_DIRECT;
}

//---------------------------------------------------------------------------
//inline void FileMem::UnmapFromMemory (void)
//{
//  // nothing to do...
//}
