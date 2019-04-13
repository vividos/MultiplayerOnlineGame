//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file JsonWindowLoader.hpp JSON window loader
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"

// forward references
class Window;

namespace Json
{
   class Value;
}

// typedefs

/// window smart pointer
typedef std::shared_ptr<Window> WindowPtr;

/// loads a window by JSON formatted window setup
class USERINTERFACE_DECLSPEC JsonWindowLoader
{
public:
   /// ctor
   JsonWindowLoader() {}

   /// loads window from JSON text, using parent to create all windows
   void Load(WindowPtr spParent, const CString& cszJsonText);

private:
   /// walks object list nodes
   static void WalkObjectList(WindowPtr spParent, Json::Value& node);

   /// walks object node and adds child nodes, if any
   static WindowPtr WalkObject(const std::string& strName, WindowPtr spParent, Json::Value& node);

   /// walks array of nodes
   static void WalkArray(WindowPtr spParent, Json::Value& node);

   /// adds window attributes to window
   static void AddAttributes(WindowPtr spWnd, Json::Value& node);
};
