//
// ulib - a collection of useful classes
// Copyright (C) 2008,2009,2012 Michael Fink
//
/// \file AutoLink.hpp auto-linking of ulib on Visual C++
//
#pragma once

// this file is used to enable auto linking to ulib
// the following macros are available:
// ULIB_VERBOSE_AUTO_LINK   when defined, prints what library is linked in
// ULIB_NO_AUTO_LINK      disables auto-linking (for whatever reason)

#ifndef ULIB_EXPORTS

   // this section is used when using ulib

   #ifdef UNIT_TEST

      // when unit testing, use static linking of single cpps
      #define ULIB_DECLSPEC

   #else

      /// this is used to import classes and functions from ulib
      #define ULIB_DECLSPEC __declspec(dllimport)

      // note: define ULIB_NO_AUTO_LINK in your project to disable auto-linking
      #ifndef ULIB_NO_AUTO_LINK

         /// library name
         #define ULIB_AUTO_LINK_LIB "ulib.lib"

         // print lib to link to
         #ifdef ULIB_VERBOSE_AUTO_LINK
            #pragma message ("ulib: auto linking to " ULIB_AUTO_LINK_LIB)
         #endif

         // link in lib
         #pragma comment (lib, ULIB_AUTO_LINK_LIB)

         #undef ULIB_AUTO_LINK_LIB

      #endif // ULIB_NO_AUTO_LINK

   #endif // UNIT_TEST

#else // ULIB_EXPORTS

   // this section is used when building ulib

   /// this is used to export classes and functions from ulib for use in projects
   #define ULIB_DECLSPEC __declspec(dllexport)

#endif // ULIB_EXPORTS
