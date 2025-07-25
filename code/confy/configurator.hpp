// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "cli.hpp"
#include "section.hpp"
#include <string>
#include <iosfwd>
#include <string_view>

namespace confy {

class config;
class basic_value;

class CONFY_API configurator : private section {
public:
    struct desc {
        std::string name; // name of the config, used for logging/debugging
        env::var env_var_prefix; // prefix for environment variables
    };

    explicit configurator(desc d = {});
    ~configurator();

    configurator(const configurator&) = delete;
    configurator& operator=(const configurator&) = delete;

    using section::add_section;
    using section::add_value;
    using section::get_child;
    using section::get_abbr_child;

    void set_values(const dict& d, value_source src = value_source::manual_override);

    struct cmd_line_options {
        std::string_view cli_prefix; // prefix for command line options
        int offset = 1; // offset in argc/argv for the command line arguments
        bool allow_unparsed = false; // whether to throw on unparsed arguments
    };
    void parse_cmd_line(int& argc, char* argv[], cmd_line_options opts = {});

    void parse_ini_file(std::istream& in, std::string_view filename = {});
    void parse_json_file(std::istream& in, std::string_view filename = {});
    void set_values_from_env_vars();
    using section::validate;

    enum command_result {
        continue_exec, // continue execution (e.g. normal operation)
        suggest_exit, // suggest the program to exit without error (e.g. help, version)
    };

    // parse command line arguments
    // execute commands if any
    // if comamnds don't suggest exit, set values from env vars and validate
    command_result configure(int& argc, char* argv[]);

    config get_config();

private:
    desc m_desc;
};

} // namespace confy
