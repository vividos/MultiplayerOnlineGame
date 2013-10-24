//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SRPClientAuthModule.cpp SRP Client authentication module
//

// includes
#include "StdAfx.h"
#include "SRPClientAuthModule.hpp"
#include "AuthException.hpp"
#include "RawMessage.hpp"
#include "SRPAuthMessages.hpp"
#include "RC4EncryptModule.hpp"
#include "SRPClient.hpp"
#include "SRPHelper.hpp"
#include "LogCategories.hpp"
#include "HighResolutionTimer.hpp"

using SRP::ClientAuthModule;

ClientAuthModule::ClientAuthModule(const AuthInfo& authInfo)
:m_bIsVerifiedServer(false),
 m_authInfo(authInfo)
{
   SRP::GroupParameter gp = SRP::Helper::GetPresetGroupParameter(c_uiDefaultPresetGroupParameter);

   m_spClient.reset(new SRP::Client(gp));
}

void ClientAuthModule::InitAuthentication()
{
   ATLASSERT(m_spClient != NULL);
   ATLASSERT(m_bIsVerifiedServer == false); // shouldn't be called when already authenticated

   HighResolutionTimer timer;
   timer.Start();

   // generate a
   BigInteger a = SRP::Helper::GenerateRandomBits(1024);

   // generate A
   BigInteger A = m_spClient->GetA(a);
   SRP::TByteArray Abin;
   SRP::Helper::ToBinaryData(A, Abin);

   // log
   CString cszText;
   cszText.Format(_T("calculating A took %u ms"), unsigned(timer.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Client::AuthSRP);

   // send first message
   AuthRequestMessage msg(m_authInfo.Username(), Abin);
   SendMessage(msg);
}

bool ClientAuthModule::IsAuthenticated() const throw()
{
   return m_bIsVerifiedServer;
}

bool ClientAuthModule::OnReceiveMessage(RawMessage& msg)
{
   ATLASSERT(m_bIsVerifiedServer == false); // shouldn't be called when already authenticated

   switch (msg.MessageId())
   {
   case msgSRPAuthResponse:
      OnMessageAuthResponse(msg);
      break;

   case msgSRPAuthVerifyResponse:
      OnMessageVerifyServer(msg);
      break;

   default:
      return false;
   }

   return true;
}

std::shared_ptr<IEncryptModule> ClientAuthModule::GetEncryptModule()
{
   ATLASSERT(m_bIsVerifiedServer == true);
   return m_spEncryptModule;
}

void ClientAuthModule::OnMessageAuthResponse(RawMessage& rawMessage)
{
   if (m_bIsVerifiedServer)
      throw AuthException(AuthException::authInternalError, _T("server already verified"), __FILE__, __LINE__);
   if (m_spClient == NULL)
      throw AuthException(AuthException::authInternalError, _T("client == NULL"), __FILE__, __LINE__);

   SRPAuthResponseMessage authResponseMessage;
   ConstVectorRefStream stream(rawMessage.Data());
   authResponseMessage.Deserialize(stream);

   // now enter verify stage of login...

   HighResolutionTimer timer;
   timer.Start();

   USES_CONVERSION;
   std::string strUsername(T2CA(m_authInfo.Username()));
   std::string strPassword(T2CA(m_authInfo.Password()));

   BigInteger s = SRP::Helper::ToInteger<BigInteger>(authResponseMessage.GetSalt());
   BigInteger B = SRP::Helper::ToInteger<BigInteger>(authResponseMessage.GetB());

   SRP::TByteArray Mc = m_spClient->CalcM(strUsername, strPassword, s, B);

   // log
   CString cszText;
   cszText.Format(_T("calculating Mc took %u ms"), unsigned(timer.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Client::AuthSRP);

   // send server our hash Mc
   SRPAuthVerifyClientMessage authVerifyMessage(Mc);
   SendMessage(authVerifyMessage);
}

void ClientAuthModule::OnMessageVerifyServer(RawMessage& rawMessage)
{
   if (m_bIsVerifiedServer)
      throw AuthException(AuthException::authInternalError, _T("server already verified"), __FILE__, __LINE__);
   if (m_spClient == NULL)
      throw AuthException(AuthException::authInternalError, _T("client == NULL"), __FILE__, __LINE__);

   SRPAuthVerifyServerMessage verifyMessage;
   ConstVectorRefStream stream(rawMessage.Data());
   verifyMessage.Deserialize(stream);

   HighResolutionTimer timer;
   timer.Start();

   // client: check server; when failed, server doesn't know password verifier
   if (!m_spClient->VerifyServer(verifyMessage.GetHs()))
      throw AuthException(AuthException::authFailed, _T("couldn't verify server"), __FILE__, __LINE__);

   m_bIsVerifiedServer = true;

   // get client session key
   SRP::TByteArray Kc = m_spClient->GetK();

   // log
   CString cszText;
   cszText.Format(_T("calculating session key took %u ms"), unsigned(timer.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Client::AuthSRP);

   m_spClient.reset(); // no longer needed

   // set up RC4 encryption
   m_spEncryptModule.reset(new RC4::EncryptModule(Kc));
}
