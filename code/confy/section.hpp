// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <string>
#include <memory>
#include <vector>

namespace confy {

class basic_value;

class CONFY_API section {
public:
    struct description {
        std::string name;
        std::string abbr;
        std::string desc;
        std::string env_var_prefix;
    };

    const description& desc() const { return m_desc; }

    section(description d);
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept = default;
    section& operator=(section&&) noexcept = default;

    void add_value(std::unique_ptr<basic_value> value);

private:
    description m_desc;
    std::vector<std::unique_ptr<basic_value>> m_values;
};

} // namespace confy