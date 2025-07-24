// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/section.hpp>
#include <confy/v/std_string.hpp>
#include <confy/v/integer.hpp>
#include <doctest/doctest.h>

TEST_CASE("basics") {
    confy::section sec({.name = "test", .abbr = "t"}, nullptr);
    CHECK(sec.abbr() == "t");
    CHECK(sec.owner() == nullptr);

    auto& ssa = sec.add_section({.name = "ssa", .abbr = "a"});
    CHECK(ssa.abbr() == "s");
    CHECK(ssa.owner() == &sec);
}
