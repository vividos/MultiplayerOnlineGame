//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file DockingWindowBase.hpp Docking window base class
//
#pragma once

// forward references
class DebugClient;
class MainFrame;

/// macro to define docking window properties
#define DECLARE_DOCKING_WINDOW(dockcaption, docksize, floatsize, dockside) \
   public: \
      virtual dockwins::CDockingSide GetPreferredDockingSide() const { return dockside; } \
      virtual CSize GetDockingSize() const { return docksize; } \
      virtual CSize GetFloatingSize() const { return floatsize; } \
      virtual CString GetDockWindowCaption() const { return dockcaption; }

/// macro to define docking window properties; same as above but uses string resource id for docking caption
#define DECLARE_DOCKING_WINDOW_ID(dockcaption_id, docksize, floatsize, dockside) \
   public: \
      virtual dockwins::CDockingSide GetPreferredDockingSide() const { return dockside; } \
      virtual CSize GetDockingSize() const { return docksize; } \
      virtual CSize GetFloatingSize() const { return floatsize; } \
      virtual CString GetDockWindowCaption() const { return CString().LoadString(dockcaption_id); }


/// base class for docking windows
class DockingWindowBase :
   public dockwins::CBoxedDockingWindowImpl<DockingWindowBase, CWindow, dockwins::CVC7LikeExBoxedDockingWindowTraits>
{
   typedef DockingWindowBase thisClass;
   typedef dockwins::CBoxedDockingWindowImpl<DockingWindowBase, CWindow, dockwins::CVC7LikeExBoxedDockingWindowTraits> baseClass;

public:
   /// ctor
   DockingWindowBase() {}
   /// dtor
   virtual ~DockingWindowBase(){}

   DECLARE_WND_CLASS_EX(_T("DockingWindow"), CS_DBLCLKS, COLOR_WINDOW)

   /// returns if window is floating
   bool IsFloating() { return !m_rcUndock.IsRectEmpty(); }

   /// called when docking window is hidden
   void OnUndocked(HDOCKBAR hBar)
   {
      baseClass::OnUndocked(hBar);
   }

   // pure virtual methods; implement by using DECLARE_DOCKING_WINDOW or DECLARE_DOCKING_WINDOW_ID macro
   virtual dockwins::CDockingSide GetPreferredDockingSide() const = 0;
   virtual CSize GetDockingSize() const = 0;
   virtual CSize GetFloatingSize() const = 0;
   virtual CString GetDockWindowCaption() const = 0;
};
