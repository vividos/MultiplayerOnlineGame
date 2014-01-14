//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Window.hpp Window base class
//
#pragma once

// includes
#include "AttributeMap.hpp"
#include "Observable.hpp"
#include <boost/noncopyable.hpp>

// forward references
class IWindowManager;
class Font;

/// font smart ptr
typedef std::shared_ptr<Font> FontPtr;

/// Window attributes
namespace WindowAttr
{
   static TCHAR Name[] = _T("Name");   ///< window name
   static TCHAR Pos[]  = _T("Pos");    ///< window position
   static TCHAR Size[] = _T("Size");   ///< window size
}

/// window smart ptr
typedef std::shared_ptr<class Window> WindowPtr;

/// \brief window base class
/// \details every window has attributes
class Window:
   public Observable,
   public AttributeMap,
   public std::enable_shared_from_this<Window>,
   public boost::noncopyable
{
public:
   /// ctor
   Window() throw() {}
   /// dtor
   virtual ~Window() throw() {}

   /// returns real window; only used for proxy window
   virtual Window& RealWindow() throw() { return *this; }
   /// returns real window; only used for proxy window; const version
   virtual const Window& RealWindow() const throw() { return *this; }

   // event map
   BEGIN_EVENT_MAP()
   END_EVENT_MAP()

   // attribute map
   BEGIN_ATTR_MAP()
      //INHERIT_ATTR(BaseClass)
      ATTR_ENTRY(WindowAttr::Name, _T("")) // unique name
      ATTR_ENTRY(WindowAttr::Pos, _T("0,0")) // position as X,Y
      ATTR_ENTRY(WindowAttr::Size, _T("100,100")) // size as Width,Height
   END_ATTR_MAP()

   /// returns parent window
   WindowPtr Parent() { return m_spParent; }
   /// returns parent window; const version
   const WindowPtr Parent() const { return m_spParent; }

   // get methods

   // some convenience get/set methods for attributes
   Size GetSize() const throw() { return GetAttrAsSize(WindowAttr::Size); } ///< returns size
   Point GetPos() const throw() { return GetAttrAsPoint(WindowAttr::Pos); } ///< returns position
   Rect GetRect() const throw() { return Rect(GetPos(), GetSize()); }       ///< returns rectangle

   // set methods

   /// creates window as child of parent window
   void Create(Window& parent)
   {
      parent.RegisterChild(shared_from_this());
   }

   /// destroys window
   void Destroy()
   {
      ATLASSERT(Parent() != NULL);
      Parent()->UnregisterChild(shared_from_this());
   }

   // virtual methods

   /// returns window manager
   virtual IWindowManager& GetWindowManager()
   {
      ATLASSERT(Parent() != NULL);
      return Parent()->GetWindowManager();
   }

   /// returns current window font (or parent font, if unset)
   virtual Font& GetFont()
   {
      ATLASSERT(Parent() != NULL);
      return m_spFont == NULL ? Parent()->GetFont() : *m_spFont;
   }

   /// registers a window as child
   virtual void RegisterChild(WindowPtr spChild) = 0;
   /// unregisters a window as child
   virtual void UnregisterChild(WindowPtr spChild) = 0;

   /// renders window area
   virtual void Render(Rect& rectArea) = 0;

   /// called to handle mouse button events
   virtual bool OnMouseButtonEvent(bool /*bPressed*/, int /*iMouseButton*/, unsigned int /*x*/, unsigned int /*y*/){ return false; }
   /// called to handle mouse motion events
   virtual void OnMouseMotionEvent(unsigned int /*x*/, unsigned int /*y*/){}
   /// called to handle keyboard events
   virtual bool OnKeyboardEvent(bool /*bKeyDown*/, unsigned int /*sym*/, unsigned int /*mod*/){ return false; }
   /// called when focus changes
   virtual void OnFocusChanged(bool /*bGotFocus*/){}

private:
   friend class Panel;
   friend class WindowProxy;

   /// parent window; not set when not created
   WindowPtr m_spParent;

   /// default font; may be nullptr
   FontPtr m_spFont;
};
