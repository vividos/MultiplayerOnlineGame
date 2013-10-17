
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

namespace boost {
namespace xint {
namespace detail {

template<class T>
void this_function_should_not_be_found_by_adl(const T&) {}

}
}
}

int main() {
    namespace opt = boost::xint::options;
    //using namespace boost::xint::options;
    boost::xint::integer_t<
        opt::threadsafe,
        opt::fixedlength<10>,
        opt::negative_modulus,
        opt::nothrow,
        opt::secure
    > test;
    this_function_should_not_be_found_by_adl(test);
}
