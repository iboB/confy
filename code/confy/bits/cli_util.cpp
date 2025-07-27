// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "cli_util.hpp"

namespace confy::cli {

namespace {
// value which is given to unset arguments
constexpr std::string_view UNSET_VALUE = "true";
}

std::optional<parsed_argument> parse_arg(std::string_view arg, std::string_view prefix) noexcept {
    if (arg.empty()) return {};
    if (arg[0] != '-') return {}; // not an argument

    parsed_argument ret;

    arg = arg.substr(1); // remove the -
    if (arg.empty() || arg[0] != '-') {
        ret.abbr = true; // single dash, abbreviated
    }
    else {
        arg = arg.substr(1); // remove the second -
    }

    if (!arg.starts_with(prefix)) return {}; // not our prefix

    arg.remove_prefix(prefix.size());

    if (arg.empty()) return {}; // just dashes

    // look for equals sign
    auto eq = arg.find('=');
    if (eq == std::string_view::npos) {
        ret.key = arg;
    }
    else {
        ret.key = arg.substr(0, eq);
        ret.value = arg.substr(eq + 1);
    }

    return ret;
}

itlib::generator<parsed_argument> filter_cmd_line(int& argc, char* argv[], std::string_view prefix, int offset) noexcept {
    parsed_argument last_p;
    bool wants_value = false;
    int new_argc = offset; // argc after parsing
    for (int i = offset; i < argc; ++i) {
        std::string_view arg = argv[i];
        auto p = parse_arg(arg, prefix);

        if (wants_value) {
            wants_value = false;

            // here we expect a value
            // so we shall interpret an irrelevant arg as a value
            if (!p) {
                last_p.value = arg;
                co_yield last_p;
                continue;
            }
            else {
                // p is relevant, this means we'll interpret last arg as true
                last_p.value = UNSET_VALUE;
                co_yield last_p;

                // do not `continue`, but go on with parsing the relevant arg p
            }
        }
        else if (!p) {
            // irrelevant arg: add to filtered argv
            argv[new_argc++] = argv[i];
            continue;
        }

        last_p = *p;

        if (last_p.value.empty()) {
            wants_value = true;
            continue;
        }

        co_yield last_p;
    }

    if (wants_value) {
        // we haven't completed the last arg
        last_p.value = UNSET_VALUE;
        co_yield last_p;
    }

    argc = new_argc;
}

} // namespace confy::cli
