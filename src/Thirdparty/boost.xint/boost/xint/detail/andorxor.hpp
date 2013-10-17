
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
    \brief Contains the functions for ANDing, ORing, and XORing.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_ANDORXOR_HPP
#define BOOST_INCLUDED_XINT_ANDORXOR_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator&=(const BOOST_XINT_RAWINT &n) {
    std::size_t len = (std::min)(length, n.length);
    const digit_t *ns = n.digits(), *nse = ns + len;
    digit_t *t = digits(len, realloc::extend);
    while (ns != nse) *t++ &= *ns++;
    length = len;
    trim();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator|=(const BOOST_XINT_RAWINT &n) {
    std::size_t len = (std::max)(length, n.length);
    if (length > n.length) {
        const digit_t *ns = n.digits(), *nse = ns + n.length;
        digit_t *t = digits(len, realloc::extend);
        while (ns != nse) *t++ |= *ns++;
    } else {
        const digit_t *ns = n.digits(), *nse = ns + len;
        digit_t *t = digits(len, realloc::extend), *te = t + length;
        while (t != te) *t++ |= *ns++;
        while (ns != nse) *t++ = *ns++;
        length = len;
    }
    trim();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator^=(const BOOST_XINT_RAWINT &n) {
    std::size_t len = (std::max)(length, n.length);
    if (length > n.length) {
        const digit_t *ns = n.digits(), *nse = ns + n.length;
        digit_t *t = digits(len, realloc::extend);
        while (ns != nse) *t++ ^= *ns++;
    } else {
        const digit_t *ns = n.digits(), *nse = ns + len;
        digit_t *t = digits(len, realloc::extend), *te = t + length;
        while (t != te) *t++ ^= *ns++;
        while (ns != nse) *t++ = *ns++;
        length = len;
    }
    trim();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator&(BOOST_XINT_RAWINT n1, const BOOST_XINT_RAWINT &n2) {
    return n1 &= n2;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator|(BOOST_XINT_RAWINT n1, const BOOST_XINT_RAWINT &n2) {
    return n1 |= n2;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator^(BOOST_XINT_RAWINT n1, const BOOST_XINT_RAWINT &n2) {
    return n1 ^= n2;
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_ANDORXOR_HPP
