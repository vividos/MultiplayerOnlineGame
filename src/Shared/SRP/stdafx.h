//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file src/SRPxint/stdafx.h Precompiled header support
//
#pragma once

// includes
#define ULIB_ASIO_NO_WINSOCK
#include "Base.hpp"

#pragma warning(push)
#pragma warning(disable: 4005) // 'STRICT' : macro redefinition
#pragma warning(disable: 4512) // 'T' : assignment operator could not be generated
#include <boost/xint/xint.hpp>
#pragma warning(pop)
