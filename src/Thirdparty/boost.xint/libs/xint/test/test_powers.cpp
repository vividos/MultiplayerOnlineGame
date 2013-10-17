
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the tests for the power functions.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

using detail::bits_per_digit;
using namespace boost::xint::options;

BOOST_AUTO_TEST_CASE(test_square) {
    {
        const size_t bits = bits_per_digit;
        const size_t nybbles = bits / 4;
        integer src;
        for (size_t i = 0; i != nybbles; ++i)
            src = (src * 16) + ((i + 0x0a) % 16);

        integer squared = square(src);
        BOOST_CHECK_EQUAL(squared / src, src);
    }

    {
        const size_t bits = bits_per_digit + (bits_per_digit / 2);
        const size_t nybbles = bits / 4;
        integer src;
        for (size_t i = 0; i != nybbles; ++i)
            src = (src * 16) + ((i + 0x0a) % 16);

        integer squared = square(src);
        BOOST_CHECK_EQUAL(squared / src, src);
    }
}

BOOST_AUTO_TEST_CASE(test_pow) {
    {
        integer n(234), n2(pow(n, 2)), n3(pow(n, 3));
        BOOST_CHECK_EQUAL(n2, 54756);
        BOOST_CHECK_EQUAL(n3, 12812904);

        BOOST_CHECK_EQUAL(n2 / n, n);
        BOOST_CHECK_EQUAL(n3 / n / n, n);
    }

    {
        integer n(-234), n2(pow(n, 2)), n3(pow(n, 3));
        BOOST_CHECK_EQUAL(n2 / n, n);
        BOOST_CHECK_EQUAL(n3 / n / n, n);
    }

    {
        integer n(-235), n2(pow(n, 2)), n3(pow(n, 3));
        BOOST_CHECK_EQUAL(n2 / n, n);
        BOOST_CHECK_EQUAL(n3 / n / n, n);
    }

    {
        integer_t<fixedlength<8> > n(-235), n2(pow(n, 2)), n3(pow(n, 3));
        BOOST_CHECK_EQUAL(n2, 0xb9);
        BOOST_CHECK_EQUAL(n3, -0xd3);
    }
}

BOOST_AUTO_TEST_CASE(test_factorial) {
    integer i1 = integer::factorial(20);
    BOOST_CHECK_EQUAL(i1, integer("21C3677C82B40000", 16));

    typedef integer_t<fixedlength<12> > integer12;
    integer12 i2 = integer12::factorial(8);
    BOOST_CHECK_EQUAL(i2, 0xd80);
}

BOOST_AUTO_TEST_CASE(test_square_root) {
    integer n(1), i(1);
    while (n < 30) {
        integer target = square(n + 1);
        while (i < target) { BOOST_CHECK_EQUAL(square_root(i), n); ++i; }
        ++n;
    }
}

} // namespace xint
} // namespace boost
