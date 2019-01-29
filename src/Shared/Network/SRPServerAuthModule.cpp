//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SRPServerAuthModule.cpp SRP Server authentication module
//

// includes
#include "StdAfx.h"
#include "SRPServerAuthModule.hpp"
#include "AuthException.hpp"
#include "RawMessage.hpp"
#include "SRPAuthMessages.hpp"
#include "RC4EncryptModule.hpp"
#include "SRPServer.hpp"
#include "SRPHelper.hpp"
#include "LogCategories.hpp"
#include <ulib/HighResolutionTimer.hpp>

using SRP::ServerAuthModule;

ServerAuthModule::ServerAuthModule(T_fnGetServerAuthInfo fnGetServerAuthInfo)
:m_fnGetServerAuthInfo(fnGetServerAuthInfo),
 m_bIsVerifiedClient(false),
 m_iAccountId(0)
{
}

void ServerAuthModule::InitAuthentication()
{
   ATLASSERT(m_bIsVerifiedClient == false); // shouldn't be called when already authenticated

   // sever does nothing initially
}

bool ServerAuthModule::IsAuthenticated() const throw()
{
   return m_bIsVerifiedClient;
}

bool ServerAuthModule::OnReceiveMessage(RawMessage& msg)
{
   ATLASSERT(m_bIsVerifiedClient == false); // shouldn't be called when already authenticated

   switch (msg.MessageId())
   {
   case msgSRPAuthRequest:
      OnMessageAuthRequest(msg);
      break;

   case msgSRPAuthVerifyRequest:
      OnMessageVerifyClient(msg);
      break;

   default:
      return false;
   }

   return true;
}

std::shared_ptr<IEncryptModule> ServerAuthModule::GetEncryptModule()
{
   ATLASSERT(m_bIsVerifiedClient == true);
   return m_spEncryptModule;
}

void ServerAuthModule::OnMessageAuthRequest(RawMessage& rawMessage)
{
   if (m_bIsVerifiedClient)
      throw AuthException(AuthException::authInternalError, _T("client already verified"), __FILE__, __LINE__);
   if (m_spServer != NULL)
      throw AuthException(AuthException::authInternalError, _T("server == NULL"), __FILE__, __LINE__);

   AuthRequestMessage authMessage;
   ConstVectorRefStream stream(rawMessage.Data());
   authMessage.Deserialize(stream);

   // get server infos
   ATLASSERT(m_fnGetServerAuthInfo != NULL);

   TByteArray vecPasswordKey, vecSalt;
   try
   {
      m_fnGetServerAuthInfo(authMessage.GetUsername(), vecPasswordKey, vecSalt, m_iAccountId);
   }
   catch(const Exception& ex)
   {
      LOG_WARN(_T("caught exception during GetServerAuthInfo(): ") + ex.Message(), Log::Server::AuthSRP);

      throw AuthException(
         AuthException::authFailed, _T("couldn't get auth info: ") + ex.Message(),
         __FILE__, __LINE__);
   }
   catch(...)
   {
      throw AuthException(
         AuthException::authFailed, _T("couldn't get auth info"), __FILE__, __LINE__);
   }

   HighResolutionTimer timer;
   timer.Start();

   SRP::GroupParameter gp = SRP::Helper::GetPresetGroupParameter(c_uiDefaultPresetGroupParameter);
   m_spServer.reset(new SRP::Server(gp));

   // generate b
   BigInteger b = SRP::Helper::GenerateRandomBits(1024);

   BigInteger PassVerifier = SRP::Helper::ToInteger<BigInteger>(vecPasswordKey);

   // calculate B
   BigInteger B = m_spServer->GetB(PassVerifier, b);
   TByteArray Bbin;
   SRP::Helper::ToBinaryData(B, Bbin);

   // calculate session key
   TByteArray Abin = authMessage.GetA();
   BigInteger A = SRP::Helper::ToInteger<BigInteger>(Abin);

   BigInteger s = SRP::Helper::ToInteger<BigInteger>(vecSalt);

   USES_CONVERSION;
   std::string strUsername(T2CA(authMessage.GetUsername()));

   m_spServer->CalcSessionKey(A, s, strUsername, PassVerifier);

   // log
   CString cszText;
   cszText.Format(_T("calculating session key took %u ms"), unsigned(timer.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Server::AuthSRP);

   // send response
   SRPAuthResponseMessage authResponseMessage(vecSalt, Bbin);
   SendMessage(authResponseMessage);
}

void ServerAuthModule::OnMessageVerifyClient(RawMessage& rawMessage)
{
   if (m_bIsVerifiedClient)
      throw AuthException(AuthException::authInternalError, _T("client already verified"), __FILE__, __LINE__);
   if (m_spServer == NULL)
      throw AuthException(AuthException::authInternalError, _T("server == NULL"), __FILE__, __LINE__);

   SRPAuthVerifyClientMessage verifyClientMessage;
   ConstVectorRefStream stream(rawMessage.Data());
   verifyClientMessage.Deserialize(stream);

   HighResolutionTimer timer;
   timer.Start();

   // check client; when failed, client doesn't know password
   TByteArray Ms;
   try
   {
      Ms = m_spServer->VerifyClient(verifyClientMessage.GetHc());
   }
   catch(std::runtime_error& ex)
   {
      throw AuthException(
         AuthException::authFailed, _T("couldn't verify client") + CString(ex.what()),
         __FILE__, __LINE__);
   }

   m_bIsVerifiedClient = true;

   // log
   CString cszText;
   cszText.Format(_T("verifying client took %u ms"), unsigned(timer.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Server::AuthSRP);

   // send server our hash
   SRPAuthVerifyServerMessage verifyServerMessage(Ms);
   SendMessage(verifyServerMessage);

   // get server session key
   TByteArray K = m_spServer->GetK();

   m_spServer.reset(); // no longer needed

   // set up RC4 encryption
   m_spEncryptModule.reset(new RC4::EncryptModule(K));
}
