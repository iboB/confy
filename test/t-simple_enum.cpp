// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "t-types.hpp"

#include <confy/types/simple_enum.hpp>

using namespace confy;

TEST_SUITE_BEGIN("simple_enum");

namespace
{
enum order { first, second, third };
}

TEST_CASE("enum")
{
    simple_enum<order> e;
    e.add_element(first, "first");
    e.add_element(third, "third");
    e.add_element(second, "second");

    test(e, first, {
        {"first", vs::env_var, svr::success, first},
        {"second", vs::env_var, svr::same_source_value, first},
        {"second", vs::config_file, svr::success, second},
        {"first", vs::env_var, svr::skipped, second},
        {"ffff", vs::cmd_line, svr::bad_value, second},
        {"third", vs::cmd_line, svr::success, third},
        {"default", vs::env_var, svr::skipped, third},
        {"default", vs::manual_override, svr::success, first},
        });

    simple_enum<order> e2;
    e2.add_element(first, "first");
    e2.add_element(third, "third");
    test(e2, second, {
        {"first", vs::env_var, svr::success, first},
        {"second", vs::config_file, svr::bad_value, first},
        {"default", vs::cmd_line, svr::success, second},
        });
}
