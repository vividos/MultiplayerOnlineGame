//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file DamageType.cpp Damage type
//

// includes
#include "StdAfx.h"
#include "DamageType.hpp"

static LPCTSTR c_pszNormal = _T("normal");
static LPCTSTR c_pszNature = _T("nature");
static LPCTSTR c_pszFrost = _T("frost");
static LPCTSTR c_pszFire = _T("fire");
static LPCTSTR c_pszShadow = _T("shadow");

DamageType DamageType::FromString(LPCTSTR pszType)
{
   if (0 == _tcscmp(pszType, c_pszNormal)) return DamageType(typeNormal);
   if (0 == _tcscmp(pszType, c_pszNature)) return DamageType(typeNature);
   if (0 == _tcscmp(pszType, c_pszFrost)) return DamageType(typeFrost);
   if (0 == _tcscmp(pszType, c_pszFire)) return DamageType(typeFire);
   if (0 == _tcscmp(pszType, c_pszShadow)) return DamageType(typeShadow);
   throw Exception(_T("invalid damage type"), __FILE__, __LINE__);
}

LPCTSTR DamageType::ToString() const throw()
{
   switch (m_enDamageType)
   {
   case typeNormal: return c_pszNormal;
   case typeNature: return c_pszNature;
   case typeFrost: return c_pszFrost;
   case typeFire: return c_pszFire;
   case typeShadow: return c_pszShadow;
   default:
      ATLASSERT(false);
      return _T("???");
   }
}
