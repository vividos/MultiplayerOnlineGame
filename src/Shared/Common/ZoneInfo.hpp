//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZoneInfo.hpp Zone infos
//
#pragma once

// needed includes
#include "Color.hpp"
#include "ByteStream.hpp"

/// info about a zone
class ZoneInfo
{
public:
   /// ctor
   ZoneInfo()
      :m_cszName("Zone0"),
       m_cBackgroundColor(255,255,255),
       m_fBrightness(1.0)
   {
   }

   // getter

   /// returns zone name
   const CString& Name() const { return m_cszName; }
   /// returns background color
   const Color BackgroundColor() const { return m_cBackgroundColor; }
   /// returns brightness
   float Brightness() const { return m_fBrightness; }

   // setter

   /// sets zone name
   void Name(const CString& cszName) { m_cszName = cszName; }
   /// sets background color
   void BackgroundColor(const Color& cBackgroundColor) { m_cBackgroundColor = cBackgroundColor; }
   /// sets brightness
   void Brightness(float fBrightness) { m_fBrightness = fBrightness; }

   // serialization

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const
   {
      stream.WriteString(m_cszName);
      stream.Write8(m_cBackgroundColor.m_color[Color::red]);
      stream.Write8(m_cBackgroundColor.m_color[Color::green]);
      stream.Write8(m_cBackgroundColor.m_color[Color::blue]);
      stream.Write8(static_cast<unsigned char>(m_fBrightness * 255.f));
   }

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream)
   {
      m_cszName = stream.ReadString(64);
      m_cBackgroundColor.m_color[Color::red] = stream.Read8();
      m_cBackgroundColor.m_color[Color::green] = stream.Read8();
      m_cBackgroundColor.m_color[Color::blue] = stream.Read8();
      m_fBrightness = stream.Read8() / 255.f;
   }

private:
   CString m_cszName;         ///< zone name
   Color m_cBackgroundColor;  ///< background color
   float m_fBrightness;       ///< brightness; [0.0; 1.0]
};
