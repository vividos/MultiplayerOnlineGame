//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/Uw2Decode.hpp Ultima Underworld 2 .ark compression format decoder
//
#pragma once

// includes

// forward references
namespace Stream
{
class IStream;
}

namespace Import
{

/// decodes uw2 data block from .ark file and returns stream with content
std::shared_ptr<Stream::IStream> Uw2Decode(std::shared_ptr<Stream::IStream> spStream, bool bCompressed, Uint32 uiDataSize);

} // namespace Import
