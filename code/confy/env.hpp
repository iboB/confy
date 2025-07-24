// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string>

namespace confy::env {

enum class var_strategy {
    automatic,    // automatically generated from name, section, and config
    manual,       // manually set by the user, section and config prefixes apply
    manual_global,// manually set by the user, section and config prefixes ignored
    none,         // no environment variable
};

struct var {
    std::string str; // name or prefix
    var_strategy strategy = var_strategy::automatic;
};

} // namespace confy::env
