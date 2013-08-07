//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SRPAuthMessages.hpp Messages for SRP authentication
//
#pragma once

// includes
#include "Message.hpp"
#include "ByteStream.hpp"

/// Messages are exchanged in this way when authenticating with SRP:
/// \msc
///    Client,Server;
///    Client->Server [label="SRP::AuthRequestMessage, sends username, A", URL="\ref SRP::AuthRequestMessage"];
///    Client<-Server [label="SRP::SRPAuthResponseMessage, sends Salt, B", URL="\ref SRP::SRPAuthResponseMessage"];
///    Client->Server [label="SRP::SRPAuthVerifyClientMessage, sends Hc", URL="\ref SRP::SRPAuthVerifyClientMessage"];
///    Client<-Server [label="SRP::SRPAuthVerifyServerMessage, sends Hs", URL="\ref SRP::SRPAuthVerifyServerMessage"];
/// \endmsc
/// There may be an initial message from server to negotiate protocol,
/// encryption and/or version check, but it's not part of SRP authentication.
namespace SRP
{

/// max. length of username
const unsigned int c_uiMaxUsernameLength = 16;

/// max. length of salt field
const unsigned int c_uiMaxSaltLength = 32;

/// fixed length of A and B values
const unsigned int c_uiABLength = 128;

/// length of shared key (SHA-1 length)
const unsigned int c_uiSharedKeyLength = 20;

/// default preset index for group parameters; uses 1024-bit prime N
const unsigned int c_uiDefaultPresetGroupParameter = 0;

/// byte array type
typedef std::vector<unsigned char> TByteArray;


/// \brief SRP auth request message
/// \details sends username, value for A
class AuthRequestMessage: public Message
{
public:
   /// ctor; use for receiving
   AuthRequestMessage()
      :Message(msgSRPAuthRequest)
   {
   }

   /// ctor; use for sending
   AuthRequestMessage(const CString& cszUsername, const TByteArray& A) throw()
      :Message(msgSRPAuthRequest),
       m_cszUsername(cszUsername),
       m_A(A)
   {
   }

   /// dtor
   virtual ~AuthRequestMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      // write username
      stream.WriteString(m_cszUsername);
      stream.WriteBlock(m_A);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      m_cszUsername = stream.ReadString(c_uiMaxUsernameLength);
      // TODO check username for invalid characters
      stream.ReadBlock(m_A, c_uiABLength);
   }

   // get methods

   /// returns username
   const CString& GetUsername() const throw() { return m_cszUsername; }

   /// returns A value
   const TByteArray& GetA() const throw() { return m_A; }

private:
   CString m_cszUsername;  ///< username
   TByteArray m_A;         ///< A value
};

/// \brief SRP auth request message
/// \details sends salt, value for B
class SRPAuthResponseMessage: public Message
{
public:
   /// ctor; use for receiving
   SRPAuthResponseMessage() throw()
      :Message(msgSRPAuthResponse)
   {
   }

   /// ctor; use for sending
   SRPAuthResponseMessage(const TByteArray& Salt, const TByteArray& B) throw()
      :Message(msgSRPAuthResponse),
       m_Salt(Salt),
       m_B(B)
   {
   }

   /// dtor
   virtual ~SRPAuthResponseMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      ATLASSERT(!m_Salt.empty() && m_Salt.size() <= c_uiMaxSaltLength);

      stream.Write8(static_cast<unsigned char>(m_Salt.size()));
      stream.WriteBlock(m_Salt);
      stream.WriteBlock(m_B);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      unsigned int uiLen = stream.Read8();
      if (uiLen == 0 || uiLen > c_uiMaxSaltLength)
         throw AuthException(AuthException::authInternalError,
            _T("SRPAuthResponseMessage invalid salt length"), __FILE__, __LINE__);

      stream.ReadBlock(m_Salt, uiLen);
      stream.ReadBlock(m_B, c_uiABLength);
   }

   // get methods

   /// returns salt value
   const TByteArray& GetSalt() const throw() { return m_Salt; }

   /// returns B value
   const TByteArray& GetB() const throw() { return m_B; }

private:
   TByteArray m_Salt;   ///< salt value
   TByteArray m_B;      ///< B value
};

/// \brief SRP verify client message
/// \details sends value for Hc
class SRPAuthVerifyClientMessage: public Message
{
public:
   /// ctor; use for receiving
   SRPAuthVerifyClientMessage() throw()
      :Message(msgSRPAuthVerifyRequest)
   {
   }

   /// ctor; use for sending
   SRPAuthVerifyClientMessage(const TByteArray& Hc) throw()
      :Message(msgSRPAuthVerifyRequest),
       m_Hc(Hc)
   {
   }

   /// dtor
   virtual ~SRPAuthVerifyClientMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.WriteBlock(m_Hc);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      stream.ReadBlock(m_Hc, c_uiSharedKeyLength);
   }

   // get methods

   /// returns client hash value
   const TByteArray& GetHc() const throw() { return m_Hc; }

private:
   TByteArray m_Hc;  ///< client hash value
};

/// \brief SRP verify server message
/// \details sends value for Hs
class SRPAuthVerifyServerMessage: public Message
{
public:
   /// ctor; use for receiving
   SRPAuthVerifyServerMessage() throw()
      :Message(msgSRPAuthVerifyResponse)
   {
   }

   /// ctor; use for sending
   SRPAuthVerifyServerMessage(const TByteArray& Hs) throw()
      :Message(msgSRPAuthVerifyResponse),
       m_Hs(Hs)
   {
   }

   /// dtor
   virtual ~SRPAuthVerifyServerMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.WriteBlock(m_Hs);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      stream.ReadBlock(m_Hs, c_uiSharedKeyLength);
   }

   // get methods

   /// returns server hash value
   const TByteArray& GetHs() const throw() { return m_Hs; }

private:
   TByteArray m_Hs;  ///< server hash value
};

} // namespace SRP
