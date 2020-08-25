// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "doctest.hpp"
#include "impl/num_from_string.hpp"

#include <cstdlib>
#include <cmath>

TEST_SUITE_BEGIN("num_from_string");

using confy::impl::num_from_string;

TEST_CASE("integers")
{
    int i = 0;
    CHECK_FALSE(num_from_string(i, "123x"));
    CHECK(i == 0);
    CHECK(num_from_string(i, "123"));
    CHECK(i == 123);
    CHECK_FALSE(num_from_string(i, "1230000000000000000000"));
    CHECK(i == 123);
    CHECK(num_from_string(i, "-123"));
    CHECK(i == -123);

    unsigned u = 0;
    CHECK_FALSE(num_from_string(u, "-123"));
    CHECK(u == 0);
}

template <typename T>
bool approx(T a, T b, T e = T(0.00001))
{
    return std::abs(a - b) < e;
}

TEST_CASE("floats")
{
    float f = 0;
    CHECK_FALSE(num_from_string(f, "x"));
    CHECK(f == 0);
    CHECK(num_from_string(f, "123"));
    CHECK(f == 123);

    double d = 10;
    CHECK(num_from_string(d, "12.3"));
    CHECK(approx(d, 12.3));

    CHECK(num_from_string(d, "1e5"));
    CHECK(d == 1e5);

    CHECK(num_from_string(d, "3e-2"));
    CHECK(approx(d, 3e-2));
}
