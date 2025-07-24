// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "env.hpp"
#include <string>

namespace confy {
struct node_desc {
    std::string name;
    std::string description;
    std::string abbr;
    env::var env_var;
};
} // namespace confy
