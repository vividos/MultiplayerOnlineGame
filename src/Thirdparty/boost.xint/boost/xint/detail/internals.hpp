
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
    \brief Contains the internal definitions required for all XInt types.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_INTERNALS_HPP
#define BOOST_INCLUDED_XINT_INTERNALS_HPP

#if defined(BOOST_XINT_NO_EXCEPTIONS) || defined(BOOST_NO_EXCEPTIONS)
    #define BOOST_XINT_TRY if (1)
    #define BOOST_XINT_CATCH_BADALLOC else if (0)
    #define BOOST_XINT_CATCH else
    #define BOOST_XINT_CATCH_E else
#else
    #define BOOST_XINT_TRY try
    #define BOOST_XINT_CATCH_BADALLOC catch (std::bad_alloc&)
    #define BOOST_XINT_CATCH catch (std::exception&)
    #define BOOST_XINT_CATCH_E catch (std::exception& e)
#endif

#include "../exceptions.hpp"
#include "../random.hpp"

#include "basic_types_and_includes.hpp"
#include "bitqueue.hpp"
#include "policies.hpp"
#include "magnitude_manager.hpp"
#include "raw_integer.hpp"

#include "log2.hpp"
#include "shift.hpp"
#include "bitfunctions.hpp"
#include "andorxor.hpp"
#include "randomgen.hpp"
#include "random_by_size.hpp"
#include "compare.hpp"

#include "addsubtract.hpp"
#include "multiply.hpp"
#include "divide.hpp"

#include "convert.hpp"
#include "streams.hpp"

#include "roots.hpp"
#include "powers.hpp"
#include "gcd.hpp"
#include "monty.hpp"
#include "modular.hpp"
#include "prime.hpp"

//! \namespace boost::xint
//! \brief All the types and functions of the library are within this namespace.

//! \namespace boost::xint::detail
//! \brief The items within this namespace are meant for internal use only.

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

inline std::size_t log10_bits(std::size_t bits) {
    // large_step and count_per_large_step are based on a minimum-32-bit integer
    assert(std::numeric_limits<boost::intmax_t>::digits >= 32);

    typedef raw_integer_t<0, false, std::allocator<digit_t> > T;
    const T large_step(1000000000), small_step(10);
    const std::size_t count_per_large_step = 9, count_per_small_step = 1;

    T n;
    pow2(n, bits);
    --n;

    std::size_t r = 0;
    while (n >= large_step) { n /= large_step; r += count_per_large_step; }
    while (n >= small_step) { n /= small_step; r += count_per_small_step; }
    return r;
}

template <typename charT>
const std::basic_string<charT>& nan_text() {
    static std::basic_string<charT> r;
    if (r.empty()) {
        std::basic_ostringstream<charT> out;
        out << "#NaN#";
        r = out.str();
    }
    return r;
}

template <typename charT>
bool is_nan_text(const std::basic_string<charT> &str) {
    return (str == nan_text<charT>());
}

template <typename T>
T get_nan() {
    static T r;
    static bool init = false;

    if (!init) {
        init = true;
        r._data().make_nan();
    }

    return BOOST_XINT_MOVE(r);
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#include "options.hpp"

#endif // BOOST_INCLUDED_XINT_INTERNALS_HPP
