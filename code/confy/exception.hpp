// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <stdexcept>

namespace confy {

class CONFY_API exception : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~exception();
};

} // namespace confy
