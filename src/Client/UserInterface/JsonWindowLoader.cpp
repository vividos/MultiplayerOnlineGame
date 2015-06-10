//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file JsonWindowLoader.cpp JSON window loader
//

// includes
#include "StdAfx.h"
#include "JsonWindowLoader.hpp"
#include "Button.hpp"
#include "TextField.hpp"
#include "EditField.hpp"
#include "ProgressBar.hpp"
#include "Slider.hpp"
#include "Checkbox.hpp"
#include "Panel.hpp"
#include <json\json.h>
#include "StringTools.hpp"

// auto-link
#pragma comment(lib, "jsoncpp.lib")

void JsonWindowLoader::Load(WindowPtr spParent, const CString& cszJsonText)
{
   Json::Reader reader;

   // convert text to UTF-8
   std::vector<unsigned char> vecUtf8Text;
   StringTools::StringToUTF8(cszJsonText, vecUtf8Text);

   std::string strUtf8JsonText(reinterpret_cast<const char*>(&vecUtf8Text[0]), vecUtf8Text.size());

   // parse; don't retain comments
   Json::Value root;
   if (!reader.parse(strUtf8JsonText, root, false))
   {
      CString cszText;
      cszText.Format(_T("error while parsing json data: %hs"),
         reader.getFormatedErrorMessages().c_str());
      throw Exception(cszText, __FILE__, __LINE__);
   }

   // walk tree and create windows
   if (root.isObject())
      WalkObjectList(spParent, root);
   else
      throw Exception(_T("wrong json format; must start with object"), __FILE__, __LINE__);
}

void JsonWindowLoader::WalkObjectList(WindowPtr spParent, Json::Value& node)
{
   ATLASSERT(node.isObject());

   // enumerate object member
   Json::Value::Members members = node.getMemberNames();
   for (size_t i=0, iMax=members.size(); i<iMax; i++)
   {
      Json::Value valMember = node[members[i]];
      WalkObject(members[i], spParent, valMember);
   }
}

WindowPtr JsonWindowLoader::WalkObject(const std::string& strName, WindowPtr spParent, Json::Value& node)
{
   if (!node.isMember("Type"))
      throw Exception(_T("json element has no Type attribute"), __FILE__, __LINE__);

   Json::Value valType = node["Type"];

   // check if we know that type
   WindowPtr spWnd;
   std::string strType = valType.asString();
   if (strType == "Panel")
   {
      // create panel
      std::shared_ptr<Panel> spPanel(new Panel());
      spWnd = spPanel;

      // add all children
      if (node.isMember("Children"))
      {
         Json::Value valChildren = node["Children"];
         if (valChildren.isArray())
            WalkArray(spPanel, valChildren);
         else
         if (valChildren.isObject())
            WalkObjectList(spPanel, valChildren);
         else
            ATLASSERT(false);
      }
   }
   else
   if (strType == "Button")
   {
      std::shared_ptr<Button> spButton(new Button());
      spWnd = spButton;
   }
   else
   if (strType == "TextField")
   {
      std::shared_ptr<TextField> spTextField(new TextField());
      spWnd = spTextField;
   }
   else
   if (strType == "EditField")
   {
      std::shared_ptr<EditField> spEditField(new EditField());
      spWnd = spEditField;
   }
   else
   if (strType == "ProgressBar")
   {
      std::shared_ptr<ProgressBar> spProgressBar(new ProgressBar());
      spWnd = spProgressBar;
   }
   else
   if (strType == "Slider")
   {
      std::shared_ptr<Slider> spSlider(new Slider());
      spWnd = spSlider;
   }
   else
   if (strType == "Checkbox")
   {
      std::shared_ptr<Checkbox> spCheckbox(new Checkbox());
      spWnd = spCheckbox;
   }
   else
      throw Exception(_T("unknown Type attribute value"), __FILE__, __LINE__);

   // set name property
   CString cszName = strName.c_str();
   spWnd->SetAttr(WindowAttr::Name, cszName);

   // check for mandatory attributes
   {
      if (!node.isMember("Pos"))
         throw Exception(_T("json element has no Pos attribute"), __FILE__, __LINE__);
      if (!node.isMember("Size"))
         throw Exception(_T("json element has no Size attribute"), __FILE__, __LINE__);
   }

   // check attribute map and add all matching attribute values
   AddAttributes(spWnd, node);

   // register when given a parent
   if (spParent != NULL)
      spParent->RegisterChild(spWnd);

   return spWnd;
}

void JsonWindowLoader::WalkArray(WindowPtr spParent, Json::Value& node)
{
   ATLASSERT(node.isArray());

   // enumerate array member
   for (Json::Value::UInt index = 0, max = node.size(); index < max; index++)
   {
      Json::Value valItem = node[index];
      WalkObjectList(spParent, valItem);
   }
}

void JsonWindowLoader::AddAttributes(WindowPtr spWnd, Json::Value& node)
{
   // get all attributes
   std::map<CString, CString> mapAttrDefaultValues;
   spWnd->GetAttributes(mapAttrDefaultValues);

   std::map<CString, CString>::const_iterator iter = mapAttrDefaultValues.begin(),
      stop = mapAttrDefaultValues.end();

   USES_CONVERSION;
   CString cszValue;
   for (; iter != stop; ++iter)
   {
      // is attribute name a member?
      std::string strAttributeName = T2CA(iter->first);
      if (node.isMember(strAttributeName))
      {
         // yes, then set window attribute
         Json::Value valDefault("");
         cszValue = node.get(strAttributeName, valDefault).asString().c_str();
         spWnd->SetAttr(iter->first, cszValue);
      }
   }
}
