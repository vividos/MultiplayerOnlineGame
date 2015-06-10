//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DisplayList.hpp OpenGL display list class
//
#pragma once

// includes
#include "OpenGL.hpp"

namespace OpenGL
{
   /// OpenGL display list wrapper
   class DisplayList
   {
   public:
      /// ctor
      DisplayList() throw()
         :m_listId(0)
      {
      }
      /// dtor
      ~DisplayList() throw()
      {
         Delete();
      }

      /// initializes display list
      void Init() throw()
      {
         Delete();
         m_listId = glGenLists(1);
      }

      /// opens display list for compiling
      void Open(bool bCompileAndExecute = false) throw()
      {
         ATLASSERT(m_listId != 0);
         glNewList(m_listId, bCompileAndExecute ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
      }

      /// closes display list
      void Close() throw()
      {
         ATLASSERT(m_listId != 0);
         glEndList();
      }

      /// calls display list
      void Call() const throw()
      {
         ATLASSERT(m_listId != 0);
         glCallList(m_listId);
      }

   private:
      /// deletes display list
      void Delete() throw()
      {
         if (m_listId != 0)
            glDeleteLists(m_listId, 1);
      }

   private:
      GLuint m_listId;  ///< display list id
   };

   /// OpenGL display list array wrapper
   class DisplayListArray
   {
   public:
      /// ctor
      DisplayListArray() throw()
         :m_listId(0),
          m_listLength(0)
      {
      }

      /// dtor
      ~DisplayListArray() throw()
      {
         if (m_listId != 0)
            glDeleteLists(m_listId, m_listLength);
      }

      /// initializes display list array
      void Init(GLsizei uiSize) throw()
      {
         m_listId = glGenLists(uiSize);
         m_listLength = static_cast<GLuint>(uiSize);
      }

      /// opens single display list for compiling
      void Open(GLuint uiRelIndex, bool bCompileAndExecute = false) throw()
      {
         ATLASSERT(m_listId != 0);
         ATLASSERT(uiRelIndex < m_listLength);
         glNewList(m_listId + uiRelIndex, bCompileAndExecute ? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
      }

      /// closes currently opened display list
      void Close() throw()
      {
         ATLASSERT(m_listId != 0);
         glEndList();
      }

      /// calls display list
      void Call(GLuint uiRelIndex) const throw()
      {
         ATLASSERT(m_listId != 0);
         ATLASSERT(uiRelIndex < m_listLength);
         glCallList(m_listId + uiRelIndex);
      }
   private:
      GLuint m_listId; ///< first display list id
      GLuint m_listLength; ///< number of consecutive list ids
   };

} // namespace OpenGL
