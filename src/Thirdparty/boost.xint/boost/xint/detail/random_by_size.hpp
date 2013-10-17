
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
    \brief Contains the random_by_size function.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_RANDOM_BY_SIZE_HPP
#define BOOST_INCLUDED_XINT_RANDOM_BY_SIZE_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

BOOST_XINT_RAWINT_TPL
template <typename GenType>
BOOST_XINT_RAWINT BOOST_XINT_RAWINT::random_by_size(GenType &_gen, bitsize_t
    bits, bool high_bit_on, bool low_bit_on, bool can_be_negative)
{
    if (bits == 0) return 0;

    // Grab a bunch of bits
    random_generator<GenType> gen(_gen);
    bitqueue_t bitqueue;
    while (bitqueue.size() < bits) bitqueue.push(gen(),
        std::numeric_limits<base_random_generator::result_type>::digits);

    // Stick them into an integer
    BOOST_XINT_RAWINT target;
    std::size_t d = bits_to_digits(bits);
    digit_t *tdigits = target.digits(d, realloc::ignore);
    target.length = (std::min)(d + 1, target.max_length());
    digit_t *pp = tdigits, *ppe = pp + target.length;
    while (pp < ppe) *pp++ = static_cast<digit_t>(bitqueue.pop(bits_per_digit));

    // Trim it to the proper length
    std::size_t index = (bits / bits_per_digit);
    digit_t mask = (digit_t(1) << (bits % bits_per_digit)) - 1;
    if (mask == 0) { mask = digit_mask; --index; }
    target.length = index + 1;
    tdigits[index] &= mask;
    for (digit_t *i = tdigits + index + 1, *ie = tdigits + target.length; i <
        ie; ++i) *i = 0;

    if (high_bit_on) setbit(target, bits - 1);
    if (low_bit_on) setbit(target, 0);
    target.negative = (can_be_negative && (gen() & 0x01));
    target.trim();
    return target;
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_RANDOM_BY_SIZE_HPP
