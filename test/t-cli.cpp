// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/cli.hpp>
#include <doctest/doctest.h>
#include <vector>

using namespace confy::cli;

TEST_CASE("parse_arg")
{
    auto p = parse_arg("");
    CHECK_FALSE(p.has_value());

    p = parse_arg("/yyy");
    CHECK_FALSE(p.has_value());

    p = parse_arg("--port");
    REQUIRE(p.has_value());
    CHECK_FALSE(p->abbr);
    CHECK(p->key == "port");
    CHECK(p->value.empty());

    p = parse_arg("-p");
    REQUIRE(p.has_value());
    CHECK(p->abbr);
    CHECK(p->key == "p");
    CHECK(p->value.empty());

    p = parse_arg("--zoom=43");
    REQUIRE(p.has_value());
    CHECK_FALSE(p->abbr);
    CHECK(p->key == "zoom");
    CHECK(p->value == "43");

    p = parse_arg("-a.b.z=true");
    REQUIRE(p.has_value());
    CHECK(p->abbr);
    CHECK(p->key == "a.b.z");
    CHECK(p->value == "true");

    p = parse_arg("--my:port", "my:");
    REQUIRE(p.has_value());
    CHECK_FALSE(p->abbr);
    CHECK(p->key == "port");
    CHECK(p->value.empty());

    p = parse_arg("-p", "my:");
    CHECK_FALSE(p.has_value());

    p = parse_arg("--my:zoom=43", "my:");
    REQUIRE(p.has_value());
    CHECK_FALSE(p->abbr);
    CHECK(p->key == "zoom");
    CHECK(p->value == "43");

    p = parse_arg("-my:a.b.z=true", "my:");
    REQUIRE(p.has_value());
    CHECK(p->abbr);
    CHECK(p->key == "a.b.z");
    CHECK(p->value == "true");
}

struct arg_result {
    struct parsed_arg {
        std::string key;
        std::string value;
        bool abbr = false;

        auto operator<=>(const parsed_arg&) const = default;
    };
    std::vector<parsed_arg> parsed;
    std::vector<std::string> rem;
};

arg_result test_filter_cmd_line(std::vector<std::string> sargv, std::string_view prefix = {}) {
    arg_result ret;

    std::vector<char*> argv;
    char exe[] = "exe";
    argv.push_back(exe); // add first arg
    for (auto& s : sargv) {
        argv.push_back(s.data());
    }
    int argc = int(argv.size());

    auto gen = filter_cmd_line(argc, argv.data(), prefix);
    for (auto& p : gen) {
        ret.parsed.push_back({std::string(p.key), std::string(p.value), p.abbr});
    }

    for (int i = 1; i < argc; ++i) {
        ret.rem.push_back(argv[i]);
    }

    return ret;
}

TEST_CASE("filter_cmd_line") {
    using v = std::vector<std::string>;
    using pa = arg_result::parsed_arg;

    auto r = test_filter_cmd_line({});
    CHECK(r.parsed.empty());
    CHECK(r.rem.empty());

    r = test_filter_cmd_line({"x", "y", "z"});
    CHECK(r.parsed.empty());
    CHECK(r.rem == v{"x", "y", "z"});

    r = test_filter_cmd_line({"--allow"});
    REQUIRE(r.parsed.size() == 1);
    CHECK(r.parsed.front() == pa{"allow", "true", false});

    auto check_parsed = [&r]() {
        CHECK(r.parsed.size() == 4);

        auto a = r.parsed.begin();
        CHECK(*a == pa{"allow", "false", false});
        ++a;
        CHECK(*a == pa{"chordata.lion", "pepe", false});
        ++a;
        CHECK(*a == pa{"b", "0", true});
        ++a;
        CHECK(*a == pa{"chordata.l", "pipi", true});
    };

    r = test_filter_cmd_line({"x", "y", "--allow=false", "--chordata.lion", "pepe", "zz", "-b=0", "-chordata.l", "pipi"});
    CHECK(r.rem == v{"x", "y", "zz"});
    check_parsed();


    r = test_filter_cmd_line(
        {"-x", "--y", "--my:allow=false", "--my:chordata.lion", "pepe", "-zz=34", "-my:b=0", "--yours:gg=45", "-my:chordata.l", "pipi" },
        "my:"
    );
    CHECK(r.rem == v{"-x", "--y", "-zz=34", "--yours:gg=45"});
    check_parsed();
}