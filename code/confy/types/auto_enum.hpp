// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#if !CONFY_NO_AUTO_ENUM
#include "basic_option.hpp"

#include "../impl/magic_enum.hpp"

#include <ostream>
#include <type_traits>

namespace confy
{
template <typename T>
class auto_enum : public basic_option<T>
{
public:
    using basic_option<T>::basic_option;

    virtual void write_value_type(std::ostream& out) const final
    {
        out << magic_enum::enum_type_name<T>();
    }

    virtual void write_value_type_desc(std::ostream& out) const override
    {
        using namespace magic_enum::ostream_operators;
        constexpr auto& values = magic_enum::enum_values<T>();

        bool first = true;
        for (const auto& v : values)
        {
            if (first) first = false;
            else out << '|';
            out << v;
        }
    }

    virtual void write_default_value(std::ostream& out) const final
    {
        if (!this->m_default_value) return;
        out << magic_enum::enum_name(*this->m_default_value);
    }

protected:
    virtual bool do_set_from_string(T& val, std::string_view str) final
    {
        auto oval = magic_enum::enum_cast<T>(str);
        if (oval.has_value())
        {
            val = *oval;
            return true;
        }
        return false;
    }
};

template <typename T>
struct option_for_type<T, std::enable_if_t<std::is_enum_v<T>>> { using type = auto_enum<T>; };

}
#endif // !CONFY_NO_SIMPLE_ENUM
