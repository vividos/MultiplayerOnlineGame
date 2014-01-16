//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PositionalPosCtrl.hpp Positional source position control
//
#pragma once

// includes
#include "Vector3d.hpp"

/// positional source position control
class PositionalPosCtrl: public CWindowImpl<PositionalPosCtrl>
{
public:
   /// ctor
   PositionalPosCtrl()
      :m_bLeftButtonDown(false)
   {
      m_ptCurrent.x = m_ptCurrent.y = 0;
   }

   DECLARE_WND_CLASS(NULL)

   /// function type to call when position has changed
   typedef std::function<void()> T_fnOnChangedPosition;

   /// inits control
   void Init(T_fnOnChangedPosition fnOnChangedPosition = T_fnOnChangedPosition())
   {
      m_fnOnChangedPosition = fnOnChangedPosition;

      CRect rc;
      GetClientRect(&rc);

      m_ptCurrent = rc.CenterPoint();
   }

   /// returns current position
   Vector3d GetPosition() const
   {
      CRect rc;
      GetClientRect(&rc);

      CPoint ptMid = rc.CenterPoint();

      const double c_dMaxDistance = 4.0;

      Vector3d vPos;
      vPos.X(c_dMaxDistance * (m_ptCurrent.x - ptMid.x) / (rc.Width() / 2));
      vPos.Z(c_dMaxDistance * (m_ptCurrent.y - ptMid.y) / (rc.Height() / 2));

      return vPos;
   }

private:
   BEGIN_MSG_MAP(PositionalPosCtrl)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
   END_MSG_MAP()

private:
// Handler prototypes (uncomment arguments if needed):
// LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
// LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

   /// erases background
   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      // disable erasing
      return FALSE;
   }

   /// paints control
   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CPaintDC dc(m_hWnd);

      dc.FillSolidRect(&dc.m_ps.rcPaint, RGB(255,255,255));

      CRect rc;
      GetClientRect(&rc);

      {
         CRect rcCenter(rc.CenterPoint(), CSize(1,1));
         rcCenter.InflateRect(2, 2);

         dc.FillSolidRect(&rcCenter, RGB(0,0,255));
      }

      {
         CRect rcCurrent(m_ptCurrent, CSize(1,1));
         rcCurrent.InflateRect(2, 2);

         dc.FillSolidRect(&rcCurrent, RGB(255,0,0));
      }

      return 0;
   }

   /// called when left mouse button is pressed
   LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_bLeftButtonDown = true;
      return 0;
   }

   /// called when left mouse button is released
   LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      m_bLeftButtonDown = false;
      return 0;
   }

   /// called when mouse is being moved
   LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      if (m_bLeftButtonDown)
      {
         m_ptCurrent = CPoint(lParam);
         Invalidate();

         if (m_fnOnChangedPosition != nullptr)
            m_fnOnChangedPosition();
      }

      return 0;
   }

private:
   /// callback to signal changed position
   T_fnOnChangedPosition m_fnOnChangedPosition;

   /// indicates if left mouse button is down
   bool m_bLeftButtonDown;

   /// current position
   CPoint m_ptCurrent;
};
