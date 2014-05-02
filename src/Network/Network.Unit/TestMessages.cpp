//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestMessages.cpp Unit tests for Message classes
//

// includes
#include "stdafx.h"
#include "LogoutMessage.hpp"
#include "ByteStream.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// someone forgot this overload in CppUnitTestAssert.h
template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<unsigned short>(const unsigned short& t) { RETURN_WIDE_STRING(t); }

namespace UnitTest
{
   /// tests all Message classes
   TEST_CLASS(TestMessages)
   {
      TEST_METHOD(TestAllMessages)
      {
         TestMessage<LogoutMessage>();
      }

      /// tests a message type
      template<typename TMessage>
      void TestMessage()
      {
         VectorStream in;
         unsigned short usMessageId = 0;
         {
            TMessage msg;
            usMessageId = msg.MessageId();
            msg.Serialize(in);
         }

         ConstVectorRefStream out(in.Data());
         {
            TMessage msg;
            msg.Deserialize(out);

            Assert::AreEqual(usMessageId, msg.MessageId(), _T("Message IDs must be equal"));
         }
      }
   };

} // namespace UnitTest
