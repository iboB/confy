// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "generic_option.hpp"

#include <vector>

namespace confy
{

class string : public generic_option<std::string>
{
public:
    string(std::string_view name, std::string_view abbr = {}, std::string_view desc = {})
        : generic_option<std::string>(name, abbr, desc)
    {
        m_value_type = "string";
    }

    void add_enum_element(std::string_view elem)
    {
        m_enum.emplace_back(elem);
    }

    template <typename Option>
    struct dsl_t : public generic_option<std::string>::dsl_t<Option>
    {
        using generic_option<std::string>::dsl_t<Option>::dsl_t;

        auto& e(std::string_view name)
        {
            this->m_option.add_enum_element(name);
            return this->self();
        }
    };
    using dsl = dsl_t<string>;

    virtual void write_value_type_desc(std::ostream& out) const override
    {
        if (!m_value_type_desc.empty())
        {
            out << m_value_type_desc;
            return;
        }

        if (m_enum.empty())
        {
            write_value_type(out);
            return;
        }

        bool first = true;
        for (auto& e : m_enum)
        {
            if (first) first = false;
            else out << '|';

            out << e;
        }
    }

    virtual bool do_set_from_string(std::string& val, std::string_view str) override final
    {
        val = str;
        return true;
    }

    virtual bool validate(const std::string& val) const override final
    {
        if (m_enum.empty()) return true;

        for (const auto& e : m_enum)
        {
            if (e == val)
            {
                return true;
            }
        }

        return false;
    }

private:
    // the string type can double as an enum
    // if the user adds a number of valid values
    std::vector<std::string> m_enum;
};

template <>
struct option_for_type<std::string> { using type = string; };

}
