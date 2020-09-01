// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "option.hpp"
#include "section.hpp"

namespace confy
{

option::~option() = default;

namespace
{
constexpr std::string_view DEFAULT_VALUE = "default";
}

option::set_value_result option::try_set_value(std::string_view val, value_source source)
{
    if (source < m_source) return set_value_result::skipped;
    if (source == m_source) return set_value_result::same_source_value;

    if (val == DEFAULT_VALUE)
    {
        if (set_from_default())
        {
            m_source = source;
            return set_value_result::success;
        }
        else
        {
            return set_value_result::bad_default;
        }
    }

    if (set_from_string(val))
    {
        m_source = source;
        return set_value_result::success;
    }

    return set_value_result::bad_value;
}

config* option::cfg() const
{
    if (!m_section) return nullptr;
    return m_section->cfg();
}

}
