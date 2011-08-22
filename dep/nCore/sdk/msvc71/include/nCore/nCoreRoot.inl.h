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
inline bool nCoreRoot::IsInit (void)
{
  return g_pCore() != NULL;
}

//---------------------------------------------------------------------------
inline void nCoreRoot::BindExternalLogger (Logger* pUserLogger)
{
  m_pUserLogger = pUserLogger;
}

//---------------------------------------------------------------------------
inline Logger* nCoreRoot::GetExternalLogger (void)
{
  return m_pUserLogger;
}

//---------------------------------------------------------------------------
inline const StringA& nCoreRoot::GetApplicationPath (void) const
{
  return m_strAppPath;
}

//---------------------------------------------------------------------------
inline const StringA& nCoreRoot::GetApplicationDirectory (void) const
{
  return m_strAppDir;
}

//---------------------------------------------------------------------------
inline const StringA& nCoreRoot::GetApplicationFileName (bool bWithExtension/*=true*/) const
{
  return bWithExtension ? m_strAppFileName : m_strAppFileTitle;
}

//---------------------------------------------------------------------------
template<> inline float nCoreRoot::RandomUnit<float> (void)
{
  NCORE_COMPILE_TIME_ASSERT(sizeof(float) == sizeof(uint32));
  static const float c_rMax = float(std::numeric_limits<uint32>::max());
  return float(nCoreRoot::Random32()) / c_rMax;
}
template<> inline double nCoreRoot::RandomUnit<double> (void)
{
  NCORE_COMPILE_TIME_ASSERT(sizeof(double) == sizeof(uint64));
  static const double c_rMax = double(std::numeric_limits<uint64>::max());
  return double(nCoreRoot::Random64()) / c_rMax;
}
template <class RealT>
inline RealT nCoreRoot::RandomUnit (void)
{
  return RealT(nCoreRoot::Random64()) / RealT(std::numeric_limits<uint64>::max());
}

//---------------------------------------------------------------------------
template <class RealT>
inline RealT nCoreRoot::RandomSymmetric (void)
{
  return RealT(2.0) * nCoreRoot::RandomUnit<RealT>() - RealT(1.0);
}

//---------------------------------------------------------------------------
template <class RealT>
inline RealT nCoreRoot::RandomRange (RealT rMin, RealT rMax)
{
  return (rMax - rMin) * nCoreRoot::RandomUnit<RealT>() + rMin;
}
