//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/PropertiesLoader.hpp Object propreties loader
//

// includes
#include "stdafx.h"
#include "PropertiesLoader.hpp"
#include "Bits.hpp"
#include "Properties.hpp"
#include "IFileSystem.hpp"
#include <ulib/stream/IStream.hpp>

using Import::GetBits;
using namespace Underworld;

void Import::PropertiesLoader::Load(ObjectProperties& properties)
{
   UaTrace("loading properties\n");

   m_spFile = m_fileSystem.OpenFile(_T("data\\comobj.dat"), true);

   // import common object properties
   LoadCommonObjectProperties(properties);

   m_spFile = m_fileSystem.OpenFile(_T("data\\objects.dat"), true);

   LoadObjectProperties(properties);
}

void Import::PropertiesLoader::LoadCommonObjectProperties(ObjectProperties& properties)
{
   std::vector<CommonObjectProperty>& vecProp = properties.GetVectorCommonObjectProperties();
   vecProp.clear();

   unsigned int uiMax = static_cast<size_t>((m_spFile->Length()-2) / 11);
   UaAssert(uiMax == 0x0200);

   vecProp.reserve(uiMax);
   Uint8 auiData[11];

   for (unsigned int ui=0; ui<uiMax; ui++)
   {
      DWORD dwBytesRead = 0;
      m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

      CommonObjectProperty prop;
      prop.uiRadius = auiData[1] & 7;
      prop.uiHeight = auiData[0];
      prop.uiMass = ((auiData[2]<<8) | auiData[1]) >> 4;
      prop.uiQualityClass = GetBits(auiData[6],  2, 2);
      prop.uiQualityType =  GetBits(auiData[10], 0, 4);
      prop.bCanHaveOwner =  GetBits(auiData[7],  7, 1) != 0;
      prop.bCanBeLookedAt = GetBits(auiData[10], 4, 1) != 0;
      prop.bCanBePickedUp = GetBits(auiData[3],  5, 1) != 0;
      prop.bIsContainer =   GetBits(auiData[3],  7, 1) != 0;

      vecProp.push_back(prop);
   }
}

/// \todo load jewelry info table
void Import::PropertiesLoader::LoadObjectProperties(ObjectProperties& properties)
{
   // melee weapons table
   {
      m_spFile->Seek(2L, Stream::IStream::seekBegin);

      std::vector<MeleeWeaponProperty>& vecProp = properties.GetVectorMeleeWeaponProperties();
      vecProp.clear();
      vecProp.reserve(0x10);

      Uint8 auiData[8];
      for (unsigned int i=0; i<0x10; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         MeleeWeaponProperty prop;
         prop.uiDamageModSlash = auiData[0];
         prop.uiDamageModBash = auiData[1];
         prop.uiDamageModStab = auiData[2];
         prop.skillType = static_cast<EMeleeSkillType>(auiData[6]);
         prop.uiDurability = auiData[7];

         UaAssert(prop.skillType == Underworld::meleeSkillSword ||
            prop.skillType == Underworld::meleeSkillAxe ||
            prop.skillType == Underworld::meleeSkillMace ||
            prop.skillType == Underworld::meleeSkillUnarmed);

         vecProp.push_back(prop);
      }
   }

   // ranged weapons table
   {
      m_spFile->Seek(0x82L, Stream::IStream::seekBegin);

      std::vector<RangedWeaponProperty>& vecProp = properties.GetVectorRangedWeaponProperties();
      vecProp.clear();
      vecProp.reserve(0x10);

      Uint8 auiData[3];
      for (unsigned int i=0; i<0x10; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         RangedWeaponProperty prop;
         prop.uiDurability = auiData[0];

         vecProp.push_back(prop);
      }
   }

   // armour and wearables table
   {
      m_spFile->Seek(0xb2L, Stream::IStream::seekBegin);

      std::vector<ArmourAndWearableProperty>& vecProp = properties.GetVectorArmourAndWearableProperties();
      vecProp.clear();
      vecProp.reserve(0x20);

      Uint8 auiData[4];
      for (unsigned int i=0; i<0x20; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         ArmourAndWearableProperty prop;
         prop.uiProtection = auiData[0];
         prop.uiDurability = auiData[1];
         prop.category = static_cast<EArmourCategory>(auiData[3]);

         UaAssert(prop.category == Underworld::armourNone ||
            prop.category == Underworld::armourBodyArmour ||
            prop.category == Underworld::armourLeggings ||
            prop.category == Underworld::armourGloves ||
            prop.category == Underworld::armourBoots ||
            prop.category == Underworld::armourHat ||
            prop.category == Underworld::armourRing);

         vecProp.push_back(prop);
      }
   }

   // critters table
   {
      m_spFile->Seek(0x0132L, Stream::IStream::seekBegin);

      std::vector<CritterProperty>& vecProp = properties.GetVectorCritterProperties();
      vecProp.clear();
      vecProp.reserve(0x40);

      Uint8 auiData[48];
      for (unsigned int i=0; i<0x40; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         CritterProperty prop;
         prop.uiNpcPower = auiData[5];

         vecProp.push_back(prop);
      }
   }

   // containers table
   {
      m_spFile->Seek(0x0d32L, Stream::IStream::seekBegin);

      std::vector<ContainerProperty>& vecProp = properties.GetVectorContainerProperties();
      vecProp.clear();
      vecProp.reserve(0x10);

      Uint8 auiData[3];
      for (unsigned int i=0; i<0x10; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         ContainerProperty prop;
         prop.uiCapacity = auiData[0];
         prop.iObjectClassAccepted = static_cast<int>(static_cast<Sint8>(auiData[1]));

         vecProp.push_back(prop);
      }
   }

   // light source table
   {
      m_spFile->Seek(0x0d62L, Stream::IStream::seekBegin);

      std::vector<LightSourceProperty>& vecProp = properties.GetVectorLightSourceProperties();
      vecProp.clear();
      vecProp.reserve(0x10);

      Uint8 auiData[2];
      for (unsigned int i=0; i<0x10; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         LightSourceProperty prop;
         prop.uiBrightness = auiData[0];
         prop.uiDuration = auiData[1];

         vecProp.push_back(prop);
      }
   }

   // todo: jewelry info table

   // animation object table
   {
      m_spFile->Seek(0x0da2L, Stream::IStream::seekBegin);

      std::vector<AnimatedObjectProperty>& vecProp = properties.GetVectorAnimatedObjectProperties();
      vecProp.clear();
      vecProp.reserve(0x10);

      Uint8 auiData[4];
      for (unsigned int i=0; i<0x10; i++)
      {
         DWORD dwBytesRead = 0;
         m_spFile->Read(auiData, SDL_TABLESIZE(auiData), dwBytesRead);

         AnimatedObjectProperty prop;
         prop.uiStartFrame = auiData[2];
         prop.uiNumberOfFrames = auiData[3];

         vecProp.push_back(prop);
      }
   }
}
