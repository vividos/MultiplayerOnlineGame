//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Uuid.hpp Universally Unique ID implementation
//
#pragma once

// includes
#include "Base.hpp"
#include <guiddef.h>

/// \brief unique id class
/// \remarks uses GUID structure from Win32
class BASE_DECLSPEC Uuid: public GUID
{
public:
   /// default ctor; creates empty uuid
   Uuid(LPCTSTR pszUuid);

   /// ctor; takes 16 bytes of data used as GUID
   Uuid(const BYTE* abData);

   /// returns empty id
   static Uuid Null();

   /// returns new unique id
   static Uuid New();

   /// generates id from GUID-style formatted string
   static Uuid FromString(LPCTSTR pszUuid);

   /// converts unique id to GUID-style formatted string
   CString ToString() const;

   /// equality comparison
   bool operator==(const Uuid& lhs) const;

   /// inequality comparison
   bool operator!=(const Uuid& lhs) const;

   /// less comparison
   bool operator<(const Uuid& lhs) const;

   /// reutrns pointer to 16 bytes of raw data
   const BYTE* Raw() const { return reinterpret_cast<const BYTE*>(this); }

private:
   /// private ctor; initializes unique id from GUID
   Uuid(GUID& g);
};
