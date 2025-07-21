// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "command.hpp"

#include <cassert>
#include <ostream>

namespace confy
{
command::command(std::string_view name, std::string_view abbr, std::string_view desc)
    : option(name, abbr, desc)
{
    m_no_env = true;
    m_source = value_source::default_val;
}

command::~command() = default;

option::set_value_result command::try_set_value(std::string_view val, value_source source)
{
    if (source < m_min_allowed_source) return set_value_result::bad_source;
    return execute(val);
}

void command::write_value_type(std::ostream& out) const
{
    out << m_arg_type;
}

void command::write_value_type_desc(std::ostream&) const {}

void command::write_default_value(std::ostream& out) const
{
    out << "do nothing";
}

bool command::set_from_default()
{
    assert(false);
    return false;
}

bool command::set_from_string(std::string_view)
{
    assert(false);
    return false;
}

}
