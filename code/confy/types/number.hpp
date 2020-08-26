// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "generic_option.hpp"

#include "../impl/num_from_string.hpp"

namespace confy
{

template <typename Num>
class number : public generic_option<Num>
{
public:
    number(std::string_view name, std::string_view abbr = {}, std::string_view desc = {})
        : generic_option<Num>(name, abbr, desc)
    {
        this->m_value_type = "num";
    }

    void set_min(Num m)
    {
        m_min = m;
    }

    void set_max(Num m)
    {
        m_max = m;
    }

    template <typename Option>
    struct dsl_t : public generic_option<Num>::template dsl_t<Option>
    {
        using generic_option<Num>::template dsl_t<Option>::dsl_t;

        auto& min(Num m)
        {
            this->m_option.set_min(m);
            return this->self();
        }

        auto& max(Num m)
        {
            this->m_option.set_max(m);
            return this->self();
        }
    };
    using dsl = dsl_t<number>;

    virtual bool do_set_from_string(Num& val, std::string_view str) override
    {
        return impl::num_from_string(val, str);
    }

    virtual bool validate(const Num& val) const override
    {
        if (m_min && val < *m_min) return false;
        if (m_max && val > *m_max) return false;
        return true;
    }

protected:
    void write_limits(std::ostream& out) const
    {
        if (m_min && m_max)
        {
            out << " from " << *m_min << " to " << *m_max;
        }
        else if (m_min)
        {
            out << " >=" << *m_min;
        }
        else if (m_max)
        {
            out << " <=" << *m_max;
        }
    }

    std::optional<Num> m_min;
    std::optional<Num> m_max;
};

}
