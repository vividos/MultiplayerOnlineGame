//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file SVGOutput.hpp SVG output class
//
#pragma once

// include
#include <map>
#include "Vector2d.hpp"

/// \brief Scalable Vector Graphics format classes
namespace SVG
{
   /// element style
   struct Style
   {
      std::map<CString, CString> m_mapStyles;

      void Add(LPCTSTR pszName, LPCTSTR pszValue)
      {
         m_mapStyles.insert(std::make_pair(pszName, pszValue));
      }

      void Add(LPCTSTR pszName, unsigned int uiValue)
      {
         CString cszTemp;
         cszTemp.Format(_T("%u"), uiValue);
         m_mapStyles.insert(std::make_pair(pszName, cszTemp));
      }

      CString GetAttrib() const throw()
      {
         CString cszAttrib, cszTemp;
         for (std::map<CString, CString>::const_iterator iter = m_mapStyles.begin(); iter != m_mapStyles.end(); ++iter)
         {
            cszTemp.Format(_T("%s:%s;"), iter->first, iter->second);
            cszAttrib += cszTemp;
         }
         cszAttrib.TrimRight(_T(";"));
         return cszAttrib;
      }
   };

   /// writer for .svg xml format
   struct Writer
   {
      /// ctor
      Writer(LPCTSTR pszFilename, unsigned int width, unsigned int height)
         :m_fd(NULL)
      {
         errno_t err = _tfopen_s(&m_fd, pszFilename, _T("wt"));
         if (err != 0 || m_fd == NULL)
            throw Exception(CString(_T("couldn't create file: ")) + pszFilename,
               __FILE__, __LINE__);

         _ftprintf(m_fd,
            _T("<?xml version=\"1.0\" standalone=\"no\"?>\n")
            _T("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n")
            _T("<svg width=\"%u\" height=\"%u\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" ")
            _T("style=\"background-color:rgb(150,150,150)\">\n"),
            width, height);
      }
      /// dtor
      ~Writer() throw()
      {
         _ftprintf(m_fd, _T("</svg>\n"));
         fclose(m_fd);
      }

      void OutputCircle(const Vector2d& vCenter, double dRadius, const Style& style = Style())
      {
         CString cszStyle = style.GetAttrib();
         _ftprintf(m_fd, _T("<circle cx=\"%3.2f\" cy=\"%3.2f\" r=\"%3.2f\" style=\"%s\"/>\n"),
            vCenter.X(), vCenter.Y(), dRadius, cszStyle);
      }

      void OutputLine(const Vector2d& v0, const Vector2d& v1, const Style& style = Style())
      {
         CString cszStyle = style.GetAttrib();
         _ftprintf(m_fd, _T("<line x1=\"%3.2f\" y1=\"%3.2f\" x2=\"%3.2f\" y2=\"%3.2f\" style=\"%s\"/>\n"),
            v0.X(), v0.Y(), v1.X(), v1.Y(), cszStyle);
      }

      void OutputPolygon(const std::vector<Vector2d>& vecPolygonPoints, const Style& style = Style())
      {
         CString cszPolyPoints, cszTemp;
         for (size_t i=0, iMax=vecPolygonPoints.size(); i<iMax; i++)
         {
            cszTemp.Format(_T("%3.2f,%3.2f "),
               vecPolygonPoints[i].X(), vecPolygonPoints[i].Y());
            cszPolyPoints += cszTemp;
         }
         cszPolyPoints.TrimRight();

         CString cszStyle = style.GetAttrib();
         _ftprintf(m_fd, _T("<polygon points=\"%s\" style=\"%s\"/>\n"),
            cszPolyPoints, cszStyle);
      }

   private:
      FILE* m_fd;
      Style m_defaultStyle;
   };

} // namespace SVG
