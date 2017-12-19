
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./config.hpp"
#include <boost/config.hpp>
#include <cstddef>

namespace test
{
    struct custom
    {
        int value_;

        std::size_t hash() const
        {
            return static_cast<std::size_t>(value_ * 10);
        }

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        friend std::size_t hash_value(custom const& x )
        {
            return x.hash();
        }
#endif

        custom(int x) : value_(x) {}
    };
}

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
namespace boost
{
    std::size_t hash_value(test::custom x)
    {
        return x.hash();
    }
}
#endif

#include "./config.hpp"

#ifdef BOOST_HASH_TEST_EXTENSIONS
#  ifdef BOOST_HASH_TEST_STD_INCLUDES
#    include <functional>
#  else
#    include <boost/functional/hash.hpp>
#  endif
#endif

#include <boost/core/lightweight_test.hpp>

#ifdef BOOST_HASH_TEST_EXTENSIONS

#include <vector>
#include <string>
#include <cctype>

void custom_tests()
{
    BOOST_HASH_TEST_NAMESPACE::hash<test::custom> custom_hasher;
    BOOST_TEST(custom_hasher(10) == 100u);
    test::custom x(55);
    BOOST_TEST(custom_hasher(x) == 550u);

    {
        using namespace BOOST_HASH_TEST_NAMESPACE;
        BOOST_TEST(custom_hasher(x) == hash_value(x));
    }

    std::vector<test::custom> custom_vector;
    custom_vector.push_back(5);
    custom_vector.push_back(25);
    custom_vector.push_back(35);

    std::size_t seed = 0;
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed, test::custom(5));
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed, test::custom(25));
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed, test::custom(35));

    std::size_t seed2 = 0;
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed2, 50u);
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed2, 250u);
    BOOST_HASH_TEST_NAMESPACE::hash_combine(seed2, 350u);

    BOOST_TEST(seed == BOOST_HASH_TEST_NAMESPACE::hash_range(
        custom_vector.begin(), custom_vector.end()));
    BOOST_TEST(seed == seed2);
}

#endif // BOOST_HASH_TEST_EXTENSIONS

int main()
{
#ifdef BOOST_HASH_TEST_EXTENSIONS
    custom_tests();
#endif
    return boost::report_errors();
}
