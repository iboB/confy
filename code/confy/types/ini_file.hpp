// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "command.hpp"

#include <istream>
#include <functional>

namespace confy
{

class CONFY_API ini_file : public command
{
public:
    ini_file(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {});

    using stream_open_func = std::function<std::unique_ptr<std::istream>(std::string_view path)>;
    void set_stream_open_func(stream_open_func func) { m_stream_open_func = std::move(func); }

    virtual set_value_result execute(std::string_view arg) override;

    template <typename Option>
    struct dsl_t : public command::dsl_t<Option>
    {
        using command::dsl_t<Option>::dsl_t;

        auto& stream_open(stream_open_func func)
        {
            this->m_option.set_stream_open_func(std::move(func));
            return this->self();
        }
    };
    using dsl = dsl_t<ini_file>;

    virtual void write_value_type_desc(std::ostream& out) const override;

protected:
    stream_open_func m_stream_open_func;
};

}
