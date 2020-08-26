// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "basic_option.hpp"

namespace confy
{
class boolean : public basic_option<bool>
{
public:
    using basic_option<bool>::basic_option;

    virtual void write_value_type(std::ostream& out) const final
    {
        out << "bool";
    }

    virtual void write_value_type_desc(std::ostream& out) const override final
    {
        out << "true|1|false|0";
    }

    virtual void write_default_value(std::ostream& out) const final
    {
        if (!m_default_value) return;

        out << (*m_default_value ? "true" : "false");
    }

    virtual bool do_set_from_string(bool& val, std::string_view str) final
    {
        if (str == "true" || str == "1")
        {
            val = true;
            return true;
        }
        else if (str == "false" || str == "0")
        {
            val = false;
            return true;
        }

        return false;
    }

    bool is_default_false() const override final { return m_default_value && !*m_default_value; }
};

template <>
struct option_for_type<bool> { using type = boolean; };
}
