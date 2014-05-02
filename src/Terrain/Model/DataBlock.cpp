//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlock.cpp Terrain height data block
//

// includes
#include "StdAfx.h"
#include "Model/DataBlock.hpp"

using namespace Terrain::Model;

DataBlock::DataBlock(unsigned int uiSize)
:m_elevationData(uiSize)
{
   ATLASSERT(true == IsPowerOfTwo(uiSize));
}

double DataBlock::HeightInterpolate(double x, double y) const throw()
{
   double p1 = m_elevationData.Height(unsigned(x), unsigned(y));
   double p2 = m_elevationData.Height(unsigned(x+1), unsigned(y));
   double p3 = m_elevationData.Height(unsigned(x), unsigned(y+1));
   double p4 = m_elevationData.Height(unsigned(x+1), unsigned(y+1));

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
