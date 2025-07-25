// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/v/std_optional.hpp>
#include <confy/v/integer.hpp>
#include <doctest/doctest.h>

using opt_int = std::optional<int32_t>;
using opt_val = confy::ref_value_for_t<opt_int>;

TEST_CASE("basics") {
    opt_int i;
    opt_val val(i, { .name = "test", .description = "A test value", .abbr = "tv" }, nullptr);

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
