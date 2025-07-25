// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/auto_enum.hpp>
#include <doctest/doctest.h>
#include <confy/dict.hpp>

enum class test_enum {
    one, two, three
};

TEST_CASE("test_enum") {
    using e_t = confy::common_value_type_traits<test_enum>;
    CHECK(e_t::to_string(test_enum::one) == "one");
    auto d = e_t::to_dict(test_enum::two);
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "two");

    test_enum val;
    e_t::set_value_from_string(val, "three");
    CHECK(val == test_enum::three);

    e_t::set_value_from_dict(val, d);
    CHECK(val == test_enum::two);
}

enum simple_test_enum {
    ichi, ni, san
};

TEST_CASE("simple_test_enum") {
    using e_t = confy::common_value_type_traits<simple_test_enum>;
    CHECK(e_t::to_string(ichi) == "ichi");
    auto d = e_t::to_dict(ni);
    CHECK(d.is_string());
    CHECK(d.get<std::string_view>() == "ni");
    simple_test_enum val;
    e_t::set_value_from_string(val, "san");
    CHECK(val == san);
    e_t::set_value_from_dict(val, d);
    CHECK(val == ni);
}
