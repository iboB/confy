// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace confy {
enum class env_var_strategy {
    automatic,    // automatically generated from name, section, and config
    manual,       // manually set by the user, section and config prefixes apply
    manual_global,// manually set by the user, section and config prefixes ignored
    none,         // no environment variable
};
} // namespace confy
