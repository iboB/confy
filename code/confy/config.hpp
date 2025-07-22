// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include <string>
#include <vector>
#include <memory>

namespace confy {
class section;

class CONFY_API config {
public:
    ~config();

    config(const config&) = delete;
    config& operator=(const config&) = delete;
    config(config&&) noexcept = default;
    config& operator=(config&&) noexcept;

public:
    friend class configurator;
    config();

    std::string m_name;

    std::vector<std::unique_ptr<section>> m_sections; // sections in this config
};

} // namespace confy