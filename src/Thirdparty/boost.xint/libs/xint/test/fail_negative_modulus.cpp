
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    This code was donated by Steven Watanabe during the review. It should fail
    because you can only specify negative_modulus on a fixed-length type.
*/

#include <boost/xint/integer.hpp>

int main() {
    boost::xint::integer_t<boost::xint::options::negative_modulus> should_fail;
}
