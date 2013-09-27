//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/Bits.hpp Bit functions
//
#pragma once

// includes

namespace Import
{

// inline functions

/// retrieves bits from given value
inline Uint8 GetBits(Uint8 uiValue, unsigned int uiStart, unsigned int uiCount)
{
   UaAssert(uiStart < 8 && uiStart + uiCount <= 8);
   return static_cast<Uint8>(uiValue >> uiStart) & static_cast<Uint8>((1 << uiCount)-1);
}

/// retrieves bits from given value
inline Uint16 GetBits(Uint16 uiValue, unsigned int uiStart, unsigned int uiCount)
{
   UaAssert(uiStart < 16 && uiStart + uiCount <= 16);
   return static_cast<Uint16>(uiValue >> uiStart) & static_cast<Uint16>((1 << uiCount)-1);
}

/// retrieves bits from given value
inline Uint32 GetBits(Uint32 uiValue, unsigned int uiStart, unsigned int uiCount)
{
   UaAssert(uiStart < 32 && uiStart + uiCount <= 32);
   return (uiValue >> uiStart) & ((1 << uiCount)-1);
}

} // namespace Import
