
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Contains tests that don't fit elsewhere.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

namespace boost {
namespace xint {

using namespace boost::xint::options;
typedef integer_t<fixedlength<8> > integer8;
typedef integer_t<fixedlength<16> > integer16;
typedef integer_t<fixedlength<32> > integer32;

BOOST_AUTO_TEST_CASE(test_log2) {
    // Check the detail::log2 template functions, unsigned and signed
    unsigned int u = (static_cast<unsigned int>(1) <<
        (std::numeric_limits<unsigned int>::digits - 1));
    BOOST_CHECK_EQUAL(detail::log2(u), static_cast<std::size_t>(
        std::numeric_limits<unsigned int>::digits));
    int s = (1 << (std::numeric_limits<int>::digits - 1));
    BOOST_CHECK_EQUAL(detail::log2(s), static_cast<std::size_t>(
        std::numeric_limits<int>::digits));

    // Check the log2 function for the integer class
    integer n(0x87654321);
    n <<= detail::bits_per_digit;
    size_t expected = 32 + detail::bits_per_digit;
    while (expected != 0) {
        n >>= 1;
        --expected;
        BOOST_CHECK_EQUAL(integer_log2(n), expected);
    }
}

BOOST_AUTO_TEST_CASE(test_gcd) {
    // These two numbers share no factors.
    integer n("47c45018f2f6e19dfbf4190c66d2bd306010261f", 16),
            m("2b6a501cd0b26e1dfd70918455621257ac2c697a", 16);
    BOOST_CHECK_EQUAL(gcd(n, m), 1);
    BOOST_CHECK_EQUAL(gcd(n * 127, m * 127), 127);
}

BOOST_AUTO_TEST_CASE(test_lcm) {
    integer n(1307), m(1663), q(3571); // Arbitrarily-selected prime numbers
    BOOST_CHECK_EQUAL(lcm(n * q, m * q), n * m * q);
}

BOOST_AUTO_TEST_CASE(test_is_prime) {
    // Not a prime number
    BOOST_CHECK(!is_probably_prime(integer("6404588795")));

    // A prime number
    BOOST_CHECK(is_probably_prime(integer("6404588797")));
}

BOOST_AUTO_TEST_CASE(test_numeric_limits) {
    BOOST_CHECK_EQUAL(std::numeric_limits<integer>::digits, 0);
    BOOST_CHECK_EQUAL(std::numeric_limits<integer>::digits10, 0);

    // Fixed-length integers

    BOOST_CHECK_EQUAL(std::numeric_limits<integer8>::digits,
        std::numeric_limits<boost::uint8_t>::digits);
    BOOST_CHECK_EQUAL(std::numeric_limits<integer8>::digits10,
        std::numeric_limits<boost::uint8_t>::digits10);

    BOOST_CHECK_EQUAL(std::numeric_limits<integer16>::digits10,
        std::numeric_limits<boost::uint16_t>::digits10);
    BOOST_CHECK_EQUAL(std::numeric_limits<integer32>::digits10,
        std::numeric_limits<boost::uint32_t>::digits10);

    const size_t maxbits = std::numeric_limits<boost::uintmax_t>::digits;
    BOOST_CHECK_EQUAL(std::numeric_limits<integer_t<fixedlength<maxbits>
        > >::digits10, std::numeric_limits<boost::uintmax_t>::digits10);

    {
        const size_t bits = detail::bits_per_digit + (detail::bits_per_digit /
            2);
        typedef integer_t<fixedlength<bits> > T; // Digit-and-a-half
        const size_t digits10 = std::numeric_limits<T>::digits10;

        T n(1);
        for (size_t i = 0; i < digits10; ++i) n *= 10;

        T n2(n * 10);
        integer check(n);
        check *= 10;

        // n2 should be truncated to a smaller number
        BOOST_CHECK_NE(integer(n2), check);
    }
}

BOOST_AUTO_TEST_CASE(test_random_by_size) {
    default_random_generator gen;
    integer n = integer::random_by_size(gen, 512, true);
    BOOST_CHECK_EQUAL(integer_log2(n), static_cast<std::size_t>(512));
    n = integer::random_by_size(gen, 512, true, true);
    BOOST_CHECK_EQUAL(integer_log2(n), static_cast<std::size_t>(512));
}

BOOST_AUTO_TEST_CASE(test_unsigned_negative_policies) {
    {
        // Test signed behavior
        typedef integer_t<negative_allowed> T;
        T t;
        BOOST_CHECK_EQUAL(--t, -1);
    }

    {
        // Test unsigned variable-length default
        typedef integer_t<negative_not_allowed> T;
        T t;
        BOOST_CHECK_THROW(--t, xint::exceptions::cannot_represent);
    }

    {
        // Test unsigned fixedlength default
        typedef integer_t<fixedlength<12>, negative_not_allowed> T;
        T t;
        BOOST_CHECK_EQUAL(--t, (std::numeric_limits<T>::max)());
    }

    {
        // Test unsigned fixedlength modulus
        typedef integer_t<fixedlength<12>, negative_modulus> T;
        T t;
        BOOST_CHECK_EQUAL(--t, (std::numeric_limits<T>::max)());
    }

    // Can't test unsigned variable-length modulus, because if it's working
    // right, it can't be compiled.

    {
        // Test exception on standard integers
        typedef integer_t<negative_exception> T;
        T t;
        BOOST_CHECK_THROW(--t, xint::exceptions::cannot_represent);
    }

    {
        // Test exception on nothrow integers
        typedef integer_t<negative_exception, nothrow> T;
        T t;
        --t;
        BOOST_CHECK(t.is_nan());
    }

    {
        // Test force-to-zero
        typedef integer_t<negative_to_zero> T;
        T t;
        BOOST_CHECK_EQUAL(--t, 0);
    }

    {
        // Test force-to-absolute
        typedef integer_t<negative_absolute> T;
        T t;
        BOOST_CHECK_EQUAL(--t, 1);
    }
}

} // namespace xint
} // namespace boost
