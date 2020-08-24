#pragma once

#include "impl/api.h"

#include "impl/config_item.hpp"

#include "value_source.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iosfwd>

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
    config* m_config = nullptr;
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

    bool no_env() const { return m_no_env; }

    ///////////////////////////////////////////////////////////////////////////
    // schema building

    // add a section with options
    void add_section(section sec);

    ///////////////////////////////////////////////////////////////////////////
    // option parsing
    void parse_ini_file(std::istream& in, std::string_view filename = {});

    // parses the command line and filters out args relevant to the config
    // this means that after this function argc and argv are changed
    // to contain only args which confy doesn't understand
    // they can be processed by additional configs or command line parsers
    void parse_cmd_line(int& argc, char* argv[]);

    ///////////////////////////////////////////////////////////////////////////
    // low-level option parsing
    // the following functions produce errors appropriately

    struct pushed_source
    {
        value_source source;
        std::string_view source_name;
    };
    struct source_stack_sentry
    {
        source_stack_sentry(pushed_source src, config& c)
            : cfg(c)
        {
            cfg.push_source(src);
        }
        ~source_stack_sentry()
        {
            cfg.pop_source();
        }
        source_stack_sentry(const source_stack_sentry&) = delete;
        source_stack_sentry operator=(source_stack_sentry&) = delete;
        config& cfg;
    };
    source_stack_sentry parser_cur_source(pushed_source src)
    {
        return source_stack_sentry(src, *this);
    }

    section* parser_get_section(std::string_view name, bool is_abbr = false);
    void parser_set_option_value(section& sec, std::string_view name, std::string_view value, bool is_abbr = false);
    void parser_set_option_value(std::string_view path, std::string_view value, bool is_abbr = false);

    void parser_add_source_error(std::string_view error);

    ///////////////////////////////////////////////////////////////////////////
    // utils

    // writes the schema in human readable format
    // it can be used for help for command line tools
    void write_schema(std::ostream& out, bool basic = false);

private:
    std::string m_name; // name of config (for logging purposes)
    std::string m_env_var_prefix; // optional prefix for environment variables
    std::string m_cmd_prefix; // optional prefix for command-line arguments
    std::ostream* m_verbose_stream = nullptr; // optional verbose output stream
    bool m_no_env = false; // disable enviroment variables

    std::vector<section> m_sections;

    // manage source stack
    void push_source(pushed_source source);
    void pop_source();

    void do_set_option_value(option& opt, std::string_view value, value_source source);
    void update_options();

    // semi-pimpl hiding config error management
    class config_error_manager;
    std::unique_ptr<config_error_manager> m_config_errors;
};

}
