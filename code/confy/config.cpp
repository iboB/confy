// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "config.hpp"

namespace confy {

config::config() = default;
config::~config() = default;

config::config(config&&) noexcept = default;
config& config::operator=(config&&) noexcept = default;

} // namespace confy
