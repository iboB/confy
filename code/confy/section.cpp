// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "section.hpp"
#include "basic_value.hpp"

namespace confy {

section::section(description d)
    : m_desc(std::move(d))
{}

section::~section() = default;

} // namespace confy
