// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "doctest.hpp"
#include "setenv.inl"

#include <cstdlib>

TEST_SUITE_BEGIN("setenv");

TEST_CASE("get")
{
    auto p = std::getenv("PATH");
    CHECK(!!p);
    p = std::getenv("confy_surely_not");
    CHECK(!p);
}

TEST_CASE("set")
{
    set_env_var("confy_foo", "aaa");
    auto p = std::getenv("confy_foo");
    REQUIRE(p);
    std::string v = p;
    CHECK(v == "aaa");

    set_env_var("confy_foo", "bbb zzz");
    set_env_var("confy_bar", "ggg");

    p = std::getenv("confy_foo");
    REQUIRE(p);
    v = p;
    CHECK(v == "bbb zzz");

    p = std::getenv("confy_bar");
    REQUIRE(p);
    v = p;
    CHECK(v == "ggg");
}

TEST_CASE("set local")
{
    std::string value = "hello world";
    {
        std::string name = "confy_some_name";
        set_env_var(name.c_str(), value.c_str());
    }

    auto p = std::getenv("confy_some_name");
    REQUIRE(p);
    CHECK(value == p);
}
