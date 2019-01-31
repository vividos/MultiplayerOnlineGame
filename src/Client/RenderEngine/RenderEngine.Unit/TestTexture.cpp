//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestTexture.cpp Unit tests for class Texture
//

// includes
#include "stdafx.h"
#include "TestRenderContext.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Bitmap.hpp"
#include <ulib/Timer.hpp>
#include "OrthoCamera.hpp"
#include "TextureParameter.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests Texture class
   TEST_CLASS(TestTexture)
   {
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         Texture t1, t2;
      }

      /// tests id of default constructed texture
      TEST_METHOD(TestNullId)
      {
         Texture t;
         Assert::AreEqual<unsigned int>(0, t.Id(), _T("default texture id must be 0"));
      }

      /// tests size of default constructed texture
      TEST_METHOD(TestNullSize)
      {
         Texture t;
         Assert::AreEqual<unsigned int>(0, t.Size(), _T("size of default texture must be 0"));
      }

      /// tests generating texture id
      TEST_METHOD(TestGenerate)
      {
         TestRenderContext rc;

         Texture t;
         t.Generate();

         Assert::IsTrue(t.Id() != 0, _T("generated texture id must not be 0"));
         Assert::AreEqual<unsigned int>(0, t.Size(), _T("size of generated texture must be 0"));
      }

      /// tests uploading square texture
      TEST_METHOD(TestUpload)
      {
         TestRenderContext rc;

         Texture t;
         t.Generate();

         const unsigned int c_uiDim = 2;
         Color colorData[c_uiDim*c_uiDim] =
         {
            Color::Red(), Color::Green(),
            Color::Blue(), Color::White(),
         };

         Bitmap bmp(c_uiDim, c_uiDim, colorData);
         t.Upload(bmp);

         Assert::IsTrue(t.Size() == c_uiDim * c_uiDim * 4,
            _T("size of texture must be dimensions to the square, times 4 for RGBA"));
      }

      /// tests uploading texture with mipmap generation
      TEST_METHOD(TestUploadMipmap)
      {
         TestRenderContext rc;

         Texture t;
         t.Generate();

         const unsigned int c_uiDim = 4;
         Bitmap bmp(c_uiDim, c_uiDim, Color::White());
         t.Upload(bmp, true);

         Assert::AreEqual(unsigned(c_uiDim * c_uiDim * 1.333 * 4), t.Size(),
            _T("size of uploaded mipmapped texture must be specific value"));
      }

      /// tests uploading texture with non power of two size
      TEST_METHOD(TestUploadNonPowerOfTwo)
      {
         TestRenderContext rc;

         Texture t;
         t.Generate();

         const unsigned int c_uiDimX = 17, c_uiDimY = 13;
         Bitmap bmp(c_uiDimX, c_uiDimY, Color::White());
         t.Upload(bmp);

         Assert::IsTrue(t.Size() == c_uiDimX * c_uiDimY * 4,
            _T("size of texture must be area times 4 for RGBA"));
      }

      BEGIN_TEST_METHOD_ATTRIBUTE(TestSpeedTextureUpload)
         TEST_IGNORE()
      END_TEST_METHOD_ATTRIBUTE()

      /// \brief tests speed of texture upload
      /// \note runs indefinitely, so it is ignored
      /// \verbatim
      /// using a 256k texture (256x256xRGBA)
      /// some numbers on my machine (with no other processes running):
      /// without the Bind() call in the inner loop (upload to always the same texture):
      ///    4800 textures uploaded in 1014 milliseconds, 211 us per texture, 1183 MB/s
      /// with Bind() call (the realisitic number):
      ///    2200 textures uploaded in 1045 milliseconds, 474 us per texture, 526 MB/s
      /// with mipmap flag on:
      ///    400 textures uploaded in 1248 milliseconds, 3120 us per texture, 106 MB/s
      /// \endverbatim
      TEST_METHOD(TestSpeedTextureUpload)
      {
         TestRenderContext rc;

         Texture t;
         t.Generate();

         Bitmap bmp(256, 256, Color::White());

         Timer ti;

         ti.Start();

         for (unsigned int uiCount = 0;;)
         {
            for (unsigned int i = 0; i < 100; i++)
            {
               t.Upload(bmp, false);
               uiCount++;
            }

            double dElapsed = ti.Elapsed();
            if (dElapsed > 1.0)
            {
               double dSecPerTexture = dElapsed / uiCount;
               double dMemoryPerSec = uiCount * t.Size() / dElapsed;

               ATLTRACE(_T("%u textures uploaded in %u milliseconds, %u us per texture, %u MB/s\n"),
                  uiCount,
                  unsigned(dElapsed * 1000.0),
                  unsigned(dSecPerTexture * 1000.0 * 1000.0),
                  unsigned(dMemoryPerSec / 1024.0 / 1024.0)
                  );
               ti.Restart();
               uiCount = 0;
            }
         }
      }

      class TestRenderScene
      {
      public:
         TestRenderScene()
         {
            // set up OpenGL
            glDisable(GL_DEPTH_TEST);

            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
         }

         void Wait()
         {
            m_rc.SwapBuffers();

            SDL_Event evt;
            bool bExitClient = false;
            while (!bExitClient)
            {
               while (SDL_PollEvent(&evt))
               {
                  if (evt.type == SDL_MOUSEBUTTONDOWN &&
                     evt.button.state == SDL_PRESSED &&
                     evt.button.button == SDL_BUTTON_LEFT)
                     bExitClient = true;
                  if (evt.type == SDL_QUIT)
                     bExitClient = true;
               }
               Sleep(0);
            }
         }

      private:
         TestRenderContext m_rc;
      };

      void RenderQuad(double u1, double u2, double v1, double v2)
      {
         glBegin(GL_TRIANGLES);
         glTexCoord2d(0.0, 0.0); glVertex2d(u1, v1);
         glTexCoord2d(1.0, 0.0); glVertex2d(u2, v1);
         glTexCoord2d(1.0, 1.0); glVertex2d(u2, v2);

         glTexCoord2d(0.0, 0.0); glVertex2d(u1, v1);
         glTexCoord2d(1.0, 1.0); glVertex2d(u2, v2);
         glTexCoord2d(0.0, 1.0); glVertex2d(u1, v2);
         glEnd();
      }

      TEST_METHOD(TestRenderTexture)
      {
         TestRenderScene rc;

         OrthoCamera cam(-1.0, 5.0, 5.0, -1.0);
         cam.Use();

         // prepare bitmaps
         Bitmap bmp1(16, 16);
         Bitmap bmp2(20, 40);

         // fill bitmap
         for (size_t i = 0; i < 256; i++)
            bmp1.Data()[i] = ((i + (i / 16)) & 1) ? Color::Black() : Color::White();

         for (size_t i = 0; i < 20 * 40; i++)
            bmp2.Data()[i] = ((i + (i / 20)) & 1) ? Color::Blue() : Color::Red();

         // prepare textures
         Texture t1, t2, t3;
         t1.Generate(); t1.Upload(bmp1);
         t2.Generate(); t2.Upload(bmp2);
         t3.Generate(); t3.Upload(bmp1, true);

         // render
         glColor3d(1.0, 1.0, 1.0);

         // try out rendering two triangles
         t1.Bind();
         RenderQuad(0.1, 0.1, 0.9, 0.9);

         // try out repeat modes
         //   TextureParameter tp1();
         //   tp1.Use();
         //   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         //   ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


         rc.Wait();
      }
   };

} // namespace UnitTest
