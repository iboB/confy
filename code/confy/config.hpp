// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "dict.hpp"

namespace confy {

class CONFY_API config {
public:
    ~config();

    config(const config&) = delete;
    config& operator=(const config&) = delete;
    config(config&&) noexcept;
    config& operator=(config&&) noexcept;

public:
    friend class configurator;
    config();

    std::string m_name;

    dict m_dict;
};

} // namespace confy
