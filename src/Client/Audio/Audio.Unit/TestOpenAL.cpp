//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestOpenAL.cpp Unit tests for classes in namespace OpenAL
//

// includes
#include "stdafx.h"
#include "OpenAL.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests classes in namespace OpenAL
TEST_CLASS(TestOpenAL)
{
   /// tests DefaultDeviceName()
   TEST_METHOD(TestDefaultDeviceName)
   {
      CString cszDeviceName = OpenAL::Device::DefaultDeviceName();
      Assert::IsTrue(!cszDeviceName.IsEmpty());
   }

   /// tests EnumDevices()
   TEST_METHOD(TestEnumDevices)
   {
      std::vector<CString> vecDevices = OpenAL::Device::EnumDevices();
      Assert::IsTrue(!vecDevices.empty());

      for (size_t i=0,iMax=vecDevices.size(); i<iMax; i++)
         ATLTRACE(_T("Device #%u: %s"), i, vecDevices[i].GetString());
   }

   /// tests Device default ctor
   TEST_METHOD(TestDeviceDefaultCtor)
   {
      OpenAL::Device d1, d2;
   }

   /// tests DopplerFactor() and SpeedOfSound()
   TEST_METHOD(TestReadDeviceProperties1)
   {
      OpenAL::Device d;

      float fDopplerFactor = d.DopplerFactor();
      float fSpeedOfSound = d.SpeedOfSound();

      Assert::IsTrue(fDopplerFactor >= 0.0);
      Assert::IsTrue(fSpeedOfSound > 300.0);

      ATLTRACE(_T("doppler factor: %2.1f"), fDopplerFactor);
      ATLTRACE(_T("speed of sound: %2.1f m/s"), fSpeedOfSound);
   }

   /// tests Vendor(), Version() and Renderer()
   TEST_METHOD(TestReadDeviceProperties2)
   {
      OpenAL::Device d;

      Assert::IsTrue(d.Vendor().IsEmpty() == FALSE);
      Assert::IsTrue(d.Version().IsEmpty() == FALSE);
      Assert::IsTrue(d.Renderer().IsEmpty() == FALSE);

      ATLTRACE(_T("vendor: [%s], version: [%s], renderer: [%s]"),
         d.Vendor().GetString(), d.Version().GetString(), d.Renderer().GetString());
   }

   /// tests GetListener()
   TEST_METHOD(TestGetListener)
   {
      OpenAL::Device d;
      OpenAL::Listener l = d.GetListener();

      float fGain = l.Gain();
      Assert::IsTrue(fGain >= 0.f);
      ATLTRACE(_T("listener gain: %2.1f"), fGain);
   }

   /// tests CreateSource()
   TEST_METHOD(TestCreateSource)
   {
      OpenAL::Device d;
      OpenAL::SourcePtr spSource = d.CreateSource();
      spSource->Attach(OpenAL::BufferPtr());
   }

   /// tests CreateBuffer()
   TEST_METHOD(TestCreateBuffer)
   {
      OpenAL::Device d;
      OpenAL::BufferPtr spBuffer = d.CreateBuffer();
   }

   /// tests CreateBuffer() and CreateSource()
   TEST_METHOD(TestCreateBufferAndSource)
   {
      OpenAL::Device d;

      OpenAL::BufferPtr spBuffer = d.CreateBuffer();

      OpenAL::SourcePtr spSource = d.CreateSource();
      spSource->Attach(spBuffer);
   }
};

} // namespace UnitTest
