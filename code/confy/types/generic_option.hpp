// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "basic_option.hpp"

#include <optional>
#include <ostream>

namespace confy
{
template <typename T>
class generic_option : public basic_option<T>
{
public:
    using basic_option<T>::basic_option;

    template <typename Option>
    struct dsl_t : public basic_option<T>::template dsl_t<Option>
    {
        using basic_option<T>::template dsl_t<Option>::dsl_t;
    };
    using dsl = dsl_t<generic_option<T>>;

    virtual void write_value_type(std::ostream& out) const final
    {
        out << m_value_type;
    }

    virtual void write_value_type_desc(std::ostream& out) const override
    {
        if (m_value_type_desc.empty()) write_value_type(out);
        else out << m_value_type_desc;
    }

    virtual void write_default_value(std::ostream& out) const final
    {
        if (this->m_default_value) out << *this->m_default_value;
    }

protected:
    std::string m_value_type;
    std::string m_value_type_desc;
};
}
