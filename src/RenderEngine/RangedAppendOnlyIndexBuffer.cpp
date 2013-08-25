//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RangedAppendOnlyIndexBuffer.cpp Index buffer for range of indices
//

// includes
#include "StdAfx.h"
#include "RangedAppendOnlyIndexBuffer.hpp"
#include "OpenGL.hpp"

RangedAppendOnlyIndexBuffer::RangedAppendOnlyIndexBuffer()
:glDrawRangeElements(NULL)
{
}

IndexRange RangedAppendOnlyIndexBuffer::Add(const std::vector<unsigned int>& vecData)
{
   ATLASSERT(!vecData.empty());

   IndexRange range;
   range.m_uiStart = m_vecIndices.size();
   range.m_uiSize = vecData.size();
   range.m_uiFirstIndex = std::numeric_limits<unsigned int>::max();
   range.m_uiLastIndex = 0;

   // determine index range
   for (size_t i=0, iMax=vecData.size(); i<iMax; i++)
   {
      range.m_uiFirstIndex = std::min(range.m_uiFirstIndex, vecData[i]);
      range.m_uiLastIndex = std::max(range.m_uiLastIndex, vecData[i]);
   }

   m_vecIndices.insert(m_vecIndices.end(), vecData.begin(), vecData.end());

   return range;
}

void RangedAppendOnlyIndexBuffer::Update()
{
   ATLASSERT(OpenGL::IsRenderThread()); // must be called in render thread

   if (m_spIndexBuffer == NULL)
   {
      m_spIndexBuffer.reset(new T_IndexBuffer);
      m_spIndexBuffer->Generate();
   }

   m_spIndexBuffer->Bind();
   m_spIndexBuffer->Upload(NULL, m_vecIndices.size());
   m_spIndexBuffer->Upload(&m_vecIndices[0], m_vecIndices.size());
}

void RangedAppendOnlyIndexBuffer::RenderRange(const IndexRange& range) const
{
   ATLASSERT(OpenGL::IsRenderThread()); // must be called in render thread
   ATLASSERT(m_spIndexBuffer != NULL);

   if (glDrawRangeElements == NULL)
   {
      const_cast<RangedAppendOnlyIndexBuffer&>(*this).glDrawRangeElements =
         (PFNGLDRAWRANGEELEMENTSWINPROC)wglGetProcAddress("glDrawRangeElements");
   }

   ATLASSERT(glDrawRangeElements != NULL);

   glDrawRangeElements(GL_TRIANGLES,
      range.m_uiFirstIndex,
      range.m_uiLastIndex,
      range.m_uiSize,
      GL_UNSIGNED_INT,
      reinterpret_cast<const GLvoid*>(range.m_uiStart*sizeof(unsigned int))
   );

   glTraceError("glDrawRangeElements");
}
