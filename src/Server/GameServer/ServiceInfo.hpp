//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServiceInfo.hpp Service infos
//
#pragma once

namespace Win32
{

/// service info for registering or implementing a service
class ServiceInfo
{
public:
   /// ctor
   ServiceInfo(const CString& cszServiceName,
      const CString& cszDisplayName,
      const CString& cszDescription,
      const CString& cszLogCategory,
      const CString& cszCommandLine)
      :m_cszServiceName(cszServiceName),
       m_cszDisplayName(cszDisplayName),
       m_cszDescription(cszDescription),
       m_cszLogCategory(cszLogCategory),
       m_cszCommandLine(cszCommandLine)
   {
   }

   // get methods

   LPCTSTR ServiceName() const { return m_cszServiceName; } ///< returns service name
   LPCTSTR DisplayName() const { return m_cszDisplayName; } ///< returns service display name
   LPCTSTR Description() const { return m_cszDescription; } ///< returns service description

   LPCTSTR LogCategory() const { return m_cszLogCategory; } ///< returns log category

   LPCTSTR CommandLine() const { return m_cszCommandLine; } ///< returns command line

private:
   CString m_cszServiceName; ///< service name
   CString m_cszDisplayName; ///< service display name
   CString m_cszDescription; ///< service description
   CString m_cszLogCategory; ///< log category
   CString m_cszCommandLine; ///< command line
};

} // namespace Win32
