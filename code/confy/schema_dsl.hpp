// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "impl/api.h"

#include "option_for_type.hpp"

#include <memory>
#include <string_view>

namespace confy
{

class config;
class section;
class option;

class CONFY_API schema_dsl
{
public:
    schema_dsl(config& cfg);
    ~schema_dsl() noexcept(false); // this destructor can throw exceptions!

    schema_dsl& sec(std::string_view name, std::string_view abbr = {}, std::string_view desc = {});

    template <typename Option>
    auto cmd(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
    {
        add_cur_option();
        m_cur_option = std::make_unique<Option>(name, abbr, desc);
        return typename Option::dsl(static_cast<Option&>(*m_cur_option), *this);
    }

    template <typename Option, typename Value>
    auto opt(Value& val, std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
    {
        add_cur_option();
        auto popt = std::make_unique<Option>(name, abbr, desc);
        popt->set_value_ptr(&val);
        m_cur_option = std::move(popt);
        return typename Option::dsl(static_cast<Option&>(*m_cur_option), *this);
    }

    template <typename Value>
    auto opt(Value& val, std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
    {
        return opt<option_for_type_t<Value>, Value>(val, name, abbr, desc);
    }

private:

    void add_cur_option();
    void add_cur_section();

    config& m_cfg;
    std::unique_ptr<section> m_cur_section;
    std::unique_ptr<option> m_cur_option;
};

}
