//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FontManager.cpp Font manager class
//

// includes
#include "StdAfx.h"
#include "FontManager.hpp"
#include <stdexcept>
#include "Bitmap.hpp"
#include <ulib/Path.hpp>
#include <shlobj.h> // for CSIDL_FONTS

// FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

/// \brief font implementation
/// \see http://www.freetype.org/freetype2/docs/tutorial/step2.html
class FontImpl: public boost::noncopyable
{
public:
   /// ctor
   FontImpl(FT_Library& library, const CString& cszTypeface, unsigned int uiHeight)
      :m_face(0)
   {
      USES_CONVERSION;
      if (FT_New_Face(library, T2CA(cszTypeface), 0, &m_face)) 
         throw Exception(_T("FT_New_Face failed"), __FILE__, __LINE__);

      // sets font size
      FT_Set_Char_Size(m_face, uiHeight << 6, uiHeight << 6, 96, 96);

      ATLASSERT(true == FT_IS_SCALABLE(m_face));
   }

   /// dtor
   ~FontImpl()
   {
      FT_Done_Face(m_face);
   }

   /// returns font face
   FT_Face& Get() { return m_face; }

private:
   /// font face
   FT_Face m_face;
};

/// \brief font manager implementation
class FontManagerImpl
{
public:
   /// ctor
   FontManagerImpl()
      :m_library(0)
   {
      if (FT_Init_FreeType(&m_library)) 
         throw Exception(_T("FT_Init_FreeType failed"), __FILE__, __LINE__);
   }

   /// dtor
   ~FontManagerImpl()
   {
      FT_Done_FreeType(m_library);
   }

   /// returns library instance
   FT_Library& Get() { return m_library; }

private:
   /// library instance
   FT_Library m_library;
};

//
// Font
//

Font::Font(FontImpl* pImpl, std::shared_ptr<FontManagerImpl> spManagerImpl)
:m_spImpl(pImpl),
 m_spManagerImpl(spManagerImpl)
{
}

unsigned int Font::Ascender()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->Get()->size->metrics.ascender >> 6;
}

unsigned int Font::Descender()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->Get()->size->metrics.descender >> 6;
}

unsigned int Font::Height()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->Get()->size->metrics.height >> 6;
}

unsigned int Font::MeasureString(const CString& cszText)
{
   ATLASSERT(m_spImpl != NULL);

   FT_Face& face = m_spImpl->Get();
   FT_Pos advance = 0;

   for (int i=0, iMax = cszText.GetLength(); i<iMax; i++)
   {
      // convert character code to glyph index
      FT_UInt glyphIndex = FT_Get_Char_Index(face, cszText[i]); 

      // load glyph for character
      if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
         throw Exception(_T("FT_Load_Glyph failed"), __FILE__, __LINE__);

      // get glyph object
      FT_Glyph glyph;
      if (FT_Get_Glyph(face->glyph, &glyph))
         throw Exception(_T("FT_Get_Glyph failed"), __FILE__, __LINE__);

      advance += glyph->advance.x;

      FT_Done_Glyph(glyph);
   }

   return advance >> 16;
}

unsigned int Font::MeasureTextLength(const CString& cszText, unsigned int uiWidth)
{
   ATLASSERT(m_spImpl != NULL);

   FT_Face& face = m_spImpl->Get();
   FT_Pos advance = 0, maxAdvance = uiWidth << 16;

   int i = 0, iMax;
   for (iMax = cszText.GetLength(); i<iMax; i++)
   {
      // convert character code to glyph index
      FT_UInt glyphIndex = FT_Get_Char_Index(face, cszText[i]);
      if (!glyphIndex)
         continue;

      // load glyph for character
      if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
         throw Exception(_T("FT_Load_Glyph failed"), __FILE__, __LINE__);

      // get glyph object
      FT_Glyph glyph;
      if (FT_Get_Glyph(face->glyph, &glyph))
         throw Exception(_T("FT_Get_Glyph failed"), __FILE__, __LINE__);

      advance += glyph->advance.x;

      FT_Done_Glyph(glyph);

      if (advance >= maxAdvance)
         break;
   }

   return i;
}

void Font::Write(const CString& cszText, Bitmap& bmp, unsigned int xpos, unsigned int ypos, Color cForeground, Color cBackground)
{
   ATLASSERT(m_spImpl != NULL);

   unsigned int uiAscender = Ascender();

   int colorDiffs[4];
   colorDiffs[0] = static_cast<int>(cForeground.m_color[0]) - cBackground.m_color[0];
   colorDiffs[1] = static_cast<int>(cForeground.m_color[1]) - cBackground.m_color[1];
   colorDiffs[2] = static_cast<int>(cForeground.m_color[2]) - cBackground.m_color[2];
   colorDiffs[3] = static_cast<int>(cForeground.m_color[3]) - cBackground.m_color[3];

   FT_Face& face = m_spImpl->Get();

   for (int i=0, iMax = cszText.GetLength(); i<iMax; i++)
   {
      // convert character code to glyph index
      FT_UInt glyphIndex = FT_Get_Char_Index(face, cszText[i]); 
      if (!glyphIndex)
         continue;

      // load glyph for character
      if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
         throw Exception(_T("FT_Load_Glyph failed"), __FILE__, __LINE__);

      // get glyph object
      FT_Glyph glyph;
      if (FT_Get_Glyph(face->glyph, &glyph))
         throw Exception(_T("FT_Get_Glyph failed"), __FILE__, __LINE__);

      // convert glyph to a bitmap, if needed
      if (glyph->format != FT_GLYPH_FORMAT_BITMAP)
      {
         if (FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1))
            throw Exception(_T("FT_Glyph_To_Bitmap failed"), __FILE__, __LINE__);
      }

      FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

      // access bitmap
      FT_Bitmap& bitmap = bitmap_glyph->bitmap;

      if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
         throw Exception(_T("FT_Bitmap contains non-gray image"), __FILE__, __LINE__);

      std::vector<Color> vecColors(bitmap.width * bitmap.rows);

      if (bitmap.width * bitmap.rows != 0)
      {
         unsigned char* data = bitmap.buffer;
         for(int y=0; y<bitmap.rows; y++)
         {
            int yoffset = y * bitmap.width;
            for(int x=0; x<bitmap.width; x++)
            {
               Color& cDest = vecColors[yoffset + x];
               cDest.m_color[0] = static_cast<unsigned char>(cBackground.m_color[0] + colorDiffs[0]*data[x]/255);
               cDest.m_color[1] = static_cast<unsigned char>(cBackground.m_color[1] + colorDiffs[1]*data[x]/255);
               cDest.m_color[2] = static_cast<unsigned char>(cBackground.m_color[2] + colorDiffs[2]*data[x]/255);
               cDest.m_color[3] = static_cast<unsigned char>(cBackground.m_color[3] + colorDiffs[3]*data[x]/255);
            }

            data += bitmap.pitch;
         }

         Bitmap bmpChar(bitmap.width, bitmap.rows, &vecColors[0]);

         // get bounding box
         FT_BBox bbox;
         FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_PIXELS, &bbox);

         bmp.Copy(bmpChar,
            xpos + bbox.xMin,
            ypos + uiAscender - bbox.yMax);
      }

      xpos += glyph->advance.x >> 16;

      FT_Done_Glyph(glyph);
   }
}

//
// FontManager
//

FontManager::FontManager()
:m_spImpl(new FontManagerImpl)
{
}

FontPtr FontManager::Create(const CString& cszTypeface, unsigned int uiHeight)
{
   CString cszFilename = Path::Combine(Path::SpecialFolder(CSIDL_FONTS), cszTypeface + _T(".ttf"));

   return FontPtr(new Font(new FontImpl(m_spImpl->Get(), cszFilename, uiHeight), m_spImpl));
}
