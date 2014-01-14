//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RawMessage.hpp Raw serialized message
//
#pragma once

// includes
#include <vector>
#include "Message.hpp"
#include "ByteStream.hpp"

/// raw message; used for deserializing
class RawMessage: public Message
{
public:
   /// ctor
   RawMessage(unsigned short usMessageId, const unsigned char* pData, size_t uiLength) throw()
      :Message(usMessageId),
       m_vecData(pData, pData + uiLength)
   {
   }

   /// ctor
   RawMessage(unsigned short usMessageId, const std::vector<unsigned char>& vecData) throw()
      :Message(usMessageId),
       m_vecData(vecData)
   {
   }

   /// dtor
   virtual ~RawMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.WriteBlock(m_vecData);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      stream.ReadBlock(&m_vecData[0], m_vecData.size());
   }

   /// returns raw data
   const std::vector<unsigned char>& Data() const throw() { return m_vecData; }

private:
   /// raw data
   std::vector<unsigned char> m_vecData;
};
