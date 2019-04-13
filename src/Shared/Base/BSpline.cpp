//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BSpline.cpp B-Spline classes
//

// includes
#include "StdAfx.h"
#include "BSpline.hpp"

//
// BSpline
//

BSpline::BSpline(unsigned int uiOrder)
:m_uiOrder(uiOrder),
 m_vecPoints(0),
 m_uiNumControlPoints(0)
{
   ATLASSERT(uiOrder >= 2);
}

void BSpline::Generate(const std::vector<float>& vecPoints)
{
   m_uiNumControlPoints = vecPoints.size();
   size_t nplusc = m_uiNumControlPoints + m_uiOrder;

   // allocate memory for vectors
   m_vecKnot.resize(nplusc);
   m_vecBase.resize(m_uiNumControlPoints);
   m_vecTemp.resize(nplusc);

   // copy control point array
   m_vecPoints.assign(vecPoints.begin(), vecPoints.end());

   // calculate knot vector
   m_vecKnot[0] = 0;
   for (size_t k=1; k<nplusc; k++)
   {
      if (k > static_cast<size_t>(m_uiOrder-2) && k < m_uiNumControlPoints+1)
         m_vecKnot[k] = m_vecKnot[k-1]+1;
      else
         m_vecKnot[k] = m_vecKnot[k-1];
   }
}

float BSpline::Get(float at) const
{
   ATLASSERT(m_uiNumControlPoints > 0);

   // generate base function for the parameter
   const_cast<BSpline&>(*this).CalcBase(at);

   // sum up all products
   float val = 0.f;
   for (size_t m=0; m < m_uiNumControlPoints; m++)
      val += m_vecBase[m] * m_vecPoints[m];

   return val;
}

/// calculates bspline base vector
void BSpline::CalcBase(float param)
{
   ATLASSERT(m_uiNumControlPoints > 0);

   size_t nplusc = m_uiNumControlPoints + m_uiOrder;

   // scale param
   param *= m_vecKnot[nplusc-1];

   // calculate first order nonrational base functions n[i]
   for (size_t i=0; i<nplusc; i++)
      m_vecTemp[i] = (param >= m_vecKnot[i] && param < m_vecKnot[i+1]) ? 1.f : 0.f;

   // calculate higher order nonrational base functions
   for (int k=2; k<=m_uiOrder; k++)
   {
      for (size_t j=0; j<nplusc-k; j++)
      {
         float d=0.f, e=0.f;

         // if the lower order base function(s) are zero skip the calculation
         if (m_vecTemp[j] != 0.f)
            d = ((param - m_vecKnot[j]) * m_vecTemp[j]) / (m_vecKnot[j+k-1] - m_vecKnot[j]);

         if (m_vecTemp[j+1] != 0.f)
            e = ((m_vecKnot[j+k] - param) * m_vecTemp[j+1]) / (m_vecKnot[j+k] - m_vecKnot[j+1]);

         m_vecTemp[j] = d+e;
      }
   }

   // pick up last point
   if (param >= (float)m_vecKnot[nplusc-1])
      m_vecTemp[m_uiNumControlPoints-1] = 1.f;

   // calculate sum for denominator of rational base functions
   float sum = 0.f;
   for (size_t m=0; m<m_uiNumControlPoints; m++)
      sum += m_vecTemp[m];

   // form rational base functions and put in r vector
   for (size_t p=0; p<m_uiNumControlPoints; p++)
      m_vecBase[p] = (sum != 0.f) ? (m_vecTemp[p] / sum) : 0.f;
}


//
// BSpline3D
//

/// generates the 3D B-spline
void BSpline3D::Generate(const std::vector<Vector3d>& vecPoints)
{
   std::vector<float> vecData(vecPoints.size(), 0.0f);

   BSpline* m_bspline[3] = { &m_bx, &m_by, &m_bz };

   for (unsigned int i=0; i<3; i++)
   {
      for (size_t s=0, sMax=vecPoints.size(); s<sMax; s++)
         vecData[s] = static_cast<float>(vecPoints[s].Data()[i]);

      m_bspline[i]->Generate(vecData);
   }
}
