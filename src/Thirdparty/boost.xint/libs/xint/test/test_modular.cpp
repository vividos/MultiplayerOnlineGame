
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains tests for the modular math functions.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

BOOST_AUTO_TEST_CASE(test_mod) {
    integer n(0x12345678);
    BOOST_CHECK_EQUAL(n % 0x1000, 0x678);

    {
        integer n("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
                m( "7c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e( "494BFA011E0060811AC144653446065CBDBAADF", 16);

        BOOST_CHECK_EQUAL(n % m, e);
    }

    {
        integer n("-2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
                m(  "7c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e(  "32F9078E116DB95EA4804C6138E5CCA94347B40", 16);

        BOOST_CHECK_EQUAL(n % m, e);
    }

    {
        integer n("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
                m("-7c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e( "494BFA011E0060811AC144653446065CBDBAADF", 16);

        BOOST_CHECK_EQUAL(n % m, e);
    }

    {
        integer n("-2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
                m( "-7c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e(  "32F9078E116DB95EA4804C6138E5CCA94347B40", 16);

        BOOST_CHECK_EQUAL(n % m, e);
    }

    {
        integer n("-6c1ab0d083d36569fb112ce738ca2e9d78b0a93d381fa9ab41aa72ece7512f4f310307b01a407d7", 16),
                m("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e("47C45018F2F6E19DFBF4190C66D2BD306010261D", 16);

        BOOST_CHECK_EQUAL(n % m, e);
    }

    // Test the mod-equal version too
    {
        integer n("-6c1ab0d083d36569fb112ce738ca2e9d78b0a93d381fa9ab41aa72ece7512f4f310307b01a407d7", 16),
                m("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
                e("47C45018F2F6E19DFBF4190C66D2BD306010261D", 16);
        n %= m;
        BOOST_CHECK_EQUAL(n, e);
    }
}

BOOST_AUTO_TEST_CASE(test_invmod) {
    // When a number is multiplied modularly by its inverse, the result should
    // be one.
    integer n("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16),
      modulus("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16);
    integer inverse = invmod(n, modulus);
    BOOST_CHECK_EQUAL(mulmod(n, inverse, modulus), 1);

    inverse = invmod(-n, modulus);
    BOOST_CHECK_EQUAL(mulmod(-n, inverse, modulus), 1);
}

} // namespace xint
} // namespace boost
