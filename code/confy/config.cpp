#include "config.hpp"

#include "impl/cmd_line_util.inl"

namespace confy
{

config::config(std::string_view name /*= {}*/)
    : m_name(name)
{}

config::~config() = default;

void config::parse_cmd_line(int& argc, char* argv[])
{
    auto parse_arg = [this](arg_split path, bool abbr, std::string_view value) {

    };
}

}
