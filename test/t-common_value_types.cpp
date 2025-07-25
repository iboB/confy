// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/integer.hpp>
#include <confy/v/boolean.hpp>
#include <confy/v/std_string.hpp>
#include <doctest/doctest.h>
#include <confy/dict.hpp>

TEST_CASE("integer") {
    using int_t = confy::common_value_type_traits<int32_t>;
    CHECK(int_t::to_string(123) == "123");
    auto d = int_t::to_dict(56);
    CHECK(d.is_number_integer());
    CHECK(d.get<int>() == 56);

    int val = 0;
    int_t::set_value_from_string(val, "456");
    CHECK(val == 456);

    int_t::set_value_from_string(val, "-2136");
    CHECK(val == -2136);

    int_t::set_value_from_dict(val, d);
    CHECK(val == 56);

    CHECK_THROWS_WITH(int_t::set_value_from_string(val, "32cm"),
        "failed to parse 32-bit integer from '32cm'");
    CHECK_THROWS_WITH(int_t::set_value_from_string(val, "not a number"),
        "failed to parse 32-bit integer from 'not a number'");

    CHECK_THROWS_WITH(int_t::set_value_from_dict(val, confy::dict("asdf")),
        "[json.exception.type_error.302] type must be number, but is string");
}

TEST_CASE("std::string") {
    using str_t = confy::common_value_type_traits<std::string>;

    CHECK(str_t::to_string("hello") == "hello");
    auto d = str_t::to_dict("world");
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "world");

    std::string val;
    str_t::set_value_from_string(val, "foo");
    CHECK(val == "foo");

    str_t::set_value_from_dict(val, d);
    CHECK(val == "world");

    CHECK_THROWS_WITH(str_t::set_value_from_dict(val, confy::dict(1.5)),
        "[json.exception.type_error.302] type must be string, but is number");
}

TEST_CASE("bool") {
    auto bool_t = confy::common_value_type_traits<bool>();

    CHECK(bool_t.to_string(true) == "true");
    CHECK(bool_t.to_string(false) == "false");
    auto d_true = bool_t.to_dict(true);
    CHECK(d_true.is_boolean());
    CHECK(d_true.get<bool>() == true);
    auto d_false = bool_t.to_dict(false);
    CHECK(d_false.is_boolean());
    CHECK(d_false.get<bool>() == false);

    bool val = false;
    bool_t.set_value_from_string(val, "true");
    CHECK(val == true);
    bool_t.set_value_from_string(val, "false");
    CHECK(val == false);
    bool_t.set_value_from_string(val, "1");
    CHECK(val == true);
    bool_t.set_value_from_string(val, "0");
    CHECK(val == false);

    bool_t.set_value_from_dict(val, d_true);
    CHECK(val == true);
    bool_t.set_value_from_dict(val, d_false);
    CHECK(val == false);

    CHECK_THROWS_WITH(bool_t.set_value_from_string(val, "not a bool"),
        "failed to parse boolean from 'not a bool'");

    CHECK_THROWS_WITH(bool_t.set_value_from_dict(val, confy::dict(1.5)),
        "[json.exception.type_error.302] type must be boolean, but is number");
}
