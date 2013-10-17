
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    Previously, this file contained the function to run all of the self-tests
    for the library. Since converting to Boost.Test, it's simply a place to
    declare the auto-generated main function, or a custom one.

    Note that not all functions have a test; all the primitives do, but the rest
    only warrant testing if I'm refactoring them, or if I had problems while
    writing them or in production.
*/

//#define CUSTOM_TEST
//#define PERFORMANCE_TEST

#if defined PERFORMANCE_TEST
    #include <boost/xint/xint.hpp>
    #include <iostream>
    #include <iomanip>

    // To ensure that the compiler doesn't optimize away important parts of the
    // tests.
    std::size_t counter;
    #define USE_NUMBER(n) counter += n._data().max_length()

    // Select the integer type to use in the tests.
    namespace xopts = boost::xint::options;
    //typedef boost::xint::integer_t<xopts::threadsafe<false> > T;
    typedef boost::xint::integer_t<> T;

    // For running comparative tests with different options
    int main() {
        const size_t bits = 2048, add_rounds = 10000, mult_rounds = 10000,
            mulmod_rounds = 1000;
        const size_t raw_number_pairs = 10;

        using namespace std;
        using namespace boost::xint;

        cout << "Generating numbers..." << endl;
        default_random_generator gen;
        std::vector<T> n1, n2;
        for (size_t x = 0; x < raw_number_pairs; ++x) {
            n1.push_back(T::random_by_size(gen, bits, true, true));
            n2.push_back(T::random_by_size(gen, bits, true, true));
        }
        const T nmod = T::random_by_size(gen, bits, true, true);

        cout << "Running addition test" << flush;
        for (size_t x = 0; x < 10; ++x) {
            for (size_t y = 0; y < add_rounds / 10; ++y) {
                for (size_t round = 0; round < raw_number_pairs; ++round) {
                    T r = n1[round] + n2[round];
                    USE_NUMBER(r);
                }
            }
            cout << "." << flush;
        }
        cout << endl;

        cout << "Running multiplication test" << flush;
        for (size_t x = 0; x < 10; ++x) {
            for (size_t y = 0; y < mult_rounds / 10; ++y) {
                for (size_t round = 0; round < raw_number_pairs; ++round) {
                    T r = n1[round] * n2[round];
                    USE_NUMBER(r);
                }
            }
            cout << "." << flush;
        }
        cout << endl;

        cout << "Running mulmod test" << flush;
        for (size_t x = 0; x < 10; ++x) {
            for (size_t y = 0; y < mulmod_rounds / 10; ++y) {
                for (size_t round = 0; round < raw_number_pairs; ++round) {
                    T r = mulmod(n1[round], n2[round], nmod);
                    USE_NUMBER(r);
                }
            }
            cout << "." << flush;
        }
        cout << endl;
    }
#elif defined CUSTOM_TEST
    #include <iostream>
    #include <iomanip>
    #include <boost/xint/integer.hpp>

    // For running one-off tests
    int main() {
        using namespace std;
        using namespace boost::xint;
        namespace xopts = boost::xint::options;

        typedef integer_t<std::allocator<detail::digit_t>,
            xopts::threadsafe<false>, xopts::fixedlength<12>,
            xopts::secure<true> >::type T;
        T::type t(0xFFFF);
        cout << hex << t << endl;
    }
#else
    #define BOOST_TEST_MAIN
    #include <boost/test/unit_test.hpp>
#endif
