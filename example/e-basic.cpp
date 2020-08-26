// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <confy/config.hpp>
#include <confy/types.hpp>

#include <iostream>
#include <string>

enum class role
{
    ceo, employee, intern
};

struct configuration
{
    std::string name;
    int age = 0;
    role role = role::intern;
};

int main(int argc, char* argv[])
{
    confy::config cfg;
    configuration c;

    cfg.schema()
        .opt(c.name, "name", "n", "The name of the person")
        .opt(c.age, "age", "a", "The age of the person")
        .opt(c.role, "role", "r", "The role of the person");

    cfg.parse_cmd_line(argc, argv);

    std::cout
        << "Name: " << c.name
        << "\nAge: " << c.age
        << "\nRole: " << confy::magic_enum::enum_name(c.role)
        << "\n";

    return 0;
}