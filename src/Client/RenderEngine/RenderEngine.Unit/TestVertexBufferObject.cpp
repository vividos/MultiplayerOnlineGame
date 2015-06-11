//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestVertexBufferObject.cpp Unit tests for class VertexBufferObject
//

// includes
#include "stdafx.h"
#include "VertexBufferObject.hpp"
#include "TestRenderContext.hpp"
#include "PerspectiveCamera.hpp"
#include "Vector3.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests class VertexBufferObject
   TEST_CLASS(TestVertexBufferObject)
   {
   public:
      Vector3d GetSphereVertex(double r, double dLat, double dLong)
      {
         Vector3d v(r, 0.0, 0.0);
         v.RotateY(dLong);
         v.RotateX(dLat);
         return v;
      }

      struct VertexData
      {
         float x, y, z;    // 12 bytes
         float nx, ny, nz; // 12 bytes
         GLubyte color[4]; //  4 bytes
         // 28 so far, 4 left

         GLubyte dummy[4];
      };

      static_assert(sizeof(VertexData) == 32, "VertexData must have size 32 bytes");

      unsigned short MapAngleToIndex(int iLat, int iLong)
      {
         iLat += 90;
         iLat /= 5;

         iLong /= 5;
         // now iLat == [0;36[ and iLong = [0;36[

         return static_cast<unsigned short>(iLat + iLong * 36);
      };

      /// tests uploading VBO data
      TEST_METHOD(TestUploadVBOData)
      {
         TestRenderContext rc;

         glClearColor(0, 0, 0, 0);
         glClear(GL_COLOR_BUFFER_BIT);

         // set up camera
         PerspectiveCamera cam;
         cam.SetPositionLookAt(Vector3d(0.0, 5.0, 5.0), Vector3d());
         cam.Use();

         double r = 3.0;

         std::vector<unsigned short> vecIndices;

         std::vector<VertexData> vecVertexData;

         glColor3ub(255, 255, 255);
         glBegin(GL_POINTS);
         //glBegin(GL_LINES);
         for (int iLong = 0; iLong < 360; iLong += 5)
            for (int iLat = -90; iLat < 90; iLat += 5)
            {
               double dLat = iLat;
               double dLong = iLong;
               Vector3d v = GetSphereVertex(r, dLat, dLong);

               VertexData data;
               data.x = static_cast<float>(v.X());
               data.y = static_cast<float>(v.Y());
               data.z = static_cast<float>(v.Z());

               v.Normalize();
               data.nx = static_cast<float>(v.X());
               data.ny = static_cast<float>(v.Y());
               data.nz = static_cast<float>(v.Z());
               data.color[0] = static_cast<GLubyte>(data.nx * 255);
               data.color[1] = static_cast<GLubyte>(data.ny * 255);
               data.color[2] = static_cast<GLubyte>(data.nz * 255);

               vecVertexData.push_back(data);

               // add triangle points
               vecIndices.push_back(MapAngleToIndex(iLat, iLong));
               vecIndices.push_back(MapAngleToIndex(iLat + 5, iLong));
               vecIndices.push_back(MapAngleToIndex(iLat, iLong + 5));

               vecIndices.push_back(MapAngleToIndex(iLat, iLong));
               vecIndices.push_back(MapAngleToIndex(iLat, iLong + 5));
               vecIndices.push_back(MapAngleToIndex(iLat + 5, iLong + 5));

               //         glNormal3fv(&data.nx);
               //         glVertex3fv(&data.x);
            }
         glEnd();

         OpenGL::VertexBufferObject<float, GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW_ARB> vbo;
         vbo.Generate();
         vbo.Bind();
         vbo.Upload(reinterpret_cast<float*>(&vecVertexData[0]), vecVertexData.size() * 32 / sizeof(float));

         OpenGL::IndexBufferObject16 ibo;
         ibo.Generate();
         ibo.Bind();
         ibo.Upload(&vecIndices[0], vecIndices.size());


         glEnableClientState(GL_VERTEX_ARRAY);

         glVertexPointer(3, GL_FLOAT, 32, (GLvoid*)offsetof(VertexData, x));
         glNormalPointer(GL_FLOAT, 32, (GLvoid*)offsetof(VertexData, nx));
         glColorPointer(3, GL_UNSIGNED_BYTE/*GL_3_BYTES*/, 32, (GLvoid*)offsetof(VertexData, color));

         glDrawElements(GL_TRIANGLES, vecIndices.size(), GL_UNSIGNED_SHORT, NULL);

         //   glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
         //   glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
         glDisableClientState(GL_VERTEX_ARRAY);


         rc.SwapBuffers();

         // wait for key
         // get another event
         SDL_Event evt;
         bool bExit = false;
         while (!bExit)
         {
            while (SDL_PollEvent(&evt))
            {
               if (evt.type == SDL_KEYDOWN)
                  bExit = true;
            }
            Sleep(0);
         }
      }
   };

} // namespace UnitTest
