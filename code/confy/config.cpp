// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "config.hpp"

#include "option.hpp"
#include "section.hpp"
#include "config_error.hpp"
#include "exception.hpp"
#include "schema_dsl.hpp"

#include "impl/cmd_line_util.inl"
#include "impl/laurel.inl"

#include <cassert>
#include <sstream>
#include <cstdlib>
#include <algorithm>

namespace confy
{

inline void output_path(std::ostream& o, const option& opt)
{
    if (!opt.sec()->name().empty())
    {
        o << opt.sec()->name() << SECTION_DELIM;
    }
    o << opt.name();
}

inline std::ostream& operator<<(std::ostream& o, const section& s)
{
    if (s.name().empty()) o << "config";
    else o << "section `" << s.name() << '`';
    return o;
}

inline std::ostream& operator<<(std::ostream& o, const option& opt)
{
    o << '`';
    output_path(o, opt);
    o << '`';
    return o;
}

using ostr = std::ostringstream;

class config::config_error_manager
{
public:
    config_error_manager()
    {
        m_sources_stack.reserve(2);
    }

    bool has_errors() const { return !errors.empty(); }
    void reset() { errors.clear(); }

    std::vector<config_error> errors;

    std::vector<config::pushed_source> m_sources_stack;

    void push_source(pushed_source source)
    {
        m_sources_stack.emplace_back(source);
    }
    void pop_source()
    {
        m_sources_stack.pop_back();
    }

    // error constructors
    config_error& make_error()
    {
        auto& e = errors.emplace_back();
        if (!m_sources_stack.empty())
        {
            e.source = m_sources_stack.back().source;
            e.source_name = m_sources_stack.back().source_name;
        }
        return e;
    }

    void no_section(std::string_view name, bool is_abbr)
    {
        auto& e = make_error();
        e.type = config_error::no_such_section;
        e.section_name = name;

        ostr out;
        out << e.source_name << " refers to missing " << (is_abbr ? " abbreviated " : " ") << " section `" << name << "`. No option values will be set from it";
        e.error_text = out.str();
    }

    void no_option(std::string_view sec, std::string_view opt, bool is_abbr)
    {
        auto& e = make_error();
        e.type = config_error::no_such_option;
        e.section_name = sec;
        e.option_name = opt;

        ostr out;
        out << e.source_name << " refers to missing " << (is_abbr ? " abbreviated " : " ") << " option `" << sec << SECTION_DELIM << opt << "`.";
        e.error_text = out.str();
    }

    void source_error(std::string_view error)
    {
        auto& e = make_error();
        e.type = config_error::bad_source;

        ostr out;
        out << "error in " << e.source_name << ": " << error;
        e.error_text = out.str();
    }

    void bad_set_value(const option& opt, std::string_view value, value_source source, option::set_value_result result)
    {
        auto& e = make_error();
        if (source == value_source::env_var)
        {
            e.source = source;
            e.source_name = "environment variable";
        }

        std::ostringstream sout;
        sout << "error setting value \"" << value << "\" to option " << opt << ". ";
        switch (result)
        {
        case option::set_value_result::same_source_value:
            e.type = config_error::same_source_value;
            sout << "A different value was set with the same source priority.";
            break;
        case option::set_value_result::bad_value:
            e.type = config_error::bad_value;
            sout << "The expected value format is ";
            opt.write_value_type_desc(sout);
            break;
        case option::set_value_result::bad_default:
            e.type = config_error::bad_default;
            sout << "The requested default value ";
            opt.write_default_value(sout);
            sout << " was incompatible with the expected format: ";
            opt.write_value_type_desc(sout);
            break;
        case option::set_value_result::bad_source:
            e.type = config_error::bad_source;
            sout << "The option doesn't accept the provided source.";
            break;
        default:
            assert(false);
        }

        e.section_name = opt.m_section->name();
        e.option_name = opt.name();
        e.opt = &opt;
        e.error_text = sout.str();
    }

    void no_value(const option& opt)
    {
        auto& e = make_error();
        e.source = value_source::default_val;
        e.source_name = "fallback to default value";
        e.type = config_error::bad_default;
        e.section_name = opt.m_section->name();
        e.option_name = opt.name();
        e.opt = &opt;

        ostr out;
        out << "VALUE SET FAILED on " << opt;
        e.error_text = out.str();
    }
};


config::config(std::string_view name /*= {}*/)
    : m_name(name)
    , m_config_errors(std::make_unique<config_error_manager>())
{
}

config::~config() = default;

namespace
{
// utility class to easily search by name or abbr
struct by
{
    using key = impl::config_item;
    bool operator()(const key& i) const { return m_cmp(i, *this); }
    bool operator()(const key* i) const { return m_cmp(*i, *this); }
    template <typename U>
    bool operator()(const std::unique_ptr<U>& i) const { return m_cmp(*i, *this); }

    static by name(std::string_view n)
    {
        by ret;
        ret.m_name = n;
        ret.m_cmp = [](const key& a, const by& b) {
            return a.name() == b.m_name;
        };
        return ret;
    }

    static by abbr(std::string_view a)
    {
        by ret;
        ret.m_abbr = a;
        ret.m_cmp = [](const key& a, const by& b) {
            return a.abbr() == b.m_abbr;
        };
        return ret;
    }

    static by any(const key& c)
    {
        by ret;
        ret.m_name = c.name();
        ret.m_abbr = c.abbr();

        ret.m_cmp = [](const key& a, const by& b) {
            return (a.name() == b.m_name)
                || (a.abbr() == b.m_abbr);
        };
        return ret;
    }

    typedef by(*what)(std::string_view val);

    typedef bool(*cmpf)(const impl::config_item& a, const by& b);
    cmpf m_cmp;

    std::string_view m_name;
    std::string_view m_abbr;
};

template <typename Container>
auto find(Container& c, by what)
{
    return std::find_if(c.begin(), c.end(), what);
}

// exceptions

struct throw_t {} _throw;
[[noreturn]] std::ostream& operator<<(std::ostream& o, const throw_t&)
{
    auto& ss = static_cast<std::ostringstream&>(o);
    throw schema_exception(ss.str());
}

[[noreturn]] void throw_duplicate_option(const section& s, const option& o)
{
    ostr() << "option with name `" << o.name() << "` or abbreviation `" << o.abbr() << "` already exists in " << s << _throw;
}

[[noreturn]] void throw_duplicate_section(const section& s)
{
    ostr() << "section with name `" << s.name() << "` or abbreviation `" << s.abbr() << "` already exists in config" << _throw;
}

[[noreturn]] void throw_bad_option_name(const option& o)
{
    ostr() << "bad option name `" << o.name() << "`. Option names cannot be empty or contain \"" << SECTION_DELIM << '"' << _throw;
}

}

void config::add_section(std::unique_ptr<section> psec)
{
    auto& sec = *psec;

    // so as to allow abbreviated options without specifying abbreviated section names
    if (sec.abbr().empty()) sec.m_abbr = sec.m_name;

    auto f = find(m_sections, by::any(sec));
    if (f != m_sections.end())
    {
        throw_duplicate_section(sec);
    }

    sec.m_config = this;

    // generate environment variable names
    if (!m_no_env)
    {
        for (auto& popt : sec.m_options)
        {
            generate_option_env_var(*popt);
        }
    }

    m_sections.emplace_back(std::move(psec));
}

void config::generate_option_env_var(option& opt)
{
    assert(!m_no_env); // should be checked externally

    if (!opt.no_env() && opt.env_var().empty())
    {
        std::ostringstream sout;
        sout << m_env_var_prefix;
        output_path(sout, opt);
        opt.m_env_var = sout.str();

        // fix risky characters
        for (auto& c : opt.m_env_var)
        {
            if (!isalnum(c) && c != '_')
            {
                c = '_';
            }
            // should we toupper?
        }
    }
}

schema_dsl config::schema()
{
    return schema_dsl(*this);
}

void config::parse_ini_file(std::istream& in, std::string_view filename)
{
    auto s = parser_cur_source({ value_source::config_file, filename });
    section* cur_sec;
    auto on_sec = [&, this](std::string_view sec) {
        cur_sec = parser_get_section(sec);
    };
    auto on_opt = [&, this](std::string_view name, std::string_view value) {
        if (cur_sec)
        {
            parser_set_option_value(*cur_sec, name, value);
        }
    };
    auto on_error = [&, this](laurel_error error, int line) {
        std::string e = laurel_error_to_text(error);
        e += " on line ";
        e += std::to_string(line);
        parser_add_source_error(e);
    };
    laurel(in, on_sec, on_opt, on_error);
}

void config::parse_cmd_line(int& argc, char* argv[])
{
    auto s = parser_cur_source({ value_source::cmd_line, "command line" });

    filter_command_line(argc, argv, m_cmd_prefix, [this](std::string_view path, bool is_abbr, std::string_view value) {
        parser_set_option_value(path, value, is_abbr);
    });
}

section* config::parser_get_section(std::string_view name, bool is_abbr /*= false*/)
{
    auto f = is_abbr ? by::abbr : by::name;
    auto sec = find(m_sections, f(name));
    if (sec == m_sections.end())
    {
        m_config_errors->no_section(name, is_abbr);
        return nullptr;
    }
    return sec->get();
}

void config::parser_set_option_value(section& sec, std::string_view name, std::string_view value, bool is_abbr /*= false*/)
{
    auto f = is_abbr ? by::abbr : by::name;
    auto opt = find(sec.m_options, f(name));
    if (opt == sec.m_options.end())
    {
        m_config_errors->no_option(sec.name(), name, is_abbr);
        return;
    }

    do_set_option_value(**opt, value, m_config_errors->m_sources_stack.back().source);
}

void config::parser_set_option_value(std::string_view path, std::string_view value, bool is_abbr /*= false*/)
{
    auto f = is_abbr ? by::abbr : by::name;
    auto split = split_option_path(path);
    auto sec = find(m_sections, f(split.section));
    if (sec == m_sections.end())
    {
        m_config_errors->no_option(split.section, split.option, is_abbr);
        return;
    }
    parser_set_option_value(**sec, split.option, value, is_abbr);
}

void config::parser_add_source_error(std::string_view error)
{
    m_config_errors->source_error(error);
}

void config::do_set_option_value(option& opt, std::string_view value, value_source source)
{
    auto result = opt.try_set_value(value, source);
    if (result > option::set_value_result::skipped) return;
    m_config_errors->bad_set_value(opt, value, source, result);
}

void config::push_source(pushed_source source)
{
    m_config_errors->push_source(source);
}

void config::pop_source()
{
    m_config_errors->pop_source();
    if (m_config_errors->m_sources_stack.empty())
    {
        update_options();
    }
}

void config::update_options()
{
    for (auto& sec : m_sections)
    {
        for (auto& opt : sec->m_options)
        {
            if (opt->m_source == value_source::none)
            {
                // first look for env_var
                if (!m_no_env && !opt->no_env())
                {
                    assert(!opt->env_var().empty());
                    auto pvalue = std::getenv(opt->env_var().c_str());
                    if (pvalue)
                    {
                        std::string_view val = pvalue;
                        if (opt->set_from_string(val))
                        {
                            continue;
                        }
                        else
                        {
                            m_config_errors->bad_set_value(*opt, val, value_source::env_var, option::set_value_result::bad_value);
                        }
                    }
                }

                // if env var was unavailable set default val
                if (opt->set_from_default())
                {
                    opt->m_source = value_source::default_val;
                }
                else
                {
                    m_config_errors->no_value(*opt);
                }
            }
        }
    }
}

void config::write_schema(std::ostream& out, bool basic) const
{
    if (m_name.empty()) out << "confy";
    else out << m_name;

    out << " config";

    for (const auto& psec : m_sections)
    {
        auto& sec = *psec;
        out << sec.name() << '\n';

        if (!basic)
        {
            if (!sec.description().empty() && !basic)
            {
                out << sec.description() << '\n';
            }
            out << '\n';
        }

        for (const auto& popt : sec.m_options)
        {
            auto& opt = *popt;
            out << "--" << m_cmd_prefix;
            output_path(out, opt);
            if (!opt.is_default_false())
            {
                out << "=<";
                opt.write_value_type(out);
                out << '>';
            }

            if (!opt.abbr().empty())
            {
                out << ", -" << m_cmd_prefix;
                if (!sec.abbr().empty()) out << sec.abbr() << SECTION_DELIM;
                out << opt.abbr();
                if (!opt.is_default_false())
                {
                    out << "=<";
                    opt.write_value_type(out);
                    out << '>';
                }
            }

            if (basic)
            {
                if (!opt.no_env())
                {
                    out << ", e: " << opt.env_var();
                }
                if (!opt.is_default_false())
                {
                    if (opt.has_default_value())
                    {
                        out << ", d: ";
                        opt.write_default_value(out);
                    }
                    else
                    {
                        out << ", Required!";
                    }
                }
                out << '\n';
            }
            else
            {
                out << '\n';
                if (!opt.is_default_false())
                {
                    out << "    ";
                    opt.write_value_type_desc(out);
                    out << '\n';
                }

                if (!opt.description().empty())
                {
                    out << "    " << opt.description() << '\n';
                }

                if (!opt.no_env())
                {
                    out << "    Environment variable: " << opt.env_var() << '\n';
                }

                if (!opt.is_default_false())
                {
                    if (opt.has_default_value())
                    {
                        out << "    Default is: ";
                        opt.write_default_value(out);
                    }
                    else
                    {
                        out << "    Required!";
                    }
                }
                out << "\n\n";
            }
        }
    }
}

section::~section() = default;

void section::add_option(std::unique_ptr<option> o)
{
    if (o->name().empty()) throw_bad_option_name(*o);

    for (auto& c : o->name())
    {
        // don't allow section delimeters in option names
        if (c == SECTION_DELIM) throw_bad_option_name(*o);
    }

    auto f = find(m_options, by::any(*o));
    if (f != m_options.end())
    {
        throw_duplicate_option(*this, *o);
    }

    o->m_section = this;

    if (m_config)
    {
        // this section is already added to the config and we're adding an option
        // we need to generate its enviroment variable
        if (!m_config->no_env())
        {
            m_config->generate_option_env_var(*o);
        }
    }

    m_options.emplace_back(std::move(o));
}


}
