#pragma once

// includes
#include <set>

/// \brief mobile attribute
/// holds one mobile attribute
class MobileAttribute
{
public:
   /// mobile attribute type
   enum T_enMobileAttributeType
   {
      mobileAttrHealthCurrent,
      mobileAttrHealthMax,
      mobileAttrManaCurrent,
      mobileAttrManaMax,
      mobileAttrStrength,
      mobileAttrDexterity
      mobileAttrIntellect
      mobileAttrDefense,
      mobileAttrVitality,
      mobileAttrCritChance,
      mobileAttrDamageResistanceNormal,
      mobileAttrDamageResistanceNature,
      mobileAttrDamageResistanceFrost
      mobileAttrDamageResistanceFire
      mobileAttrDamageResistanceShadow,
   };

   /// ctor; initializes item attribute
   MobileAttribute(T_enMobileAttributeType enType, unsigned int uiAttributeValue)
      :m_enType(enType),
      m_uiAttributeValue(uiAttributeValue)
   {
   }

   // get methods

   /// returns type
   T_enMobileAttributeType Type() const throw() { return m_enType; }

   /// returns value
   unsigned int Value() const throw() { return m_uiValue; }


   // set methods

   /// sets new value
   void Value(unsigned int uiNewValue) throw() { m_uiValue = uiNewValue; }

   /// sort function object
   struct SortByType
   {
      bool operator()(const MobileAttribute& lhs, const MobileAttribute& rhs) const throw()
      {
         return lhs.m_enType < rhs.m_enType;
      }
   };

private:
   T_enMobileAttributeType m_enType;   ///< attribute type
   unsigned int m_uiValue;             ///< attribute value
};

/// item attribute set type
typedef std::set<MobileAttribute, MobileAttribute::SortByType> MobileAttributeSet;
