
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This code was donated by Steven Watanabe during the review, and contains
    tests for the overloads.

    I suspect these tests are invalid and do nothing now, since I've had to
    remove the types from the mpl::vector.
*/

#include <boost/xint/integer.hpp>
#include <boost/mpl/vector.hpp>

#ifndef BOOST_XINT_SINGLE_TEST_PROGRAM
    #define BOOST_TEST_MAIN
#endif
#include <boost/test/unit_test.hpp>


struct any {
    template <class T>
    any(const T&) {}
};

struct no_overload {};

no_overload operator+(any, any) { return no_overload(); }
no_overload operator-(any, any) { return no_overload(); }
no_overload operator*(any, any) { return no_overload(); }
no_overload operator/(any, any) { return no_overload(); }
no_overload operator%(any, any) { return no_overload(); }

no_overload operator|(any, any) { return no_overload(); }
no_overload operator&(any, any) { return no_overload(); }
no_overload operator^(any, any) { return no_overload(); }

no_overload operator==(any, any) { return no_overload(); }
no_overload operator!=(any, any) { return no_overload(); }
no_overload operator<(any, any) { return no_overload(); }
no_overload operator>(any, any) { return no_overload(); }
no_overload operator<=(any, any) { return no_overload(); }
no_overload operator>=(any, any) { return no_overload(); }

no_overload operator~(any) { return no_overload(); }

template<class T>
boost::test_tools::predicate_result not_overloaded(const T&) {
    if(typeid(T) == typeid(no_overload)) return true;
    else {
        boost::test_tools::predicate_result result(false);
        //result.message() =
        return result;
    }
}

#define CHECK_NOT_OVERLOADED(expr)                                             \
    BOOST_CHECK_MESSAGE(typeid(expr) == typeid(no_overload),                   \
        "Unexpected overload found for expression: " #expr)

struct unrelated_type {};

typedef boost::mpl::vector<
    //std::string,
    //const char*,
    //unrelated_type
> other_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_arithmetic, T, other_types) {
    boost::xint::integer i;
    T arg = T();

    CHECK_NOT_OVERLOADED(i + arg);
    CHECK_NOT_OVERLOADED(arg + i);

    CHECK_NOT_OVERLOADED(i - arg);
    CHECK_NOT_OVERLOADED(arg - i);

    CHECK_NOT_OVERLOADED(i * arg);
    CHECK_NOT_OVERLOADED(arg * i);

    CHECK_NOT_OVERLOADED(i / arg);
    CHECK_NOT_OVERLOADED(arg / i);

    CHECK_NOT_OVERLOADED(i % arg);
    CHECK_NOT_OVERLOADED(arg % i);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_bitwise, T, other_types) {
    boost::xint::integer i;
    T arg = T();

    CHECK_NOT_OVERLOADED(i & arg);
    CHECK_NOT_OVERLOADED(arg & i);

    CHECK_NOT_OVERLOADED(i | arg);
    CHECK_NOT_OVERLOADED(arg | i);

    CHECK_NOT_OVERLOADED(i ^ arg);
    CHECK_NOT_OVERLOADED(arg ^ i);
}

BOOST_AUTO_TEST_CASE(test_complement) {
    boost::xint::integer i;
    CHECK_NOT_OVERLOADED(~i);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_compare, T, other_types) {
    boost::xint::integer i;
    T arg = T();

    CHECK_NOT_OVERLOADED(i == arg);
    CHECK_NOT_OVERLOADED(arg == i);

    CHECK_NOT_OVERLOADED(i != arg);
    CHECK_NOT_OVERLOADED(arg != i);

    CHECK_NOT_OVERLOADED(i < arg);
    CHECK_NOT_OVERLOADED(arg < i);

    CHECK_NOT_OVERLOADED(i > arg);
    CHECK_NOT_OVERLOADED(arg > i);

    CHECK_NOT_OVERLOADED(i <= arg);
    CHECK_NOT_OVERLOADED(arg <= i);

    CHECK_NOT_OVERLOADED(i >= arg);
    CHECK_NOT_OVERLOADED(arg >= i);
}
