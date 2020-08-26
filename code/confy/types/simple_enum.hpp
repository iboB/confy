// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "../option.hpp"

#include <optional>
#include <ostream>
#include <type_traits>

namespace confy
{
template <typename T>
class simple_enum : public basic_option<T>
{
public:
    using basic_option<T>::basic_option;

    void add_element(T e, std::string_view name)
    {
        m_elements.emplace_back(element{ std::string(name), e });
    }

    template <typename Option>
    struct dsl_t : public basic_option<T>::template dsl_t<Option>
    {
        using basic_option<T>::template dsl_t<Option>::dsl_t;

        auto& e(T e, std::string_view name)
        {
            this->m_option.add_element(e, name);
            return this->self();
        }
    };
    using dsl = dsl_t<simple_enum<T>>;

    virtual void write_value_type(std::ostream& out) const final
    {
        if (m_value_type.empty()) out << this->name();
        else out << m_value_type;
    }

    virtual void write_value_type_desc(std::ostream& out) const override
    {
        bool first = true;
        for (auto& e : m_elements)
        {
            if (first) first = false;
            else out << '|';

            out << e.name;
        }
    }

    virtual void write_default_value(std::ostream& out) const final
    {
        if (!this->m_default_value) return;

        for (auto& e : m_elements)
        {
            if (e.value == *this->m_default_value)
            {
                out << e.name;
                return;
            }
        }

        out << "*unknown*";
    }

protected:
    virtual bool do_set_from_string(T& val, std::string_view str) final
    {
        for (const auto& e : m_elements)
        {
            if (e.name == str)
            {
                val = e.value;
                return true;
            }
        }
        return false;
    }

    struct element
    {
        std::string name;
        T value;
    };
    std::vector<element> m_elements;

    std::string m_value_type;
};

template <typename T>
struct option_for_type<T, std::enable_if_t<std::is_enum_v<T>>> { using type = simple_enum<T>; };

}
