// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "number.hpp"

#include <type_traits>

namespace confy
{

template <typename Int>
class integer : public number<Int>
{
public:
    using number<Int>::number;

    virtual void write_value_type_desc(std::ostream& out) const override
    {
        if (!this->m_value_type_desc.empty())
        {
            out << this->m_value_type_desc;
            return;
        }

        out << "A " << (sizeof(Int) * 8) << "-bit integer";
        this->write_limits(out);
    }
};

template <typename T>
struct option_for_type<T, std::enable_if_t<std::is_integral_v<T>>> { using type = integer<T>; };

}
