#pragma once

#include "impl/api.h"
#include "impl/config_item.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iosfwd>
#include <functional>

namespace confy
{

class option;
class config;
class config_error;

class CONFY_API section : public impl::config_item
{
public:
    using impl::config_item::config_item;
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept;
    section& operator=(section&&) noexcept;

    void add_option(std::unique_ptr<option> o);

private:
    friend class config;
    std::vector<std::unique_ptr<option>> m_options;
};

class CONFY_API config
{
public:
    config(std::string_view name = {});
    ~config();

    config(const config&) = delete;
    config& operator=(const config&) = delete;
    config(config&&) = delete;
    config& operator=(config&&) = delete;

    ///////////////////////////////////////////////////////////////////////////
    // setup
    using open_config_file_func = std::function<std::unique_ptr<std::istream>(std::string_view path)>;

    ///////////////////////////////////////////////////////////////////////////
    // schema building

    // add a section with options
    void add_section(section sec);

    ///////////////////////////////////////////////////////////////////////////
    // option parsing
    void parse_ini_file(std::string_view path);
    void parse_ini_file(std::istream& in);

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
    open_config_file_func m_open_config_file_func; // optional config file open func

    std::vector<section> m_sections;

    // semi-pimpl hiding config error management
    class config_error_manager;
    std::unique_ptr<config_error_manager> m_config_errors;
};

}
