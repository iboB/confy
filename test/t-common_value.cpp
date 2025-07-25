// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/std_string.hpp>
#include <confy/v/integer.hpp>
#include <doctest/doctest.h>
#include <confy/dict.hpp>
#include "setenv.inl"

template <typename T>
std::string to_string(const T& val) {
    return std::to_string(val);
}

const std::string& to_string(const std::string& val) {
    return val;
}

const std::string to_string(bool b) {
    return b ? "true" : "false";
}

template <typename T>
void ref_test(T val_a, T val_b) {
    using value_type = T;
    using ref_type = confy::ref_value_for_t<value_type>;

    // value
    {
        value_type val = {};
        ref_type ref(val);
        CHECK(ref.source() == confy::value_source::none);
        CHECK(&ref.val() == &val);
        ref.set_value(val_a, confy::value_source::cmd_line);
        CHECK(val == val_a);
        ref.set_value(val_b, confy::value_source::default_val);
        CHECK(val == val_a);
        CHECK(ref.source() == confy::value_source::cmd_line);
        ref.set_value(val_b);
        CHECK(val == val_b);
        CHECK(ref.source() == confy::value_source::manual_override);
        CHECK(ref.required());
    }

    // dsl
    {
        value_type val = {};
        ref_type ref(val);
        typename ref_type::dsl dsl(ref);

        auto& rr = dsl
            .name("test")
            .desc("test value")
            .abbr("t")
            .env_var("TEST")
            .default_val(val_a)
            .optional()
            .val();

        CHECK(&rr == &ref);
        CHECK(ref.name() == "test");
        CHECK(ref.desc() == "test value");
        CHECK(ref.abbr() == "t");
        CHECK(ref.env_var_data().str == "TEST");
        CHECK(ref.env_var_data().strategy == confy::env::var_strategy::manual);
        CHECK(ref.source() == confy::value_source::default_val);
        CHECK_FALSE(ref.required());
        CHECK(val == val_a);
    }

    // str
    {
        value_type val = {};
        ref_type ref(val);
        auto str_a = to_string(val_a);
        auto str_b = to_string(val_b);
        ref.set_from_string(str_a, confy::value_source::cmd_line);
        CHECK(val == val_a);
        ref.set_from_string(str_b, confy::value_source::default_val);
        CHECK(val == val_a);
        CHECK(ref.source() == confy::value_source::cmd_line);
        CHECK(ref.to_string() == str_a);
        ref.set_from_string(str_b, confy::value_source::manual_override);
        CHECK(val == val_b);
        CHECK(ref.source() == confy::value_source::manual_override);
        CHECK(ref.to_string() == str_b);
    }

    // dict
    {
        value_type val = {};
        ref_type ref(val);
        confy::dict dict_a;
        dict_a = val_a;
        confy::dict dict_b;
        dict_b = val_b;
        ref.set_from_dict(dict_a, confy::value_source::cmd_line);
        CHECK(val == val_a);
        ref.set_from_dict(dict_b, confy::value_source::default_val);
        CHECK(val == val_a);
        CHECK(ref.source() == confy::value_source::cmd_line);
        CHECK(ref.to_dict() == dict_a);
        ref.set_from_dict(dict_b, confy::value_source::manual_override);
        CHECK(val == val_b);
        CHECK(ref.source() == confy::value_source::manual_override);
        CHECK(ref.to_dict() == dict_b);
    }

    // env var
    {
        value_type val = {};
        const std::string env_var_name = "CONFY_TEST_VALUE";
        ref_type ref(val, {.name = env_var_name}, nullptr);

        ref.try_set_from_env_var();
        CHECK(ref.source() == confy::value_source::none);

        auto str_a = to_string(val_a);
        set_env_var(env_var_name.c_str(), str_a.c_str());
        ref.try_set_from_env_var();
        CHECK(val == val_a);
        CHECK(ref.source() == confy::value_source::env_var);
        set_env_var(env_var_name.c_str(), nullptr);
    }
}

TEST_CASE("integer") {
    ref_test(-34, 56);
    ref_test(43u, 336u);

    using ref_type = confy::common_value<int&>;

    // validate
    {
        int val = {};
        ref_type ref(val);
        ref_type::dsl(ref).name("test");
        CHECK_THROWS_WITH(ref.validate(), "test: value is required but not set");
        ref_type::dsl(ref).optional();
        CHECK_NOTHROW(ref.validate());
        ref_type::dsl(ref).required();
        CHECK_THROWS_WITH(ref.validate(), "test: value is required but not set");
        ref.set_value(34);
        CHECK_NOTHROW(ref.validate());
    }
}

TEST_CASE("std::string") {
    ref_test<std::string>("hello", "world");
}
