//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file SRPCommon.hpp Common defines for SRP classes
//
#pragma once

// includes
#include <vector>
#pragma warning(push)
#pragma warning(disable: 4005) // 'STRICT' : macro redefinition
#pragma warning(disable: 4456) // declaration of 'ee' hides previous local declaration
#pragma warning(disable: 4512) // 'T' : assignment operator could not be generated
#include <boost/xint/xint.hpp>
#pragma warning(pop)

/// \brief classes to implement SRP-6a algorithm using boost::xint
/// \see http://srp.stanford.edu/design.html
/// \see http://tools.ietf.org/html/rfc5054
/// \see http://tools.ietf.org/html/rfc2945
/// \details
/// SRP authentication works as follows:
/// - Server stores username I, password verifier v and used salt s in a database. This
///   is done when the account is created.
/// - When a client wants to log in, it calculates public key A from private random number a.
/// - The server calculates a public key B from password verifier v and private random number b.
///   The server must have received A before sending out B.
/// - Server and client can now calculate a shared secret K that isn't known to anyone, and
///   that can be used to seed an encryption algorithm like RC4 after authentication.
/// - Client must calculate a hash Mc of K and other parameters and shows it to the server.
/// - The server calculates Mc too, and checks the received value. It aborts when unequal.
/// - The server calculates its own proof Ms and shows it to the client.
/// - The server has to verify Mc before sending out Ms.
/// - The client calculates Ms too, and checks the received value. It aborts when unequal.
/// - Authentication is complete.
///
/// Here's a sequence diagram which infos have to be sent:
/// \msc
///    Client,Server;
///    Client->Server [label="sends username I, public key A"];
///    Client<-Server [label="sends Salt, public key B"];
///    Client->Server [label="sends computed hash Mc"];
///    Client<-Server [label="sends computed hash Ms"];
/// \endmsc
namespace SRP
{

/// type for holding large-integer bits; needed for data transfer
typedef boost::xint::binary_t BinaryData;

/// type for hash numbers; always 160 bits
typedef boost::xint::integer_t<
   boost::xint::options::fixedlength<160>,
   boost::xint::options::copy_on_write,
   boost::xint::options::negative_modulus> HashNumber;

/// type for unlimited big integer numbers
typedef boost::xint::integer BigInteger;
//typedef boost::xint::integer_t<boost::xint::options::copy_on_write> BigInteger;

/// modulus and generator values
struct GroupParameter
{
   BigInteger N; ///< modulus value
   BigInteger g; ///< generator value
   unsigned int uiNumBits; ///< number of bits in N
};

} // namespace SRP
