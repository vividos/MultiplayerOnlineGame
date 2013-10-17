
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
    \brief Declares items relating to random numbers.
*/

#ifndef BOOST_INCLUDED_XINT_RANDOM_HPP
#define BOOST_INCLUDED_XINT_RANDOM_HPP

#include <ctime>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace boost {
namespace xint {

namespace detail {
//! @cond detail

    // This is a template only so that the static member variables don't cause
    // problems in header-only mode.
    template <typename T = void>
    class strong_random_generator_t {
        public:
        typedef unsigned int result_type;

        strong_random_generator_t();
        ~strong_random_generator_t();
        result_type operator()();

        static const bool has_fixed_range;
        static const result_type min_value;
        static const result_type max_value;
        result_type min BOOST_PREVENT_MACRO_SUBSTITUTION () const;
        result_type max BOOST_PREVENT_MACRO_SUBSTITUTION () const;
        double entropy() const;

        private:
        struct impl_t;
        impl_t *impl;
    };

    //! The base class for random_generator.
    struct base_random_generator {
        typedef unsigned int result_type;
        virtual result_type operator()()=0;
        virtual ~base_random_generator() { }
    };

    //! Support class for the random generator code.
    template <class T>
    struct random_generator: public base_random_generator {
        typedef boost::uniform_int<result_type> dist_t;
        typedef boost::variate_generator<T&, dist_t> gen_t;
        random_generator(T& g): gen(g,
            dist_t((std::numeric_limits<result_type>::min)(),
                (std::numeric_limits<result_type>::max)())) { }
        result_type operator()() { return gen(); }
        gen_t gen;
    };

//! @endcond detail
} // namespace detail

/*! \brief A convenience class for general-purpose (insecure) random numbers.

    This is nothing more than a very thin shell around one of the faster
    pseudo-random number generators provided by Boost.Random.

    \see \ref random
    \see \ref integer_t::random_by_size
    \see \ref integer_t::random_prime
*/
class default_random_generator: public boost::mt19937 {
    public:
    default_random_generator(): mt19937(boost::uint32_t(time(0)+clock())) { }
};

/*! \brief A convenience class for cryptographically-secure random numbers.

    Note that the cryptographic security of this generator is dependent on the
    cryptographic security of the operating system's underlying strong random
    generator.

    \see \ref random
    \see \ref integer_t::random_by_size
    \see \ref integer_t::random_prime
*/
typedef detail::strong_random_generator_t<> strong_random_generator;

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_RANDOM_HPP
