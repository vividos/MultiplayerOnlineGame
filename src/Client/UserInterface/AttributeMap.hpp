//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AttributeMap.hpp Attribute map
//
#pragma once

// includes
#include "Color.hpp"
#include "SizePointRect.hpp"
#include <ulib/Exception.hpp>
#include <map>

/// starts new attribute map
#define BEGIN_ATTR_MAP() \
   virtual void GetAttributes(std::map<CString, CString>& mapAttrDefaultValues) const throw() override {

/// attr name entry
#define ATTR_ENTRY(AttrName, DefaultValue) \
   mapAttrDefaultValues.insert(std::make_pair<CString, CString>(AttrName, DefaultValue));

/// inherits attribute map entries from base class
#define INHERIT_ATTR_MAP(BaseClass) \
   BaseClass::GetAttributes(mapAttrDefaultValues);

/// attr map end
#define END_ATTR_MAP() \
   }


/// attribute map
class AttributeMap
{
public:
   /// ctor
   AttributeMap() throw()
      :m_bInited(false)
   {
   }
   /// dtor
   virtual ~AttributeMap() throw()
   {
   }

   /// returns attributes map; overwritten by BEGIN_ATTR_MAP macro
   virtual void GetAttributes(std::map<CString, CString>& /*mapAttrDefaultValues*/) const throw() = 0
   {
      ATLASSERT(false); // must use BEGIN_ATTR_MAP macro to implement event map!
   }

   /// called when attribute has changed
   virtual void OnAttributeChanged(const CString& /*cszAttributeName*/) throw()
   {
   }

   // get methods

   /// returns attribute as string
   CString GetAttr(const CString& cszAttributeName) const throw()
   {
      Init();

      // search attribute value
      std::map<CString, CString>::const_iterator iter = m_mapAttrValues.find(cszAttributeName);
      if (iter == m_mapAttrValues.end())
      {
         // not found? check for default value
         iter = m_mapAttrDefaultValues.find(cszAttributeName);
         if (iter == m_mapAttrDefaultValues.end())
         {
            ATLASSERT(false); // there must be a default value!
            return _T("");
         }
      }

      return iter->second;
   }

   /// returns attribute as boolean value
   bool GetAttrAsBool(const CString& cszAttributeName) const
   {
      CString cszValue = GetAttr(cszAttributeName);
      cszValue.MakeLower();
      if (cszValue == _T("false"))
         return false;
      if (cszValue == _T("true"))
         return true;
      throw Exception(_T("invalid bool attribute value: ") + cszValue, __FILE__, __LINE__);
   }

   /// returns attribute as unsigned integer value
   unsigned int GetAttrAsUnsignedInt(const CString& cszAttributeName) const
   {
      CString cszValue = GetAttr(cszAttributeName);
      unsigned int uiValue = static_cast<unsigned int>(_tcstoul(cszValue, NULL, 10));
      return uiValue;
   }

   /// returns attribute as color; may be in format "#RRGGBB" or "#RRGGBBAA"
   Color GetAttrAsColor(const CString& cszAttributeName) const
   {
      CString cszValue = GetAttr(cszAttributeName);
      if (cszValue.Left(1) != _T("#") ||
          (cszValue.GetLength() != 6+1 && cszValue.GetLength() != 8+1))
         throw Exception(_T("invalid Color attribute value: ") + cszValue, __FILE__, __LINE__);

      // parse as color value, #RRGGBB or #RRGGBBAA or predefined color
      unsigned int uiValue = _tcstoul(cszValue.Mid(1), NULL, 16);

      if (cszValue.GetLength() == 6+1)
      {
         return Color(
            static_cast<BYTE>((uiValue >> 16) & 0xFF),
            static_cast<BYTE>((uiValue >> 8) & 0xFF),
            static_cast<BYTE>(uiValue & 0xFF), 255);
      }
      else
         return Color(
            static_cast<BYTE>((uiValue >> 24) & 0xFF),
            static_cast<BYTE>((uiValue >> 16) & 0xFF),
            static_cast<BYTE>((uiValue >> 8) & 0xFF),
            static_cast<BYTE>(uiValue & 0xFF));

      throw Exception(_T("invalid color attribute value: ") + cszValue, __FILE__, __LINE__);
   }

   /// returns attribute as size value; format is "height,width" (comma separated)
   Size GetAttrAsSize(const CString& cszAttributeName) const
   {
      CString cszValue = GetAttr(cszAttributeName);
      int iPos = cszValue.Find(_T(','));
      if (iPos == -1)
         throw Exception(_T("invalid Size attribute value: ") + cszValue, __FILE__, __LINE__);
      return Size(_ttoi(cszValue.Left(iPos)), _ttoi(cszValue.Mid(iPos+1)));
   }

   /// returns attribute as point value; format is "x,y" (comma separated)
   Point GetAttrAsPoint(const CString& cszAttributeName) const
   {
      CString cszValue = GetAttr(cszAttributeName);
      int iPos = cszValue.Find(_T(','));
      if (iPos == -1)
         throw Exception(_T("invalid Point attribute value: ") + cszValue, __FILE__, __LINE__);
      return Point(_ttoi(cszValue.Left(iPos)), _ttoi(cszValue.Mid(iPos+1)));
   }

   // set methods

   /// sets new attribute string value
   void SetAttr(const CString& cszAttributeName, const CString& cszValue, bool bSilent = false) throw()
   {
      Init();
      if (m_mapAttrValues.find(cszAttributeName) == m_mapAttrValues.end())
         m_mapAttrValues.insert(std::make_pair(cszAttributeName, cszValue));
      else
         m_mapAttrValues[cszAttributeName] = cszValue;

      if (!bSilent)
         OnAttributeChanged(cszAttributeName);
   }

   /// sets new attribute bool value
   void SetAttrBool(const CString& cszAttributeName, bool bValue) throw()
   {
      SetAttr(cszAttributeName, bValue ? _T("true") : _T("false"), false);
   }

   /// sets new attribute size value
   void SetAttr(const CString& cszAttributeName, const Size& value) throw()
   {
      CString cszValue;
      cszValue.Format(_T("%i,%i"), value.Width(), value.Height());
      SetAttr(cszAttributeName, cszValue);
   }

   /// sets new attribute point value
   void SetAttr(const CString& cszAttributeName, const Point& value) throw()
   {
      CString cszValue;
      cszValue.Format(_T("%i,%i"), value.X(), value.Y());
      SetAttr(cszAttributeName, cszValue);
   }

   /// sets new attribute color value
   void SetAttr(const CString& cszAttributeName, const Color& value) throw()
   {
      CString cszValue;
      cszValue.Format(_T("#%02x%02x%02x%02x"),
         value.m_color[0], value.m_color[1], value.m_color[2], value.m_color[3]);
      SetAttr(cszAttributeName, cszValue);
   }

   /// sets new attribute value as unsigned integer value
   void SetAttr(const CString& cszAttributeName, unsigned int uiValue)
   {
      CString cszValue;
      cszValue.Format(_T("%ui"), uiValue);
      SetAttr(cszAttributeName, cszValue);
   }

   // operators

   /// get operator
   CString operator[](const CString& cszAttributeName) const throw() { return GetAttr(cszAttributeName); }

private:
   /// initializes attributes list
   void Init() const throw()
   {
      if (m_bInited)
         return;

      GetAttributes(const_cast<std::map<CString, CString>&>(m_mapAttrDefaultValues));

      const_cast<bool&>(m_bInited) = true;
   }

   /// proxy class for setting attribute
   class AttrSetProxy
   {
   public:
      /// called when user assigns value
      AttributeMap& operator=(const CString& cszValue) throw()
      {
         m_attrMap.SetAttr(m_cszAttributeName, cszValue);
         return m_attrMap;
      }

   private:
      friend class AttributeMap;

      /// ctor; can only be created by AttributeMap
      AttrSetProxy(class AttributeMap& attrMap, const CString& cszAttributeName)
         :m_attrMap(attrMap),
          m_cszAttributeName(cszAttributeName)
      {
      }

   private:
      AttributeMap& m_attrMap;      ///< attribute map to use
      CString m_cszAttributeName;   ///< attribute name
   };

public:
   /// set operator
   AttrSetProxy operator[](const CString& cszAttributeName) throw() { return AttrSetProxy(*this, cszAttributeName); }

private:
   /// attribute values
   std::map<CString, CString> m_mapAttrValues;

   /// attribute default values
   std::map<CString, CString> m_mapAttrDefaultValues;

   /// indicates if attribute map is initialized
   bool m_bInited;
};
