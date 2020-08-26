// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "command.hpp"

#include <functional>

namespace confy
{
class function final : public command
{
public:
    using command::command;

    using execute_func = std::function<option::set_value_result(function& self, std::string_view arg)>;

    void set_execute_func(execute_func func) { m_func = std::move(func); }

    virtual set_value_result execute(std::string_view arg) override
    {
        return m_func(*this, arg);
    }

    template <typename Option>
    struct dsl_t : public command::dsl_t<Option>
    {
        using command::dsl_t<Option>::dsl_t;

        auto& func(execute_func f)
        {
            this->m_option.set_execute_func(f);
            return this->self();
        }
    };
    using dsl = dsl_t<function>;

private:
    execute_func m_func;
};
}
