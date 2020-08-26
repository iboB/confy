// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "doctest.hpp"

#include <confy/value_source.hpp>
#include <confy/option.hpp>

#include <vector>

using vs = confy::value_source;
using svr = confy::option::set_value_result;

template <typename T>
struct type_test
{
    std::string value;
    vs source;

    svr expected_result;
    T expected_value;
};

template <typename Option, typename T>
void test(Option& opt, T initial_value, std::vector<type_test<T>> tests)
{
    T val = initial_value;
    opt.set_value_ptr(&val);

    for (auto& t : tests)
    {
        auto r = opt.try_set_value(t.value, t.source);
        CHECK(r == t.expected_result);
        CHECK(val == t.expected_value);
    }

    opt.set_value_ptr(nullptr);
}
