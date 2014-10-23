//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Panel.hpp Panel window
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Window.hpp"
#include <vector>

/// Panel attributes
namespace PanelAttr
{
   static TCHAR ShowBackground[] = _T("ShowBackground");    ///< indicates if background should be shown
   static TCHAR BackgroundColor[] = _T("BackgroundColor");  ///< background color
};

/// \brief panel window
/// \details a panel can contain child windows
class USERINTERFACE_DECLSPEC Panel: public Window
{
public:
   /// ctor
   Panel() throw()
   {
   }
   /// dtor
   virtual ~Panel() throw() {}

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Window)
      ATTR_ENTRY(PanelAttr::ShowBackground, _T("false"))
      ATTR_ENTRY(PanelAttr::BackgroundColor, _T("#000000ff")) // default: transparent
   END_ATTR_MAP()

   /// registers a child window in the panel
   virtual void RegisterChild(WindowPtr spChild) override
   {
      m_vecAllChildWindows.push_back(spChild);
      spChild->m_spParent = shared_from_this();
   }

   /// unregisters a child window
   virtual void UnregisterChild(WindowPtr spChild) override
   {
      for (size_t i=0, iMax = m_vecAllChildWindows.size(); i<iMax; i++)
      {
         if (m_vecAllChildWindows[i] == spChild)
         {
            m_vecAllChildWindows.erase(m_vecAllChildWindows.begin() + i);
            break;
         }
      }
   }

   virtual void Render(Rect& rectArea) throw() override;

   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override;
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override;

   /// called to destroy panel
   virtual void Destroy() override
   {
      // this does't really remove the children from the panel, since the panel could be
      // re-shown by calling Create().

      // unregister all handler of all children windows
      for (size_t i = 0, iMax = m_vecAllChildWindows.size(); i < iMax; i++)
         m_vecAllChildWindows[i]->UnregisterAllHandler();

      Window::Destroy();
   }

   /// finds window by name, including child windows
   WindowPtr FindByName(LPCTSTR pszWindowName);

   /// unregisters an event of a child by given event id
   bool Un(T_RegisteredEventId id)
   {
      for (size_t i = 0, iMax = m_vecAllChildWindows.size(); i < iMax; i++)
         if (m_vecAllChildWindows[i]->Un(id))
            return true;

      return false;
   }

private:
   /// all child windows (in order)
   std::vector<WindowPtr> m_vecAllChildWindows;
};
