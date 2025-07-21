// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <doctest/doctest.h>
#include "impl/cmd_line_util.inl"

#include <vector>

TEST_SUITE_BEGIN("command line utils");

TEST_CASE("arg_split")
{
    auto t = split_option_path("");
    CHECK(t.section.empty());
    CHECK(t.option.empty());

    t = split_option_path("xxx=yyy");
    CHECK(t.section.empty());
    CHECK(t.option == "xxx=yyy");

    t = split_option_path("sec.oo");
    CHECK(t.section == "sec");
    CHECK(t.option == "oo");

    t = split_option_path("animalia.chordata.lion");
    CHECK(t.section == "animalia.chordata");
    CHECK(t.option == "lion");
}

TEST_CASE("parse_single_arg")
{
    auto p = parse_single_arg("");
    CHECK_FALSE(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path.empty());
    CHECK(p.value.empty());

    p = parse_single_arg("/yyy");
    CHECK_FALSE(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path.empty());
    CHECK(p.value.empty());

    p = parse_single_arg("--port");
    CHECK(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path == "port");
    CHECK(p.value.empty());

    p = parse_single_arg("-p");
    CHECK(p.relevant);
    CHECK(p.abbr);
    CHECK(p.path == "p");
    CHECK(p.value.empty());

    p = parse_single_arg("--zoom=43");
    CHECK(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path == "zoom");
    CHECK(p.value == "43");

    p = parse_single_arg("-a.b.z=true");
    CHECK(p.relevant);
    CHECK(p.abbr);
    CHECK(p.path == "a.b.z");
    CHECK(p.value == "true");

    p = parse_single_arg("--my:port", "my:");
    CHECK(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path == "port");
    CHECK(p.value.empty());

    p = parse_single_arg("-p", "my:");
    CHECK_FALSE(p.relevant);
    CHECK(p.abbr);
    CHECK(p.path.empty());
    CHECK(p.value.empty());

    p = parse_single_arg("--my:zoom=43", "my:");
    CHECK(p.relevant);
    CHECK_FALSE(p.abbr);
    CHECK(p.path == "zoom");
    CHECK(p.value == "43");

    p = parse_single_arg("-my:a.b.z=true", "my:");
    CHECK(p.relevant);
    CHECK(p.abbr);
    CHECK(p.path == "a.b.z");
    CHECK(p.value == "true");
}

struct arg
{
    std::string section;
    std::string option;
    bool abbr;
    std::string value;
};

bool cmp(const std::vector<std::string>& a, const std::vector<std::string>& b)
{
    return a == b;
}

TEST_CASE("filter_command_line")
{
    std::vector<arg> args;
    std::string_view prefix;
    auto parse = [&args, &prefix](std::vector<std::string> sargv) {
        args.clear();
        auto single = [&args](std::string_view path, bool abbr, std::string_view value) {
            auto split = split_option_path(path);
            args.emplace_back(arg{ std::string{split.section}, std::string{split.option}, abbr, std::string{value} });
        };
        std::vector<char*> argv;
        char exe[] = "exe";
        argv.push_back(exe); // add first arg
        for (auto& a : sargv)
        {
            argv.emplace_back(a.data());
        }
        int argn = int(argv.size());
        filter_command_line(argn, argv.data(), prefix, single);

        std::vector<std::string> ret;
        for (int i = 0; i < argn; ++i)
        {
            ret.emplace_back(argv[i]);
        }
        return ret;
    };

    auto f = parse({});
    CHECK(args.empty());
    CHECK(cmp(f, { "exe" }));

    f = parse({ "x", "y", "z" });
    CHECK(args.empty());
    CHECK(cmp(f, { "exe", "x", "y", "z" }));

    f = parse({ "--allow" });
    CHECK(args.size() == 1);
    CHECK(cmp(f, { "exe" }));
    auto a = args.begin();
    CHECK(a->section.empty());
    CHECK(a->option == "allow");
    CHECK_FALSE(a->abbr);
    CHECK(a->value == UNSET_VALUE);

    f = parse({ "x", "y", "--allow=false", "--chordata.lion", "pepe", "zz", "-b=0", "-chordata.l", "pipi" });
    CHECK(args.size() == 4);
    CHECK(cmp(f, { "exe", "x", "y", "zz" }));

    a = args.begin();
    CHECK(a->section.empty());
    CHECK(a->option == "allow");
    CHECK_FALSE(a->abbr);
    CHECK(a->value == "false");

    ++a;
    CHECK(a->section == "chordata");
    CHECK(a->option == "lion");
    CHECK_FALSE(a->abbr);
    CHECK(a->value == "pepe");

    ++a;
    CHECK(a->section.empty());
    CHECK(a->option == "b");
    CHECK(a->abbr);
    CHECK(a->value == "0");

    ++a;
    CHECK(a->section == "chordata");
    CHECK(a->option == "l");
    CHECK(a->abbr);
    CHECK(a->value == "pipi");

    prefix = "my:";
    f = parse({ "-x", "--y", "--my:allow=false", "--my:chordata.lion", "pepe", "-zz=34", "-my:b=0", "--yours:gg=45", "-my:chordata.l", "pipi" });
    CHECK(args.size() == 4);
    CHECK(cmp(f, { "exe", "-x", "--y", "-zz=34", "--yours:gg=45" }));

    a = args.begin();
    CHECK(a->section.empty());
    CHECK(a->option == "allow");
    CHECK_FALSE(a->abbr);
    CHECK(a->value == "false");

    ++a;
    CHECK(a->section == "chordata");
    CHECK(a->option == "lion");
    CHECK_FALSE(a->abbr);
    CHECK(a->value == "pepe");

    ++a;
    CHECK(a->section.empty());
    CHECK(a->option == "b");
    CHECK(a->abbr);
    CHECK(a->value == "0");

    ++a;
    CHECK(a->section == "chordata");
    CHECK(a->option == "l");
    CHECK(a->abbr);
    CHECK(a->value == "pipi");
}
