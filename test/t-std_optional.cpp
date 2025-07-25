// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/std_optional.hpp>
#include <confy/v/integer.hpp>
#include <doctest/doctest.h>

using opt_int = std::optional<int32_t>;
using opt_val = confy::std_optional<int32_t>;
using opt_ref_val = confy::ref_value_for_t<opt_int>;

TEST_CASE("basics") {
    opt_int i;
    opt_ref_val val(i, { .name = "test", .description = "A test value", .abbr = "tv" }, nullptr);

    CHECK(val.name() == "test");
    CHECK(val.desc() == "A test value");
    CHECK(val.abbr() == "tv");
    CHECK(val.env_var_data().str.empty());
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::automatic);

    CHECK(!val.val().has_value());
    CHECK(val.source() == confy::value_source::default_val);

    val.set_value(42, confy::value_source::cmd_line);
    CHECK(val.val().has_value());
    CHECK(*val.val() == 42);

    val.set_value(std::nullopt, confy::value_source::manual_override);
    CHECK(!val.val().has_value());
}

TEST_CASE("dsl") {
    opt_val val;
    opt_val::dsl dsl(val);

    auto& rr = dsl
        .name("test")
        .desc("test value")
        .abbr("t")
        .env_var("TEST")
        .default_val(75)
        .optional()
        .val();

    CHECK(&rr == &val);
    CHECK(val.name() == "test");
    CHECK(val.desc() == "test value");
    CHECK(val.abbr() == "t");
    CHECK(val.env_var_data().str == "TEST");
    CHECK(val.env_var_data().strategy == confy::env::var_strategy::manual);
    CHECK(val.source() == confy::value_source::none);
    CHECK(val.val() == 75);
    CHECK_FALSE(val.required());
}

TEST_CASE("str") {
    opt_val val({.name = "test_value" }, nullptr);
    val.set_from_string("103", confy::value_source::config_file);
    CHECK(val.val() == 103);
    CHECK(val.to_string() == "103");
    CHECK_THROWS_WITH(val.set_from_string("nope", confy::value_source::manual_override),
        "test_value: failed to parse 32-bit integer from 'nope'");
    CHECK(val.val() == 103);
    CHECK(val.source() == confy::value_source::config_file);
    val.set_from_string("57", confy::value_source::config_file);
    CHECK(val.val() == 57);
    val.set_from_string("null", confy::value_source::cmd_line);
    CHECK_FALSE(val.val().has_value());
    CHECK(val.to_string() == "null");
}

TEST_CASE("dict") {
    using dict = confy::dict;
    opt_val val({.name = "test_value" }, nullptr);
    val.set_from_dict(dict(42), confy::value_source::config_file);
    CHECK(val.val() == 42);
    auto d = val.to_dict();
    CHECK(d.is_number_integer());
    CHECK(d.get<int32_t>() == 42);

    val.set_from_dict(dict(59), confy::value_source::config_file);
    CHECK(val.val() == 59);

    val.set_from_dict({}, confy::value_source::cmd_line);
    CHECK_FALSE(val.val().has_value());
    CHECK(val.source() == confy::value_source::cmd_line);
    CHECK(val.to_dict().is_null());

    CHECK_THROWS_WITH(val.set_from_dict(dict("not a number"), confy::value_source::cmd_line),
        "test_value: [json.exception.type_error.302] type must be number, but is string");
}

TEST_CASE("validate") {
    opt_val val({.name = "test"}, nullptr);
    opt_val::dsl(val)
        .validate([](int i) { return i > 5; })
        .validate([](int i) { return i < 10; }, "<10")
        ;

    CHECK_NOTHROW(val.validate());

    val.set_value(7);
    CHECK_NOTHROW(val.validate());

    val.set_value(3);
    CHECK_THROWS_WITH(val.validate(), "test: value '3' failed validation");

    val.set_value(12);
    CHECK_THROWS_WITH(val.validate(), "test: value '12' failed validation: <10");
}
