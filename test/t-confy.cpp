// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>

#include <confy/config.hpp>

using namespace confy;

TEST_SUITE_BEGIN("confy");

TEST_CASE("setup")
{
    config cfg("asdf");
    CHECK(cfg.name() == "asdf");
    CHECK(cfg.env_var_prefix().empty());
    CHECK(cfg.cmd_prefix().empty());
    CHECK(cfg.verbose_stream() == nullptr);
    CHECK(cfg.no_env() == false);
    CHECK(cfg.sections().empty());

}