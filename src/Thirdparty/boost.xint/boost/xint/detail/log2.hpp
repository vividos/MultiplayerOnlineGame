
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
    \brief Contains the log2 implementations.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_LOG2_HPP
#define BOOST_INCLUDED_XINT_LOG2_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

inline std::size_t log2_base(boost::uintmax_t n, std::size_t maxbits) {
    boost::uintmax_t lo = 0, hi = maxbits;
    while (lo < hi) {
        boost::uintmax_t s = lo + ((hi - lo) >> 1);
        if (n >= (boost::uintmax_t(1) << s)) lo = s + 1;
        else hi = s;
    }
    return std::size_t(lo);
}

template <typename T>
std::size_t log2_helper(const T n,
    typename boost::enable_if<boost::is_unsigned<T> >::type* = 0)
{
    return log2_base(n, std::numeric_limits<T>::digits);
}

template <typename T>
std::size_t log2_helper(const T n,
    typename boost::enable_if<boost::is_signed<T> >::type* = 0)
{
    typedef typename boost::make_unsigned<T>::type uT;
    T nn = (n < 0 ? -n : n);
    return log2_base(static_cast<uT>(nn), std::numeric_limits<T>::digits);
}

template <typename T>
std::size_t log2(const T n,
    typename boost::enable_if<boost::is_integral<T> >::type* = 0)
{
    return log2_helper(n);
}

BOOST_XINT_RAWINT_TPL
std::size_t log2(const BOOST_XINT_RAWINT &n) {
    std::size_t r = bits_per_digit * (n.length - 1);
    return r + log2(n[n.length - 1]);
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_LOG2_HPP
