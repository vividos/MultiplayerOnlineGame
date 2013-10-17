
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
    \brief Contains the function definitions for the random number classes.

    This file will be included by the library itself when needed.
*/

#ifndef BOOST_INCLUDED_XINT_RANDOMGEN_HPP
#define BOOST_INCLUDED_XINT_RANDOMGEN_HPP

////////////////////////////////////////////////////////////////////////////////
// The secure random generator

//! @cond
namespace boost {
namespace xint {
namespace detail {

#define BOOST_XINT_INLINE inline

#ifdef _WIN32
    template <typename T>
    struct strong_random_generator_t<T>::impl_t {
        typedef BOOLEAN (WINAPI *RtlGenRandomFn)(PVOID, ULONG);
        typedef DWORD result_type;

        impl_t(): dll(0), fn(0) {
            // This should work under WinXP, Vista, and Win7. No guarantees
            // about future compatibility, but I doubt that Microsoft will get
            // rid of it (it's too useful), and I doubt that they'll change it
            // now that it's well-known (it would break too many programs). We
            // could also use the rand_s function in more recent versions of
            // Visual C++, but that causes compatibility problems with older
            // versions of Windows.
            dll=LoadLibraryA("Advapi32.dll");
            if (dll != 0) fn=RtlGenRandomFn(GetProcAddress(dll,
                "SystemFunction036"));
            if (fn == 0) {
                destroy();
                exception_handler<>::call(__FILE__, __LINE__,
                    exceptions::no_strong_random());
            }
        }

        ~impl_t() { destroy(); }

        result_type operator()() {
            result_type r=0;
            if (!fn(&r, sizeof(result_type)))
                exception_handler<>::call(__FILE__, __LINE__,
                exceptions::no_strong_random("RtlGenRandom failed"));
            return r;
        }

        void destroy() { if (dll) FreeLibrary(dll); }

        HMODULE dll;
        RtlGenRandomFn fn;
    };

    template <typename T>
    BOOST_XINT_INLINE double strong_random_generator_t<T>::entropy() const
        { return 32; }
#else
    template <typename T>
    struct strong_random_generator_t<T>::impl_t {
        typedef unsigned char result_type;

        impl_t(): rng("/dev/urandom", std::ios::binary) {
            // This should be supported under most non-Windows systems. Note
            // that we're using /dev/urandom, not /dev/random -- /dev/random is
            // more secure, but it can be VERY slow.
            if (!rng) exception_handler<>::call(__FILE__, __LINE__,
                exceptions::no_strong_random());
        }

        result_type operator()() {
            int r=rng.get();
            if (r==EOF) exception_handler<>::call(__FILE__, __LINE__,
                exceptions::no_strong_random("/dev/urandom returned EOF"));
            return static_cast<result_type>(r);
        }

        std::ifstream rng;
    };

    template <typename T>
    BOOST_XINT_INLINE double strong_random_generator_t<T>::entropy() const
        { return 8; }
#endif

template <typename T>
const bool strong_random_generator_t<T>::has_fixed_range = true;

template <typename T>
const typename strong_random_generator_t<T>::result_type
    strong_random_generator_t<T>::min_value =
    (std::numeric_limits<typename
    strong_random_generator_t<T>::impl_t::result_type>::min)();

template <typename T>
const typename strong_random_generator_t<T>::result_type
    strong_random_generator_t<T>::max_value =
    (std::numeric_limits<typename
    strong_random_generator_t<T>::impl_t::result_type>::max)();

template <typename T>
BOOST_XINT_INLINE strong_random_generator_t<T>::strong_random_generator_t():
    impl(new impl_t) { }

template <typename T>
BOOST_XINT_INLINE strong_random_generator_t<T>::~strong_random_generator_t() {
    delete impl; }

template <typename T>
BOOST_XINT_INLINE typename strong_random_generator_t<T>::result_type
    strong_random_generator_t<T>::operator()() { return (*impl)(); }

template <typename T>
BOOST_XINT_INLINE typename strong_random_generator_t<T>::result_type
    strong_random_generator_t<T>::min BOOST_PREVENT_MACRO_SUBSTITUTION () const
    { return min_value; }

template <typename T>
BOOST_XINT_INLINE typename strong_random_generator_t<T>::result_type
    strong_random_generator_t<T>::max BOOST_PREVENT_MACRO_SUBSTITUTION () const
    { return max_value; }

} // namespace detail
} // namespace xint
} // namespace boost
//! @endcond

#endif // BOOST_INCLUDED_XINT_RANDOMGEN_HPP
