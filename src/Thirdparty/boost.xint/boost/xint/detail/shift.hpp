
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
    \brief Contains the bit-shifting function definitions.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_SHIFT_HPP
#define BOOST_INCLUDED_XINT_SHIFT_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator<<=(std::size_t byBits) {
    if (byBits == 0) return *this;

    // bits/dchunks is distance to move
    std::size_t dchunks = byBits / bits_per_digit, bits = byBits %
        bits_per_digit;
    std::size_t anslen = length + dchunks + (bits != 0 ? 1 : 0);
    digit_t *dstart = digits(anslen);

    // nlen is one greater than top source byte to move
    std::size_t nlen = (std::min)(length, anslen - dchunks - (bits != 0 ? 1 :
        0));

    if (bits != 0 && dchunks + 1 < anslen) {
        // Handle both bits and dchunks in one pass
        const digit_t *s = dstart + nlen - 1;
        digit_t *t = dstart + nlen + dchunks, *te = dstart + max_length();
        length = (std::min<size_t>)(t - dstart + 1, max_length());

        while (t >= te && s > dstart) { --t; --s; }
        if (t < te) *t-- = *s >> (bits_per_digit - bits); else --t;
        while (s > dstart) {
            *t = (*s-- << bits);
            *t-- |= (*s >> (bits_per_digit - bits));
        }
        *t-- = (*s << bits);

        while (t >= dstart) *t-- = 0;
    } else if (dchunks != 0 && dchunks < anslen) {
        length = anslen;
        memmove(dstart + dchunks, dstart, sizeof(digit_t) * nlen);
        memset(dstart, 0, dchunks * sizeof(digit_t));
    } else {
        set(0);
    }

    trim();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT& BOOST_XINT_RAWINT::operator>>=(std::size_t byBits) {
    if (byBits == 0) return *this;

    std::size_t dchunks = byBits / bits_per_digit, bits = byBits %
        bits_per_digit, bits2 = bits_per_digit - bits;

    digit_t *tgt = digits(0);
    if (dchunks < length) {
        if (bits != 0) {
            // Handle both bits and dchunks in one pass
            const digit_t *s = tgt + dchunks, *se = tgt + length - 1;
            digit_t *t = tgt;
            while (s != se) {
                *t = (*s++ >> bits);
                *t++ |= (*s << bits2);
            }
            *t++ = (*s >> bits);
            length = (t - tgt);
        } else if (dchunks != 0) {
            memmove(tgt, tgt + dchunks, sizeof(digit_t) * (length - dchunks));
            length -= dchunks;
        }
    } else set(0);
    trim();
    return *this;
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator<<(BOOST_XINT_RAWINT n, std::size_t bits) {
    return (n <<= bits);
}

BOOST_XINT_RAWINT_TPL
BOOST_XINT_RAWINT operator>>(BOOST_XINT_RAWINT n, std::size_t bits) {
    return (n >>= bits);
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_INCLUDED_XINT_SHIFT_HPP
