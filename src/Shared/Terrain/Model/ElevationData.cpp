//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ElevationData.cpp Elevation data for terrain
//

// includes
#include "StdAfx.h"
#include "ElevationData.hpp"

using namespace Terrain::Model;

ElevationData::ElevationData(unsigned int uiSize)
:m_vecElevation((uiSize+1)*(uiSize+1), 0.0f),
 m_uiSize(uiSize),
 m_arrayMapper(uiSize+1, uiSize+1)
{
   ATLASSERT(true == IsPowerOfTwo(uiSize));
}

double ElevationData::InterpolatedHeight(double x, double y) const
{
   double p1 = Height(unsigned(x), unsigned(y));
   double p2 = Height(unsigned(x+1), unsigned(y));
   double p3 = Height(unsigned(x), unsigned(y+1));
   double p4 = Height(unsigned(x+1), unsigned(y+1));

   double dx = x - unsigned(x);
   double dy = y - unsigned(y);

   // use bilinear interpolation
   LinearTransform<double> tx1(0.0, 1.0, p1, p2);
   double h1 = tx1(dx);

   LinearTransform<double> tx2(0.0, 1.0, p3, p4);
   double h2 = tx2(dx);

   LinearTransform<double> ty(0.0, 1.0, h1, h2);

   double average = ty(dy);

   return average;
}
