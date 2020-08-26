// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "t-types.hpp"

#include <confy/types/boolean.hpp>
#include <confy/types/integer.hpp>
#include <confy/types/real.hpp>
#include <confy/types/string.hpp>

using namespace confy;

TEST_SUITE_BEGIN("basic types");

TEST_CASE("boolean")
{
    boolean b;
    CHECK(b.source() == vs::none);
    test(b, false, {
        {"1", vs::env_var, svr::success, true},
        {"0", vs::env_var, svr::same_source_value, true},
        {"false", vs::config_file, svr::success, false},
        {"true", vs::env_var, svr::skipped, false},
        {"asd", vs::cmd_line, svr::bad_value, false},
        {"true", vs::cmd_line, svr::success, true},
        {"default", vs::env_var, svr::skipped, true},
        {"default", vs::manual_override, svr::success, false},
    });
}

TEST_CASE("integer")
{
    integer<int> i1("i1");
    test(i1, 67, {
        {"1", vs::env_var, svr::success, 1},
        {"0", vs::env_var, svr::same_source_value, 1},
        {"333", vs::config_file, svr::success, 333},
        {"54", vs::env_var, svr::skipped, 333},
        {"4asd", vs::cmd_line, svr::bad_value, 333},
        {"-55", vs::cmd_line, svr::success, -55},
        {"default", vs::env_var, svr::skipped, -55},
        {"default", vs::manual_override, svr::success, 67},
    });

    integer<unsigned> i2("i2");
    i2.set_max(55);
    test(i2, 100u, {
        {"-2", vs::env_var, svr::bad_value, 100},
        {"5", vs::env_var, svr::success, 5},
        {"default", vs::cmd_line, svr::bad_default, 5},
        {"300", vs::cmd_line, svr::bad_value, 5},
    });

    integer<long long> i3("i3");
    i3.set_min(-50);
    test(i3, 100ll, {
        {"-5", vs::env_var, svr::success, -5},
        {"default", vs::config_file, svr::success, 100},
        {"-300", vs::cmd_line, svr::bad_value, 100},
    });

    integer<unsigned long long> i4("i4");
    i4.set_min(2);
    i4.set_max(55);
    test(i4, 80ull, {
        {"-2", vs::env_var, svr::bad_value, 80},
        {"default", vs::env_var, svr::bad_default, 80},
        {"1", vs::env_var, svr::bad_value, 80},
        {"100", vs::env_var, svr::bad_value, 80},
    });
    CHECK(i4.source() == vs::none);

    test(i4, 80ull, {
        {"3", vs::env_var, svr::success, 3},
    });
}

TEST_CASE("real")
{
    real<float> f1("f1");
    test(f1, 3.f, {
        {".5", vs::env_var, svr::success, 0.5f},
        {"0", vs::env_var, svr::same_source_value, 0.5f},
        {"1e2", vs::config_file, svr::success, 1e2f},
        {"5.4", vs::env_var, svr::skipped, 1e2f},
        {"4x", vs::cmd_line, svr::bad_value, 1e2f},
        {"-1.5", vs::cmd_line, svr::success, -1.5f},
        {"default", vs::env_var, svr::skipped, -1.5f},
        {"default", vs::manual_override, svr::success, 3},
    });


    real<double> f2("f2");
    f2.set_max(1);
    test(f2, 2.0, {
        {"1.0001", vs::env_var, svr::bad_value, 2.},
        {"-0.5", vs::env_var, svr::success, -0.5},
        {"default", vs::cmd_line, svr::bad_default, -0.5},
    });

    real<double> f3("f3");
    f3.set_min(-6);
    test(f3, 100., {
        {"-5", vs::env_var, svr::success, -5.},
        {"default", vs::config_file, svr::success, 100.},
        {"-1e2", vs::cmd_line, svr::bad_value, 100.},
    });

    real<float> f4("f4");
    f4.set_min(2);
    f4.set_max(5);
    test(f4, 6.f, {
        {"-2", vs::env_var, svr::bad_value, 6.f},
        {"default", vs::env_var, svr::bad_default, 6.f},
        {"1", vs::env_var, svr::bad_value, 6.f},
        {"100", vs::env_var, svr::bad_value, 6.f},
    });
    CHECK(f4.source() == vs::none);

    test(f4, 6.f, {
        {"3e0", vs::env_var, svr::success, 3.f},
    });
}

TEST_CASE("string")
{
    string s1("s1");
    std::string def = "asdf";

    test(s1, def, {
        {"xxx", vs::env_var, svr::success, "xxx"},
        {"yyy", vs::env_var, svr::same_source_value, "xxx"},
        {"zzz", vs::config_file, svr::success, "zzz"},
        {"www", vs::env_var, svr::skipped, "zzz"},
        {"qqq", vs::cmd_line, svr::success, "qqq"},
        {"default", vs::env_var, svr::skipped, "qqq"},
        {"default", vs::manual_override, svr::success, "asdf"},
    });

    string s2("asdf");
    s2.add_enum_element("xxx");
    s2.add_enum_element("yyy");
    s2.add_enum_element("zzz");

    test(s2, def, {
        {"xxx", vs::env_var, svr::success, "xxx"},
        {"yyy", vs::env_var, svr::same_source_value, "xxx"},
        {"www", vs::config_file, svr::bad_value, "xxx"},
        {"yyy", vs::config_file, svr::success, "yyy"},
        {"zzz", vs::cmd_line, svr::success, "zzz"},
        {"default", vs::manual_override, svr::bad_default, "zzz"},
    });
}
