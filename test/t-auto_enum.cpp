// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "t-types.hpp"

#include <confy/impl/magic_enum.hpp>
#if MAGIC_ENUM_SUPPORTED

#include <confy/types/auto_enum.hpp>

using namespace confy;

TEST_SUITE_BEGIN("auto_enum");

namespace
{
enum order { first, second, third };
enum class ord { first, second, third };
}

TEST_CASE("enum")
{
    auto_enum<order> e;

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

    auto_enum<ord> e2;
    test(e2, ord::first, {
        {"first", vs::env_var, svr::success, ord::first},
        {"second", vs::env_var, svr::same_source_value, ord::first},
        {"second", vs::config_file, svr::success, ord::second},
        {"first", vs::env_var, svr::skipped, ord::second},
        {"ffff", vs::cmd_line, svr::bad_value, ord::second},
        {"third", vs::cmd_line, svr::success, ord::third},
        {"default", vs::env_var, svr::skipped, ord::third},
        {"default", vs::manual_override, svr::success, ord::first},
    });
}

#endif
