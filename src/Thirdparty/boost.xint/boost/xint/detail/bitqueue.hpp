
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
    \brief Contains the bitqueue_t class, used internally.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_XINT_INCLUDED_BITQUEUET_HPP
#define BOOST_XINT_INCLUDED_BITQUEUET_HPP

//! @cond detail
namespace boost {
namespace xint {
namespace detail {

//! \brief Used internally to reorder bits into different-sized groupings.
class bitqueue_t {
    public:
    bitqueue_t(): bits_pending(0) { }
    void push(doubledigit_t bits, std::size_t count);
    doubledigit_t pop(std::size_t requestedbits);
    bool empty() const { return pending.empty(); }
    std::size_t size() const { return bits_pending; }

    private:
    static const std::size_t ddbits =
        std::numeric_limits<doubledigit_t>::digits;
    typedef std::pair<doubledigit_t, std::size_t> indata_t;
    typedef std::queue<indata_t> inqueue_t;

    std::size_t bits_pending;
    inqueue_t pending;
};

inline void bitqueue_t::push(doubledigit_t bits, std::size_t count) {
    if (count < ddbits) {
        doubledigit_t mask = (doubledigit_t(1) << count) - 1;
        bits &= mask;
    }

    if (pending.empty()) {
        pending.push(std::make_pair(bits, count));
    } else {
        indata_t &n(pending.back());
        if (n.second + count <= ddbits) {
            n.first |= bits << n.second;
            n.second += count;
        } else {
            pending.push(std::make_pair(bits, count));
        }
    }
    bits_pending += count;
}

inline doubledigit_t bitqueue_t::pop(std::size_t requestedbits) {
    doubledigit_t buffer = 0;
    std::size_t bits_in_buffer = 0;
    while (bits_in_buffer < requestedbits && !pending.empty()) {
        indata_t &n(pending.front());
        std::size_t maxbits = requestedbits - bits_in_buffer, actualbits =
            (std::min)(n.second, maxbits);
        buffer |= (n.first << bits_in_buffer);

        n.first >>= actualbits;
        n.second -= actualbits;
        bits_in_buffer += actualbits;
        bits_pending -= actualbits;

        if (n.second == 0) pending.pop();
    }
    return (buffer & ((doubledigit_t(1) << requestedbits) - 1));
}

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond detail

#endif // BOOST_XINT_INCLUDED_BITQUEUET_HPP
