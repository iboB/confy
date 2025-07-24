// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "cli.hpp"
#include "dict.hpp"
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
        std::string cli_prefix; // prefix for command line options
        env::var env_var_prefix; // prefix for environment variables
    };

    explicit configurator(desc d = {});
    ~configurator();

    configurator(const configurator&) = delete;
    configurator& operator=(const configurator&) = delete;

    void parse_cmd_line(int& argc, char* argv[]);
    void parse_ini_file(std::istream& in, std::string_view filename = {});
    void parse_json_file(std::istream& in, std::string_view filename = {});

    using section::add_section;
    using section::add_value;
    using section::get_child;
    using section::get_abbr_child;

    void set_values_from_env_vars();

    config get_config();

private:
    desc m_desc;
};

} // namespace confy
