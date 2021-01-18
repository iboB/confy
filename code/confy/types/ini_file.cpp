// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "ini_file.hpp"

#include "../config.hpp"

#include <fstream>

namespace confy
{

ini_file::ini_file(std::string_view name /*= {}*/, std::string_view abbr /*= {}*/, std::string_view desc /*= {}*/)
    : command(name, abbr, desc)
{
    if (m_name.empty())
    {
        m_name = "config";
    }

    m_arg_type = "path";
}

option::set_value_result ini_file::execute(std::string_view arg)
{
    std::unique_ptr<std::istream> in;
    if (!m_stream_open_func)
    {
        std::string path{arg};
        in = std::make_unique<std::ifstream>(path);

    }
    else
    {
        in = m_stream_open_func(arg);
    }

    if (!in) return set_value_result::bad_source;

    cfg()->parse_ini_file(*in, arg);
    return set_value_result::success;
}

void ini_file::write_value_type_desc(std::ostream& out) const
{
    out << "Load a .ini file with options";
}

}
