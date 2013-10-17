
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This file contains the tests for the addition and subtraction functions.
*/

#include <boost/xint/integer.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

#include <iostream>

namespace boost {
namespace xint {

using std::endl;
using std::dec;
using std::hex;
namespace xopts = boost::xint::options;

namespace {

void _test(int section, int test, integer n, integer m) {
    integer answer(m + n);
    if (answer - m != n || answer - n != m) {
        std::ostringstream out;
        out << "testAddSubtract, section " << section << ", " << test << endl;

        out << "n     = " << n;
        if (section==0) out << " (" << hex << n << dec << ')';
        out << endl;

        out << "m     = " << m;
        if (section==0) out << " (" << hex << m << dec << ')';
        out << endl;

        out << "a     = " << answer;
        if (section==0) out << " (" << hex << answer << dec << ')';
        out << endl << endl;

        out << "a - m = " << (answer-m)
            << (answer-m == n ? " (good)" : " (should be n, isn't)")
            << endl;
        out << "a - n = " << (answer-n)
            << (answer-n == m ? " (good)" : " (should be m, isn't)")
            << endl;
        BOOST_FAIL(out.str());
    }
}

} // namespace

BOOST_AUTO_TEST_CASE(test_add_subtract) {
    boost::mt19937 random(42u);

    {
        integer n("-3000000080000000600000000", 16),
            m("8000000090000000600000000", 16);
        _test(0, 0, n, m);
    }

    for (int i=0; i<10000; ++i) {
        integer n = integer::random_by_size(random, detail::bits_per_digit*4);
        integer m = integer::random_by_size(random, detail::bits_per_digit*4);
        _test(1, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n = integer::random_by_size(random, detail::bits_per_digit*4,
            false, false,true);
        integer m = integer::random_by_size(random, detail::bits_per_digit*4,
            false, false, true);
        _test(2, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n = integer::random_by_size(random, detail::bits_per_digit*4,
            false, false, true);
        integer m = integer::random_by_size(random, detail::bits_per_digit*3,
            false, false, true);
        _test(3, i, n, m);
    }

    for (int i=0; i<1000; ++i) {
        integer n = integer::random_by_size(random, detail::bits_per_digit*3,
            false, false, true);
        integer m = integer::random_by_size(random, detail::bits_per_digit*4,
            false, false, true);
        _test(4, i, n, m);
    }

    // Test the += and -= functions
    {
        integer n("123456789abcdef0123456789abcdef", 16), n2;
        integer m("123456789abcdef0", 16);
        integer sum("123456789ABCDEF13579BE02468ACDF", 16);

        n2 = n;
        n2 += m;
        BOOST_CHECK_EQUAL(n2, sum);

        n2 -= m;
        BOOST_CHECK_EQUAL(n2, n);
    }

    // Test the auto-convert-literals stuff. Don't really care about the test
    // results, it's whether or not this compiles that matters.
    {
        integer n1(10), n2 = n1 + 5, n3 = 5 + n2;
        BOOST_CHECK_EQUAL(n2, 15);
        BOOST_CHECK_EQUAL(n3, 20);
    }
}

BOOST_AUTO_TEST_CASE(test_negative_zero) {
    integer n(5), m(5), a(n-m);
    BOOST_CHECK_EQUAL(a, 0);
    BOOST_CHECK_EQUAL(a.sign(), 0);
    BOOST_CHECK_EQUAL(a.sign(true), 1);

    a=-a;
    BOOST_CHECK_EQUAL(a, 0);
    BOOST_CHECK_EQUAL(a.sign(), 0);
    BOOST_CHECK_EQUAL(a.sign(true), -1);
}

BOOST_AUTO_TEST_CASE(test_increment_variable) {
    integer one(1), zero(0), negative_one(-1);
    BOOST_CHECK_EQUAL(++one, 2);
    BOOST_CHECK_EQUAL(++zero, 1);
    BOOST_CHECK_EQUAL(++negative_one, 0);

    integer max1(detail::digit_mask);
    BOOST_CHECK_EQUAL(++max1, integer(detail::digit_overflowbit));
}

BOOST_AUTO_TEST_CASE(test_increment_fixed) {
    typedef integer_t<xopts::fixedlength<detail::bits_per_digit> > T;
    T one(1), zero(0), negative_one(-1);
    BOOST_CHECK_EQUAL(++one, 2);
    BOOST_CHECK_EQUAL(++zero, 1);
    BOOST_CHECK_EQUAL(++negative_one, 0);

    T max1(detail::digit_mask);
    BOOST_CHECK_EQUAL(++max1, 0);
}

BOOST_AUTO_TEST_CASE(test_decrement_variable) {
    integer one(1), zero(0), negative_one(-1);
    BOOST_CHECK_EQUAL(--one, 0);
    BOOST_CHECK_EQUAL(--zero, -1);
    BOOST_CHECK_EQUAL(--negative_one, -2);

    integer min1 = -integer(detail::digit_mask), expected =
        -integer(detail::digit_overflowbit);
    BOOST_CHECK_EQUAL(--min1, expected);
}

BOOST_AUTO_TEST_CASE(test_decrement_fixed) {
    typedef integer_t<xopts::fixedlength<detail::bits_per_digit> > T;
    T one(1), zero(0), negative_one(-1);
    BOOST_CHECK_EQUAL(--one, 0);
    BOOST_CHECK_EQUAL(--zero, -1);
    BOOST_CHECK_EQUAL(--negative_one, -2);

    T min1 = -T(detail::digit_mask);
    BOOST_CHECK_EQUAL(--min1, 0);
}

BOOST_AUTO_TEST_CASE(test_difference) {
    typedef integer_t<xopts::negative_not_allowed> T;
    T n1(3), n2(5);
    BOOST_CHECK_THROW(n1 - n2, xint::exceptions::cannot_represent);
    BOOST_CHECK_EQUAL(difference(n1, n2), 2);
    BOOST_CHECK_EQUAL(difference(n2, n1), 2);
}

} // namespace xint
} // namespace boost
