
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
    \brief Contains the magnitude_manager_t class.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_MAGNITUDE_MANAGER_HPP
#define BOOST_INCLUDED_XINT_MAGNITUDE_MANAGER_HPP

namespace boost {
namespace xint {
namespace detail {
//! @cond detail

template <bitsize_t Bits, bool Secure, class Allocator>
class magnitude_manager_t {
    public:
    typedef allocator_t<Bits, Secure, Allocator> Alloc;

    magnitude_manager_t(): raw_data(zerodata()) { inc(); }
    explicit magnitude_manager_t(std::size_t length, bool readonly = false):
        raw_data(Alloc::alloc(length, readonly)) { inc(); }
    magnitude_manager_t(const magnitude_manager_t<Bits, Secure, Allocator>
        &copy): raw_data(copy.raw_data) { inc(); }
    template <bitsize_t B, bool S, class A>
    magnitude_manager_t(const magnitude_manager_t<B, S, A> &copy):
        raw_data(copy.raw_data) { inc(); }
    ~magnitude_manager_t() { dec(); }

    magnitude_manager_t& operator=(const magnitude_manager_t<Bits, Secure,
        Allocator>& copy) { reset(copy.raw_data); return *this; }

    bool is_nan() const { return (raw_data == 0); }
    void make_nan() { dec(); raw_data = 0; }

    void resize_and_uniquify(std::size_t newsize = 0, realloc::strategy strategy
        = realloc::copy) { raw_data = Alloc::realloc_and_uniquify(raw_data,
        newsize, strategy); }

    bool same_storage(const magnitude_manager_t<Bits, Secure, Allocator> &n)
        const { return raw_data == n.raw_data; }

    // Do not call these on a NaN
    std::size_t max_length() const { return raw_data->max_length; }
    bool is_unique() const { return (raw_data->copies() == 1); }
    digit_t *digits() { return raw_data->digits; }
    const digit_t *digits() const { return raw_data->digits; }
    void trim() { fixedlength_t<Bits>::trim(raw_data); }

    void _swap(magnitude_manager_t<Bits, Secure, Allocator>& i2) {
        using std::swap;
        swap(raw_data, i2.raw_data);
    }

    private:
    static magnitude_t *zerodata() { static magnitude_manager_t z(1, true);
        return z.raw_data; }

    void reset(magnitude_t *p) { if (raw_data != p) { dec(); raw_data = p;
        inc(); } }
    void dec() { if (raw_data) Alloc::detach(raw_data); }

    #ifdef BOOST_XINT_USE_COW
        void inc() { if (raw_data) Alloc::attach(raw_data); }
    #else
        void inc() {
            if (!raw_data) return;
            Alloc::attach(raw_data);
            if (raw_data->copies() > 1 && !raw_data->readonly) {
                // Copy-on-Write disabled, deep-copy it
                resize_and_uniquify();
            }
        }
    #endif

    magnitude_t *raw_data;

    template <bitsize_t B, bool S, class A>
    friend class magnitude_manager_t;
};

template <bitsize_t Bits, bool Secure, class Allocator>
void swap(magnitude_manager_t<Bits, Secure, Allocator>& p1,
    magnitude_manager_t<Bits, Secure, Allocator>& p2)
{
    p1._swap(p2);
}

//! @endcond detail
} // namespace detail
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_MAGNITUDE_MANAGER_HPP
