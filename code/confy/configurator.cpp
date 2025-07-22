// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "configurator.hpp"
#include "basic_value.hpp"

namespace confy {

configurator::configurator(std::string_view name)
    : m_name(name)
{}

configurator::~configurator() = default;

void configurator::parse_command_line(int& argc, char* argv[]) {
    for (auto arg : cli::filter_cmd_line(argc, argv, m_cli_prefix)) {
        m_cli_values.push_back({arg});
    }
}

} // namespace confy
