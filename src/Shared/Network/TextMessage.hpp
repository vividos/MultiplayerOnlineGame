//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TextMessage.hpp Message for sending/receiving text
//
#pragma once

// includes
#include "Message.hpp"
#include "ByteStream.hpp"

/// max length of message text
const unsigned int c_uiMaxTextMessageLength = 256;

/// max length of message recipient
const unsigned int c_uiMaxTextRecipientLength = 64;

/// \brief text message
/// \details text message, e.g. chat, say, yell, whisper, party, guild, zone
/// or world message. message can be sent in both directions.
class TextMessage: public Message
{
public:
   /// text type
   enum T_enTextMessageType
   {
      textMsgInvalid,///< invalid type
      textMsgSay,    ///< say message; players nearby receive this
      textMsgYell,   ///< say message; players nearby receive this; bigger radius
      textMsgWhisper,///< whisper to another player; recipient must be filled
      textMsgParty,  ///< party channel message
      textMsgGuild,  ///< guild channel message
      textMsgZone,   ///< zone message; used for admin only
      textMsgWorld,  ///< world message; used for admin only

      textMsgMax = textMsgWorld
   };

   /// ctor; used for sending
   TextMessage(T_enTextMessageType enTextMessageType, const CString& cszText, const CString& cszRecipient = CString()) throw()
      :Message(msgText),
       m_enTextMessageType(enTextMessageType),
       m_cszText(cszText),
       m_cszRecipient(enTextMessageType == textMsgWhisper ? cszRecipient : _T(""))
   {
      if (enTextMessageType == textMsgWhisper)
         ATLASSERT(!cszRecipient.IsEmpty());
      else
         ATLASSERT(cszRecipient.IsEmpty());
   }

   /// ctor; used for receiving
   TextMessage() throw()
      :Message(msgText),
       m_enTextMessageType(textMsgInvalid)
   {
   }

   // dtor
   virtual ~TextMessage() throw() {}

   // get methods

   /// text message type
   T_enTextMessageType Type() const throw() { return m_enTextMessageType; }

   /// returns message text
   const CString& Text() const throw() { return m_cszText; }

   /// returns message recipient
   const CString& Recipient() const throw()
   {
      ATLASSERT(m_enTextMessageType == textMsgWhisper);
      return m_cszRecipient;
   }

   // serialization

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const
   {
      stream.Write8(static_cast<unsigned char>(m_enTextMessageType & 0xff));
      stream.WriteString(m_cszText);
      if (m_enTextMessageType == textMsgWhisper)
         stream.WriteString(m_cszRecipient);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream)
   {
      m_enTextMessageType = static_cast<T_enTextMessageType>(stream.Read8());
      if (m_enTextMessageType > textMsgMax || m_enTextMessageType == textMsgInvalid)
         throw Exception(_T("invalid text message type"), __FILE__, __LINE__);

      m_cszText = stream.ReadString(c_uiMaxTextMessageLength);
      if (m_enTextMessageType == textMsgWhisper)
         m_cszRecipient = stream.ReadString(c_uiMaxTextRecipientLength);
   }

private:
   /// text message type
   T_enTextMessageType m_enTextMessageType;

   /// message text
   CString m_cszText;

   /// message recipient; only for textMsgWhisper
   CString m_cszRecipient;
};
