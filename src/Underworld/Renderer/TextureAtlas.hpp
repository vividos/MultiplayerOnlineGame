//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/TextureAtlas.hpp Texture atlas
//

// includes

class IFileSystem;
class IndexedImage;
class TexCoord2f;
class Bitmap;
class Texture;

/// \brief texture atlas for Underworld textures and object graphics
/// the atlas contains all textures needed during the game; this way we only use
/// one texture for the whole geometry of the level and can put all vertices
/// into one VBO.
class TextureAtlas
{
public:
   /// loads texture atlas
   void Load(IFileSystem& fileSystem);

   /// generates texture from bitmap; must occur in render thread
   void Generate();

   /// gets min/max UV coordinates
   void GetUV(Uint16 uiPos, TexCoord2f& coordMin, TexCoord2f& coordMax);

   /// returns texture
   std::shared_ptr<Texture> GetTexture() { return m_spTexture; }

   /// cleans up texture
   void Done()
   {
      m_spTexture.reset();
   }

private:
   /// paste images into dest image, with borders for correct filtering on textures
   static void PasteImageWithBorders(const IndexedImage& src, IndexedImage& dest, unsigned int xpos, unsigned int ypos);

   /// generates bitmap from indexed image
   static void GenerateBitmap(const IndexedImage& src, Bitmap& dest);

private:
   /// bitmap
   std::shared_ptr<Bitmap> m_spBitmap;

   /// texture used for texture atlas
   std::shared_ptr<Texture> m_spTexture;
};
