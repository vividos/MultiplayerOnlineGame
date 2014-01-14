//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file DamageType.hpp Damage type
//
#pragma once

// includes
#include "World.hpp"

/// damage type
class WORLD_DECLSPEC DamageType
{
public:
   /// damage type enum
   enum T_enDamageType
   {
      typeNormal = 0,
      typeNature = 1,
      typeFrost = 2,
      typeFire = 3,
      typeShadow = 4,
      typeMax ///< max. value
   };

   /// ctor
   DamageType(T_enDamageType enDamageType = typeNormal)
      :m_enDamageType(enDamageType)
   {
      ATLASSERT(enDamageType < typeMax);
   }

   /// returns damage type from string
   static DamageType FromString(LPCTSTR pszType);

   /// returns damage type
   T_enDamageType Type() const throw() { return m_enDamageType; }

   /// formats type as string
   LPCTSTR ToString() const throw();

private:
   /// damage type
   T_enDamageType m_enDamageType;
};
