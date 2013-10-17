
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
    \brief Contains the definitions for the bit-manipulation functions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_BITFUNCTIONS_HPP
#define BOOST_INCLUDED_XINT_BITFUNCTIONS_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
bool getbit(const BOOST_XINT_RAWINT &n, std::size_t bit_index) {
    std::size_t index = bit_index / bits_per_digit;
    if (index < n.length) {
        digit_t mask = (digit_t(1) << (bit_index % bits_per_digit));
        return ((n[index] & mask) != 0);
    } else return false;
}

BOOST_XINT_RAWINT_TPL
void setbit(BOOST_XINT_RAWINT &n, std::size_t bit_index) {
    if (Bits != 0 && bit_index >= (n.max_length() * bits_per_digit)) return;
    std::size_t index = (bit_index / bits_per_digit);
    digit_t mask = (digit_t(1) << (bit_index % bits_per_digit));
    n.digits(index + 1, realloc::extend)[index] |= mask;
    if (n.length < index + 1) n.length = index + 1;
    n.trim();
}

BOOST_XINT_RAWINT_TPL
void clearbit(BOOST_XINT_RAWINT &n, std::size_t bit_index) {
    std::size_t index = (bit_index / bits_per_digit);
    if (index >= n.length) return; // Bit can't be set, no need to clear it.
    digit_t mask = (digit_t(1) << (bit_index % bits_per_digit));
    n.digits(0)[index] &= ~mask;
    n.trim();
}

BOOST_XINT_RAWINT_TPL
size_t lowestbit(const BOOST_XINT_RAWINT &n, std::size_t valueIfZero) {
    if (n.is_zero()) return valueIfZero;

    const digit_t *d = n.digits(), *p = d, *pe = p + n.length;
    while (p != pe && *p == 0) ++p;
    std::size_t r = (bits_per_digit * (p - d));

    digit_t digit = *p;
    while ((digit & 0x01)==0) { digit >>= 1; ++r; }

    return r;
}

BOOST_XINT_RAWINT_TPL
size_t highestbit(const BOOST_XINT_RAWINT &n, std::size_t valueIfZero) {
    if (n.is_zero()) return valueIfZero;
    return ((n.length - 1) * bits_per_digit) + log2(n[n.length - 1]);
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT BOOST_XINT_RAWINT::operator~() const {
    if (Bits == 0) {
        exception_handler<>::call(__FILE__, __LINE__,
            exceptions::too_big("operator~ on unlimited integers results in "
            "an infinitely long number."));
    } else {
        BOOST_XINT_RAWINT r;
        const digit_t *s = digits(), *se = s + length;
        digit_t *t = r.digits(r.max_length(), realloc::extend), *te = t +
            r.max_length();
        while (s != se) *t++ = ~(*s++);
        while (t != te) *t++ = ~digit_t(0);
        r.trim();
        return r;
    }
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_BITFUNCTIONS_HPP
