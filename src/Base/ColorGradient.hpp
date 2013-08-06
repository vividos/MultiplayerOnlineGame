//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ColorGradient.hpp Color gradient class
//
#pragma once

// includes
#include <vector>
#include "Color.hpp"

/// color gradient
class ColorGradient
{
public:
   /// adds another color
   void Add(Color c)
   {
      m_vecColors.push_back(c);
   }

   /// get gradient color based on delta value in range [0.0; 1.0]
   Color Get(double dDelta) const
   {
      ATLASSERT(m_vecColors.empty() == false);

      if (dDelta <= 0.0)
         return m_vecColors.front();
      if (dDelta >= 1.0)
         return m_vecColors.back();

      if (m_vecColors.size() == 1)
         return m_vecColors.front();

      size_t uiMaxColor = m_vecColors.size();
      size_t uiColor = static_cast<size_t>((uiMaxColor - 1) * dDelta);
      ATLASSERT(uiColor <= uiMaxColor - 1);

      dDelta *= double(uiMaxColor-1);
      dDelta -= std::floor(dDelta); // remove integer part

      return Interpolate<Color>(m_vecColors[uiColor], m_vecColors[uiColor+1], dDelta);
   }

private:
   /// colors list
   std::vector<Color> m_vecColors;
};
