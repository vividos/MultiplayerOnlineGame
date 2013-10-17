
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This code was donated by Steven Watanabe during the review.
*/

#include <boost/xint/integer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>

typedef boost::mpl::vector<
    char, signed char, unsigned char,
    short, unsigned short,
    int, unsigned,
    long, unsigned long
#ifndef BOOST_NO_LONG_LONG
    , boost::long_long_type, boost::ulong_long_type
#endif
> integer_types;

using boost::xint::integer;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_arithmetic, T, integer_types) {
    boost::mt19937 random;
    boost::uniform_int<T> dist((std::numeric_limits<T>::min)(),
        (std::numeric_limits<T>::max)());
    for(int i = 0; i < 100; ++i) {
        integer n = integer::random_by_size(random, boost::xint::detail::bits_per_digit*4);
        T m = dist(random);

        integer expected_sum = n + integer(m);
        integer sum1 = n + m;
        integer sum2 = m + n;
        BOOST_CHECK_EQUAL(sum1, expected_sum);
        BOOST_CHECK_EQUAL(sum2, expected_sum);

        integer expected_difference1 = n - integer(m);
        integer expected_difference2 = integer(m) - n;
        integer difference1 = n - m;
        integer difference2 = m - n;
        BOOST_CHECK_EQUAL(difference1, expected_difference1);
        BOOST_CHECK_EQUAL(difference2, expected_difference2);

        integer expected_product = n * integer(m);
        integer product1 = n * m;
        integer product2 = m * n;
        BOOST_CHECK_EQUAL(product1, expected_product);
        BOOST_CHECK_EQUAL(product2, expected_product);

        if(m != 0) {
            integer expected_quotient1 = n / integer(m);
            integer quotient1 = n / m;
            BOOST_CHECK_EQUAL(quotient1, expected_quotient1);

            integer expected_modulus1 = n % integer(m);
            integer modulus1 = n % m;
            BOOST_CHECK_EQUAL(modulus1, expected_modulus1);
        }
        if(n != 0) {
            integer expected_quotient2 = integer(m) / n;
            integer quotient2 = m / n;
            BOOST_CHECK_EQUAL(quotient2, expected_quotient2);

            integer expected_modulus2 = integer(m) % n;
            integer modulus2 = m % n;
            BOOST_CHECK_EQUAL(modulus2, expected_modulus2);
        }
    }
}
