
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
    \brief Contains tests for the integer_t type's initializations.
*/

#include <boost/xint/integer.hpp>

#include <iomanip>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using namespace std;
using boost::test_tools::output_test_stream;

namespace boost {
namespace xint {

using namespace boost::xint::options;
typedef integer_t<fixedlength<8> > integer8;
typedef integer_t<fixedlength<12> > integer12;
typedef integer_t<fixedlength<100> > integer100;

BOOST_AUTO_TEST_CASE(test_digit_mask) {
    // See whether the mask is correct at the boundaries.
    {
        const size_t bits = detail::bits_per_digit - 1;
        typedef integer_t<fixedlength<bits> > T;
        detail::doubledigit_t t1 = (detail::doubledigit_t(1) << bits) - 1;
        T t(detail::doubledigit_t(0) - 1);
        detail::doubledigit_t t2 = to<detail::doubledigit_t>(t);
        BOOST_CHECK_EQUAL(t1, t2);
    }

    {
        const size_t bits = detail::bits_per_digit;
        typedef integer_t<fixedlength<bits> > T;
        detail::doubledigit_t t1 = (detail::doubledigit_t(1) << bits) - 1;
        T t(detail::doubledigit_t(0) - 1);
        detail::doubledigit_t t2 = to<detail::doubledigit_t>(t);
        BOOST_CHECK_EQUAL(t1, t2);
    }

    {
        const size_t bits = detail::bits_per_digit + 1;
        typedef integer_t<fixedlength<bits> > T;
        detail::doubledigit_t t1 = (detail::doubledigit_t(1) << bits) - 1;
        T t(detail::doubledigit_t(0) - 1);
        detail::doubledigit_t t2 = to<detail::doubledigit_t>(t);
        BOOST_CHECK_EQUAL(t1, t2);
    }
}

BOOST_AUTO_TEST_CASE(test_fixed_from_nan) {
    typedef integer12 T;
    BOOST_CHECK_THROW(T i(nothrow_integer::nan()), exceptions::not_a_number);
}

BOOST_AUTO_TEST_CASE(test_fixed_from_string) {
    integer src("0x12345678901234567890", autobase);
    typedef integer12 T;
    T i(src);
    BOOST_CHECK_EQUAL(i, 0x890);
}

BOOST_AUTO_TEST_CASE(test_fixed_from_integer) {
    const std::string original("0x123456789012345678901234567890");
    const std::string trimmed("0x6789012345678901234567890");
    integer src(original, autobase);

    typedef integer100 T;
    T i(src);

    output_test_stream output;
    output << hex << showbase << i;
    BOOST_CHECK(output.is_equal(trimmed));

    BOOST_CHECK_EQUAL(i, T(trimmed, autobase));
    BOOST_CHECK_EQUAL(integer(i), integer(trimmed, autobase));
}

BOOST_AUTO_TEST_CASE(test_fixed_from_fixed) {
    const std::string original("0x123456789012345678901234567890");
    typedef integer12 T12;
    typedef integer100 T100;

    T100 original_int(original, autobase);
    T12 smaller(original_int);
    T100 larger(smaller);
    larger = T100(original, autobase);

    BOOST_CHECK_EQUAL(smaller, 0x890);
    BOOST_CHECK_EQUAL(larger, original_int);
}

BOOST_AUTO_TEST_CASE(test_integer_from_fixed) {
    integer12 n(0x890);
    integer i(n);
    BOOST_CHECK_EQUAL(i, 0x890);
}

BOOST_AUTO_TEST_CASE(test_unary_operators) {
    integer8 zero(0), one(1);
    BOOST_CHECK_EQUAL(!zero, true);
    BOOST_CHECK_EQUAL(!one, false);

    BOOST_CHECK_EQUAL(+one, 1);
    BOOST_CHECK_EQUAL(-one, -1);

    // Digit-and-a-half
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef integer_t<fixedlength<bits>, negative_not_allowed> T;
    boost::uintmax_t usrc = 0x12345678;
    T t(usrc);
    BOOST_CHECK_EQUAL(~t, ~usrc);

    const std::size_t int32bits = 31; // 31 bits for magnitude, 1 for sign
    typedef integer_t<fixedlength<int32bits>, negative_allowed> U;
    boost::int32_t ssrc = 0x12345678, exp = -0x6DCBA987;
    U u(ssrc);
    BOOST_CHECK_EQUAL(~u, exp);
}

BOOST_AUTO_TEST_CASE(test_fixed_add_subtract) {
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef integer_t<fixedlength<bits> > T; // Digit-and-a-half

    T n1(detail::digit_mask), n2;
    n2 = n1;
    BOOST_CHECK_EQUAL(n1, detail::digit_mask);
    BOOST_CHECK_EQUAL(n2, n1);

    T n3 = n1 + n2;

    integer check = (integer(n1) + integer(n2)) & (integer::pow2(bits) - 1);
    BOOST_CHECK_EQUAL(integer(n3), check);

    typedef integer8 T8;
    T8 n4 = T8(-255) - 2;
    BOOST_CHECK_EQUAL(n4, -1);

    n4 = 255;
    n4 += 2;
    BOOST_CHECK_EQUAL(n4, 1);
}

BOOST_AUTO_TEST_CASE(test_unsigned_negative_modulus) {
    const size_t bits = detail::bits_per_digit + (detail::bits_per_digit / 2);
    typedef integer_t<fixedlength<bits>, negative_not_allowed> T;

    const T max_value(boost::uintmax_t(-1));
    T t(0);
    BOOST_CHECK_EQUAL(--t, max_value);
    BOOST_CHECK_EQUAL(T(1) - 3, max_value - 1);
    BOOST_CHECK_EQUAL(T("-1"), max_value);
}

} // namespace xint
} // namespace boost
