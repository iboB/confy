// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/configurator.hpp>
#include <confy/v/integer.hpp>
#include <confy/v/std_string.hpp>
#include <confy/v/auto_enum.hpp>
#include <confy/audit_log.hpp>

#include <iostream>
#include <string>

enum class role_t {
    ceo, employee, intern
};

struct configuration {
    std::string name;
    int age = 0;
    role_t role = role_t::intern;
};

int main(int argc, char* argv[]) try {
    confy::configurator cfg;
    configuration c;

    //.cmd<confy::ini_file>()
    cfg.add_value(c.name, {"name", "The name of the person", "n"});
    cfg.add_value(c.age, {"age", "The age of the person", "a"});
    cfg.add_value(c.role, {"role", "The role of the person", "r"});

    if (cfg.configure(argc, argv) == confy::configurator::command_result::suggest_exit) {
        return 0;
    }

    confy::audit_log(cfg, std::cout);

    //std::cout
    //    << "Name: " << c.name
    //    << "\nAge: " << c.age
    //    //<< "\nRole: " << confy::magic_enum::enum_name(c.role)
    //    << "\n";

    return 0;
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
catch (...) {
    std::cerr << "Unknown exception" << std::endl;
    return 1;
}
