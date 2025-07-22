// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "cli.hpp"
#include "dict.hpp"
#include "section.hpp"
#include <string>
#include <string_view>

namespace confy {

class config;

class CONFY_API configurator {
public:
    configurator(std::string_view name = "configuration");
    ~configurator();

    configurator(const configurator&) = delete;
    configurator& operator=(const configurator&) = delete;

    // only parse command line and don't touch config values
    void parse_command_line(int& argc, char* argv[]);

    void parse_ini_file(std::istream& in, std::string_view filename = {});

    void parse_json_file(std::string_view filename);

    void add_user_overrides(dict overrides);

    enum command_result {
        continue_exec, // continue execution (e.g. normal operation)
        suggested_exit, // suggest the program to exit without error (e.g. help, version)
    };

    // utility func
    // parse command line, execute commands, and configure()
    command_result configure(int& argc, char* argv[]);

    // configure from existing store
    void configure();

    section& add_section(section::description desc);
    section& get_default_section() const noexcept;
    section* find_section(std::string_view name) const noexcept;

    config get_config();

private:
    std::string m_name; // name of config (for logging/debugging only)

    std::string m_cli_prefix; // prefix for command line options

    std::string m_env_var_prefix; // prefix for environment variables
    bool m_no_env = false; // disable environment variables

    // value store
    struct cli_arg : public cli::parsed_argument {
        bool used = false;
    };
    std::vector<cli_arg> m_cli_values;
    dict m_config_file_values;
    dict m_user_overrides;

    bool m_configure_done = false;

    // configuration
    std::vector<std::unique_ptr<section>> m_sections; // sections in this config, first section is default
};

} // namespace confy