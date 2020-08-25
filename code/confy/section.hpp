// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "impl/config_item.hpp"

#include <vector>
#include <memory>

namespace confy
{

class config;
class option;

class CONFY_API section : public impl::config_item
{
public:
    using impl::config_item::config_item;
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept = delete;
    section& operator=(section&&) noexcept = delete;

    void add_option(std::unique_ptr<option> o);

    const std::vector<std::unique_ptr<option>>& options() const { return m_options; }

private:
    friend class config;
    config* m_config = nullptr;
    std::vector<std::unique_ptr<option>> m_options;
};

}