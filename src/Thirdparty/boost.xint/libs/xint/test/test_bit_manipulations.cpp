
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the functions that test the low-level bit manipulations.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <iomanip>

namespace boost {
namespace xint {

using std::hex;
using namespace boost::xint::options;

BOOST_AUTO_TEST_CASE(test_unfixed_bit_shifting) {
    const std::string nSrc="1a2b3c4d5e6f7f6e5d4c3b2a1a2b3c4d5e6f7f6e5d4c3b2a1";
    integer n(nSrc, 16), n2;

    // If this fails, abort the rest of the test.
    if (to_string(n, 16) != nSrc) BOOST_FAIL("failure in reading string, can't "
        "test");

    // Null-shifting

    BOOST_CHECK_EQUAL(n >> 0, n);
    BOOST_CHECK_EQUAL(n << 0, n);

    // Bits only

    n2 = n << 4;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc+'0');

    n2 >>= 4;
    BOOST_CHECK_EQUAL(n2, n);

    // Digits only

    const int cDigitsPerDigit=(detail::bits_per_digit / 4);
    n2 <<= detail::bits_per_digit;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc + std::string(cDigitsPerDigit, '0'));

    n2 >>= detail::bits_per_digit;
    BOOST_CHECK_EQUAL(n2, n);

    // Bits and digits simultaneously

    const int bits_per_digit2=(detail::bits_per_digit + detail::bits_per_digit/2);
    const int cDigitsPerDigit2=(bits_per_digit2 / 4);
    n2 <<= bits_per_digit2;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc + std::string(cDigitsPerDigit2, '0'));

    n2 >>= bits_per_digit2;
    BOOST_CHECK_EQUAL(n2, n);

    // Right-shift to oblivion

    n2 >>= (detail::bits_per_digit * n2._get_length());
    BOOST_CHECK_EQUAL(n2.sign(), 0);

    // Single-digit by bits

    n = n2 = 0xEF;
    n2 <<= 4;
    BOOST_CHECK_EQUAL(to_string(n2, 16), "ef0");

    n2 >>= 4;
    BOOST_CHECK_EQUAL(n2, n);
}

BOOST_AUTO_TEST_CASE(test_fixed_bit_shifting) {
    const std::string nSrc="1a2b3c4d5e6f7f6e5d4c3b2a1";
    typedef integer_t<fixedlength<100> > integer100;
    integer100 n(nSrc, 16), n2;

    // If this fails, abort the rest of the test.
    BOOST_REQUIRE_EQUAL(to_string(n, 16), nSrc);

    BOOST_MESSAGE("** Null-shifting");

    BOOST_CHECK_EQUAL(n >> 0, n);
    BOOST_CHECK_EQUAL(n << 0, n);

    BOOST_MESSAGE("** Bits only");

    n2 = n << 4;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(1)+'0');

    n2 >>= 4;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(1));

    BOOST_MESSAGE("** Digits only");

    const int cDigitsPerDigit=(detail::bits_per_digit / 4);
    n2 <<= detail::bits_per_digit;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(cDigitsPerDigit) +
        std::string(cDigitsPerDigit, '0'));

    n2 >>= detail::bits_per_digit;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(cDigitsPerDigit));

    n2 = n >> detail::bits_per_digit;
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(0, nSrc.length() -
        cDigitsPerDigit));

    BOOST_MESSAGE("** Bits & digits both");

    n2 = n << (detail::bits_per_digit + 4);
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(cDigitsPerDigit + 1) +
        std::string(cDigitsPerDigit + 1, '0'));

    n2 = n >> (detail::bits_per_digit + 4);
    BOOST_CHECK_EQUAL(to_string(n2, 16), nSrc.substr(0, nSrc.length() -
        cDigitsPerDigit -1));

    BOOST_MESSAGE("** Left-shift to oblivion");

    n2 <<= 100;
    BOOST_CHECK_EQUAL(to_string(n2, 16), "0");
}

BOOST_AUTO_TEST_CASE(test_bitwise_operations) {
    // These numbers look like mirror images, but they're deliberately not.
    integer n("0123456789abcdefedcba9876543210", 16),
        m("fedcba98765432100123456789abcdef", 16),
        ored("FEDEBEDE7EDEBEDEFFFFFFFFFFFFFFFF", 16),
        xored("FECE8ECE0ECE8ECEFFFFFFFFFFFFFFFF", 16),
        anded("103010701030100000000000000000", 16);

    BOOST_CHECK_EQUAL(m | n, ored);
    BOOST_CHECK_EQUAL(m ^ n, xored);
    BOOST_CHECK_EQUAL(m & n, anded);

    integer ne;
    ne = n; ne |= m; BOOST_CHECK_EQUAL(ne, ored);
    ne = n; ne ^= m; BOOST_CHECK_EQUAL(ne, xored);
    ne = n; ne &= m; BOOST_CHECK_EQUAL(ne, anded);

    BOOST_CHECK_EQUAL(lowestbit(n, 0), bitsize_t(4));
    BOOST_CHECK_EQUAL(highestbit(n, 0), bitsize_t(117));

    typedef integer_t<fixedlength<12> > integer12;
    integer12 nn(0xABC), mm(0x123);
    BOOST_CHECK_EQUAL(mm | nn, 0xBBF);
    BOOST_CHECK_EQUAL(mm ^ nn, 0xB9F);
    BOOST_CHECK_EQUAL(mm & nn, 0x20);
}

} // namespace xint
} // namespace boost
