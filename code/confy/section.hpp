// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"
#include <memory>
#include <itlib/flat_map.hpp>

namespace confy {

class CONFY_API section final : public node {
public:
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept = default;
    section& operator=(section&&) noexcept = default;

    //void add_value(std::unique_ptr<basic_value> value);

private:
    itlib::flat_map<std::string, std::unique_ptr<node>> m_children;
};

} // namespace confy