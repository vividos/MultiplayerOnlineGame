//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Flags8.hpp Flags value with 8 bit
//
#pragma once

/// flags value with 8 bit
class Flags8
{
public:
   /// base type
   typedef unsigned char BaseType;

   /// ctor
   Flags8(unsigned char ucValue = 0) throw()
      :m_ucValue(ucValue)
   {
   }

   /// returns flags value
   unsigned char Value() const throw() { return m_ucValue; }

   /// returns a bit in the flags
   template <unsigned int NBit>
   bool GetBit() const throw()
   {
      return (m_ucValue & (1 << NBit)) != 0;
   }

   /// sets a bit in the flags
   template <unsigned int NBit>
   void SetBit(bool bBit) throw()
   {
      if (bBit)
         m_ucValue |= (1 << NBit);
      else
         m_ucValue &= ~(1 << NBit);
   }

private:
   unsigned char m_ucValue; ///< flags value
};
