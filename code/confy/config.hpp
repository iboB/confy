#pragma once

#include "impl/api.h"
#include "impl/config_item.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iosfwd>

namespace confy
{

class option;

class CONFY_API section : public impl::config_item
{
public:
    section();
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept = delete;
    section& operator=(section&&) noexcept = delete;

private:
    std::vector<std::unique_ptr<option>> m_options;
};

class CONFY_API config
{
public:
    config(std::string_view name = {});
    ~config();

    config(const config&) = delete;
    config& operator=(const config&) = delete;

    // add a section with options
    void add_section(section sec);

    // parses the command line and filters out args relevant to the config
    // this means that after this function argc and argv are changed
    // to contain only args which confy doesn't understand
    // they can be processed by additional configs or command line parsers
    void parse_cmd_line(int& argc, char* argv[]);

    // writes the schema in human readable format
    // it can be used for help for command line tools
    void write_schema(std::ostream& out);

private:
    std::string m_name; // name of config (for logging purposes)
    std::string m_env_var_prefix; // optional prefix for environment variables
    std::string m_cmd_prefix; // optional prefix for command-line arguments

    std::vector<section> m_sections;
};

}
