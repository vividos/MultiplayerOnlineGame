//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestNamedTextureMap.cpp Unit tests for class NamedTextureMap
//

// includes
#include "stdafx.h"
#include "NamedTextureMap.hpp"
#include "Color.hpp"
#include "Bitmap.hpp"
#include "TestRenderContext.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   // defines
   LPCTSTR c_pszTextureName1 = _T("tex1");
   LPCTSTR c_pszTextureName2 = _T("tex2");

   /// tests class NamedTextureMap
   TEST_CLASS(TestNamedTextureMap)
   {
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         NamedTextureMap m1, m2;
      }

      /// tests method IsTextureAvail() with empty map
      TEST_METHOD(TestIsTextureAvailEmptyMap)
      {
         NamedTextureMap m;

         Assert::IsFalse(m.IsTextureAvail(c_pszTextureName1), _T("texture must not be available"));
      }

      /// tests method Get() with empty map
      TEST_METHOD(TestGetEmptyMap)
      {
         NamedTextureMap m;

         try
         {
            m.Get(c_pszTextureName1);
            Assert::Fail(_T("must throw exception, but didn't"));
         }
         catch (const Exception&)
         {
         }
      }

      /// tests Register() and IsTextureAvail() methods
      TEST_METHOD(TestRegisterIsTextureAvail)
      {
         NamedTextureMap m;

         TexturePtr spTex(new Texture);
         m.Register(c_pszTextureName1, spTex);

         Assert::IsTrue(m.IsTextureAvail(c_pszTextureName1), _T("texture must be available"));
      }

      /// tests Register() and Get() methods
      TEST_METHOD(TestRegisterGet)
      {
         NamedTextureMap m;

         TexturePtr spTex(new Texture);
         m.Register(c_pszTextureName1, spTex);

         TexturePtr spTex2 = m.Get(c_pszTextureName1);
         Assert::IsTrue(spTex.get() == spTex2.get(), _T("textures must be the same"));

         Assert::IsTrue(spTex.use_count() != 1, _T("texture smart pointer must not be unique"));
         Assert::IsTrue(spTex2.use_count() != 1, _T("texture smart pointer must not be unique"));
      }

      /// tests Unregister() method on empty map
      TEST_METHOD(TestUnregisterEmptyMap)
      {
         NamedTextureMap m;

         try
         {
            m.Unregister(c_pszTextureName1);
            Assert::Fail(_T("must throw exception, but didn't"));
         }
         catch (const Exception&)
         {
         }
      }

      /// tests Register() and Unregister() methods
      TEST_METHOD(TestRegisterUnregister)
      {
         NamedTextureMap m;

         TexturePtr spTex(new Texture);
         m.Register(c_pszTextureName1, spTex);

         Assert::IsTrue(m.IsTextureAvail(c_pszTextureName1), _T("texture must be available"));

         TexturePtr spTex2 = m.Unregister(c_pszTextureName1);

         Assert::IsFalse(m.IsTextureAvail(c_pszTextureName1), _T("texture must not be available"));

         Assert::IsTrue(spTex.get() == spTex2.get(), _T("textures must be the same"));
      }

      /// tests TextureSize() and NumTextures() methods with empty map
      TEST_METHOD(TestTextureSizeNumTexturesEmptyMap)
      {
         NamedTextureMap m;

         Assert::AreEqual<unsigned int>(0, m.TextureSize(), _T("texture size must be 0"));
         Assert::AreEqual<unsigned int>(0, m.NumTextures(), _T("number of textures must be 0"));
      }

      /// tests TextureSize() and NumTextures() methods with filled map
      TEST_METHOD(TestTextureSizeNumTexturesFilledMapMap)
      {
         TestRenderContext rc;

         const unsigned int c_uiDim1 = 16, c_uiDim2 = 32;

         std::shared_ptr<Texture> spTex1(new Texture);
         spTex1->Generate();
         spTex1->Bind();
         spTex1->Upload(Bitmap(c_uiDim1, c_uiDim1, Color::White()));

         std::shared_ptr<Texture> spTex2(new Texture);
         spTex2->Generate();
         spTex2->Bind();
         spTex2->Upload(Bitmap(c_uiDim2, c_uiDim2, Color::White()));

         NamedTextureMap m;

         m.Register(c_pszTextureName1, spTex1);

         Assert::AreEqual(spTex1->Size(), m.TextureSize(), _T("texture size must be equal to tex1"));
         Assert::AreEqual<unsigned int>(1, m.NumTextures(), _T("number of textures must be 1"));

         m.Register(c_pszTextureName2, spTex2);

         Assert::AreEqual(spTex1->Size() + spTex2->Size(), m.TextureSize(), _T("texture size must be tex1 + tex2 size"));
         Assert::AreEqual<unsigned int>(2, m.NumTextures(), _T("number of textures must be 2"));

         m.Unregister(c_pszTextureName1);

         Assert::AreEqual(spTex2->Size(), m.TextureSize(), _T("texture size must be equal to tex2"));
         Assert::AreEqual<unsigned int>(1, m.NumTextures(), _T("number of textures must be 1"));

         m.Unregister(c_pszTextureName2);

         Assert::AreEqual<unsigned int>(0, m.TextureSize(), _T("texture size must be zero"));
         Assert::AreEqual<unsigned int>(0, m.NumTextures(), _T("number of textures must be 0"));
      }
   };

} // namespace UnitTest
