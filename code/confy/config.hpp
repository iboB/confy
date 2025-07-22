// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <string>

namespace confy {

class CONFY_API config {
public:

private:
    std::string m_name; // name of config (for logging/debugging only)
    std::string m_env_var_prefix; // prefix for environment variables
};

} // namespace confy