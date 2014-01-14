//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Path.hpp Path class
//
#pragma once

// includes
#include "Base.hpp"

/// file and folder path class
class BASE_DECLSPEC Path
{
public:
   // ctors

   /// default ctor
   Path() throw()
   {
   }

   /// ctor that takes a path
   Path(const CString& cszPath)
      :m_cszPath(cszPath)
   {
   }

   // operators

   /// returns CString
   operator const CString&() const throw() { return m_cszPath; }
   /// returns raw string pointer
   operator LPCTSTR() const throw() { return m_cszPath; }

   // getters

   /// returns filename and extension
   CString FilenameAndExt() const throw();

   /// returns filename without extension
   CString FilenameOnly() const throw();

   /// returns if path represents a file and if it exists
   bool FileExists() const throw();

   /// returns if path represents a folder and if it exists
   bool FolderExists() const throw();

   // methods

   /// canonicalizes path by removing '..', etc.
   bool Canonicalize();

   /// combine path with given second part and return new path
   Path Combine(const CString& cszPart2) throw();

   /// adds a backslash at the end of the path
   static void AddEndingBackslash(CString& cszPath) throw();

   /// combine both path parts and return new path
   static Path Combine(const CString& cszPart1, const CString& cszPart2) throw()
   {
      Path part1(cszPart1);
      return part1.Combine(cszPart2);
   }

   // public members

   /// path separator string
   static const TCHAR Separator[2];

   /// path separator character
   static const TCHAR SeparatorCh = _T('\\');

private:
   /// path
   CString m_cszPath;
};
