//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AABox.hpp Axis aligned box
//
#pragma once

// includes
#include "Vector3d.hpp"

/// \brief axis aligned box
/// \details can be used as a bounding box
class AABox
{
public:
   /// ctor
   AABox()
   {
      m_adMin[0] = m_adMin[1] = m_adMin[2] = +std::numeric_limits<double>::infinity();
      m_adMax[0] = m_adMax[1] = m_adMax[2] = -std::numeric_limits<double>::infinity();
   }

   /// update axis aligned box when used as a bounding box
   void UpdateBound(const Vector3d& vPoint) throw()
   {
      m_adMin[0] = std::min(m_adMin[0], vPoint.X());
      m_adMin[1] = std::min(m_adMin[1], vPoint.Y());
      m_adMin[2] = std::min(m_adMin[2], vPoint.Z());

      m_adMax[0] = std::max(m_adMax[0], vPoint.X());
      m_adMax[1] = std::max(m_adMax[1], vPoint.Y());
      m_adMax[2] = std::max(m_adMax[2], vPoint.Z());
   }

   /// returns min vector
   Vector3d Min() const throw() { return Vector3d(m_adMin); }

   /// returns max vector
   Vector3d Max() const throw() { return Vector3d(m_adMax); }

private:
   /// min values
   double m_adMin[3];

   /// max values
   double m_adMax[3];
};
