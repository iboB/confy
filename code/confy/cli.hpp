// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <itlib/generator.hpp> // C++23: replace with std::generator
#include <string_view>
#include <optional>

namespace confy::cli {

struct parsed_argument {
    std::string_view key;
    std::string_view value;
    bool abbr = false; // abbreviated argument (single dash)
};

CONFY_API std::optional<parsed_argument> parse_arg(std::string_view arg, std::string_view prefix = {}) noexcept;

// yield relevant args and filter command line so that only irrelevant args remain
// relevant args are those that start with one or two dashes and prefix
CONFY_API itlib::generator<parsed_argument> filter_cmd_line(int& argc, char* argv[], std::string_view prefix = {}) noexcept;

} // namespace confy::cli
