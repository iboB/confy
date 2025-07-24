// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "configurator.hpp"
#include "config.hpp"
#include "bits/throw_ex.hpp"
#include <itlib/qalgorithm.hpp>
#include <fstream>
#include <sstream>
#include <cassert>

#include "bits/laurel.inl"

namespace confy {

configurator::configurator(desc d)
    : section({ .name = {}, .description = {}, .abbr = {}, .env_var = d.env_var_prefix }, nullptr)
    , m_desc(std::move(d))
{
}

configurator::~configurator() = default;

void configurator::parse_command_line(int& argc, char* argv[]) {
    for (auto& arg : cli::filter_cmd_line(argc, argv, m_desc.cli_prefix)) {
        node* child;
        if (arg.abbr) {
            child = get_abbr_child(arg.key);
        }
        else {
            child = get_child(arg.key);
        }

        if (!child) {
            // no such node, this is an error
            throw_ex{} << m_desc.name << ": unknown command line argument: "
                << arg.key << (arg.abbr ? " (abbr)" : "") << " = " << arg.value;
        }

        child->set_from_string(arg.value, value_source::cmd_line);
    }
}

void configurator::parse_ini_file(std::istream& in, std::string_view filename) {
    std::string cur_section;
    auto on_section = [&](std::string_view name) {
        cur_section = std::string(name) + ".";
    };
    auto on_key = [&](std::string_view key, std::string_view value) {
        auto full_key = cur_section + std::string(key);
        auto* child = get_child(full_key);
        if (!child) {
            throw_ex{} << filename << ": unknown key: " << full_key << "( = " << value << ")";
        }
        child->set_from_string(value, value_source::config_file);
    };
    auto on_error = [&](laurel_error error, int line) {
        throw_ex{} << filename << ":" << line << " " << laurel_error_to_text(error);
    };
    laurel(in, on_section, on_key, on_error);
}

void configurator::parse_json_file(std::istream& in, std::string_view filename) {
    try {
        auto json = dict::parse(in);
        set_from_dict(json, value_source::config_file);
    }
    catch (std::exception& ex) {
        throw_ex{} << filename << ": " << ex.what();
    }
}

void configurator::set_values_from_env_vars() {
    try_set_from_env_var();
}

} // namespace confy
