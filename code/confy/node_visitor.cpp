// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "node_visitor.hpp"

namespace confy {

node_visitor::~node_visitor() noexcept = default;

void node_visitor::on(const section&) {}
void node_visitor::on(const value&) {}

} // namespace confy
