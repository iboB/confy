// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "../option.hpp"

#include <optional>

namespace confy
{

template <typename T>
class basic_option : public option
{
public:
    using option::option;

    void set_value_ptr(T* ptr)
    {
        m_value_ptr = ptr;
        if (ptr) m_default_value = *ptr;
    }

    template <typename Option>
    struct dsl_t : public option::dsl_t<Option>
    {
        using option::dsl_t<Option>::dsl_t;
        auto& val(T& v)
        {
            this->m_option.set_value_ptr(&v);
            return this->self();
        }
    };
    using dsl = dsl_t<basic_option<T>>;

    virtual bool has_default_value() const final
    {
        return m_default_value.has_value();
    }

protected:
    virtual bool validate(const T&) const { return true; }

    virtual bool set_from_default() final
    {
        if (!m_default_value) return false;
        if (!validate(*m_default_value)) return false;
        if (!m_value_ptr) return true; // successful non-set :)
        *m_value_ptr = *m_default_value;
        return true;
    }

    virtual bool set_from_string(std::string_view str) final
    {
        T val;
        if (!do_set_from_string(val, str)) return false;
        if (!validate(val)) return false;
        if (!m_value_ptr) return true;
        *m_value_ptr = std::move(val);
        return true;
    }

    virtual bool do_set_from_string(T& val, std::string_view str) = 0;

    T* m_value_ptr = nullptr;
    std::optional<T> m_default_value;
};

}
